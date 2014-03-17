/******************************************************************************
**  This is free software: you can redistribute it and/or modify
**  it under the terms of the GNU General Public License as published by
**  the Free Software Foundation, either version 3 of the License, or
**  (at your option) any later version.
**
**  This is distributed in the hope that it will be useful,
**  but WITHOUT ANY WARRANTY; without even the implied warranty of
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**  GNU General Public License for more details.
**
**  You should have received a copy of the GNU General Public License
**  along with this code.  If not, see <http://www.gnu.org/licenses/>.
**
**
**
**  File:         crash-kmod.c
**  Author(s):    Jonathon Pendlum (jon.pendlum@gmail.com)
**  Description:  Kernel driver for CRASH project. Creates DMA buffers,
**                mmaps control registers, and implements DMA transfers
**                with mutexs to prevent conflicts.
**
******************************************************************************/
#include <linux/module.h>
#include <linux/version.h>
#include <linux/kernel.h>
#include <linux/device.h>
#include <linux/platform_device.h>
#include <linux/miscdevice.h>
#include <linux/cdev.h>
#include <linux/fs.h>
#include <linux/io.h>
#include <linux/mm.h>
#include <linux/of.h>
#include <linux/of_address.h>
#include <linux/interrupt.h>
#include <linux/poll.h>
#include <linux/atomic.h>
#include <linux/sched.h>
#include <linux/wait.h>
#include <linux/mutex.h>
#include <linux/types.h>
#include <linux/slab.h>
#include <asm/page.h>
#include <asm/ioctl.h>
#include "crash-kmod.h"

/*
 * Global device data for CRASH driver
 * Used to hold physical address of control / status registers, their MUTEX,
 * interrupts, and timeout
 */
struct crash_dev_drvdata {
  struct platform_device  *pdev;
  struct miscdevice       mdev;
  uint32_t volatile       *regs;            // Pointer (kernel virtual space) to Control / Status registers
  uint32_t                regs_phys_addr;   // Control / Status registers
  size_t                  regs_len;         // Control / Status registers length
  struct mutex            s2mm_mutex;       // Mutex for S2MM DMAs (i.e. reads)
  struct mutex            mm2s_mutex;       // Mutex for M2SS DMAs (i.e. writes)
  unsigned int            irq;              // IRQ
  atomic_t                irq_s2mm;         // Unacknowledged IRQ
  atomic_t                irq_mm2s;         // Unacknowledged IRQ
  wait_queue_head_t       irq_s2mm_wait;    // Wait queue for interrupts
  wait_queue_head_t       irq_mm2s_wait;    // Wait queue for interrupts
};

/*
 * Local data for each file descriptor
 * Used to hold information about DMA buffer
 */
struct crash_private_data {
  struct crash_dev_drvdata  *d;                   // Pointer to device data
  struct page               *dma_buff;            // DMA buffer
  uint32_t                  dma_phys_addr;        // Physical address of DMA buffer
  size_t                    dma_buff_len;         // Length of DMA buffer
};

static const struct of_device_id crash_of_ids[] = {
  { .compatible = "crash" },
  { }
};

static int crash_open(struct inode *i, struct file *filp)
{
  struct crash_dev_drvdata *d = container_of(filp->private_data, struct crash_dev_drvdata, mdev);
  struct crash_private_data *pd = kmalloc(sizeof(struct crash_private_data), GFP_KERNEL);

  // Set reference to global device data
  pd->d = d;

  if (pd == 0)
  {
    dev_err(&d->pdev->dev, "crash_open(): Error allocating crash_private_data\n");
    return -ENOMEM;
  }

  pd->dma_buff = alloc_pages(GFP_KERNEL, PAGE_ORDER);
  if (!pd->dma_buff) {
    kfree(pd);
    dev_err(&d->pdev->dev, "crash_open(): Error allocating DMA buffer\n");
    return -ENOMEM;
  }
  pd->dma_phys_addr = (uint32_t)page_to_phys(pd->dma_buff);
  pd->dma_buff_len = (1 << PAGE_ORDER) * PAGE_SIZE;
  dev_info(&d->pdev->dev, "crash_open(): Allocated DMA buffer\n");

  // Save to private data to keep track of DMA buffer
  filp->private_data = pd;

  return 0;
}

