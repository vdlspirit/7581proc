// 
// The table provides some sample values of PLL1_M and PLL1_D:
//
// Please select PLL1_M values such that 0 < PLL1_M <= 64
// +--------------------+---------------+--------+--------+
// | (CLK)Desired       | (CLKIN) Input |        |        |
// | Device Speed (MHz) | Clock (MHz)   | PLL1_M | PLL1_D |
// +--------------------+---------------+--------+--------+
// | 1000               | 100           | 19     | 0      |
// | 1000               | 100 (EVM)     | 39     | 1      |
// | 1250               | 100           | 24     | 0      |
// | 1000               | 50            | 39     | 1      |
// | 1000               | 156.25        | 63     | 4      |
// +--------------------+---------------+--------+--------+
//
// +--------------------+---------------+--------+--------+
// | PA PLL VCO         | (CLKIN) Input |        |        |
// | Rate (MHz)         | Clock (MHz)   | PLL1_M | PLL1_D |
// +--------------------+---------------+--------+--------+
// | 1050               | 100.00 (EVM)  | 20     | 0      |
// | 1044               | 122.88        | 31     | 1      |
// | 1050               | 122.88        | 204    | 11     |
// | 1050               | 156.25        | 335    | 24     |
// +--------------------+---------------+--------+--------+
//
// +--------------------+---------------+--------+--------+
// | DDR3 PLL VCO       | (CLKIN) Input |        |        |
// | Rate (MHz)         | Clock (MHz)   | PLL1_M | PLL1_D |
// +--------------------+---------------+--------+--------+
// | 1333               | 66.667 (EVM)  | 19     | 0      |
// | 1066               | 66.667        | 31     | 1      |
// | 800                | 66.667        | 11     | 0      |
// +--------------------+---------------+--------+--------+

#include "setup6678.h"
#include <ti/csl/csl_cacheAux.h>
#include <ti/sysbios/family/c66/Cache.h>
#include <stdio.h>
#include <stdlib.h>

#define PLL1_M 63  
#define PLL1_D 4


#define DDR3_BASE_ADDRESS            0x80000000

// Global Register and constant definitions
#define REF_CLOCK_KHZ 156250

// Global timeout value
#define GTIMEOUT 1000

//*****************************************************
// Power definitions
#define PSC_BASE            0x02350000
#define PSC_PTCMD           *( unsigned int* )( PSC_BASE+0x120 )
#define PSC_PTSTAT          *( unsigned int* )( PSC_BASE+0x128 )
#define PSC_PDCTL_BASE      ( PSC_BASE+0x300 )
#define PSC_MDSTAT_BASE     ( PSC_BASE+0x800 )
#define PSC_MDCTL_BASE      ( PSC_BASE+0xA00 )

#define LPSC_EMIF25_SPI 3 // EMIF16
#define LPSC_TSIP 4
#define LPSC_DEBUGSS_TRC 5
// PD 1
#define LPSC_TETB_TRC 6
// PD 2
#define LPSC_PKTPROC 7  // PA
#define LPSC_CPGMAC 8   // SGMII
#define LPSC_Crypto  9  // SA
// PD 3
#define LPSC_PCIEX 10
// PD 4
#define LPSC_SRIO 11
// PD 5
#define LPSC_Hyperbridge 12
// PD 7
#define LPSC_MSMCSRAM 14

// Power domains definitions
#define PD0         (0)     // Power Domain-0
#define PD1         (1)     // Power Domain-1
#define PD2         (2)     // Power Domain-2
#define PD3         (3)     // Power Domain-3
#define PD4         (4)     // Power Domain-4
#define PD5         (5)     // Power Domain-5
#define PD6         (6)     // Power Domain-6
#define PD7         (7)     // Power Domain-7
#define PD8         (8)     // Power Domain-8
#define PD9         (9)     // Power Domain-9
#define PD10        (10)    // Power Domain-10
#define PD11        (11)    // Power Domain-11
#define PD12        (12)    // Power Domain-12
#define PD13        (13)    // Power Domain-13
#define PD14        (14)    // Power Domain-14
#define PD15        (15)    // Power Domain-15
#define PD16        (16)    // Power Domain-16
#define PD17        (17)    // Power Domain-17

#define PSC_SYNCRESET (0x1)
#define PSC_DISABLE   (0x2)
#define PSC_ENABLE    (0x3)

//*****************************************************
// BOOT and CONFIG dsp system modules Definitions
#define CHIP_LEVEL_REG  0x02620000
#define DEVSTAT                     (*(unsigned int*)(CHIP_LEVEL_REG + 0x0020))
#define KICK0                       (*(unsigned int*)(CHIP_LEVEL_REG + 0x0038))
#define KICK1                       (*(unsigned int*)(CHIP_LEVEL_REG + 0x003C))
#define TINPSEL                     (*(unsigned int*)(CHIP_LEVEL_REG + 0x0300))
#define TOUTPSEL                    (*(unsigned int*)(CHIP_LEVEL_REG + 0x0304))
#define MAINPLLCTL0                 (*(unsigned int*)(CHIP_LEVEL_REG + 0x0328))
#define MAINPLLCTL1                 (*(unsigned int*)(CHIP_LEVEL_REG + 0x032C))
#define DDR3PLLCTL0                 (*(unsigned int*)(CHIP_LEVEL_REG + 0x0330))
#define DDR3PLLCTL1                 (*(unsigned int*)(CHIP_LEVEL_REG + 0x0334))
#define PAPLLCTL0                   (*(unsigned int*)(CHIP_LEVEL_REG + 0x0338))
#define PAPLLCTL1                   (*(unsigned int*)(CHIP_LEVEL_REG + 0x033C))
#define OBSCLKCTL                   (*(unsigned int*)(CHIP_LEVEL_REG + 0x03AC))

// DDR3 tuning registers
#define DATA0_GTLVL_INIT_RATIO      (*(unsigned int*)(CHIP_LEVEL_REG + 0x043C))
#define DATA1_GTLVL_INIT_RATIO      (*(unsigned int*)(CHIP_LEVEL_REG + 0x0440))
#define DATA2_GTLVL_INIT_RATIO      (*(unsigned int*)(CHIP_LEVEL_REG + 0x0444))
#define DATA3_GTLVL_INIT_RATIO      (*(unsigned int*)(CHIP_LEVEL_REG + 0x0448))
#define DATA4_GTLVL_INIT_RATIO      (*(unsigned int*)(CHIP_LEVEL_REG + 0x044C))
#define DATA5_GTLVL_INIT_RATIO      (*(unsigned int*)(CHIP_LEVEL_REG + 0x0450))
#define DATA6_GTLVL_INIT_RATIO      (*(unsigned int*)(CHIP_LEVEL_REG + 0x0454))
#define DATA7_GTLVL_INIT_RATIO      (*(unsigned int*)(CHIP_LEVEL_REG + 0x0458))
#define DATA8_GTLVL_INIT_RATIO      (*(unsigned int*)(CHIP_LEVEL_REG + 0x045C))

#define DATA0_WRLVL_INIT_RATIO      (*(unsigned int*)(CHIP_LEVEL_REG + 0x040C))
#define DATA1_WRLVL_INIT_RATIO      (*(unsigned int*)(CHIP_LEVEL_REG + 0x0410))
#define DATA2_WRLVL_INIT_RATIO      (*(unsigned int*)(CHIP_LEVEL_REG + 0x0414))
#define DATA3_WRLVL_INIT_RATIO      (*(unsigned int*)(CHIP_LEVEL_REG + 0x0418))
#define DATA4_WRLVL_INIT_RATIO      (*(unsigned int*)(CHIP_LEVEL_REG + 0x041C))
#define DATA5_WRLVL_INIT_RATIO      (*(unsigned int*)(CHIP_LEVEL_REG + 0x0420))
#define DATA6_WRLVL_INIT_RATIO      (*(unsigned int*)(CHIP_LEVEL_REG + 0x0424))
#define DATA7_WRLVL_INIT_RATIO      (*(unsigned int*)(CHIP_LEVEL_REG + 0x0428))
#define DATA8_WRLVL_INIT_RATIO      (*(unsigned int*)(CHIP_LEVEL_REG + 0x042C))

#define DDR3_CONFIG_REG_0           (*(unsigned int*)(CHIP_LEVEL_REG + 0x0404))
#define DDR3_CONFIG_REG_12          (*(unsigned int*)(CHIP_LEVEL_REG + 0x0434))
#define DDR3_CONFIG_REG_23          (*(unsigned int*)(CHIP_LEVEL_REG + 0x0460))
#define DDR3_CONFIG_REG_24          (*(unsigned int*)(CHIP_LEVEL_REG + 0x0464))

#define SGMII_SERDES_CFGPLL         (*(unsigned int*)(CHIP_LEVEL_REG + 0x340))
#define SGMII_SERDES_CFGRX0         (*(unsigned int*)(CHIP_LEVEL_REG + 0x344))
#define SGMII_SERDES_CFGTX0         (*(unsigned int*)(CHIP_LEVEL_REG + 0x348))
#define SGMII_SERDES_CFGRX1         (*(unsigned int*)(CHIP_LEVEL_REG + 0x34C))
#define SGMII_SERDES_CFGTX1         (*(unsigned int*)(CHIP_LEVEL_REG + 0x350))

#define KICK0_UNLOCK (0x83E70B13)
#define KICK1_UNLOCK (0x95A4F1E0)
#define KICK_LOCK    0

/* PA PLL Observation Clock Control Register */
//#define OBSCLKCTL (*((unsigned int *) 0x026203AC))
#define PA_PLL_OBS_CLK_SEL_MASK (1 << 4) /* OBSCLKCTL Register Bit 4 - set to 0 to see PA PLL reference (input) clock, set to 1 to see PA PLL output*/
#define PA_PLL_OBS_CLK_EN_MASK  (1 << 5) /* OBSCLKCTL Register Bit 5 - set to 1 to enable power to PA PLL observation clock*/

/* PA PLL Registers */
#define BYPASS_BIT_SHIFT 23
#define CLKF_BIT_SHIFT   6
#define CLKR_BIT_SHIFT   0
//#define DEVSTAT    (*((unsigned int *) 0x02620020))
//#define PAPLLCTL0  (*((unsigned int *) 0x02620338))
//#define PAPLLCTL1  (*((unsigned int *) 0x0262033C))
#define PASSCLKSEL_MASK    (1 << 17)    /* Tells the configuration of the PASSCLKSEL pin */
#define PA_PLL_BYPASS_MASK (1 << BYPASS_BIT_SHIFT)    /* Tells whether the PA PLL is in BYPASS mode or not */
#define PA_PLL_CLKOD_MASK  (0x00780000) /* Tells the output divider value for the PA PLL */
#define PA_PLL_CLKF_MASK   (0x0007FFC0) /* Tells the multiplier value for the PA PLL */
#define PA_PLL_CLKR_MASK   (0x0000003F) /* Tells the divider value for the PA PLL */

//*****************************************************
// Timeout definitions
int _GEL_Global_Timeout1 = 0;
#define TIMEOUT_ID 10

//********************************************************
int iRESULT = 0;

//*****************************************************
// Extended Memory Controller (XMC) Configuration
#define XMC_BASE_ADDR (0x08000000)
#define XMPAX2_L     (*(unsigned int*)(XMC_BASE_ADDR + 0x00000010))
#define XMPAX2_H     (*(unsigned int*)(XMC_BASE_ADDR + 0x00000014))

// DDR3 definitions
#define DDR_BASE_ADDR          0x21000000

