# WSPR Design

## Introduction

WSPR is a stands for Weak Signal Propogation Reporter, more information can be found at [https://www.sigidwiki.com/wiki/WSPR]https://www.sigidwiki.com/wiki/WSPR

WSPR is designed to be able to report incredibly weak signals, and has various base points for detection around the world. This allows communication at an extremely slow speed for location data to provide telemetry. This can be seen on WSPRNet.

[http://www.wsprnet.org/drupal/WSPRnet/map]WSPRNet

For use the protocol requires an amateur radio license. For this project, I have  aquired one for use of WSPRNet, however anyone using this project will need to aquire a license for themselves in order to use this technology.

## Base Design

The project is based on [https://github.com/alexf91/WSPR-Beacon/]alexf91/WSPR-Beacon however has been completely rewritten.

The initial code was designed for an Arduino based solution with a USB interface. The re-write disregards the code apart from the WSPR encoding library.

A special mention here is [https://github.com/threeme3/WsprryPi]threeme3/WsprryPi