static int crash_close(struct inode *i, struct file *filp)
{
  struct crash_private_data *pd = filp->private_data;

  // For safeties sake, stop all transfers
  crash_clear_bit(pd->d->regs, DMA_MM2S_XFER_EN);
  crash_clear_bit(pd->d->regs, DMA_S2MM_XFER_EN);

  __free_pages(pd->dma_buff, PAGE_ORDER);
  kfree(pd);
  dev_info(&pd->d->pdev->dev, "crash_close(): Freed DMA buffer\n");
  return 0;
}

static int crash_mmap(struct file *filp, struct vm_area_struct *vma)
{
  struct crash_private_data *pd = filp->private_data;
  unsigned long mmap_type = vma->vm_pgoff << PAGE_SHIFT;

  if (mmap_type == MMAP_REGS) {
    if (vma)
    vma->vm_page_prot = pgprot_noncached(vma->vm_page_prot);
    if (remap_pfn_range(vma, vma->vm_start, pd->d->regs_phys_addr >> PAGE_SHIFT, pd->d->regs_len, vma->vm_page_prot)) {
      dev_err(&pd->d->pdev->dev, "crash_mmap(): Failed to mmap control registers\n");
      return -EIO;
    }
    dev_info(&pd->d->pdev->dev, "crash_mmap(): Memory Mapped control registers\n");
    return 0;
  } else if (mmap_type == MMAP_DMA_BUFF) {
    if (remap_pfn_range(vma, vma->vm_start, page_to_pfn(pd->dma_buff), pd->dma_buff_len, vma->vm_page_prot)) {
      dev_err(&pd->d->pdev->dev, "crash_mmap(): Failed to mmap DMA buffer\n");
      return -EIO;
    }
    dev_info(&pd->d->pdev->dev, "crash_mmap(): Memory Mapped DMA buffer\n");
    return 0;
  }
  dev_err(&pd->d->pdev->dev, "crash_mmap(): Invalid offset\n");
  return -EINVAL;
}

