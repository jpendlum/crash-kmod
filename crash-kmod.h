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
**  File:         crash-kmod.h
**  Author(s):    Jonathon Pendlum (jon.pendlum@gmail.com)
**  Description:  Register mappings and other useful information for use
**                with CRASH library.
**
******************************************************************************/
#ifndef CRASH_KMOD_H
#define CRASH_KMOD_H

#ifdef __cplusplus
extern "C" {
#endif

#define MODULE_NAME                   "crash"
#define PAGE_ORDER                    8
#define INTERRUPT_TIMEOUT_MSEC        1000
#define MMAP_REGS                     0x1000
#define MMAP_DMA_BUFF                 0x2000
#define REGS_ADDR_SIZE                256
#define REGS_TOTAL_ADDR_SPACE         0x20000
#define RX_PHASE_CAL                  460
#define TX_PHASE_CAL                  467

// IDs
#define DMA_PLBLOCK_ID                0
#define USRP_INTF_PLBLOCK_ID          1
#define SPEC_SENSE_PLBLOCK_ID         2
#define GLOBAL_PLBLOCK_ID             127
// Control / Status Register
#define DMA_BASE                      REGS_ADDR_SIZE*DMA_PLBLOCK_ID
#define USRP_INTF_BASE                REGS_ADDR_SIZE*USRP_INTF_PLBLOCK_ID
#define SPEC_SENSE_BASE               REGS_ADDR_SIZE*SPEC_SENSE_PLBLOCK_ID
#define GLOBAL_BASE                   REGS_ADDR_SIZE*GLOBAL_PLBLOCK_ID

// Control / Status Registers
// For use with the crash_write_reg / crash_read_reg / etc macros.
//
// Example Register FOO:
// FOO_BASE     - Base address of the register
// FOO_OFFSET   - Offset from LSB of the register
// FOO_N        - Bitwidth of the register
//
// Register read:  crash_read_reg(reg,FOO)
// Notice that the macro uses the register name without suffixes like _BASE or _OFFSET

// -- DMA
#define DMA_BANK0_BASE                    0 + DMA_BASE
#define DMA_BANK0_OFFSET                  0
#define DMA_BANK0_N                       32
#define DMA_MM2S_XFER_EN_BASE             DMA_BANK0_BASE
#define DMA_MM2S_XFER_EN_OFFSET           0
#define DMA_MM2S_XFER_EN_N                1
#define DMA_S2MM_XFER_EN_BASE             DMA_BANK0_BASE
#define DMA_S2MM_XFER_EN_OFFSET           1
#define DMA_S2MM_XFER_EN_N                1
#define DMA_RESET_MM2S_CMD_FIFO_BASE      DMA_BANK0_BASE
#define DMA_RESET_MM2S_CMD_FIFO_OFFSET    2
#define DMA_RESET_MM2S_CMD_FIFO_N         1
#define DMA_RESET_S2MM_CMD_FIFO_BASE      DMA_BANK0_BASE
#define DMA_RESET_S2MM_CMD_FIFO_OFFSET    3
#define DMA_RESET_S2MM_CMD_FIFO_N         1
#define DMA_MM2S_CMD_FIFO_LOOP_BASE       DMA_BANK0_BASE
#define DMA_MM2S_CMD_FIFO_LOOP_OFFSET     4
#define DMA_MM2S_CMD_FIFO_LOOP_N          8
#define DMA_S2MM_CMD_FIFO_LOOP_BASE       DMA_BANK0_BASE
#define DMA_S2MM_CMD_FIFO_LOOP_OFFSET     12
#define DMA_S2MM_CMD_FIFO_LOOP_N          8
#define DMA_STS_FIFO_AUTO_READ_BASE       DMA_BANK0_BASE
#define DMA_STS_FIFO_AUTO_READ_OFFSET     20
#define DMA_STS_FIFO_AUTO_READ_N          1
#define DMA_RESET_STS_FIFO_BASE           DMA_BANK0_BASE
#define DMA_RESET_STS_FIFO_OFFSET         21
#define DMA_RESET_STS_FIFO_N              1
#define DMA_CLEAR_MM2S_XFER_CNT_BASE      DMA_BANK0_BASE
#define DMA_CLEAR_MM2S_XFER_CNT_OFFSET    22
#define DMA_CLEAR_MM2S_XFER_CNT_N         1
#define DMA_CLEAR_S2MM_XFER_CNT_BASE      DMA_BANK0_BASE
#define DMA_CLEAR_S2MM_XFER_CNT_OFFSET    23
#define DMA_CLEAR_S2MM_XFER_CNT_N         1
#define DMA_MM2S_XFER_IN_PROGRESS_BASE    DMA_BANK0_BASE
#define DMA_MM2S_XFER_IN_PROGRESS_OFFSET  24
#define DMA_MM2S_XFER_IN_PROGRESS_N       1
#define DMA_S2MM_XFER_IN_PROGRESS_BASE    DMA_BANK0_BASE
#define DMA_S2MM_XFER_IN_PROGRESS_OFFSET  25
#define DMA_S2MM_XFER_IN_PROGRESS_N       1

#define DMA_BANK1_BASE                    1 + DMA_BASE
#define DMA_BANK1_OFFSET                  0
#define DMA_BANK1_N                       32
#define DMA_S2MM_INTERRUPT_BASE           DMA_BANK1_BASE
#define DMA_S2MM_INTERRUPT_OFFSET         0
#define DMA_S2MM_INTERRUPT_N              1
#define DMA_MM2S_INTERRUPT_BASE           DMA_BANK1_BASE
#define DMA_MM2S_INTERRUPT_OFFSET         1
#define DMA_MM2S_INTERRUPT_N              1

#define DMA_BANK2_BASE                    2 + DMA_BASE
#define DMA_BANK2_OFFSET                  0
#define DMA_BANK2_N                       32
#define DMA_MM2S_CMD_ADDR_BASE            DMA_BANK2_BASE
#define DMA_MM2S_CMD_ADDR_OFFSET          DMA_BANK2_OFFSET
#define DMA_MM2S_CMD_ADDR_N               DMA_BANK2_N

#define DMA_BANK3_BASE                    3 + DMA_BASE
#define DMA_BANK3_OFFSET                  0
#define DMA_BANK3_N                       32
#define DMA_MM2S_CMD_DATA_BASE            DMA_BANK3_BASE
#define DMA_MM2S_CMD_DATA_OFFSET          DMA_BANK3_OFFSET
#define DMA_MM2S_CMD_DATA_N               DMA_BANK3_N
#define DMA_MM2S_CMD_SIZE_BASE            DMA_BANK3_BASE
#define DMA_MM2S_CMD_SIZE_OFFSET          0
#define DMA_MM2S_CMD_SIZE_N               23
#define DMA_MM2S_CMD_TDEST_BASE           DMA_BANK3_BASE
#define DMA_MM2S_CMD_TDEST_OFFSET         23
#define DMA_MM2S_CMD_TDEST_N              4
#define DMA_MM2S_CMD_EN_BASE              DMA_BANK3_BASE
#define DMA_MM2S_CMD_EN_OFFSET            31
#define DMA_MM2S_CMD_EN_N                 1

#define DMA_BANK4_BASE                    4 + DMA_BASE
#define DMA_BANK4_OFFSET                  0
#define DMA_BANK4_N                       32
#define DMA_S2MM_CMD_ADDR_BASE            DMA_BANK4_BASE
#define DMA_S2MM_CMD_ADDR_OFFSET          DMA_BANK4_OFFSET
#define DMA_S2MM_CMD_ADDR_N               DMA_BANK4_N

#define DMA_BANK5_BASE                    5 + DMA_BASE
#define DMA_BANK5_OFFSET                  0
#define DMA_BANK5_N                       32
#define DMA_S2MM_CMD_DATA_BASE            DMA_BANK5_BASE
#define DMA_S2MM_CMD_DATA_OFFSET          DMA_BANK5_OFFSET
#define DMA_S2MM_CMD_DATA_N               DMA_BANK5_N
#define DMA_S2MM_CMD_SIZE_BASE            DMA_BANK5_BASE
#define DMA_S2MM_CMD_SIZE_OFFSET          0
#define DMA_S2MM_CMD_SIZE_N               23
#define DMA_S2MM_CMD_TDEST_BASE           DMA_BANK5_BASE
#define DMA_S2MM_CMD_TDEST_OFFSET         23
#define DMA_S2MM_CMD_TDEST_N              4
#define DMA_S2MM_CMD_EN_BASE              DMA_BANK5_BASE
#define DMA_S2MM_CMD_EN_OFFSET            31
#define DMA_S2MM_CMD_EN_N                 1

#define DMA_BANK6_BASE                    6 + DMA_BASE
#define DMA_BANK6_OFFSET                  0
#define DMA_BANK6_N                       32
#define DMA_MM2S_STS_FIFO_BASE            DMA_BANK6_BASE
#define DMA_MM2S_STS_FIFO_OFFSET          DMA_BANK6_OFFSET
#define DMA_MM2S_STS_FIFO_N               DMA_BANK6_N

#define DMA_BANK7_BASE                    7 + DMA_BASE
#define DMA_BANK7_OFFSET                  0
#define DMA_BANK7_N                       32
#define DMA_S2MM_STS_FIFO_BASE            DMA_BANK7_BASE
#define DMA_S2MM_STS_FIFO_OFFSET          DMA_BANK7_OFFSET
#define DMA_S2MM_STS_FIFO_N               DMA_BANK7_N

#define DMA_BANK8_BASE                    8 + DMA_BASE
#define DMA_BANK8_OFFSET                  0
#define DMA_BANK8_N                       32
#define DMA_MM2S_STS_FIFO_EMPTY_BASE      DMA_BANK8_BASE
#define DMA_MM2S_STS_FIFO_EMPTY_OFFSET    0
#define DMA_MM2S_STS_FIFO_EMPTY_N         1
#define DMA_S2MM_STS_FIFO_EMPTY_BASE      DMA_BANK8_BASE
#define DMA_S2MM_STS_FIFO_EMPTY_OFFSET    2
#define DMA_S2MM_STS_FIFO_EMPTY_N         1

#define DMA_BANK9_BASE                    9 + DMA_BASE
#define DMA_BANK9_OFFSET                  0
#define DMA_BANK9_N                       32
#define DMA_MM2S_CMD_FIFO_EMPTY_BASE      DMA_BANK9_BASE
#define DMA_MM2S_CMD_FIFO_EMPTY_OFFSET    0
#define DMA_MM2S_CMD_FIFO_EMPTY_N         1
#define DMA_S2MM_CMD_FIFO_EMPTY_BASE      DMA_BANK9_BASE
#define DMA_S2MM_CMD_FIFO_EMPTY_OFFSET    2
#define DMA_S2MM_CMD_FIFO_EMPTY_N         1

#define DMA_BANK10_BASE                   10 + DMA_BASE
#define DMA_BANK10_OFFSET                 0
#define DMA_BANK10_N                      32
#define DMA_MM2S_XFER_CNT_BASE            DMA_BANK10_BASE
#define DMA_MM2S_XFER_CNT_OFFSET          0
#define DMA_MM2S_XFER_CNT_N               16

#define DMA_BANK11_BASE                   11 + DMA_BASE
#define DMA_BANK11_OFFSET                 0
#define DMA_BANK11_N                      32
#define DMA_S2MM_XFER_CNT_BASE            DMA_BANK11_BASE
#define DMA_S2MM_XFER_CNT_OFFSET          16
#define DMA_S2MM_XFER_CNT_N               16

#define DMA_BANK12_BASE                   12 + DMA_BASE
#define DMA_BANK12_OFFSET                 0
#define DMA_BANK12_N                      32
#define DMA_CHECKWORD_BASE                DMA_BANK12_BASE
#define DMA_CHECKWORD_OFFSET              0
#define DMA_CHECKWORD_N                   32

#define DMA_BANK13_BASE                   13 + DMA_BASE
#define DMA_BANK13_OFFSET                 0
#define DMA_BANK13_N                      32
#define DMA_DEBUG_CNT_BASE                DMA_BANK13_BASE
#define DMA_DEBUG_CNT_OFFSET              0
#define DMA_DEBUG_CNT_N                   32

// -- USRP Interface
#define USRP_BANK0_BASE                   0 + USRP_INTF_BASE
#define USRP_BANK0_OFFSET                 0
#define USRP_BANK0_N                      32
#define USRP_RX_ENABLE_BASE               USRP_BANK0_BASE
#define USRP_RX_ENABLE_OFFSET             0
#define USRP_RX_ENABLE_N                  1
#define USRP_TX_ENABLE_BASE               USRP_BANK0_BASE
#define USRP_TX_ENABLE_OFFSET             1
#define USRP_TX_ENABLE_N                  1
#define USRP_RX_ENABLE_SIDEBAND_BASE      USRP_BANK0_BASE
#define USRP_RX_ENABLE_SIDEBAND_OFFSET    2
#define USRP_RX_ENABLE_SIDEBAND_N         1
#define USRP_TX_ENABLE_SIDEBAND_BASE      USRP_BANK0_BASE
#define USRP_TX_ENABLE_SIDEBAND_OFFSET    3
#define USRP_TX_ENABLE_SIDEBAND_N         1
#define USRP_RX_FIFO_RESET_BASE           USRP_BANK0_BASE
#define USRP_RX_FIFO_RESET_OFFSET         4
#define USRP_RX_FIFO_RESET_N              1
#define USRP_TX_FIFO_RESET_BASE           USRP_BANK0_BASE
#define USRP_TX_FIFO_RESET_OFFSET         5
#define USRP_TX_FIFO_RESET_N              1
#define USRP_RX_FIFO_BYPASS_BASE          USRP_BANK0_BASE
#define USRP_RX_FIFO_BYPASS_OFFSET        6
#define USRP_RX_FIFO_BYPASS_N             1
#define USRP_RX_FIFO_OVERFLOW_CLR_BASE    USRP_BANK0_BASE
#define USRP_RX_FIFO_OVERFLOW_CLR_OFFSET  7
#define USRP_RX_FIFO_OVERFLOW_CLR_N       1
#define USRP_TX_FIFO_UNDERFLOW_CLR_BASE   USRP_BANK0_BASE
#define USRP_TX_FIFO_UNDERFLOW_CLR_OFFSET 8
#define USRP_TX_FIFO_UNDERFLOW_CLR_N      1
#define USRP_AXIS_MASTER_TDEST_BASE       USRP_BANK0_BASE
#define USRP_AXIS_MASTER_TDEST_OFFSET     29
#define USRP_AXIS_MASTER_TDEST_N          3

#define USRP_BANK1_BASE                   1 + USRP_INTF_BASE
#define USRP_BANK1_OFFSET                 0
#define USRP_BANK1_N                      32
#define USRP_USRP_MODE_CTRL_BASE          USRP_BANK1_BASE
#define USRP_USRP_MODE_CTRL_OFFSET        0
#define USRP_USRP_MODE_CTRL_N             8

#define USRP_BANK2_BASE                   2 + USRP_INTF_BASE
#define USRP_BANK2_OFFSET                 0
#define USRP_BANK2_N                      32
#define USRP_RX_PACKET_SIZE_BASE          USRP_BANK2_BASE
#define USRP_RX_PACKET_SIZE_OFFSET        0
#define USRP_RX_PACKET_SIZE_N             24
#define USRP_RX_FIX2FLOAT_BYPASS_BASE     USRP_BANK2_BASE
#define USRP_RX_FIX2FLOAT_BYPASS_OFFSET   24
#define USRP_RX_FIX2FLOAT_BYPASS_N        1
#define USRP_RX_CIC_BYPASS_BASE           USRP_BANK2_BASE
#define USRP_RX_CIC_BYPASS_OFFSET         25
#define USRP_RX_CIC_BYPASS_N              1
#define USRP_RX_HB_BYPASS_BASE            USRP_BANK2_BASE
#define USRP_RX_HB_BYPASS_OFFSET          26
#define USRP_RX_HB_BYPASS_N               1
#define USRP_TX_FIX2FLOAT_BYPASS_BASE     USRP_BANK2_BASE
#define USRP_TX_FIX2FLOAT_BYPASS_OFFSET   27
#define USRP_TX_FIX2FLOAT_BYPASS_N        1
#define USRP_TX_CIC_BYPASS_BASE           USRP_BANK2_BASE
#define USRP_TX_CIC_BYPASS_OFFSET         28
#define USRP_TX_CIC_BYPASS_N              1
#define USRP_TX_HB_BYPASS_BASE            USRP_BANK2_BASE
#define USRP_TX_HB_BYPASS_OFFSET          29
#define USRP_TX_HB_BYPASS_N               1

#define USRP_BANK3_BASE                   3 + USRP_INTF_BASE
#define USRP_BANK3_OFFSET                 0
#define USRP_BANK3_N                      32
#define USRP_RX_CIC_DECIM_BASE            USRP_BANK3_BASE
#define USRP_RX_CIC_DECIM_OFFSET          0
#define USRP_RX_CIC_DECIM_N               11
#define USRP_TX_CIC_INTERP_BASE           USRP_BANK3_BASE
#define USRP_TX_CIC_INTERP_OFFSET         16
#define USRP_TX_CIC_INTERP_N              11

#define USRP_BANK4_BASE                   4 + USRP_INTF_BASE
#define USRP_BANK4_OFFSET                 0
#define USRP_BANK4_N                      32
#define USRP_RX_GAIN_BASE                 USRP_BANK4_BASE
#define USRP_RX_GAIN_OFFSET               0
#define USRP_RX_GAIN_N                    32

#define USRP_BANK5_BASE                   5 + USRP_INTF_BASE
#define USRP_BANK5_OFFSET                 0
#define USRP_BANK5_N                      32
#define USRP_TX_GAIN_BASE                 USRP_BANK5_BASE
#define USRP_TX_GAIN_OFFSET               0
#define USRP_TX_GAIN_N                    32

#define USRP_BANK6_BASE                   6 + USRP_INTF_BASE
#define USRP_BANK6_OFFSET                 0
#define USRP_BANK6_N                      32
#define USRP_RX_RESET_CAL_BASE            USRP_BANK6_BASE
#define USRP_RX_RESET_CAL_OFFSET          0
#define USRP_RX_RESET_CAL_N               1
#define USRP_RX_PHASE_INIT_BASE           USRP_BANK6_BASE
#define USRP_RX_PHASE_INIT_OFFSET         1
#define USRP_RX_PHASE_INIT_N              10
#define USRP_TX_RESET_CAL_BASE            USRP_BANK6_BASE
#define USRP_TX_RESET_CAL_OFFSET          16
#define USRP_TX_RESET_CAL_N               1
#define USRP_TX_PHASE_INIT_BASE           USRP_BANK6_BASE
#define USRP_TX_PHASE_INIT_OFFSET         17
#define USRP_TX_PHASE_INIT_N              10
#define USRP_RX_PHASE_EN_BASE             USRP_BANK6_BASE
#define USRP_RX_PHASE_EN_OFFSET           28
#define USRP_RX_PHASE_EN_N                1
#define USRP_RX_PHASE_INCDEC_BASE         USRP_BANK6_BASE
#define USRP_RX_PHASE_INCDEC_OFFSET       29
#define USRP_RX_PHASE_INCDEC_N            1
#define USRP_TX_PHASE_EN_BASE             USRP_BANK6_BASE
#define USRP_TX_PHASE_EN_OFFSET           30
#define USRP_TX_PHASE_EN_N                1
#define USRP_TX_PHASE_INCDEC_BASE         USRP_BANK6_BASE
#define USRP_TX_PHASE_INCDEC_OFFSET       31
#define USRP_TX_PHASE_INCDEC_N            1

#define USRP_BANK7_BASE                   7 + USRP_INTF_BASE
#define USRP_BANK7_OFFSET                 0
#define USRP_BANK7_N                      32
#define USRP_CLOCK_LOCKED_BASE            USRP_BANK7_BASE
#define USRP_CLOCK_LOCKED_OFFSET          0
#define USRP_CLOCK_LOCKED_N               1
#define USRP_RX_FIFO_OVERFLOW_BASE        USRP_BANK7_BASE
#define USRP_RX_FIFO_OVERFLOW_OFFSET      1
#define USRP_RX_FIFO_OVERFLOW_N           1
#define USRP_TX_FIFO_UNDERFLOW_BASE       USRP_BANK7_BASE
#define USRP_TX_FIFO_UNDERFLOW_OFFSET     2
#define USRP_TX_FIFO_UNDERFLOW_N          1
#define USRP_RX_CAL_COMPLETE_BASE         USRP_BANK7_BASE
#define USRP_RX_CAL_COMPLETE_OFFSET       3
#define USRP_RX_CAL_COMPLETE_N            1
#define USRP_TX_CAL_COMPLETE_BASE         USRP_BANK7_BASE
#define USRP_TX_CAL_COMPLETE_OFFSET       4
#define USRP_TX_CAL_COMPLETE_N            1
#define USRP_RX_PHASE_BUSY_BASE           USRP_BANK7_BASE
#define USRP_RX_PHASE_BUSY_OFFSET         5
#define USRP_RX_PHASE_BUSY_N              1
#define USRP_TX_PHASE_BUSY_BASE           USRP_BANK7_BASE
#define USRP_TX_PHASE_BUSY_OFFSET         6
#define USRP_TX_PHASE_BUSY_N              1
#define USRP_UART_BUSY_BASE               USRP_BANK7_BASE
#define USRP_UART_BUSY_OFFSET             7
#define USRP_UART_BUSY_N                  1
#define USRP_CLK_RX_PHASE_BASE            USRP_BANK7_BASE
#define USRP_CLK_RX_PHASE_OFFSET          10
#define USRP_CLK_RX_PHASE_N               10
#define USRP_CLK_TX_PHASE_BASE            USRP_BANK7_BASE
#define USRP_CLK_TX_PHASE_OFFSET          20
#define USRP_CLK_TX_PHASE_N               10

// -- Spectrum Sense
#define SPEC_SENSE_BANK0_BASE                         0 + SPEC_SENSE_BASE
#define SPEC_SENSE_BANK0_OFFSET                       0
#define SPEC_SENSE_BANK0_N                            32
#define SPEC_SENSE_ENABLE_FFT_BASE                    SPEC_SENSE_BANK0_BASE
#define SPEC_SENSE_ENABLE_FFT_OFFSET                  0
#define SPEC_SENSE_ENABLE_FFT_N                       1
#define SPEC_SENSE_AXIS_MASTER_TDEST_BASE             SPEC_SENSE_BANK0_BASE
#define SPEC_SENSE_AXIS_MASTER_TDEST_OFFSET           29
#define SPEC_SENSE_AXIS_MASTER_TDEST_N                3

#define SPEC_SENSE_BANK1_BASE                         1 + SPEC_SENSE_BASE
#define SPEC_SENSE_BANK1_OFFSET                       0
#define SPEC_SENSE_BANK1_N                            32
#define SPEC_SENSE_AXIS_CONFIG_TDATA_BASE             SPEC_SENSE_BANK1_BASE
#define SPEC_SENSE_AXIS_CONFIG_TDATA_OFFSET           0
#define SPEC_SENSE_AXIS_CONFIG_TDATA_N                5
#define SPEC_SENSE_AXIS_CONFIG_TVALID_BASE            SPEC_SENSE_BANK1_BASE
#define SPEC_SENSE_AXIS_CONFIG_TVALID_OFFSET          5
#define SPEC_SENSE_AXIS_CONFIG_TVALID_N               1
#define SPEC_SENSE_OUTPUT_MODE_BASE                   SPEC_SENSE_BANK1_BASE
#define SPEC_SENSE_OUTPUT_MODE_OFFSET                 8
#define SPEC_SENSE_OUTPUT_MODE_N                      2
#define SPEC_SENSE_ENABLE_THRESHOLD_IRQ_BASE          SPEC_SENSE_BANK1_BASE
#define SPEC_SENSE_ENABLE_THRESHOLD_IRQ_OFFSET        10
#define SPEC_SENSE_ENABLE_THRESHOLD_IRQ_N             1
#define SPEC_SENSE_ENABLE_THRESH_SIDEBAND_BASE        SPEC_SENSE_BANK1_BASE
#define SPEC_SENSE_ENABLE_THRESH_SIDEBAND_OFFSET      11
#define SPEC_SENSE_ENABLE_THRESH_SIDEBAND_N           1
#define SPEC_SENSE_ENABLE_NOT_THRESH_SIDEBAND_BASE    SPEC_SENSE_BANK1_BASE
#define SPEC_SENSE_ENABLE_NOT_THRESH_SIDEBAND_OFFSET  12
#define SPEC_SENSE_ENABLE_NOT_THRESH_SIDEBAND_N       1
#define SPEC_SENSE_CLEAR_THRESHOLD_LATCHED_BASE       SPEC_SENSE_BANK1_BASE
#define SPEC_SENSE_CLEAR_THRESHOLD_LATCHED_OFFSET     13
#define SPEC_SENSE_CLEAR_THRESHOLD_LATCHED_N          1

#define SPEC_SENSE_BANK2_BASE                         2 + SPEC_SENSE_BASE
#define SPEC_SENSE_BANK2_OFFSET                       0
#define SPEC_SENSE_BANK2_N                            32
#define SPEC_SENSE_THRESHOLD_BASE                     SPEC_SENSE_BANK2_BASE
#define SPEC_SENSE_THRESHOLD_OFFSET                   0
#define SPEC_SENSE_THRESHOLD_N                        32

#define SPEC_SENSE_BANK3_BASE                         3 + SPEC_SENSE_BASE
#define SPEC_SENSE_BANK3_OFFSET                       0
#define SPEC_SENSE_BANK3_N                            32
#define SPEC_SENSE_THRESHOLD_EXCEEDED_INDEX_BASE      SPEC_SENSE_BANK3_BASE
#define SPEC_SENSE_THRESHOLD_EXCEEDED_INDEX_OFFSET    0
#define SPEC_SENSE_THRESHOLD_EXCEEDED_INDEX_N         16
#define SPEC_SENSE_THRESHOLD_EXCEEDED_BASE            SPEC_SENSE_BANK3_BASE
#define SPEC_SENSE_THRESHOLD_EXCEEDED_OFFSET          31
#define SPEC_SENSE_THRESHOLD_EXCEEDED_N               1

#define SPEC_SENSE_BANK4_BASE                         4 + SPEC_SENSE_BASE
#define SPEC_SENSE_BANK4_OFFSET                       0
#define SPEC_SENSE_BANK4_N                            32
#define SPEC_SENSE_THRESHOLD_EXCEEDED_MAG_BASE        SPEC_SENSE_BANK4_BASE
#define SPEC_SENSE_THRESHOLD_EXCEEDED_MAG_OFFSET      0
#define SPEC_SENSE_THRESHOLD_EXCEEDED_MAG_N           32

// -- Global
#define GLOBAL_BANK0_BASE                 0 + GLOBAL_BASE
#define GLOBAL_BANK0_OFFSET               0
#define GLOBAL_BANK0_N                    32
#define GLOBAL_RESET_BASE                 GLOBAL_BANK0_BASE
#define GLOBAL_RESET_OFFSET               0
#define GLOBAL_RESET_N                    1

#define GLOBAL_BANK1_BASE                 1 + GLOBAL_BASE
#define GLOBAL_BANK1_OFFSET               0
#define GLOBAL_BANK1_N                    32
#define GLOBAL_M_AXI_AWPROT_BASE          GLOBAL_BANK1_BASE
#define GLOBAL_M_AXI_AWPROT_OFFSET        0
#define GLOBAL_M_AXI_AWPROT_N             3
#define GLOBAL_M_AXI_AWCACHE_BASE         GLOBAL_BANK1_BASE
#define GLOBAL_M_AXI_AWCACHE_OFFSET       3
#define GLOBAL_M_AXI_AWCACHE_N            4
#define GLOBAL_M_AXI_AWUSER_BASE          GLOBAL_BANK1_BASE
#define GLOBAL_M_AXI_AWUSER_OFFSET        7
#define GLOBAL_M_AXI_AWUSER_N             5
#define GLOBAL_M_AXI_ARPROT_BASE          GLOBAL_BANK1_BASE
#define GLOBAL_M_AXI_ARPROT_OFFSET        12
#define GLOBAL_M_AXI_ARPROT_N             3
#define GLOBAL_M_AXI_ARCACHE_BASE         GLOBAL_BANK1_BASE
#define GLOBAL_M_AXI_ARCACHE_OFFSET       15
#define GLOBAL_M_AXI_ARCACHE_N            4
#define GLOBAL_M_AXI_ARUSER_BASE          GLOBAL_BANK1_BASE
#define GLOBAL_M_AXI_ARUSER_OFFSET        19
#define GLOBAL_M_AXI_ARUSER_N             5

// USRP Modes
// Command (upper nibble)
#define CMD_TX_MODE                       (0x1 << 4)
#define CMD_RX_MODE                       (0x2 << 4)
// RX / TX mode (lower nibble)
#define RX_ADC_RAW_MODE                   0x0
#define RX_ADC_DC_OFF_MODE                0x1
#define RX_TEST_SINE_MODE                 0x2
#define RX_TEST_PATTERN_MODE              0x3
#define RX_ALL_1s_MODE                    0x4
#define RX_ALL_0s_MODE                    0x5
#define RX_I_1s_Q_0s_MODE                 0x6
#define RX_I_0s_Q_1s_MODE                 0x7
#define RX_CAL_INTF_MODE                  0x8
#define RX_TX_LOOPBACK_MODE               0x9
#define TX_PASSTHRU_MODE                  0x0
#define TX_DAC_RAW_MODE                   0x1
#define TX_DAC_DC_OFF_MODE                0x2
#define TX_SINE_TEST_MODE                 0x3

#define CRASH_IOCTL_BASE 'W'
#define CRASH_RESET                       _IO(CRASH_IOCTL_BASE, 0x40)
#define CRASH_SET_INTERRUPTS              _IO(CRASH_IOCTL_BASE, 0x41)
#define CRASH_GET_INTERRUPTS              _IO(CRASH_IOCTL_BASE, 0x42)
#define CRASH_DMA_WRITE                   _IO(CRASH_IOCTL_BASE, 0x43)
#define CRASH_DMA_READ                    _IO(CRASH_IOCTL_BASE, 0x44)
#define CRASH_GET_DMA_PHYS_ADDR           _IO(CRASH_IOCTL_BASE, 0x45)

// Macros to make reading / writing registers easier. Notice their input format matches the register definitions above.
// The use of _full and _range versions allows us to avoid compiler warnings
#define crash_read_reg(reg,name)            (name##_N == 8*sizeof(reg[0])) ? (crash_read_reg_full(reg,name)) : (crash_read_reg_range(reg,name))
#define crash_read_reg_full(reg,name)       reg[name##_BASE]
#define crash_read_reg_range(reg,name)      ((reg[name##_BASE] >> name##_OFFSET) & ((1 << name##_N)-1))
#define crash_write_reg(reg,name,val)       (name##_N == 8*sizeof(reg[0])) ? (crash_write_reg_full(reg,name,val)) : (crash_write_reg_range(reg,name,val))
#define crash_write_reg_full(reg,name,val)  reg[name##_BASE] = val
#define crash_write_reg_range(reg,name,val) reg[name##_BASE] = (((val) & ((1 << name##_N)-1)) << name##_OFFSET) | (reg[name##_BASE] & ~(((1 << name##_N)-1) << name##_OFFSET))
#define crash_get_bit(reg,name)             ((reg[name##_BASE] >> name##_OFFSET) & 1)
#define crash_set_bit(reg,name)             reg[name##_BASE] = (reg[name##_BASE] | (1 << name##_OFFSET))
#define crash_clear_bit(reg,name)           reg[name##_BASE] = (reg[name##_BASE] & ~(1 << name##_OFFSET))

#ifdef __cplusplus
}
#endif

#endif