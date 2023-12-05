# Design for transmitting images

## Introduction
The current design works with 2 Datagram message types, WSPR and Lora(Wan). Both are essentially from an abstracted point of view methods of data transmition

* LoraWan can send real packets
* WSPR sends a few bits of information for location and callsign - hidden underneath this though is the "pseudo-random" syncronisation vector, which could be exploited for additional data transmission.

Overall the purpose of this part of the project is to be able to decode the datagram transmissions back into images, while assuming an EXTREMELY high packet losdd.