static long crash_ioctl(struct file *filp, unsigned int cmd, unsigned long arg) {

  struct crash_private_data *pd = filp->private_data;
  volatile uint32_t *regs = pd->d->regs;
  unsigned int i = 0;
  unsigned int j = 0;
  uint32_t buff;

  switch (cmd) {
    case CRASH_RESET:
      // Grab mutexes so we do not reset in the middle of a DMA
      if (mutex_lock_interruptible(&pd->d->mm2s_mutex)) return -EINTR;
      if (mutex_lock_interruptible(&pd->d->s2mm_mutex)) {
        mutex_unlock(&pd->d->mm2s_mutex);
        return -EINTR;
      }
      crash_set_bit(regs, GLOBAL_RESET);
      crash_clear_bit(regs, GLOBAL_RESET);
      // Set CACHE bits that affects whether AXI ACP transfers are cached or not.
      // This should not be changed unless you know what you are doing.
      crash_write_reg(regs, GLOBAL_M_AXI_AWPROT,  0x00);      //  AWPROT: "000"
      crash_write_reg(regs, GLOBAL_M_AXI_AWCACHE, 0x0F);      // AWCACHE: "1111"
      crash_write_reg(regs, GLOBAL_M_AXI_AWUSER,  0x1F);      //  AWUSER: "11111"
      crash_write_reg(regs, GLOBAL_M_AXI_ARPROT,  0x00);      //  ARPROT: "000"
      crash_write_reg(regs, GLOBAL_M_AXI_ARCACHE, 0x0F);      // ARCACHE: "1111"
      crash_write_reg(regs, GLOBAL_M_AXI_ARUSER,  0x1F);      //  ARUSER: "11111"
      mutex_unlock(&pd->d->mm2s_mutex);
      mutex_unlock(&pd->d->s2mm_mutex);
      break;

    case CRASH_SET_INTERRUPTS:
      // Grab mutexes so we do not change interrupt configuration in the middle of a DMA
      if (mutex_lock_interruptible(&pd->d->mm2s_mutex)) return -EINTR;
      if (mutex_lock_interruptible(&pd->d->s2mm_mutex)) {
        mutex_unlock(&pd->d->mm2s_mutex);
        return -EINTR;
      }
      crash_write_reg(regs, DMA_BANK1, arg);
      mutex_unlock(&pd->d->mm2s_mutex);
      mutex_unlock(&pd->d->s2mm_mutex);
      break;

    case CRASH_GET_INTERRUPTS:
      buff = crash_read_reg(regs, DMA_BANK0);
      if(copy_to_user((uint32_t *)arg,&buff,sizeof(uint32_t))) return -EFAULT;
      break;

    case CRASH_GET_DMA_PHYS_ADDR:
      if(copy_to_user((uint32_t *)arg,&pd->dma_phys_addr,sizeof(uint32_t))) return -EFAULT;
      break;

    case CRASH_DMA_WRITE:
      if (mutex_lock_interruptible(&pd->d->mm2s_mutex)) return -EINTR;
      crash_write_reg(regs, DMA_MM2S_CMD_ADDR, pd->dma_phys_addr);
      crash_write_reg(regs, DMA_MM2S_CMD_DATA, arg);
      crash_set_bit(regs, DMA_MM2S_XFER_EN);
      // Check if transfer interrupt is enabled.
      if (crash_get_bit(regs, DMA_MM2S_INTERRUPT)) {
        if (wait_event_interruptible_timeout(pd->d->irq_mm2s_wait, atomic_read(&pd->d->irq_mm2s) > 0, msecs_to_jiffies(INTERRUPT_TIMEOUT_MSEC)) == 0) {
          dev_err(&pd->d->pdev->dev, "crash_ioctl(): DMA timeout (Interrupt)\n");
          crash_clear_bit(regs, DMA_MM2S_XFER_EN);
          mutex_unlock(&pd->d->mm2s_mutex);
          return -ETIMEDOUT;
        }
        atomic_dec(&pd->d->irq_mm2s);
      } else {
        // Poll until DMA is complete or we timeout.
        // TODO: Use something less hackish than a counter for the timeout code
        i = 0;
        while(crash_get_bit(regs, DMA_MM2S_STS_FIFO_EMPTY)) {
          if (i > 1000000) {
            dev_err(&pd->d->pdev->dev, "crash_ioctl(): DMA timeout (Polling)\n");
            crash_clear_bit(regs, DMA_MM2S_XFER_EN);
            mutex_unlock(&pd->d->mm2s_mutex);
            return -ETIMEDOUT;
          }
          i++;
        }
      }
      // Dummy read on status register, which causes it to read the FIFO (and therefore return the FIFO to the empty state)
      i = crash_read_reg(regs, DMA_MM2S_STS_FIFO);
      crash_clear_bit(regs, DMA_MM2S_XFER_EN);
      mutex_unlock(&pd->d->mm2s_mutex);
      break;

    case CRASH_DMA_READ:
      if (mutex_lock_interruptible(&pd->d->s2mm_mutex)) return -EINTR;
      crash_write_reg(regs, DMA_S2MM_CMD_ADDR, pd->dma_phys_addr);
      crash_write_reg(regs, DMA_S2MM_CMD_DATA, arg);
      crash_set_bit(regs, DMA_S2MM_XFER_EN);
      // Check if transfer interrupt is enabled.
      if (crash_get_bit(regs, DMA_S2MM_INTERRUPT)) {
        if (wait_event_interruptible_timeout(pd->d->irq_s2mm_wait, atomic_read(&pd->d->irq_s2mm) > 0, msecs_to_jiffies(INTERRUPT_TIMEOUT_MSEC)) == 0) {
          dev_err(&pd->d->pdev->dev, "crash_ioctl(): DMA timeout (Interrupt)\n");
          crash_clear_bit(regs, DMA_S2MM_XFER_EN);
          mutex_unlock(&pd->d->s2mm_mutex);
          return -ETIMEDOUT;
        }
        atomic_dec(&pd->d->irq_s2mm);
      } else {
        // Poll until DMA is complete or we timeout.
        // TODO: Use something less hackish than a counter for the timeout code
        j = 0;
        while(crash_get_bit(regs, DMA_S2MM_STS_FIFO_EMPTY)) {
          if (j > 1000000) {
            dev_err(&pd->d->pdev->dev, "crash_ioctl(): DMA timeout (Polling)\n");
            crash_clear_bit(regs, DMA_S2MM_XFER_EN);
            mutex_unlock(&pd->d->s2mm_mutex);
            return -ETIMEDOUT;
          }
          j++;
        }
      }
      // Dummy read on DMA status register, which causes it to read the FIFO (and therefore return the FIFO to the empty state)
      j = crash_read_reg(regs, DMA_S2MM_STS_FIFO);
      crash_clear_bit(regs, DMA_S2MM_XFER_EN);
      mutex_unlock(&pd->d->s2mm_mutex);
      break;

    default:
      return -EFAULT;
  }
  return 0;
}

