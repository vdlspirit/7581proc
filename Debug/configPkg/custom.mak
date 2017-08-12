## THIS IS A GENERATED FILE -- DO NOT EDIT
.configuro: .libraries,e66 linker.cmd \
  package/cfg/client_pe66.oe66 \

linker.cmd: package/cfg/client_pe66.xdl
	$(SED) 's"^\"\(package/cfg/client_pe66cfg.cmd\)\"$""\"E:/PancyrCM/PlatformTest/P_6CA_Reliz1AddSinxBagVideo/Debug/configPkg/\1\""' package/cfg/client_pe66.xdl > $@
