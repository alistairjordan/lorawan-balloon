ifeq ($(SYSDRV_PARAM), )
	SYSDRV_PARAM:=../../Makefile.param
	include $(SYSDRV_PARAM)
endif

CURRENT_DIR := $(shell pwd)
INC_FLAGS_SX1278 := -I$(CURRENT_DIR)/src/
SRC_SX1278 := $(wildcard $(CURRENT_DIR)/src/*.c)
OBJ := $(SRC_SX1278:%.c=%.o)
M_OUT_DIR ?= ../out

export OBJ
export INC_FLAGS_SX1278

MODULE_NAME := sx1278

build_target := modules

all: $(build_target)
	@echo "build $(MODULE_NAME) done"

.PHONY: modules clean

$(MODULE_NAME)-objs := spi-sx1278.o lora.o
obj-m := $(MODULE_NAME).o

modules:
	$(MAKE) ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) -C $(KERNEL_DIR) M=$(shell pwd) $@ -j12
	cp $(shell pwd)/sx1278.ko $(M_OUT_DIR)
	@rm -rf *.o *~ .depend .*.cmd  *.mod.c .tmp_versions *.symvers modules.order *.mod

clean:
	@rm -rf *.o *~ .depend .*.cmd  *.mod.c .tmp_versions *.ko *.symvers modules.order *.mod
