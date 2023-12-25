#
# Copyright (c) 2018, 2022 CivetWeb Developers
# Copyright (c) 2013 No Face Press, LLC
# License http://opensource.org/licenses/mit-license.php MIT License
#

#This makefile is used to test the other Makefiles


PROG = rest_server
SRC =  rest.c cJSON/cJSON.c cJSON/cJSON_Utils.c send_json.c lorawan_send.c b64.c/encode.c b64.c/decode.c b64.c/buffer.c
LORAWAN_SRC = LoraMac/spidev_lib/spidev_lib.c LoraMac/board.c LoraMac/main.c LoraMac/spi-board.c LoraMac/delay.c LoraMac/sx1276.c LoraMac/sx1276-board.c LoraMac/LoRaMac.c LoraMac/LoRaMacAdr.c LoraMac/LoRaMacClassB.c LoraMac/LoRaMacCommands.c LoraMac/LoRaMacConfirmQueue.c LoraMac/LoRaMacCrypto.c LoraMac/LoRaMacParser.c LoraMac/LoRaMacSerializer.c LoraMac/timer.c LoraMac/utilities.c LoraMac/systime.c
LORAMAC_COMMON_SRC = LoraMac/common/LmHandler/LmHandler.c LoraMac/common/LmHandler/packages/FragDecoder.c LoraMac/common/LmHandler/packages/LmhpClockSync.c LoraMac/common/LmHandler/packages/LmhpCompliance.c LoraMac/common/LmHandler/packages/LmhpFragmentation.c LoraMac/common/LmHandler/packages/LmhpRemoteMcastSetup.c
LORAMAC_COMMON_SRC += LoraMac/common/CayenneLpp.c LoraMac/common/LmHandlerMsgDisplay.c
LORAMAC_SOFT_SE_SRC += LoraMac/soft-se/aes.c LoraMac/soft-se/cmac.c LoraMac/soft-se/soft-se-hal.c LoraMac/soft-se/soft-se.c
LORAMAC_REGION_SRC = LoraMac/region/RegionAS923.c LoraMac/region/RegionAU915.c LoraMac/region/RegionBaseUS.c LoraMac/region/Region.c LoraMac/region/RegionCN470A20.c LoraMac/region/RegionCN470A26.c LoraMac/region/RegionCN470B20.c LoraMac/region/RegionCN470B26.c LoraMac/region/RegionCN470.c LoraMac/region/RegionCN779.c LoraMac/region/RegionCommon.c LoraMac/region/RegionEU433.c LoraMac/region/RegionEU868.c LoraMac/region/RegionIN865.c LoraMac/region/RegionKR920.c LoraMac/region/RegionRU864.c LoraMac/region/RegionUS915.c 
TOP = ./civetweb
CIVETWEB_LIB = libcivetweb.a

CFLAGS = -I$(TOP)/include -IcJSON $(COPT) -DNO_FILES -DMG_EXPERIMENTAL_INTERFACES -DSOFT_SE
LIBS = -lpthread
INCLUDE = -ILoraMac -ILoraMac/region -ILoraMac/common -ILoraMac/common/LmHandler -ILoraMac/common/LmHandler/packages -ILoraMac/soft-se

include $(TOP)/resources/Makefile.in-os

ifeq ($(TARGET_OS),LINUX)
	LIBS += -ldl
	LIBS += -lm
endif

all: $(PROG)

$(PROG): $(CIVETWEB_LIB) $(LORAWAN_SRC) $(LORAMAC_COMMON_SRC) $(LORAMAC_REGION_SRC) $(LORAMAC_SOFT_SE_SRC)
	$(CC) -o $@ $(CFLAGS) $(INCLUDE) $(LDFLAGS) $(LORAWAN_SRC) $(LORAMAC_COMMON_SRC) $(LORAMAC_REGION_SRC) $(LORAMAC_SOFT_SE_SRC) $(CIVETWEB_LIB) $(LIBS)

$(CIVETWEB_LIB):
	$(MAKE) -C $(TOP) WITH_IPV6=1 WITH_WEBSOCKET=1 COPT='-DNO_SSL -DMG_EXPERIMENTAL_INTERFACES' clean lib
	cp $(TOP)/$(CIVETWEB_LIB) .

clean:
	rm -f $(CIVETWEB_LIB) $(PROG)

.PHONY: all clean

