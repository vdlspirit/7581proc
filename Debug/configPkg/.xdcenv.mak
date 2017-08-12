#
_XDCBUILDCOUNT = 
ifneq (,$(findstring path,$(_USEXDCENV_)))
override XDCPATH = C:/ti53/pdk_C6678_1_1_2_6/packages;C:/ti53/ndk_2_21_02_43/packages;C:/ti53/bios_6_34_02_18/packages;C:/ti53/mcsdk_2_01_02_06/demos;C:/ti53/ccsv5/ccs_base;C:/ti53/ipc_1_25_00_04/packages;C:/ti53/edma3_lld_02_11_05_02/packages
override XDCROOT = C:/ti53/xdctools_3_24_05_48
override XDCBUILDCFG = ./config.bld
endif
ifneq (,$(findstring args,$(_USEXDCENV_)))
override XDCARGS = 
override XDCTARGETS = 
endif
#
ifeq (0,1)
PKGPATH = C:/ti53/pdk_C6678_1_1_2_6/packages;C:/ti53/ndk_2_21_02_43/packages;C:/ti53/bios_6_34_02_18/packages;C:/ti53/mcsdk_2_01_02_06/demos;C:/ti53/ccsv5/ccs_base;C:/ti53/ipc_1_25_00_04/packages;C:/ti53/edma3_lld_02_11_05_02/packages;C:/ti53/xdctools_3_24_05_48/packages;..
HOSTOS = Windows
endif