#define DDR_MIDR                    (*(unsigned int*)(DDR_BASE_ADDR + 0x00000000))
#define DDR_SDCFG                   (*(unsigned int*)(DDR_BASE_ADDR + 0x00000008))
#define DDR_SDRFC                   (*(unsigned int*)(DDR_BASE_ADDR + 0x00000010))
#define DDR_SDTIM1                  (*(unsigned int*)(DDR_BASE_ADDR + 0x00000018))
#define DDR_SDTIM2                  (*(unsigned int*)(DDR_BASE_ADDR + 0x00000020))
#define DDR_SDTIM3                  (*(unsigned int*)(DDR_BASE_ADDR + 0x00000028))
#define DDR_PMCTL                   (*(unsigned int*)(DDR_BASE_ADDR + 0x00000038))
#define DDR_ZQCFG                   (*(unsigned int*)(DDR_BASE_ADDR + 0x000000C8))
#define DDR_RDWR_LVL_RMP_CTRL       (*(unsigned int*)(DDR_BASE_ADDR + 0x000000D8))
#define DDR_TMPALRT                 (*(unsigned int*)(DDR_BASE_ADDR + 0x000000CC))
#define DDR_RDWR_LVL_CTRL           (*(unsigned int*)(DDR_BASE_ADDR + 0x000000DC))
#define DDR_DDRPHYC                 (*(unsigned int*)(DDR_BASE_ADDR + 0x000000E4))

//#define RD_DQS_SLAVE_RATIO_1333 0x34
//#define WR_DQS_SLAVE_RATIO_1333 0x45
//#define WR_DATA_SLAVE_RATIO_1333 0x85
//#define FIFO_WE_SLAVE_RATIO_1333 0xBC

#define RD_DQS_SLAVE_RATIO_1333 0x34
#define WR_DQS_SLAVE_RATIO_1333 0xB8
#define WR_DATA_SLAVE_RATIO_1333 0xF8
#define FIFO_WE_SLAVE_RATIO_1333 0x127

#define RD_DQS_SLAVE_RATIO_1066 0x34
#define WR_DQS_SLAVE_RATIO_1066 0x37
#define WR_DATA_SLAVE_RATIO_1066 0x77
#define FIFO_WE_SLAVE_RATIO_1066 0xA0

//*****************************************************
// PLL 1 definitions (DSP clk and subsystems)
#define PLL1_BASE           0x02310000
#define PLL1_PLLCTL              (*(unsigned int*)(PLL1_BASE + 0x100))   // PLL1 Control
#define PLL1_SECCTL              (*(unsigned int*)(PLL1_BASE + 0x108))   // PLL1 Sec Control
#define PLL1_PLLM                (*(unsigned int*)(PLL1_BASE + 0x110))   // PLL1 Multiplier
#define PLL1_DIV1                (*(unsigned int*)(PLL1_BASE + 0x118))   // DIV1 divider
#define PLL1_DIV2                (*(unsigned int*)(PLL1_BASE + 0x11C))   // DIV2 divider
#define PLL1_DIV3                (*(unsigned int*)(PLL1_BASE + 0x120))   // DIV3 divider
#define PLL1_CMD                 (*(unsigned int*)(PLL1_BASE + 0x138))   // CMD control
#define PLL1_STAT                (*(unsigned int*)(PLL1_BASE + 0x13C))   // STAT control
#define PLL1_ALNCTL              (*(unsigned int*)(PLL1_BASE + 0x140))   // ALNCTL control
#define PLL1_DCHANGE             (*(unsigned int*)(PLL1_BASE + 0x144))   // DCHANGE status
#define PLL1_CKEN                (*(unsigned int*)(PLL1_BASE + 0x148))   // CKEN control
#define PLL1_CKSTAT              (*(unsigned int*)(PLL1_BASE + 0x14C))   // CKSTAT status
#define PLL1_SYSTAT              (*(unsigned int*)(PLL1_BASE + 0x150))   // SYSTAT status
#define PLL1_DIV4                (*(unsigned int*)(PLL1_BASE + 0x160))   // DIV4 divider
#define PLL1_DIV5                (*(unsigned int*)(PLL1_BASE + 0x164))   // DIV5 divider
#define PLL1_DIV6                (*(unsigned int*)(PLL1_BASE + 0x168))   // DIV6 divider
#define PLL1_DIV7                (*(unsigned int*)(PLL1_BASE + 0x16C))   // DIV7 divider
#define PLL1_DIV8                (*(unsigned int*)(PLL1_BASE + 0x170))   // DIV8 divider
#define PLL1_DIV9                (*(unsigned int*)(PLL1_BASE + 0x174))   // DIV9 divider
#define PLL1_DIV10               (*(unsigned int*)(PLL1_BASE + 0x178))   // DIV10 divider
#define PLL1_DIV11               (*(unsigned int*)(PLL1_BASE + 0x17C))   // DIV11 divider
#define PLL1_DIV12               (*(unsigned int*)(PLL1_BASE + 0x180))   // DIV12 divider
#define PLL1_DIV13               (*(unsigned int*)(PLL1_BASE + 0x184))   // DIV13 divider
#define PLL1_DIV14               (*(unsigned int*)(PLL1_BASE + 0x188))   // DIV14 divider
#define PLL1_DIV15               (*(unsigned int*)(PLL1_BASE + 0x18C))   // DIV15 divider
#define PLL1_DIV16               (*(unsigned int*)(PLL1_BASE + 0x190))   // DIV16 divider

#define PLL_REG_RSCTL_VALUE_KEY                  (0x5A69)
#define PLL_REG_RSCFG_FIELD_POWER_ON_RESET       (1<<29)


//*****************************************************
// CACHE definitions
#define CACHE_BASE          0x01840000
#define CACHE_L2CFG             (*( unsigned int* )( CACHE_BASE ))
#define CACHE_L1PCFG            (*( unsigned int* )( CACHE_BASE + 0x0020 ))
#define CACHE_L1DCFG            (*( unsigned int* )( CACHE_BASE + 0x0040 ))
#define L2WBINV                 (CACHE_BASE + 0x5004) // L2WBINV Control
#define L2INV                   (CACHE_BASE + 0x5008) // L2INV Control
#define L1PINV                  (CACHE_BASE + 0x5028) // L1PINV Control
#define L1DWBINV                (CACHE_BASE + 0x5044) // L1DWBINV Control
#define L1DINV                  (CACHE_BASE + 0x5048) // L1DINV Control

//*****************************************************
// EDMA3 definitions
#define EDMA3_TPCC0_BASE    0x02700000
#define IERH_0                (EDMA3_TPCC0_BASE + 0x1054) // IERH Control
#define EERH_0                (EDMA3_TPCC0_BASE + 0x1024) // EERH Control
#define ICRH_0                (EDMA3_TPCC0_BASE + 0x1074) // ICRH Control
#define ECRH_0                (EDMA3_TPCC0_BASE + 0x100C) // ECRH Control
#define IER_0                 (EDMA3_TPCC0_BASE + 0x1050) // IER Control
#define EER_0                 (EDMA3_TPCC0_BASE + 0x1020) // EER Control
#define ICR_0                 (EDMA3_TPCC0_BASE + 0x1070) // ICR Control
#define ECR_0                 (EDMA3_TPCC0_BASE + 0x1008) // ECR Control
#define IECRH_0               (EDMA3_TPCC0_BASE + 0x105C) // IECRH Control
#define IECR_0                (EDMA3_TPCC0_BASE + 0x1058) // IECR Control
#define EECRH_0               (EDMA3_TPCC0_BASE + 0x102C) // EECRH Control
#define EECR_0                (EDMA3_TPCC0_BASE + 0x1028) // EECR Control

#define EDMA3_TPCC1_BASE    0x02720000
#define IERH_1                (EDMA3_TPCC1_BASE + 0x1054) // IERH Control
#define EERH_1                (EDMA3_TPCC1_BASE + 0x1024) // EERH Control
#define ICRH_1                (EDMA3_TPCC1_BASE + 0x1074) // ICRH Control
#define ECRH_1                (EDMA3_TPCC1_BASE + 0x100C) // ECRH Control
#define IER_1                 (EDMA3_TPCC1_BASE + 0x1050) // IER Control
#define EER_1                 (EDMA3_TPCC1_BASE + 0x1020) // EER Control
#define ICR_1                 (EDMA3_TPCC1_BASE + 0x1070) // ICR Control
#define ECR_1                 (EDMA3_TPCC1_BASE + 0x1008) // ECR Control
#define IECRH_1               (EDMA3_TPCC1_BASE + 0x105C) // IECRH Control
#define IECR_1                (EDMA3_TPCC1_BASE + 0x1058) // IECR Control
#define EECRH_1               (EDMA3_TPCC1_BASE + 0x102C) // EECRH Control
#define EECR_1                (EDMA3_TPCC1_BASE + 0x1028) // EECR Control

#define EDMA3_TPCC2_BASE    0x02740000
#define IERH_2                (EDMA3_TPCC2_BASE + 0x1054) // IERH Control
#define EERH_2                (EDMA3_TPCC2_BASE + 0x1024) // EERH Control
#define ICRH_2                (EDMA3_TPCC2_BASE + 0x1074) // ICRH Control
#define ECRH_2                (EDMA3_TPCC2_BASE + 0x100C) // ECRH Control
#define IER_2                 (EDMA3_TPCC2_BASE + 0x1050) // IER Control
#define EER_2                 (EDMA3_TPCC2_BASE + 0x1020) // EER Control
#define ICR_2                 (EDMA3_TPCC2_BASE + 0x1070) // ICR Control
#define ECR_2                 (EDMA3_TPCC2_BASE + 0x1008) // ECR Control
#define IECRH_2               (EDMA3_TPCC2_BASE + 0x105C) // IECRH Control
#define IECR_2                (EDMA3_TPCC2_BASE + 0x1058) // IECR Control
#define EECRH_2               (EDMA3_TPCC2_BASE + 0x102C) // EECRH Control
#define EECR_2                (EDMA3_TPCC2_BASE + 0x1028) // EECR Control

//*****************************************************
// GPIO definitions
#define GPIO_BASE           0x02320000
#define GPIO_BITEN          (*(unsigned int*)(GPIO_BASE + 0x0008)) // BITEN Control
#define GPIO_DIR            (*(unsigned int*)(GPIO_BASE + 0x0010)) // DIR Control
#define GPIO_OUT_DATA       (*(unsigned int*)(GPIO_BASE + 0x0014)) // OUT_DATA Control
#define GPIO_IN_DATA        (*(unsigned int*)(GPIO_BASE + 0x0020)) // IN_DATA Register
#define GPIO_CLR_RIS_TRIG   (*(unsigned int*)(GPIO_BASE + 0x0028)) // CLR_RIS_TRIG Control
#define GPIO_CLR_FAL_TRIG   (*(unsigned int*)(GPIO_BASE + 0x0030)) // CLR_FAL_TRIG Control

#define GPIO_DEFAULT_DIR    0xFFFF2CFF  // GP08,GP09,GP12,GP14,GP15 (Nand) are outputs
#define GPIO_DEFAULT_OUT    0x0000D000  // GP15, GP14, GP12 default to HIGH

// Used for eeprom programming
#define LITTLE_END 1
#define BIG_END 2

