
#ifeq ($(SYSDRV_PARAM), )
   LORAWAN_BRIDGE_PARAM:=../../../Makefile.param
    include $(LORAWAN_BRIDGE_PARAM)
#endif

export LC_ALL=C
SHELL:=/bin/bash

CURRENT_DIR := $(shell pwd)
PKG_NAME := lbr
PKG_BIN := out

all:
	@test -f $(PKG_BIN)/usr/sbin/$(PKG_NAME)_noexist || (\
	mkdir -p $(CURRENT_DIR)/$(PKG_BIN)/usr/sbin; \
	$(SYSDRV_CROSS)-gcc -g main.c -o $(CURRENT_DIR)/$(PKG_BIN)/usr/sbin/$(PKG_NAME); \
	)
	$(call MAROC_COPY_PKG_TO_SYSDRV_OUTPUT, $(SYSDRV_DIR_OUT_ROOTFS), $(PKG_BIN))
#	$(MAKE) ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) -C $(KERNEL_DIR) M=$(shell pwd) $@ -j12

clean: distclean

distclean:
	-rm -rf $(PKG_NAME) $(PKG_BIN)