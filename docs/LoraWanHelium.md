# LoraWan, Integration with a LoraWan Network, etc.

## Introduction

LoraWan is based on two parts, described how it should be, but isn't really in the linux kernel (Not my Job!(?)!):
* PHY - A FSK based protocol called Lora. Essentially using Chirp Spectrum Spreading.
* MAC - The LoraWan protocol. Essentially an extremely basic IP stack which supports "UDP" and "TCP" packets. (They are in quotes because, actually it's closer to IPv4 but the devices have absolutely no idea what subnet they are on) 

## Getting the actual device to integrate with Linux.

This is suprisingly a tough one. 
* Easy part - Rockchip provided SPI device driver, which has a filesystem interface.
* Bad Part - No one seems to be able to have designed something which is suitable for this project (But starnight has done some amazing work!) [https://patchwork.kernel.org/project/linux-wpan/patch/20170910093026.4386-1-starnight@g.ncu.edu.tw/]

_Overall_ - there are two choices. 
* Adapt starnights work on a branch which he didn't try 802.15.4 as an implementation [https://github.com/starnight/LoRa/tree/file-ops]
* Hack the thing with python, and replace the RPi.GPIO library [https://github.com/raspberrypi-tw/lora-sx1276]

The former, while a PITA initially, actually provides what initially appears to be an easy to use interface to Lora, with functions such as spreading factor, band etc all controllable using ioctl calls.

It appears that Starnight has also produced a LoraWan driver, which is currently written for the 4.11 kernel. I've updated this to work with the 5.10 Kernel being used (Should upgrade this to use mainline at somepoint!). The driver has been missing several parts, such as coding for regions outside ones he lives in (He has a edu.tw email, and apparently they use AU923 over there!). I've started adding them in to stop the project having any black spots, but currently I've only added EU bands as that's the region I'm doing testing in.