// SGMII definitions 
#define SGMII_SERDES_BASE  0x02090000
#define SGMII_SLIVER_BASE1 0x02090900
#define SGMII_SLIVER_BASE2 0x02090940
#define SGMII_SERDES_CONTROL_PORT1      (*(unsigned int*)(SGMII_SERDES_BASE  + 0x210))
#define SGMII_SERDES_MR_ADV_PORT1       (*(unsigned int*)(SGMII_SERDES_BASE  + 0x218))
#define SGMII_SERDES_AUX_CFG_PORT1      (*(unsigned int*)(SGMII_SERDES_BASE  + 0x238))
#define SGMII_SERDES_CONTROL_PORT0      (*(unsigned int*)(SGMII_SERDES_BASE  + 0x110))
#define SGMII_SERDES_MR_ADV_PORT0       (*(unsigned int*)(SGMII_SERDES_BASE  + 0x118))
#define SGMII_SERDES_AUX_CFG_PORT0      (*(unsigned int*)(SGMII_SERDES_BASE  + 0x138))
#define SGMII_SLIVER_MACCONTROL1        (*(unsigned int*)(SGMII_SLIVER_BASE1 + 0x004))
#define SGMII_SLIVER_MAXLEN1            (*(unsigned int*)(SGMII_SLIVER_BASE1 + 0x010))
#define SGMII_SLIVER_MACCONTROL2        (*(unsigned int*)(SGMII_SLIVER_BASE2 + 0x004))
#define SGMII_SLIVER_MAXLEN2            (*(unsigned int*)(SGMII_SLIVER_BASE2 + 0x010))

#define CPSW3G_BASE             0x02090800
#define CPSW3G_CONTROL_REG              (*(unsigned int*)(CPSW3G_BASE + 0x004))
#define CPSW3G_STAT_PORT_REG            (*(unsigned int*)(CPSW3G_BASE + 0x00C))
#define CPSW3G_ALE_CONTROL_REG          (*(unsigned int*)(CPSW3G_BASE + 0x608))
#define CPSW3G_ALE_PORT_0_CTL_REG       (*(unsigned int*)(CPSW3G_BASE + 0x640))
#define CPSW3G_ALE_PORT_1_CTL_REG       (*(unsigned int*)(CPSW3G_BASE + 0x644))
#define CPSW3G_ALE_PORT_2_CTL_REG       (*(unsigned int*)(CPSW3G_BASE + 0x648))
#define CPSW3G_ALE_PORT_STATE_FORWARD   0x3


//TIMER 0 definitions
#define TIMER0_CNTLO                     (*(unsigned int*)(0x02200010))
#define TIMER0_CNTHI                     (*(unsigned int*)(0x02200014))
#define TIMER0_PRDLO                     (*(unsigned int*)(0x02200018))
#define TIMER0_PRDHI                     (*(unsigned int*)(0x0220001C))
#define TIMER0_TCR                       (*(unsigned int*)(0x02200020))
#define TIMER0_TGCR                      (*(unsigned int*)(0x02200024))


#define L2EDSTAT 0x01846004
#define L2EDCMD    0x01846008
#define L2EDADDR 0x0184600C
#define L2EDCPEC 0x01846018
#define L2EDCNEC 0x0184601C
#define L2EDCEN    0x01846030

#define L1PEDSTAT  0x01846404
#define L1PEDCMD 0x01846408
#define L1PEDADDR  0x0184640C

#define SMCERRAR 0x0BC00008
#define  SMCERRXR  0x0BC0000C
#define SMEDCC   0x0BC00010
#define SMCEA    0x0BC00014
#define SMSECC   0x0BC00018

#define PLL_REINIT_MAX_COUNT (10)

// Flag for i2c eeprom programming process
int i2cprog=0;

/*
 * The delay function is called from wherever a delay is needed.  The parameter is the 
 * delay period (in milliseconds)
 */
void Delay_milli_seconds(int myDelay)
{      
	unsigned int i;
//    Set_Timeout(myDelay);
//    while(Get_Timeout());
//    Kill_Timeout();
	for(i=1000000*myDelay;i>0;i--)
	{

	}

}

/**
 *  @brief Simple DDR3 test
 *
 *  @details
 *      This function performs a simple DDR3 test for a memory range
 *      specified below and returns -1 for failure and 0 for success.
 */

#define DDR3_TEST_START_ADDRESS (0x81000000)

#define DDR3_TEST_END_ADDRESS   (DDR3_TEST_START_ADDRESS + (4 * 100))

ddr3_memory_test ()
{
    unsigned int index, value;

    printf( "DDR3 memory test... Started\n" );

    /* Write a pattern */
    for (index = DDR3_TEST_START_ADDRESS; index < DDR3_TEST_END_ADDRESS; index += 4) {
        *(unsigned int*)index = index;
    }

    /* Read and check the pattern */
    for (index = DDR3_TEST_START_ADDRESS; index < DDR3_TEST_END_ADDRESS; index += 4) {

        value = *(unsigned int*)index;

        if (value  != index) {
            printf( "DDR3 memory test... Failed\n" );
            return -1;
        }
    }

    /* Write a pattern for complementary values */
    for (index = DDR3_TEST_START_ADDRESS; index < DDR3_TEST_END_ADDRESS; index += 4) {
        *(unsigned int*)index = ~index;
    }

    /* Read and check the pattern */
    for (index = DDR3_TEST_START_ADDRESS; index < DDR3_TEST_END_ADDRESS; index += 4) {

        value = *(unsigned int*)index;

        if (value  != ~index) {
            printf( "DDR3 memory test... Failed\n" );
            return -1;
        }
    }

    printf( "DDR3 memory test... Passed\n" );
    return 0;
}


/****************************************************************************
 *
 * NAME
 *      OnHalt
 *
 * PURPOSE:
 *      Called by CCS each time the cpu is halted.
 *      You may use it to do specific custom action each the cpu is halted
 *      (Displays some info with printf, etc).
 *
 * USAGE
 *      This routine is a callback routine and called by CCS only.
 *
 * RETURN VALUE
 *      NONE
 *
 * REFERENCE
 *
 ****************************************************************************/

/****************************************************************************
 *
 * NAME
 *      Global_Default_Setup_Silent
 *
 *
 ****************************************************************************/
void  setupCPU()
{
    //float gel_ver = GEL_VERSION;
    int count;

    /* DDR PLL settings for 1328 MHz */
   // unsigned int PLLM_DDR = 16;
   // unsigned int PLLD_DDR = 1;

    unsigned int PLLM_PASS = 20;
    unsigned int PLLD_PASS = 0;
    int status;

    Set_DSP_Cache();

        for (count = 0; count < PLL_REINIT_MAX_COUNT; count++) {
            // Setup Pll1 DSP @ TARGET_FREQ
            status = Init_PLL(PLL1_M, PLL1_D);

            if (status == -1) {
                printf( "Error in Setting up main PLL, please power cycle the board and re-run Global Default Setup...\n" );
                while (1);
            }

            if (!count) {

                // Setup all Power Domains on
                Set_Psc_All_On( );
            }
            // Setup Pll3 pass clk @ 1050 MHz
            Init_Pll3(PLLM_PASS, PLLD_PASS);

            // Setup Pll2 DDR3 PLL @ 667 MHz
     /*       Init_Pll2(PLLM_DDR, PLLD_DDR);

            printf( "DDR begin (1333 auto)\n");
            gxmc_setup();
            ddr3_setup_auto_lvl_1333(0);
            printf( "DDR done\n");*/
            gxmc_setup();
            ddr3_setup_auto_lvl_1333(0);
            if(ddr3_memory_test() == 0)
            {
                break;
            }
        }
     configSGMIISerdes();
     EnableEDC_OneforAll();
     setCpSwConfig();

    Cache_inv((void *) (DDR3_TEST_START_ADDRESS), 512, Cache_Type_L1D, TRUE);
    printf( "Global Default Setup... Done.\n" );

}


/****************************************************************************
 *
 * NAME
 *      Wait_Soft
 *
 * PURPOSE:
 *      Wait for a specified delay in number of empty loop.
 *
 * USAGE
 *      This routine can be called as:
 *
 *      Wait_Soft(nloop)
 *
 *      nloop - (i) number of empty loop to do for delay
 *
 * RETURN VALUE
 *      NONE
 *
 * REFERENCE
 *
 ****************************************************************************/
Wait_Soft( int nloop )
{
    int i;

    // 1 sec ~ 40000 loop on P4 3.4GHz
    for( i = 0 ; i < nloop ; i++ )
    {
    }
}
/****************************************************************************
 *
 * NAME
 *      gprog_pll1_values
 *
 * PURPOSE:
 *      Wait for a specified delay in number of empty loop.
 *
 * USAGE
 *      This routine can be called as:
 *
 *      Wait_Soft(nloop)
 *
 *      nloop - (i) number of empty loop to do for delay
 *
 * RETURN VALUE
 *      NONE
 *
 * REFERENCE
 *
 ****************************************************************************/
gprog_pll1_values(unsigned int pll_multiplier, unsigned int pll_divider, unsigned int odiv)
{
    int TEMP;

    /* Check the Range for the parameters */

    if (odiv > 15)
    {
        printf ( " invalid output divide range, should be less than 15... \n");
    }

    if (pll_multiplier > 4095)
    {
        printf ( " invalid pll multiplier range, should be less than 4095... \n");
    }

    if (pll_divider > 63)
    {
        printf ( " invalid pll multiplier range, should be less than 63... \n");
    }

    /* Set the PLL Multiplier, Divider, BWADJ                                    *
     * The PLLM[5:0] bits of the multiplier are controlled by the PLLM Register  *
     * inside the PLL Controller and the PLLM[12:6] bits are controlled by the   *
     * chip-level MAINPLLCTL0 Register.                                          *
     * PLL Control Register (PLLM)  Layout                                       *
     * |31...6   |5...0        |                                                 *
     * |Reserved |PLLM         |                                                 *
     *                                                                           *
     * Main PLL Control Register (MAINPLLCTL0)                                   *
     * |31...24   |23...19   |18...12    | 11...6   |5...0 |                     *
     * |BWADJ[7:0]| Reserved |PLLM[12:6] | Reserved | PLLD |                     */

    /* Set pll multipler (13 bit field) */
    PLL1_PLLM     = (pll_multiplier & 0x0000003F); /* bits[5:0]  */
    TEMP          = (pll_multiplier & 0x1FC0) >> 6;/* bits[12:6] */
    MAINPLLCTL0  &=~(0x0007F000);                /*Clear PLLM field */
    MAINPLLCTL0  |=((TEMP << 12) & 0x0007F000);

   /* Set the BWADJ     (12 bit field)                                          *
     * BWADJ[11:8] and BWADJ[7:0] are located in MAINPLLCTL0 and MAINPLLCTL1     *
     * registers. BWADJ[11:0] should be programmed to a value equal to half of   *
     * PLLM[12:0] value (round down if PLLM has an odd value)                    *
     * Example: If PLLM = 15, then BWADJ = 7                                     */
    TEMP = ((pll_multiplier + 1) >> 1) - 1; /* Divide the pllm by 2 */
    MAINPLLCTL0 &=~(0xFF000000);  /* Clear the BWADJ Field */
    MAINPLLCTL0 |=  ((TEMP << 24) & 0xFF000000);
    MAINPLLCTL1 &=~(0x0000000F);   /* Clear the BWADJ field */
    MAINPLLCTL1 |= ((TEMP >> 8) & 0x0000000F);

    /* Set the pll divider (6 bit field)                                         *
     * PLLD[5:0] is located in MAINPLLCTL0                                       */
    MAINPLLCTL0   &= ~(0x0000003F);    /* Clear the Field */
    MAINPLLCTL0   |= (pll_divider & 0x0000003F);

    /* Set the OUTPUT DIVIDE (4 bit field) in SECCTL */
    PLL1_SECCTL    &= ~(0x00780000);     /* Clear the field       */
    PLL1_SECCTL   |= ((odiv << 19) & 0x00780000) ;
 
}

//menuitem "EVMC6678L HW Setup";

