# Lorawan Bridge Design

## Introduction
A design for the "LoraWan Bridge". 
Essentially, this is a REST API to interface with supported technologies of the Semtec sx1276 chip.

## Rest API Design

Currently, only LoraWan will be supported, however the API has been designed to take advantage of other features of the sx1276 in the future.

### Lorawan Bridge API

The current endpoints are being developed:

#### GET /lorawan
{
    "functions": ["send","receive","mac","region"]
}

#### PUT /lorawan/send

_Example request_: 
{
    "port": 1234
    "payload64": "SGVsbG8gV29ybGQK"
}

_Example return_:
{
    "id": 1234
}

#### GET /lorawan/send_status

_Example request:_
/lorawan/send_status/1234

_Example returns:_
_Transmit done_:
{
    "status": "done"
}
_Oversize Packet:_
{
    "status": "oversize"
}
_In-process:_
{
    "status": "sending"
}
_No ack:_
{
    "status": "noack"
}

#### GET /lorawan/receive/latest

_Example Return:_
_OK_
{
    "id": 14
    "port": 1234
    "payload64": "SGVsbG8gV29ybGQK"
}
Payload64 is in base64 format.

#### GET /lorawan/receive/17
Returns the received data by ID of packet.
_Example Return:_
{
    "id": 17
    "port": 1234
    "payload64": "SGVsbG8gV29ybGQyCg=="
}
Payload64 is in base64 format.

#### GET /lorawan/mac
Returns transport layer information, note return data has been randomised :)
_Example Return:_
{
    "class": "A"
    "deviceEui": "6082E95C1E1BC609"
    "appEui": "6081F9474AEA66B4"
    "appKey": "5D617484FB8E396D4BEFEEBB1341B849"
}

#### PUT /lorawan/mac
Sets transport layer information
_Example Request:_
{
    "class": "A"
    "deviceEui": "6082E95C1E1BC609"
    "appEui": "6081F9474AEA66B4"
    "appKey": "5D617484FB8E396D4BEFEEBB1341B849"
}

#### GET /lorawan/region
Gets the lorawan region
_Example Return:_
{
    "region": "EU868"
}

#### PUT /lorawan/region
Sets the lorawan region
_Example Request:_
{
    "region": "AS923"
}
_Example Returns:_
_Region changed, or the same_
{
    "status": "ok"
}
_Region not recognized_
{
    "status": "notfound"
}

