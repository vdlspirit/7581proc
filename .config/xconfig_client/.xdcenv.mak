#
_XDCBUILDCOUNT = 0
ifneq (,$(findstring path,$(_USEXDCENV_)))
override XDCPATH = C:/ti5.3/pdk_C6678_1_1_2_6/packages;C:/ti5.3/ndk_2_21_02_43/packages;C:/ti5.3/bios_6_34_02_18/packages;C:/ti5.3/mcsdk_2_01_02_06/demos;C:/ti5.3/ccsv5/ccs_base;C:/ti5.3/ipc_1_25_00_04/packages;C:/ti5.3/edma3_lld_02_11_05_02/packages;D:/PancyrCM/PlatformTest/P_6CA_Reliz1AddSinxBagVideo/.config
override XDCROOT = C:/ti5.3/xdctools_3_24_05_48
override XDCBUILDCFG = ./config.bld
endif
ifneq (,$(findstring args,$(_USEXDCENV_)))
override XDCARGS = 
override XDCTARGETS = 
endif
#
ifeq (0,1)
PKGPATH = C:/ti5.3/pdk_C6678_1_1_2_6/packages;C:/ti5.3/ndk_2_21_02_43/packages;C:/ti5.3/bios_6_34_02_18/packages;C:/ti5.3/mcsdk_2_01_02_06/demos;C:/ti5.3/ccsv5/ccs_base;C:/ti5.3/ipc_1_25_00_04/packages;C:/ti5.3/edma3_lld_02_11_05_02/packages;D:/PancyrCM/PlatformTest/P_6CA_Reliz1AddSinxBagVideo/.config;C:/ti5.3/xdctools_3_24_05_48/packages;..
HOSTOS = Windows
endif