/****************************************************************************
 *
 * NAME
 *      Init PLL
 *
 * PURPOSE:
 *      Performs the main PLL initialization
 *
 * USAGE
 *      This function can be called as below.
 *       Init_PLL()
 *
 * RETURN VALUE
 *      iresult = 0: Success; -1=fail
 *
 * REFERENCE
 *
 * NOTE
 *     Note that this function requires pll_mult and pll_div arguments to be
 *     actual register values, i.e., the values are original values - 1
 *     (pll_* = calculated_value_of_pll_* - 1)
 *
 ****************************************************************************/
int Init_PLL(int pll_mult, int pll_div )
{
    int i, TEMP;
    /* Default dividers */
    unsigned int div2=3, div5=5, div8=64;
    int dsp_freq;
    int dsp_freM,dsp_freD;

    printf ( "Main PLL (PLL1) Setup ... \n");
    /*Unlock Boot Config*/
    KICK0 = 0x83E70B13;
    KICK1 = 0x95A4F1E0;

    // Only core0 can set PLL
//    if (DNUM == 0) Check CPU
    {

        /* 1. Wait for Stabilization time (min 100 us)                                *
         * The below loop is good enough for the Gel file to get minimum of        *
         * 100 micro seconds, this should be appropriately modified for port       *
         * to a C function                                                         *
         * Minimum delay in GEL can be 1 milli seconds, so program to 1ms=1000us,  *
         * more than required, but should be Okay                                  */
        Delay_milli_seconds(1);

        /* 2. Check the status of BYPASS bit in SECCTL register,                   *
         *    execute following steps if                                           *
         *    BYPASS == 1 (if bypass enabled), if BYPASS==0 then Jump to Step 3    */
        TEMP = PLL1_SECCTL &  0x00800000; /* Check the Bit 23 value */

        if (TEMP != 0) /* PLL BYPASS is enabled, we assume if not in Bypass ENSAT = 1 */
        {
            printf ( "PLL in Bypass ... \n");

            /* 2a. Usage Note 9: For optimal PLL operation, the ENSAT bit in the PLL control *
             * registers for the Main PLL, DDR3 PLL, and PA PLL should be set to 1.          *
             * The PLL initialization sequence in the boot ROM sets this bit to 0 and        *
             * could lead to non-optimal PLL operation. Software can set the bit to the      *
             * optimal value of 1 after boot                                                 *
             * Ref: http://www.ti.com/lit/er/sprz334b/sprz334b.pdf                           *
             * |31...7   |6     |5 4       |3...0      |                                     *
             * |Reserved |ENSAT |Reserved  |BWADJ[11:8]|                                     */

            MAINPLLCTL1 = MAINPLLCTL1 | 0x00000040;

            /* 2b. Clear PLLEN bit */
            PLL1_PLLCTL &= ~(1 << 0);
            
            /* 2c. Clear PLLENSRC bit */
            PLL1_PLLCTL &= ~(1 << 5);

            /* 2d. Wait for 4 RefClks   *
             * Assuming slowest Ref clock of 25MHz, min: 160 ns delay */
            Delay_milli_seconds(1);

            /* 2e. Bypass needed to perform PWRDN cycle for C6670 and C6678                  *
             * Needed on all devices when in NOBOOT, I2C or SPI boot modes                   *
             * Ref: Figure 4-2 of http://www.ti.com/lit/ug/sprugv2a/sprugv2a.pdf             *
             * PLL Secondary Control Register (SECCTL)  Layout                               *
             * |31...24  |23     |22...19       |18...0   |                                  *
             * |Reserved |BYPASS |OUTPUT DIVIDE |Reserved |                                  */
            PLL1_SECCTL |= 0x00800000; /* Set the Bit 23 */

            /* 2f. Advisory 8: Multiple PLLs May Not Lock After Power-on Reset Issue         *
             * In order to ensure proper PLL startup, the PLL power_down pin needs to be     *
             * toggled. This is accomplished by toggling the PLLPWRDN bit in the PLLCTL      *
             * register. This needs to be done before the main PLL initialization            *
             * sequence                                                                      *
             * Ref: Figure 4-1 of http://www.ti.com/lit/ug/sprugv2a/sprugv2a.pdf             *
             * PLL Control Register (PLLCTL)  Layout                                         *
             * |31...4   |3      |2        |1        |0        |                             *
             * |Reserved |PLLRST |Reserved |PLLPWRDN |Reserved |                             */

            PLL1_PLLCTL   |= 0x00000002; /*Power Down the PLL */

            /* 2g. Stay in a loop such that the bit is set for 5 µs (minimum) and           *
             * then clear the bit.                                                          */

            Delay_milli_seconds(1); /* This is more than required delay */  

            /* 2h. Power up the PLL */  
            PLL1_PLLCTL   &= ~(0x00000002); 

        }
        else
        {
            /* 3. Enable BYPASS in the PLL controller */
            printf ( "PLL not in Bypass, Enable BYPASS in the PLL Controller... \n");
            
            /* 3a. Clear PLLEN bit (bypass enabled in PLL controlelr mux) */
            PLL1_PLLCTL &= ~(1 << 0);
            
            /* 3b. Clear PLLENSRC bit (enable PLLEN to control PLL controller mux) */
            PLL1_PLLCTL &= ~(1 << 5);
            
            /* 3c. Wait for 4 RefClks (to make sure the PLL controller *
             * mux switches properly to the bypass)                    *
             * Assuming slowest Ref clock of 25MHz, min: 160 ns delay  */
            Delay_milli_seconds(1);
        }

        /* Wait for PLL Reset assertion Time (min: 50 us)                          *
         * Minimum delay in GEL can be 1 milli seconds, so program to 1ms=1000us,  *
         * more than required, but should be Okay                                  */
        //Delay_milli_seconds(1);

        /* 4, 5, 6 and 7 are done here:                                              *
         * Program the necessary multipliers/dividers and BW adjustments             */
        gprog_pll1_values(pll_mult, pll_div, 1);

        /* 8. Set PLL dividers if needed */
        
        /* part of 8, go stat bit needs to be zero here                             *
         * Read the GOSTAT bit in PLLSTAT to make sure the bit returns to 0 to      *  
         * indicate that the GO operation has completed                             */
        /* wait for the GOSTAT, but don't trap if lock is never read */
        for (i = 0; i < 1000; i++)
        {
            if ( (PLL1_STAT & 0x00000001) == 0 ) {
                break;
            }
        }
        if ( i == 1000 ) {
            printf ( "Error while waiting for GOSTAT bit returning to 0 ... \n");
            return(-1);
        }

        /* part of 8, Set PLL dividers if needed */
        PLL1_DIV2 = (0x8000) | (div2 - 1);
        PLL1_DIV5 = (0x8000) | (div5 - 1);
        PLL1_DIV8 = (0x8000) | (div8 - 1);

        /* part of 8, Program ALNCTLn */
        /* Set bit 1, 4 and 7 */
        PLL1_ALNCTL |= ( (1 << 1) | (1 << 4) | (1 << 7));

        /* part of 8, Set GOSET bit in PLLCMD to initiate the GO operation to change the divide *
         * values and align the SYSCLKs as programmed                                           */
        PLL1_CMD     |= 0x00000001;

        /* part of 8, Read the GOSTAT bit in PLLSTAT to make sure the bit returns to 0 to      *  
         * indicate that the GO operation has completed                             */

        /* wait for the GOSTAT, but don't trap if lock is never read */
        for (i = 0; i < 1000; i++)
        {

            if ( (PLL1_STAT & 0x00000001) == 0 ) {
                break;
            }

        }

        if ( i == 1000 ) {
            printf ( "Error while waiting for GOSTAT bit returning to 0 ... \n");
            return(-1);
        }

        /* 9. Place PLL in Reset, In PLLCTL, write PLLRST = 1 (PLL is reset)         */
        PLL1_PLLCTL |= 0x00000008;
        
        /* 10. Wait for the PLL Reset duration time (min: 7us    )                */
        Delay_milli_seconds(1);

        /* 11. In PLLCTL, write PLLRST = 0 to bring PLL out of reset */
        PLL1_PLLCTL &= ~(0x00000008);

        /* 
         * 12. PLL Lock Delay needs to be 500 RefClk periods * (PLLD + 1)
         * i.e., Wait for at least 500 * CLKIN cycles * (PLLD + 1) (PLL lock time)
         * Using 2000 25ns RefClk periods per DM
         * Wait for PLL to lock min 50 micro seconds
         * 
         * */
        Delay_milli_seconds(1);

        /* 13. In SECCTL, write BYPASS = 0 to enable PLL mux to switch to PLL mode */
        PLL1_SECCTL &= ~(0x00800000); /* Release Bypass */

        /* 14. Set the PLLEN */
        PLL1_PLLCTL |= (1 << 0);
        
        /* 15. The PLL and PLL Controller are now initialized in PLL mode - Complete */

        // Compute the real dsp freq (*100)
        dsp_freq = (((REF_CLOCK_KHZ/10) * ((pll_mult+1)/2))/(pll_div+1));

        // Displayed frequency setup
        // dsp freq in MHz
        dsp_freM = dsp_freq / 100;

        // dsp freq first decimal if freq expressed in MHz
        dsp_freD = ((dsp_freq - dsp_freM * 100) + 5) / 10;

        // Add roundup unit to MHz displayed and reajust decimal value if necessary...
        if (dsp_freD > 9)
        {
            dsp_freD = dsp_freD - 10;
            dsp_freM = dsp_freM + 1;
        }

        // Print freq info...
//        printf( "PLL1 Setup for DSP @ %d.%d MHz.\n",,,,, dsp_freM, dsp_freD );
//        printf( "           SYSCLK2 = %f MHz, SYSCLK5 = %f MHz.\n",,,,, ((float)(dsp_freq/100)/div2), ((float)(dsp_freq/100)/div5));
//        printf( "           SYSCLK8 = %f MHz.\n",,,,, ((float)(dsp_freq/100)/div8));
//        printf( "PLL1 Setup... Done.\n" );
        return (0);
    }

}


/****************************************************************************
 *
 * NAME
 *      RoundToHigherInt
 *
 * PURPOSE:
 *      This routine will round a float value to higher int.
 *      ex.: RoundToHigherInt(7.1)=8, RoundToHigherInt(7.7)=8,
 *           RoundToHigherInt(7.0)=7
 *
 * USAGE
 *      This routine can be called as:
 *
 *      RoundToHigherInt(fValue)
 *
 *      fValue - (i) float value to be converted.
 *
 * RETURN VALUE
 *      Converted int value.
 *
 * REFERENCE
 *
 ****************************************************************************/



/****************************************************************************
 *
 * NAME
 *      Set_Wait
 *
 * PURPOSE:
 *      Wait for a specified delay in msec.
 *
 * USAGE
 *      This routine can be called as:
 *
 *      Set_Wait(msec)
 *
 *      msec - (i) delay period in msec
 *
 * RETURN VALUE
 *      NONE
 *
 * REFERENCE
 *
 ****************************************************************************/
Set_Wait(msec)
{
    // Temporary call a soft loop
    // 1 sec ~ 23500 loop on P4 3.4GHz because of CCS 4.x slow Set_Timeout problem
    Wait_Soft(msec*24);
}

//menuitem "EVMC6678L Init Functions";

/****************************************************************************
 *
 * NAME
 *      Init_Functions_Help
 *
 * PURPOSE:
 *      Prints the help for the init functions menu.
 *
 * USAGE
 *
 *      Called from GEL menu only
 *
 * RETURN VALUE
 *      NONE
 *
 * REFERENCE
 *
 ****************************************************************************/
