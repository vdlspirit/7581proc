/*
 * setup6678.h
 *
 
*/

extern void Set_Psc_All_On();
extern void Set_Timeout();
extern int Get_Timeout();
extern void Kill_Timeout();
extern void  Set_DSP_Cache();
extern int Init_PLL(int pll_mult, int pll_div );
extern void Init_Pll3(unsigned int multiplier, unsigned int divider);
extern void Init_Pll2(unsigned int multiplier, unsigned int divider);
extern void gxmc_setup();
extern void ddr3_setup_auto_lvl_1333();
extern void configSGMIISerdes();
void EnableEDC_OneforAll();
void setCpSwConfig();
extern void setupCPU();
extern Set_PSC_State(unsigned int pd,unsigned int id,unsigned int state);