static irqreturn_t crash_irq_handler(int irq, void *pdata)
{
  struct crash_dev_drvdata *d = pdata;
  volatile uint32_t *regs = d->regs;

  // Check if we received a IRQ due to a DMA transfer
  if (mutex_is_locked(&d->s2mm_mutex) && !crash_get_bit(regs, DMA_S2MM_STS_FIFO_EMPTY)) {
    atomic_inc(&d->irq_s2mm);
    wake_up_interruptible(&d->irq_s2mm_wait);
  } else if (mutex_is_locked(&d->mm2s_mutex) && !crash_get_bit(regs, DMA_MM2S_STS_FIFO_EMPTY)) {
    atomic_inc(&d->irq_mm2s);
    wake_up_interruptible(&d->irq_mm2s_wait);
  } else {
    dev_err(&d->pdev->dev, "crash_irq_handler(): Received errant interrupt\n");
  }
  return IRQ_HANDLED;
}

static struct file_operations fops = {
  .owner = THIS_MODULE,
  .open = crash_open,
  .release = crash_close,
  .mmap = crash_mmap,
  .unlocked_ioctl = crash_ioctl,
};

static int crash_probe(struct platform_device *pdev)
{
  struct crash_dev_drvdata *d;
  struct resource *regs, *irq;
  int result;

  d = devm_kzalloc(&pdev->dev, sizeof(struct crash_dev_drvdata), GFP_KERNEL);
  if (!d) {
    dev_err(&pdev->dev, "crash_probe(): Error allocating memory for crash_dev_data\n");
    return -ENOMEM;
  }
  d->pdev = pdev;
  dev_set_drvdata(&pdev->dev, d);

  regs = platform_get_resource(pdev, IORESOURCE_MEM, 0);
  if (!regs) {
    dev_err(&pdev->dev, "crash_probe(): Error getting regs resource from devicetree\n");
    return -EIO;
  }

  d->regs = (uint32_t*)devm_ioremap_resource(&pdev->dev, regs);
  if (d->regs < 0) {
    dev_err(&pdev->dev, "crash_probe(): Error mapping control & status registers\n");
    return -EIO;
  }

  irq = platform_get_resource(pdev, IORESOURCE_IRQ, 0);
  if (!irq) {
    dev_err(&pdev->dev, "crash_probe(): Error getting IRQ resource from devicetree\n");
    return -EIO;
  }
  d->irq = irq->start;
  init_waitqueue_head(&d->irq_s2mm_wait);
  init_waitqueue_head(&d->irq_mm2s_wait);

  // Setup control registers
  d->regs_phys_addr = (uint32_t)regs->start;
  d->regs_len = resource_size(regs);

  // Initialize misc device
  d->mdev.name = "crash";
  d->mdev.fops = &fops,
  d->mdev.minor = MISC_DYNAMIC_MINOR;

  result = misc_register(&d->mdev);
  if (result) {
    dev_err(&pdev->dev, "crash_probe(): Failed to register misc device\n");
  }

  // Setup interrupt handler
  if (devm_request_irq(&d->pdev->dev, d->irq, crash_irq_handler, 0, "crash", d)) {
    dev_err(&d->pdev->dev, "crash_probe(): Could not request IRQ %d\n", d->irq);
    return -EBUSY;
  } else {
    atomic_set(&d->irq_s2mm, 0);
    atomic_set(&d->irq_mm2s, 0);
  }

  // Setup mutexs for access to DMA transfers
  mutex_init(&d->s2mm_mutex);
  mutex_init(&d->mm2s_mutex);

  dev_info(&d->pdev->dev, "crash_probe(): Probe complete\n");
  return 0;
}

static int crash_remove(struct platform_device *pdev)
{
  struct crash_dev_drvdata *d;
  d = dev_get_drvdata(&pdev->dev);

  devm_free_irq(&d->pdev->dev, d->irq, d);

  misc_deregister(&d->mdev);

  devm_kfree(&pdev->dev, d);

  return 0;
}

static struct platform_driver crash_driver = {
  .driver = {
    .name = "crash",
    .of_match_table = crash_of_ids,
    .owner = THIS_MODULE,
  },
  .probe = crash_probe,
  .remove = crash_remove,
};

static int __init crash_init(void)
{
  printk(KERN_INFO "%s crash_init(): Registering module\n",MODULE_NAME);
  return platform_driver_register(&crash_driver);
}

static void __exit crash_exit(void)
{
  printk(KERN_INFO "%s crash_exit(): Cleaning up module\n", MODULE_NAME);
  platform_driver_unregister(&crash_driver);
}

module_init(crash_init);
module_exit(crash_exit);
MODULE_AUTHOR("Jonathon Pendlum");
MODULE_DESCRIPTION("Allows userland access to FPGA processing blocks in CRASH framework");
MODULE_LICENSE("GPL");