void Init_Functions_Help()
{
    printf("\n\n");
    printf("                  INIT FUNCTIONS MENU HELP\n");
    printf("                  ------------------------\n\n");
    printf("This menu is to do EVM & DSP initialization.\n\n");
    printf("Global_Default_Setup: same function as when connecting to target. Will do:\n");
    printf("                      Set_DSP_Cache, Set_Pll1(TARGET_MULTIPLIER), Set_Pll2_666_7_MHz,\n");
    printf("                      Set_Psc_All_On, Set_Pin_Board.\n");
    printf("Set_DSP_Cache: setup DSP default cache settings (for L1P, L1D, L2).\n");
    printf("Set_Pll1_xxx_MHz: setup DSP speed (PLL1) to specified MHz.\n");
    printf("Set_Pll2_666_7_MHz: setup ddr speed (PLL2) to specified MHz.\n");
    printf("Set_Psc_All_On: power-on all DSP peripherals domain/modules.\n");
    printf("Set_Pin_Board: setup all DSP IO pins and also EVM settings to default.\n\n");
}

/****************************************************************************
 *
 * NAME
 *      Global_Default_Setup
 *
 * PURPOSE:
 *      Setup almost everything ready for a new debug session:
 *      DSP modules and EVM board modules.
 *
 * USAGE
 *      This routine can be called as:
 *
 *      Global_Default_Setup()
 *
 * RETURN VALUE
 *      NONE
 *
 * REFERENCE
 *
 ****************************************************************************/

/****************************************************************************
 *
 * NAME
 *      Set_DSP_Cache
 *
 * PURPOSE:
 *      Setup the DSP caches with predefined cache size.
 *
 * USAGE
 *      This routine can be called as:
 *
 *      Set_DSP_Cache()
 *
 * RETURN VALUE
 *      NONE
 *
 * REFERENCE
 *
 ****************************************************************************/
void  Set_DSP_Cache( )
{
    int l1p, l1d, l2;

    printf( "Setup Cache... \n");

    // Modify for the desired cache settings needed
    CACHE_L1PCFG = 4;           // L1P on, MAX size
    CACHE_L1DCFG = 4;           // L1D on, MAX size
    CACHE_L2CFG  = 0;           // L2 off, use as RAM

    l1p = (CACHE_L1PCFG & 0x7);

    if ( l1p == 0 )
    {
        printf( "L1P = 0K   \n" );
    }
    if ( l1p == 1 )
    {
        printf( "L1P = 4K   \n" );
    }
    if ( l1p == 2 )
    {
        printf( "L1P = 8K   \n" );
    }
    if ( l1p == 3 )
    {
        printf( "L1P = 16K   \n" );
    }
    if ( l1p >= 4 )
    {
        printf( "L1P = 32K   \n" );
    }

    l1d = (CACHE_L1DCFG & 0x7);
    if ( l1d == 0 )
    {
        printf( "L1D = 0K   \n" );
    }
    if ( l1d == 1 )
    {
        printf( "L1D = 4K   \n" );
    }
    if ( l1d == 2 )
    {
        printf( "L1D = 8K   \n" );
    }
    if ( l1d == 3 )
    {
        printf( "L1D = 16K   \n" );
    }
    if ( l1d >= 4 )
    {
        printf( "L1D = 32K   \n" );
    }

    l2 = (CACHE_L2CFG & 0x7);
    if ( l2 == 0 )
    {
        printf( "L2 = ALL SRAM   \n" );
    }
    if ( l2 == 1 )
    {
        printf( "L2 = 31/32 SRAM   \n" );
    }
    if ( l2 == 2 )
    {
        printf( "L2 = 15/16 SRAM   \n" );
    }
    if ( l2 == 3 )
    {
        printf( "L2 = 7/8 SRAM   \n" );
    }
    if ( l2 == 4 )
    {
        printf( "L2 = 3/4 SRAM   \n" );
    }
    if ( l2 == 5 )
    {
        printf( "L2 = 1/2 SRAM   \n" );
    }
    if ( l2 >= 6 )
    {
        printf( "L2 = ALL CACHE   \n" );
    }

    printf( "Setup Cache... Done.\n");
}

/****************************************************************************
 *
 * NAME
 *      Set_Pll2_666_7_MHz
 *
 * PURPOSE:
 *      Setup PLL 2 DDR @ 666.7 MHz with External clock
 *
 * USAGE
 *      This routine can be called as:
 *
 *      Set_Pll2_666_7_MHz()
 *
 * RETURN VALUE
 *      NONE
 *
 * REFERENCE
 *
 ****************************************************************************/

/****************************************************************************
 *
 * NAME
 *      Set_Psc_All_On
 *
 * PURPOSE:
 *      Enable all PSC modules and DSP power domains on ALWAYSON, and wait
 *      for these power transitions to complete.
 *
 * USAGE
 *      This routine can be called as:
 *
 *      Set_Psc_All_On()
 *
 * RETURN VALUE
 *      NONE
 *
 * REFERENCE
 *
 ****************************************************************************/
void Set_Psc_All_On( )
{
    //unsigned int i=0;

    // Only core0 can set PSC
//    if (DNUM == 0) //Check core number !!!
    {
        printf( "Power on all PSC modules and DSP domains... \n");\

        Set_PSC_State(PD0, LPSC_EMIF25_SPI, PSC_ENABLE);
        Set_PSC_State(PD0, LPSC_TSIP, PSC_ENABLE);
        Set_PSC_State(PD1, LPSC_TETB_TRC, PSC_ENABLE);
        Set_PSC_State(PD2, LPSC_PKTPROC, PSC_ENABLE);
        Set_PSC_State(PD2, LPSC_CPGMAC, PSC_ENABLE);
        Set_PSC_State(PD2, LPSC_Crypto, PSC_ENABLE);
        Set_PSC_State(PD3, LPSC_PCIEX, PSC_ENABLE);
        Set_PSC_State(PD4, LPSC_SRIO, PSC_ENABLE);
        Set_PSC_State(PD5, LPSC_Hyperbridge, PSC_ENABLE);
        Set_PSC_State(PD7, LPSC_MSMCSRAM, PSC_ENABLE);

        printf( "Power on all PSC modules and DSP domains... Done.\n" );
    }

}

/****************************************************************************
 *
 * NAME
 *      Set_Pin_Board
 *
 * PURPOSE:
 *      Configure DSP IO pin and board for normal operation
 *
 * USAGE
 *      This routine can be called as:
 *
 *      Set_Pin_Board()
 *
 * RETURN VALUE
 *      =1 SUCCESS, =0 FAIL
 *
 * REFERENCE
 *
 ****************************************************************************/
int Set_Pin_Board( )
{
    int iResult=1;

    // Only core0 can set GPIO
//    if (DNUM == 0)
    {
        printf( "Set Board and DSP IO Pins... \n" );

        // Configure DSP GP IO related to board functions
        GPIO_BITEN=0;                           // Disable GPIO irq
        GPIO_OUT_DATA=GPIO_DEFAULT_OUT;         // Set pins correct out values
        GPIO_DIR=GPIO_DEFAULT_DIR;              // Configure io directions for EVM

        // Checks if we got an error
        if (iResult)
            printf( "Set Board and DSP IO Pins... Done.\n" );
        else
            printf( "Set Board and DSP IO Pins... ERROR!\n");
    }
    return(iResult);
}

//menuitem "EVMC6678L Debug Tools";

/****************************************************************************
 *
 * NAME
 *      Debug_Tools_Help
 *
 * PURPOSE:
 *      Prints the help for the debug tools menu.
 *
 * USAGE
 *
 *      Called from GEL menu only
 *
 * RETURN VALUE
 *      NONE
 *
 * REFERENCE
 *
 ****************************************************************************/
void Debug_Tools_Help()
{
    printf("\n\n");
    printf("                  DEBUG TOOLS MENU HELP\n");
    printf("                  ---------------------\n\n");
    printf("This menu is to do some specifics debug functions on EVM, or to get\n");
    printf(" some debug informations.\n\n");
    printf("Invalidate_Cache: allows to purge all DSP cache (destroy L1P, L1D, L2).\n");
    printf("Flush_Cache: allows to flush all DSP cache (L1P, L1D, L2) to lower\n");
    printf("             memory and then invalidates the cache contents.\n");
    printf("Core_Info: displays the DSP core id where GEL is currently running.\n");
    printf("Ask_MDIO_Control: ask for MDIO control access to the PHY.\n");
    printf("Release_MDIO_Control: release of MDIO control access to the PHY.\n");
    printf("Displays_Dsp_Role_Info: displays if the current dsp is dsp1 or dsp2.\n");
    printf("Dump_FPGA_Registers: displays a list of all FPGA internal registers values.\n\n");
    //printf("Write_Image_2DSP_EEprom: write compiled dsp eeprom boot image to dsp eeprom,\n");
    //printf("                         last compiled image endianess must match current dsp mode.\n\n");
}


/****************************************************************************
 *
 * NAME
 *      Invalidate_Cache
 *
 * PURPOSE:
 *      Invalidate L1P, L1D and L2 cache (purge all cache contents).
 *
 * USAGE
 *      This routine can be called as:
 *
 *      Invalidate_Cache()
 *
 * RETURN VALUE
 *      NONE
 *
 * REFERENCE
 *
 ****************************************************************************/
void Invalidate_Cache()
{
    printf( "Invalidate All Cache...\n" );

    /* Invalidate L1P cache */
    *(int*)L1PINV = 1;

    // Wait for cache operation to finish
    // Wait for transition to finish for max timeout time...
    while(( *(int*)L1PINV & 0x0001 ) ) Wait_Soft(150);

    /* Invalidate L1D cache */
    *(int*)L1DINV = 1;

    // Wait for cache operation to finish
    // Wait for transition to finish for max timeout time...
    while( ( *(int*)L1DINV & 0x0001 ) ) Wait_Soft(150);

    /* Invalidate L2 cache */
    *(int*)L2INV = 1;

    // Wait for transition to finish for max timeout time...
    while( ( *(int*)L2INV & 0x0001 ) )Wait_Soft(150);

    printf( "Invalidate All Cache... Done.\n" );
}

/****************************************************************************
 *
 * NAME
 *      Flush_Cache
 *
 * PURPOSE:
 *      Invalidate L1P, FLUSH L1D and L2 cache (writeback dirty lines to
 *      lower memory and invalidates cache content).
 *
 * USAGE
 *      This routine can be called as:
 *
 *      Flush_Cache()
 *
 * RETURN VALUE
 *      NONE
 *
 * REFERENCE
 *
 ****************************************************************************/
void Flush_Cache()
{
    printf( "Flush All Cache...\n" );

    /* Invalidate L1P cache */
    *(int*)L1PINV = 1;

    // Wait for cache operation to finish
    // Wait for transition to finish for max timeout time...
    while(( *(int*)L1PINV & 0x0001 ) ) Wait_Soft(150);

    /* Flush L1D cache (writeback dirty lines to lower memory and invalidate all) */
    *(int*)L1DWBINV = 1;

    // Wait for cache operation to finish
    // Wait for transition to finish for max timeout time...
    while( ( *(int*)L1DWBINV & 0x0001 ) ) Wait_Soft(150);

    /* Flush L2 cache (writeback dirty lines to lower memory and invalidate all) */
    *(int*)L2WBINV = 1;

    // Wait for cache operation to finish
    // Wait for transition to finish for max timeout time...
    while(( *(int*)L2WBINV & 0x0001 ) ) Wait_Soft(150);

    printf( "Flush All Cache... Done.\n" );
}

/****************************************************************************
 *
 * NAME
 *      Core_Info
 *
 * PURPOSE:
 *      Display on the screen information about the current running DSP core
 *
 * USAGE
 *
 *      Called from program or GEL menu
 *
 * RETURN VALUE
 *
 * REFERENCE
 *
****************************************************************************/
/****************************************************************************
 *
 * NAME
 *      Memory_Map_Help
 *
 * PURPOSE:
 *      Prints the help for the memory map menu.
 *
 * USAGE
 *
 *      Called from GEL menu only
 *
 * RETURN VALUE
 *      NONE
 *
 * REFERENCE
 *
 ****************************************************************************/
