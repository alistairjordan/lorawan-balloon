
#ifeq ($(SYSDRV_PARAM), )
#   SYSDRV_PARAM:=../../../Makefile.param
#    include $(SYSDRV_PARAM)
#endif

export LC_ALL=C
SHELL:=/bin/bash

CURRENT_DIR := $(shell pwd)
PKG_NAME := wspr
PKG_BIN := out

all:
	@test -f $(PKG_BIN)/usr/sbin/$(PKG_NAME) || (\
	mkdir -p $(CURRENT_DIR)/$(PKG_BIN)/usr/sbin; \
	gcc main.c wspr.c -o $(CURRENT_DIR)/$(PKG_BIN)/usr/sbin/$(PKG_NAME); \
	)
	$(call MAROC_COPY_PKG_TO_SYSDRV_OUTPUT, $(SYSDRV_DIR_OUT_ROOTFS), $(PKG_BIN))

clean: distclean

distclean:
	-rm -rf $(PKG_NAME) $(PKG_BIN)