void Memory_Map_Help()
{
    printf("\n\n");
    printf("                  MEMORY MAP MENU HELP\n");
    printf("                  --------------------\n\n");
    printf("This menu is to do CCS memory map configuration for DSP.\n\n");
    printf("Setup_Memory_Map: Configure CCS memory map to match DSP memory map (called at GEL init.).\n");
    printf("Clear_Memory_Map: Clears (disables) CCS memory map configuration.\n\n");
}





/*--------------------------------------------------------------*/
/* gxmc_setup()                                                  */
/* XMC MPAX register setting to access DDR3 config space        */
/*--------------------------------------------------------------*/

#define XMC_BASE_ADDR (0x08000000)
//#define XMPAX2_L     (*(int*)(XMC_BASE_ADDR + 0x00000010))
//#define XMPAX2_H     (*(int*)(XMC_BASE_ADDR + 0x00000014))

void gxmc_setup()
{  
    /* mapping for ddr emif registers XMPAX*2 */
    XMPAX2_L = 0x100000FF;     /* replacement addr + perm*/
    XMPAX2_H =  0x2100000B;    /* base addr + seg size (64KB)*/  //"1B"-->"B" by xj

    printf( "XMC Setup ... Done \n" );

}

/*--------------------------------------------------------------*/
/* ddr3_setup()                                                  */
/* DDR3 initialization                                          */
/*--------------------------------------------------------------*/
int flag;
void ddr3_setup_auto_lvl_1333()
{
    int TEMP;
    KICK0 = KICK0_UNLOCK;
    KICK1 = KICK1_UNLOCK;

    /* Wait for PLL to lock = min 500 ref clock cycles. 
       With refclk = 100MHz, = 5000 ns = 5us */
    Delay_milli_seconds(1);

    /***************** 3.2 DDR3 PLL Configuration ************/
    /* Done before */

    /**************** 3.0 Leveling Register Configuration ********************/
    /* Using partial automatic leveling due to errata */

    /**************** 3.3 Leveling register configuration ********************/
    DDR3_CONFIG_REG_0 &= ~(0x007FE000);  // clear ctrl_slave_ratio field
    DDR3_CONFIG_REG_0 |= 0x00200000;     // set ctrl_slave_ratio to 0x100
    DDR3_CONFIG_REG_12 |= 0x08000000;    // Set invert_clkout = 1
    DDR3_CONFIG_REG_0 |= 0xF;            // set dll_lock_diff to 15

    //From 4.2.1 Executing Partial Automatic Leveling -- Start
    DDR3_CONFIG_REG_23 |= 0x00000200;    //Set bit 9 = 1 to use forced ratio leveling for read DQS
    //From 4.2.1 Executing Partial Automatic Leveling -- End

    //Values with invertclkout = 1
    /**************** 3.3 Partial Automatic Leveling ********************/

    DATA0_WRLVL_INIT_RATIO = 0x53;
    DATA1_WRLVL_INIT_RATIO = 0x4E;
    DATA2_WRLVL_INIT_RATIO = 0x4D;
    DATA3_WRLVL_INIT_RATIO = 0x47;
    DATA4_WRLVL_INIT_RATIO = 0x31;
    DATA5_WRLVL_INIT_RATIO = 0x2E;
    DATA6_WRLVL_INIT_RATIO = 0x23;
    DATA7_WRLVL_INIT_RATIO = 0x1D;
    DATA8_WRLVL_INIT_RATIO = 0x3A;

    DATA0_GTLVL_INIT_RATIO = 0xBE;
    DATA1_GTLVL_INIT_RATIO = 0xC3;
    DATA2_GTLVL_INIT_RATIO = 0xAB;
    DATA3_GTLVL_INIT_RATIO = 0xB1;
    DATA4_GTLVL_INIT_RATIO = 0x96;
    DATA5_GTLVL_INIT_RATIO = 0x99;
    DATA6_GTLVL_INIT_RATIO = 0x8C;
    DATA7_GTLVL_INIT_RATIO = 0x92;
    DATA8_GTLVL_INIT_RATIO = 0xA6;

    //Do a PHY reset. Toggle DDR_PHY_CTRL_1 bit 15 0->1->0
    DDR_DDRPHYC &= ~(0x00008000);
    DDR_DDRPHYC |= (0x00008000);
    DDR_DDRPHYC &= ~(0x00008000);

    /***************** 3.4 Basic Controller and DRAM Configuration ************/
    DDR_SDRFC    = 0x00005162;    // enable configuration 

    /* DDR_SDTIM1   = 0x1113783C; */
    TEMP = 0;
    TEMP |= 0x8 << 25; // T_RP bit field 28:25
    TEMP |= 0x8 << 21; // T_RCD bit field 24:21
    TEMP |= 0x9 << 17; // T_WR bit field 20:17
    TEMP |= 0x17 << 12; // T_RAS bit field 16:12
    TEMP |= 0x20 << 6; // T_RC bit field 11:6
    TEMP |= 0x7 << 3; // T_RRD bit field 5:3
    TEMP |= 0x4; // T_WTR bit field 2:0
    DDR_SDTIM1 = TEMP;

    /* DDR_SDTIM2   = 0x30717FE3; */
    TEMP = 0;
    TEMP |= 0x3 << 28; // T_XP bit field 30:28
    TEMP |= 0x71 << 16; // T_XSNR bit field 24:16
    TEMP |= 0x1ff << 6; // T_XSRD bit field 15:6
    TEMP |= 0x4 << 3; // T_RTP bit field 5:3
    TEMP |= 0x3; // T_CKE bit field 2:0
    DDR_SDTIM2 = TEMP;

    /*  DDR_SDTIM3   = 0x559F86AF; */
    TEMP = 0;
    TEMP |= 0x5 << 28; // T_PDLL_UL bit field 31:28 (fixed value)
    TEMP |= 0x5 << 24; // T_CSTA bit field 27:24 (fixed value)
    TEMP |= 0x4 << 21; // T_CKESR bit field 23:21
    TEMP |= 0x3f << 15; // T_ZQCS bit field 20:15
    TEMP |= 0x6a << 4; // T_RFC bit field 12:4
    TEMP |= 0xf; // T_RAS_MAX bit field 3:0 (fixed value)
    DDR_SDTIM3 = TEMP; 

    DDR_DDRPHYC  = 0x0010010F;

    DDR_ZQCFG    = 0x70073214; 

    DDR_PMCTL    = 0x0;

    DDR_SDRFC = 0x00005162; // enable configuration

    /* DDR_SDCFG    = 0x63062A32; */
    /* New value with DYN_ODT disabled and SDRAM_DRIVE = RZQ/7 //0x63222A32;    // last config write DRAM init occurs */
    TEMP = 0;
    TEMP |= 0x3 << 29; // SDRAM_TYPE bit field 31:29 (fixed value)
    TEMP |= 0x0 << 27; // IBANK_POS bit field 28:27
    TEMP |= 0x3 << 24; // DDR_TERM bit field 26:24
    TEMP |= 0x0 << 21; // DYN_ODT bit field 22:21
    TEMP |= 0x1 << 18; // SDRAM_DRIVE bit field 19:18
    TEMP |= 0x2 << 16; // CWL bit field 17:16
    TEMP |= 0x0 << 14; // NM bit field 15:14
    TEMP |= 0xA << 10; // CL bit field 13:10
    TEMP |= 0x4 << 7; // ROWSIZE bit field 9:7
    TEMP |= 0x3 << 4; // IBANK bit field 6:4
    TEMP |= 0x0 << 3; // EBANK bit field 3:3
    TEMP |= 0x2; // PAGESIZE bit field 2:0
    DDR_SDCFG = TEMP;

    //Wait 600us for HW init to complete
    Delay_milli_seconds(1);

    DDR_SDRFC = 0x00001450;       //Refresh rate = (7.8*666MHz)

    /**************** 4.2.1 Executing Partial Automatic Leveling ********************/

    DDR_RDWR_LVL_RMP_CTRL = 0x80000000; //enable full leveling

    DDR_RDWR_LVL_CTRL = 0x80000000; //Trigger full leveling - This ignores read DQS leveling result and uses ratio forced value  

    //(0x34) instead
    //Wait for min 1048576 DDR clock cycles for leveling to complete = 1048576 * 1.5ns = 1572864ns = 1.57ms.
    //Actual time = ~10-15 ms
    Delay_milli_seconds(1);
    printf("\nDDR3 initialization is complete.\n");
}



//menuitem "CPSW Functions";


void configSGMIISerdes()
{

    printf( "configSGMIISerdes Setup... Begin\n" );


    /* Unlock Chip Level Registers */
    KICK0 = KICK0_UNLOCK;
    KICK1 = KICK1_UNLOCK;

    SGMII_SERDES_CONTROL_PORT1 = 0x0;
    SGMII_SERDES_CONTROL_PORT0 = 0x0;
    /* Multiply to be 8 with Quarter Rate in the Rx registers */
    SGMII_SERDES_CFGPLL = 0x00000041;


    /* Wait */
    Wait_Soft(100);


    //31:25    Reserved    0000000
    //23:24    LOOPBACK    00
    //   22    ENOC        1
    //21:18    EQ          0001
    //17:15    CDR         001 -- first order threshold of 17
    //14:12    LOS         000 -- tie off
    //11:10    ALIGN       01  -- Comma Aligned
    //09:07    TERM        100 -- tie off (100)
    //   06    INVPAIR     0
    //05:04    RATE        01  -- tie off (10)  //00 = Full Rate, 01 = Half Rate (*0.5), 10 = Quarter Rate (*0.25)
    //03:01    BUSWIDTH    000 -- tie off
    //   00    ENRX        1
    // 0000 0000 0100 0100 0000 0010 0001 0001 = 0x0044_0211 -- My estimated value
    // 0000 0000 0100 0100 0000 0100 0001 0001 = 0x0044_0411 -- New DV value
    // 0000 0000 0000 1000 0000 1000 0100 0001 = 0x0008_0841 -- Original DV value
    SGMII_SERDES_CFGRX0 = 0x00700621;
    SGMII_SERDES_CFGRX1 = 0x00700621;


    //31:22    Reserved    0
    //21:20    LOOPBACK    00
    //19:18    RDTCT       00  -- tie off
    //   17    ENIDL       0   -- tie off
    //   16    MYSNC       1   -- tie off
    //15:12    DEMPHASIS   ???? - 0001 Lets give some de-emphasis
    //11:08    SWING       ????
    //   07    CM          1   -- tie off
    //   06    INVPAIR     0
    //05:04    RATE        01  -- tie off
    //03:01    BUSWIDTH    000 -- tie off
    //   00    ENTX        1
    // 0000 0000 0011 0001 ???? ???? 1001 0001 = 0x0031_1E91 -- My estimated value
    // 0000 0000 0000 0001 0000 1111 0001 0001 = 0x0001_0F11 -- New DV value
    // 0000 0000 0100 0000 0001 1110 0100 0001 = 0x0040_1e41 -- Original DV value

    SGMII_SERDES_CFGTX0 = 0x000108A1;
    SGMII_SERDES_CFGTX1 = 0x000108A1;

    SGMII_SERDES_AUX_CFG_PORT0 = 0x00000041;
    SGMII_SERDES_AUX_CFG_PORT1 = 0x00000041;

    /* waitforclock() */
    Wait_Soft(1000);

    SGMII_SERDES_MR_ADV_PORT0 = 0x1;
    SGMII_SERDES_MR_ADV_PORT1 = 0x1;
    Wait_Soft(100);
    SGMII_SERDES_CONTROL_PORT1 = 0x1;
    SGMII_SERDES_CONTROL_PORT0 = 0x1;

    /*Configuring Sliver */
    SGMII_SLIVER_MAXLEN2 = 0x2520;
    SGMII_SLIVER_MAXLEN1 = 0x2520;

    SGMII_SLIVER_MACCONTROL2 = 0xA1;
    SGMII_SLIVER_MACCONTROL1 = 0xA1;


    /* Lock Chip Level Registers */
    KICK0 = KICK_LOCK;
    KICK1 = KICK_LOCK;

    printf("\nSGMII SERDES has been configured.\n");

}

/* Initialize switch configuration */
void setCpSwConfig()
{
    /* Enable Port 0 */
    CPSW3G_CONTROL_REG = 0x4;
    CPSW3G_STAT_PORT_REG = 0xF;

    /* Enable ALE. */
    CPSW3G_ALE_CONTROL_REG = 0x80000000;
    /* Enable ALE port state to Forward */
    CPSW3G_ALE_PORT_0_CTL_REG = 0x3;
    CPSW3G_ALE_PORT_1_CTL_REG = 0x3;
    CPSW3G_ALE_PORT_2_CTL_REG = 0x3;

}
//menuitem "PA PLL Functions";


/* Print the current PA PLL configuration */
void getPaPllConfig()
{
    unsigned int passclksel = (DEVSTAT & PASSCLKSEL_MASK);
    unsigned int papllctl0val = PAPLLCTL0;
    unsigned int papllbypass;
    unsigned int papllod;
    unsigned int papllclkf;
    unsigned int papllclkr;
    unsigned int paplloutput; 

    /* Tells the configuration of the PASSCLKSEL pin */
    if (passclksel == PASSCLKSEL_MASK)  printf("PA PLL is using PASS_CLK as the input\n");
    else  printf("PA PLL is using SYSCLK/ALTCORECLK as the input\n");

    /* Tells whether the PA PLL is in BYPASS mode or not */
    papllbypass = (papllctl0val & PA_PLL_BYPASS_MASK);
    if (papllbypass == PA_PLL_BYPASS_MASK) printf("PA PLL is in bypass mode\n");
    else printf("PA PLL is in PLL mode\n");

    /* Tells the output divider value for the PA PLL */
    papllod = (((papllctl0val & PA_PLL_CLKOD_MASK) >> 19) + 1);
    printf("PA PLL fixed output divider = %d\n",papllod);

    /* Tells the multiplier value for the PA PLL */
    papllclkf = (((papllctl0val & PA_PLL_CLKF_MASK) >> 6) + 1);
    printf("PA PLL programmable multiplier = %d\n",papllclkf);

    /* Tells the divider value for the PA PLL */
    papllclkr = (((papllctl0val & PA_PLL_CLKR_MASK) >> 0) +1);
    printf("PA PLL programmable divider = %d\n",papllclkr);

    /* Final multiply/divide value */
    paplloutput = ((papllclkf)/(papllod * papllclkr));
    printf("the output frequency should be %d times the PA reference clock\n",paplloutput);
}

/****************************************************************************
 *
 * NAME
 *      Init_Pll2 (unsigned int multiplier, unsigned int divider)
 *
 * PURPOSE:
 *      Setup PLL 2 frequency for DDR3 clock.
 *      The Frequency is based on an external ddr3 clk input ref of 66.65 MHz clock.
 *
 * USAGE
 *      This routine can be called as:
 *
 *      Init_Pll2( unsigned int multiplier, unsigned int divider)
 *
 *      multiplier - (i) Multiplier
 *      divider    - (i) Divider
 *      Note that the values above are actual register values, i.e., 
 *      the computed values - 1
 * RETURN VALUE
 *      None 
 *
 * REFERENCE
 *
 ****************************************************************************/
void Init_Pll2(unsigned int multiplier, unsigned int divider)
{
    unsigned int temp;

    printf ( "DDR3 PLL (PLL2) Setup ... \n");
    /*Unlock Boot Config*/
    KICK0 = 0x83E70B13;
    KICK1 = 0x95A4F1E0;

//    if (DNUM == 0)
    {
        /* Usage Note 9: For optimal PLL operation, the ENSAT bit in the PLL control *
         * registers for the Main PLL, DDR3 PLL, and PA PLL should be set to 1.      *
         * The PLL initialization sequence in the boot ROM sets this bit to 0 and    *
         * could lead to non-optimal PLL operation. Software can set the bit to the  *
         * optimal value of 1 after boot                                             *
         * DDR3PLLCTL1 Bit map                                                         *
         * |31...7   |6     |5 4       |3...0      |                                 *
         * |Reserved |ENSAT |Reserved  |BWADJ[11:8]|                                 */
        DDR3PLLCTL1 |= 0x00000040;

        /* Put the PLL in PLL Mode                                                   *
         * DDR3PLLCTL0 Bit map                                                         *
         * |31...24    |23     |22...19       |18...6   |5...0 |                     *
         * |BWADJ[7:0] |BYPASS |Reserved      |PLLM     |PLLD  |                     */
        DDR3PLLCTL0 |= 0x00800000; /* Set the Bit 23 */

        /* Wait for the PLL Reset time (min: 5 us)                                */
        //Delay_milli_seconds(1);

        

        /* Program the necessary multipliers/dividers and BW adjustments             */
        /* Set the divider values */
        DDR3PLLCTL0 &= ~(0x0000003F);
        DDR3PLLCTL0 |= (divider & 0x0000003F);

        /* Set the Multipler values */
        DDR3PLLCTL0 &= ~(0x0007FFC0);
        DDR3PLLCTL0 |= ((multiplier << 6) & 0x0007FFC0 );

        /* Set the BWADJ */
        temp = ((multiplier + 1) >> 1) - 1;
        DDR3PLLCTL0 &= ~(0xFF000000); 
        DDR3PLLCTL0 |= ((temp << 24) & 0xFF000000);
        DDR3PLLCTL1 &= ~(0x0000000F);
        DDR3PLLCTL1 |= ((temp >> 8) & 0x0000000F);
        
        /* In PLL Controller, reset the PLL (bit 13 in DDR3PLLCTL1 register)         */
        DDR3PLLCTL1 |= 0x00002000;
        
        /* Wait for the PLL Reset time (min: 5 us)                                */
        Delay_milli_seconds(1);

        /*In DDR3PLLCTL1, write PLLRST = 0 to bring PLL out of reset */
        DDR3PLLCTL1 &= ~(0x00002000);

        /* Wait at least 500 * REFCLK cycles * PLLD (this is the PLL lock time) */
        Delay_milli_seconds(1);

        /* Put the PLL in PLL Mode                                                   *
         * DDR3PLLCTL0 Bit map                                                         *
         * |31...24    |23     |22...19       |18...6   |5...0 |                     *
         * |BWADJ[7:0] |BYPASS |Reserved      |PLLM     |PLLD  |                     */
        DDR3PLLCTL0 &= ~(0x00800000); /* ReSet the Bit 23 */

        printf( "DDR3 PLL Setup... Done.\n" );
    }
}

/****************************************************************************
 *
 * NAME
 *      Init_Pll3
 *
 * PURPOSE:
 *      Setup PLL 3 frequency for DSP PASS clock.
 *      The Frequency is based on an external pass clk input ref of 122.88 MHz clock.
 *
 * USAGE
 *      Init_Pll3( unsigned int multiplier, unsigned int divider)
 *
 *      multiplier - (i) Multiplier
 *      divider    - (i) Divider
 *      Note that the values above are actual register values, i.e., 
 *      the computed values - 1
 *
 * REFERENCE
 *
 ****************************************************************************/
void Init_Pll3(unsigned int multiplier, unsigned int divider)
{
    unsigned int temp;

    printf ( "PA PLL (PLL3) Setup ... \n");
    /*Unlock Boot Config*/
    KICK0 = 0x83E70B13;
    KICK1 = 0x95A4F1E0;

//    if (DNUM == 0)
    {
        /* Usage Note 9: For optimal PLL operation, the ENSAT bit in the PLL control *
         * registers for the Main PLL, PA PLL, and PA PLL should be set to 1.      *
         * The PLL initialization sequence in the boot ROM sets this bit to 0 and    *
         * could lead to non-optimal PLL operation. Software can set the bit to the  *
         * optimal value of 1 after boot                                             *
         * PAPLLCTL1 Bit map                                                         *
         * |31...7   |6     |5 4       |3...0      |                                 *
         * |Reserved |ENSAT |Reserved  |BWADJ[11:8]|                                 */
        PAPLLCTL1 |= 0x00000040;

        /* Put the PLL in PLL Mode                                                   *
         * PAPLLCTL0 Bit map                                                         *
         * |31...24    |23     |22...19       |18...6   |5...0 |                     *
         * |BWADJ[7:0] |BYPASS |Reserved      |PLLM     |PLLD  |                     */
        PAPLLCTL0 |= 0x00800000; /* Set the Bit 23 */

        /* Wait for the PLL Reset time (min: 5 us)                                */
        //Delay_milli_seconds(1);

        /* Program the necessary multipliers/dividers and BW adjustments             */
        /* Set the divider values */
        PAPLLCTL0 &= ~(0x0000003F);
        PAPLLCTL0 |= (divider & 0x0000003F);

        /* Set the Multipler values */
        PAPLLCTL0 &= ~(0x0007FFC0);
        PAPLLCTL0 |= ((multiplier << 6) & 0x0007FFC0 );

        /* Set the BWADJ */
        temp = ((multiplier + 1) >> 1) - 1;
        PAPLLCTL0 &= ~(0xFF000000); 
        PAPLLCTL0 |= ((temp << 24) & 0xFF000000);
        PAPLLCTL1 &= ~(0x0000000F);
        PAPLLCTL1 |= ((temp >> 8) & 0x0000000F);
        
        /* In PLL Controller, reset the PLLRST (bit 14) in PAPLLCTL1 register         */
        PAPLLCTL1 |= 0x00004000;

        /* Wait for the PLL Reset time (min: 5 us)                                */
        Delay_milli_seconds(1);
        
        /* In PLL Controller, set PLLSEL (bit 13) in PAPLLCTL1 register         */
        PAPLLCTL1 |= 0x00002000;

        /*In PAPLLCTL1, write PLLRST = 0 to bring PLL out of reset */
        PAPLLCTL1 &= ~(0x00004000);

        /* Wait at least 500 * REFCLK cycles * PLLD (this is the PLL lock time) */
        Delay_milli_seconds(1);

        /* Put the PLL in PLL Mode                                                   *
         * PAPLLCTL0 Bit map                                                         *
         * |31...24    |23     |22...19       |18...6   |5...0 |                     *
         * |BWADJ[7:0] |BYPASS |Reserved      |PLLM     |PLLD  |                     */
        PAPLLCTL0 &= ~(0x00800000); /* ReSet the Bit 23 */

        printf( "PA PLL Setup... Done.\n" );
    }

}
/*--------------------------------------------------------------*/
/* EVMC6678L MENU                                              */
/*--------------------------------------------------------------*/
//menuitem "DDR EVMC6678L Functions";
/****************************************************************************
 *
 * NAME
 *      Init XMC
 *
 * PURPOSE:
 *      Performs the Extended Memory Controller Setup
 *
 * USAGE
 *      This function can be called as below.
 *       InitXMC()
 *
 * RETURN VALUE
 *      NONE
 *
 * REFERENCE
 *
 ****************************************************************************/
void InitXMC()
{
   gxmc_setup();
}

/****************************************************************************
 *
 * NAME
 *      Init Emif
 *
 * PURPOSE:
 *      Performs the External Memory Interface initialization
 *
 * USAGE
 *      This function can be called as below.
 *       InitEmif()
 *
 * RETURN VALUE
 *      NONE
 *
 * REFERENCE
 *
 ****************************************************************************/
void InitEmif_EVM()
{

    ddr3_setup_auto_lvl_1333();
}

/*--------------------------------------------------------------*/
/* L2 EDC Functions                                              */
/* Enable/Disable L2 EDC and Check the Status              */
/*--------------------------------------------------------------*/

/*Enable EDC on L2*/
CGEM_enableL2EDC ()
{
    unsigned int status = 0;

    *(unsigned int *)(L2EDCMD) = 0x1; //enable EDC

    //Check the status
    status = *(unsigned int *)(L2EDSTAT);

    if ((status<<28) == 0x10000000)
        printf("L2 error detection/correction logic is enabled.\n",2);
    else  
        printf("L2 error detection/correction logic enable is failed.\n",2);
}

/*Disable EDC on L2*/
CGEM_disableL2EDC ()
{
    unsigned int status = 0;

    *(unsigned int *)(L2EDCMD) = 0x4;

    //Check the status
    status = *(unsigned int *)(L2EDSTAT);

    if ((status<<28) == 0x40000000)
        printf("L2 error detection/correction logic is disabled.\n");
    else  
        printf("L2 error detection/correction logic disable is failed.\n");
}

/*Enable EDC on DMC reads from an external address (Hits L2 cache) if L2EDCMD is enabled*/
CGEM_enableEDCDMCExternal ()
{
    *(unsigned int *)(L2EDCEN) |= 0x1;  //Set DL2CEN(bit0)=1
}

/*Enable EDC on PMC reads from an external address (Hits L2 cache) if L2EDCMD is enabled*/
CGEM_enableEDCPMCExternal ()
{
    *(unsigned int *)(L2EDCEN) |= 0x2;  //Set PL2CEN(bit1)=1
}

/*Enable EDC on DMC reads from L2SRAM if L2EDCMD is enabled*/
CGEM_enableEDCDMCL2SRAM ()
{
    *(unsigned int *)(L2EDCEN) |= 0x4;  //Set DL2SEN(bit2)=1
}

/*Enable EDC on PMC reads from L2SRAM if L2EDCMD is enabled*/
CGEM_enableEDCPMCL2SRAM ()
{
    *(unsigned int *)(L2EDCEN) |= 0x8;  //Set PL2SEN(bit3)=1
}

/*Enable EDC on SDMA reads from L2SRAM if L2EDCMD is enabled. This includes SRAM under cache*/
CGEM_enableEDCSDMAL2SRAM ()
{
    *(unsigned int *)(L2EDCEN) |= 0x10; //Set SDMAEN(bit4)=1
}

/*Enable all bits in L2EDCEN*/
CGEM_enableEDCL2EDCEN ()
{
    //Set DL2CEN(bit0),PL2CEN(bit1),DL2SEN(bit2),PL2SEN(bit3),SDMAEN(bit4)=1
    *(unsigned int *)(L2EDCEN) |= 0x1F; 
}

/*Disable all bits in L2EDCEN*/
CGEM_disableEDCL2EDCEN ()
{
    //Clear DL2CEN(bit0),PL2CEN(bit1),DL2SEN(bit2),PL2SEN(bit3),SDMAEN(bit4)=0
    *(unsigned int *)(L2EDCEN) &= 0xFFFFFFE0; 
}

/*--------------------------------------------------------------*/
/* L1P EDC Functions                                         */
/* Enable/Disable L1P EDC and Check the Status             */
/*--------------------------------------------------------------*/

/*Enable EDC on L1P*/
CGEM_enableL1PEDC ()
{
    unsigned int status = 0;

    *(unsigned int *)(L1PEDCMD) = 0x1;  //Set EN(bit0)=1  

    //Check the status
    status = *(unsigned int *)(L1PEDSTAT);

    if ((status<<28) == 0x10000000)
        printf("L1P error detection logic is enabled.\n");
    else  
        printf("L1P error detection logic enable is failed.\n");
}

/*Disable EDC on L1P*/
CGEM_disableL1PEDC ()
{
    unsigned int status = 0;

    *(unsigned int *)(L1PEDCMD) = 0x4;  //Set DIS(bit2)=1 

    //Check the status
    status = *(unsigned int *)(L1PEDSTAT);

    if ((status<<28) == 0x40000000)
        printf("L1P error detection logic is disabled.\n");
    else  
        printf("L1P error detection logic disable is failed.\n");
}

/*--------------------------------------------------------------*/
/* MSMC EDC Functions                                          */
/* Enable/Disable MSMC EDC and Check the Status              */
/*--------------------------------------------------------------*/

/*Enable EDC on MSMC*/
MSMC_enableEDC ()
{
    unsigned int status = 0;

    *(unsigned int *)(SMEDCC) &= 0x7FFFFFFF;  //Clear SEN(bit31)=0  
    *(unsigned int *)(SMEDCC) |= 0x40000000;  //Set ECM(bit30)=1  

    //Check the status
    status = *(unsigned int *)(SMEDCC);

    if ((status>>30)==0x1)
        printf("MSMC error detection/correction logic is enabled.\n");
    else  
        printf("MSMC error detection/correction logic enable is failed.\n");

}

/* Note: Once MSMC EDC is enabled, error correction stays enabled until the MSMC is reset */

/*--------------------------------------------------------------*/
/* EVMC6678L EDC MENU                                              */
/*--------------------------------------------------------------*/
//menuitem "EVMC6678L EDC Functions";

/****************************************************************************
 *
 * NAME
 *      Enable the EDC functions
 *
 * PURPOSE:
 *      Performs the the EDC enable for All of L1 and L2 memory
 *
 * USAGE
 *      This function can be called as below.
 *       EnableEDC_OneforAll()
 *
 * RETURN VALUE
 *      NONE
 *
 * REFERENCE
 *
 ****************************************************************************/

void EnableEDC_OneforAll()
{
    CGEM_enableL1PEDC();
    CGEM_enableEDCL2EDCEN();
    CGEM_enableL2EDC();
    MSMC_enableEDC();
}

/****************************************************************************
 *
 * NAME
 *      Enable the EDC functions
 *
 * PURPOSE:
 *      Performs the the EDC enable for L1P 
 *
 * USAGE
 *      This function can be called as below.
 *       L1PEDCEnable()
 *
 * RETURN VALUE
 *      NONE
 *
 * REFERENCE
 *
 ****************************************************************************/

void L1PEDCEnable()
{
    CGEM_enableL1PEDC();
}

/****************************************************************************
 *
 * NAME
 *      Enable the EDC functions
 *
 * PURPOSE:
 *      Performs the the EDC enable for L2 memory
 *
 * USAGE
 *      This function can be called as below.
 *       L2EDCEnable()
 *
 * RETURN VALUE
 *      NONE
 *
 * REFERENCE
 *
 ****************************************************************************/
void L2EDCEnable()
{
    CGEM_enableEDCL2EDCEN();
    CGEM_enableL2EDC();
}   

/****************************************************************************
 *
 * NAME
 *      Enable the EDC functions
 *
 * PURPOSE:
 *      Performs the the EDC enable for MSMC memory
 *
 * USAGE
 *      This function can be called as below.
 *       MSMC_enableEDC()
 *
 * RETURN VALUE
 *      NONE
 *
 * REFERENCE
 *
 ****************************************************************************/

void MSMCEDCEnable()
{
    MSMC_enableEDC();
} 

/****************************************************************************
 *
 * NAME
 *      Disable the EDC functions
 *
 * PURPOSE:
 *      Performs the the EDC disable for All of L1 and L2 memory
 *
 * USAGE
 *      This function can be called as below.
 *       DisableEDC_OneforAll()
 *
 * RETURN VALUE
 *      NONE
 *
 * REFERENCE
 *
 ****************************************************************************/
void DisableEDC_OneforAll()
{
    CGEM_disableL1PEDC();
    CGEM_disableEDCL2EDCEN();
    CGEM_disableL2EDC();
}

/****************************************************************************
 *
 * NAME
 *      Disable the EDC functions
 *
 * PURPOSE:
 *      Performs the the EDC disable for L1P memory
 *
 * USAGE
 *      This function can be called as below.
 *       CGEM_disableL1PEDC()
 *
 * RETURN VALUE
 *      NONE
 *
 * REFERENCE
 *
 ****************************************************************************/
void L1PEDCDisable()
{
    CGEM_disableL1PEDC();
}

/****************************************************************************
 *
 * NAME
 *      Disable the EDC functions
 *
 * PURPOSE:
 *      Performs the the EDC disable for L2 memory
 *
 * USAGE
 *      This function can be called as below.
 *       L2EDCDisable()
 *
 * RETURN VALUE
 *      NONE
 *
 * REFERENCE
 *
 ****************************************************************************/
void L2EDCDisable()
{
    CGEM_disableEDCL2EDCEN();
    CGEM_disableL2EDC();
}   

/****************************************************************************
 *
 * NAME
 *      Set_PSC_State
 *
 * PURPOSE:
 *      Set a new power state for the specified domain id in a power controler
 *      domain. Wait for the power transition to complete.
 *
 * USAGE
 *      This routine can be called as:
 *
 *      Set_PSC_State(unsigned int pd,unsigned int id,unsigned int state)
 *
 *      pd    - (i) power domain.
 *
 *      id    - (i) module id to use for module in the specified power domain
 *
 *      state - (i) new state value to set
 *                  0 = RESET
 *                  1 = SYNC RESET
 *                  2 = DISABLE
 *                  3 = ENABLE
 *
 * RETURN VALUE
 *      0 if ok, !=0 for error
 *
 * REFERENCE
 *
 ****************************************************************************/
Set_PSC_State(unsigned int pd,unsigned int id,unsigned int state)
{
    unsigned int* mdctl;
    unsigned int* mdstat;
    unsigned int* pdctl;
    int ret=0;

    // Only core0 can set PSC

        mdctl = ( unsigned int* )(PSC_MDCTL_BASE + ( 4 * id ));
        mdstat = ( unsigned int* )( PSC_MDSTAT_BASE + ( 4 * id ));
        pdctl = ( unsigned int* )(PSC_PDCTL_BASE + ( 4 * pd ));

        // If state is already set, do nothing
        if ( ( *mdstat & 0x1f ) == state )
        {
            return(0);
        }

        Wait_Soft(150);
        // Check if we got timeout error while waiting
        if (PSC_PTSTAT & (0x1 << pd))
        {
        	printf( "Set_PSC_State... Timeout Error #01 pd=%d, md=%d!\n",pd,id);
            ret=1;
        }
        else
        {
            // Set power domain control
            *pdctl = (*pdctl) | 0x00000001;

            // Set MDCTL NEXT to new state
            *mdctl = ((*mdctl) & ~(0x1f)) | state;

            // Start power transition by setting PTCMD GO to 1
            PSC_PTCMD = (PSC_PTCMD) | (0x1<<pd);

            // Wait for PTSTAT GOSTAT to clear
            Wait_Soft(150);
            if ((PSC_PTSTAT & (0x1 << pd)) != 0 ) {
            	printf( "Set_PSC_State... Timeout Error #02 pd=%d, md=%d!\n",pd,id);
                return 1;
            }

            // Verify state changed
            Wait_Soft(150);
            if(( *mdstat & 0x1f ) != state ) {
                if ((pd == 2) && (id == 9) ) {
                	printf( "Security Accelerator disabled!\n");
                } else {
                	printf( "Set_PSC_State... Timeout Error #03 pd=%d, md=%d!\n",pd,id);
                }
                return 1;
            }
        }

    return(ret);
}

