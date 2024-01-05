/*!
 * \file      Region.c
 *
 * \brief     Region implementation.
 *
 * \copyright Revised BSD License, see section \ref LICENSE.
 *
 * \code
 *                ______                              _
 *               / _____)             _              | |
 *              ( (____  _____ ____ _| |_ _____  ____| |__
 *               \____ \| ___ |    (_   _) ___ |/ ___)  _ \
 *               _____) ) ____| | | || |_| ____( (___| | | |
 *              (______/|_____)_|_|_| \__)_____)\____)_| |_|
 *              (C)2013-2017 Semtech
 *
 *               ___ _____ _   ___ _  _____ ___  ___  ___ ___
 *              / __|_   _/_\ / __| |/ / __/ _ \| _ \/ __| __|
 *              \__ \ | |/ _ \ (__| ' <| _| (_) |   / (__| _|
 *              |___/ |_/_/ \_\___|_|\_\_| \___/|_|_\\___|___|
 *              embedded.connectivity.solutions===============
 *
 * \endcode
 *
 * \author    Miguel Luis ( Semtech )
 *
 * \author    Gregory Cristian ( Semtech )
 *
 * \author    Daniel Jaeckle ( STACKFORCE )
 */
#include "LoRaMac.h"
#include "Region.h"
#include <stdio.h>


// Setup regions
//#ifdef REGION_AS923
#include "RegionAS923.h"
//#define AS923_CASE                                 case LORAMAC_REGION_AS923:
#define AS923_IS_ACTIVE( )                          { return true; }
#define AS923_GET_PHY_PARAM( )                      { return RegionAS923GetPhyParam( getPhy ); }
#define AS923_SET_BAND_TX_DONE( )                   { RegionAS923SetBandTxDone( txDone ); break; }
#define AS923_INIT_DEFAULTS( )                      { RegionAS923InitDefaults( params ); break; }
#define AS923_VERIFY( )                             { return RegionAS923Verify( verify, phyAttribute ); }
#define AS923_APPLY_CF_LIST( )                      { RegionAS923ApplyCFList( applyCFList ); break; }
#define AS923_CHAN_MASK_SET( )                      { return RegionAS923ChanMaskSet( chanMaskSet ); }
#define AS923_COMPUTE_RX_WINDOW_PARAMETERS( )       { RegionAS923ComputeRxWindowParameters( datarate, minRxSymbols, rxError, rxConfigParams ); break; }
#define AS923_RX_CONFIG( )                          { return RegionAS923RxConfig( rxConfig, datarate ); }
#define AS923_TX_CONFIG( )                          { return RegionAS923TxConfig( txConfig, txPower, txTimeOnAir ); }
#define AS923_LINK_ADR_REQ( )                       { return RegionAS923LinkAdrReq( linkAdrReq, drOut, txPowOut, nbRepOut, nbBytesParsed ); }
#define AS923_RX_PARAM_SETUP_REQ( )                 { return RegionAS923RxParamSetupReq( rxParamSetupReq ); }
#define AS923_NEW_CHANNEL_REQ( )                    { return RegionAS923NewChannelReq( newChannelReq ); }
#define AS923_TX_PARAM_SETUP_REQ( )                 { return RegionAS923TxParamSetupReq( txParamSetupReq ); }
#define AS923_DL_CHANNEL_REQ( )                     { return RegionAS923DlChannelReq( dlChannelReq ); }
#define AS923_ALTERNATE_DR( )                       { return RegionAS923AlternateDr( currentDr, type ); }
#define AS923_NEXT_CHANNEL( )                       { return RegionAS923NextChannel( nextChanParams, channel, time, aggregatedTimeOff ); }
#define AS923_CHANNEL_ADD( )                        { return RegionAS923ChannelAdd( channelAdd ); }
#define AS923_CHANNEL_REMOVE( )                     { return RegionAS923ChannelsRemove( channelRemove ); }
#define AS923_APPLY_DR_OFFSET( )                    { return RegionAS923ApplyDrOffset( downlinkDwellTime, dr, drOffset ); }
#define AS923_RX_BEACON_SETUP( )                    { RegionAS923RxBeaconSetup( rxBeaconSetup, outDr ); break; }
// #else
// #define AS923_IS_ACTIVE( )
// #define AS923_GET_PHY_PARAM( )
// #define AS923_SET_BAND_TX_DONE( )
// #define AS923_INIT_DEFAULTS( )
// #define AS923_GET_NVM_CTX( )
// #define AS923_VERIFY( )
// #define AS923_APPLY_CF_LIST( )
// #define AS923_CHAN_MASK_SET( )
// #define AS923_COMPUTE_RX_WINDOW_PARAMETERS( )
// #define AS923_RX_CONFIG( )
// #define AS923_TX_CONFIG( )
// #define AS923_LINK_ADR_REQ( )
// #define AS923_RX_PARAM_SETUP_REQ( )
// #define AS923_NEW_CHANNEL_REQ( )
// #define AS923_TX_PARAM_SETUP_REQ( )
// #define AS923_DL_CHANNEL_REQ( )
// #define AS923_ALTERNATE_DR( )
// #define AS923_NEXT_CHANNEL( )
// #define AS923_CHANNEL_ADD( )
// #define AS923_CHANNEL_REMOVE( )
// #define AS923_APPLY_DR_OFFSET( )
// #define AS923_RX_BEACON_SETUP( )
// #endif

//#ifdef REGION_AU915
#include "RegionAU915.h"
//#define AU915_CASE                                 case LORAMAC_REGION_AU915:
#define AU915_IS_ACTIVE( )                          { return true; }
#define AU915_GET_PHY_PARAM( )                      { return RegionAU915GetPhyParam( getPhy ); }
#define AU915_SET_BAND_TX_DONE( )                   { RegionAU915SetBandTxDone( txDone ); break; }
#define AU915_INIT_DEFAULTS( )                      { RegionAU915InitDefaults( params ); break; }
#define AU915_VERIFY( )                             { return RegionAU915Verify( verify, phyAttribute ); }
#define AU915_APPLY_CF_LIST( )                      { RegionAU915ApplyCFList( applyCFList ); break; }
#define AU915_CHAN_MASK_SET( )                      { return RegionAU915ChanMaskSet( chanMaskSet ); }
#define AU915_COMPUTE_RX_WINDOW_PARAMETERS( )       { RegionAU915ComputeRxWindowParameters( datarate, minRxSymbols, rxError, rxConfigParams ); break; }
#define AU915_RX_CONFIG( )                          { return RegionAU915RxConfig( rxConfig, datarate ); }
#define AU915_TX_CONFIG( )                          { return RegionAU915TxConfig( txConfig, txPower, txTimeOnAir ); }
#define AU915_LINK_ADR_REQ( )                       { return RegionAU915LinkAdrReq( linkAdrReq, drOut, txPowOut, nbRepOut, nbBytesParsed ); }
#define AU915_RX_PARAM_SETUP_REQ( )                 { return RegionAU915RxParamSetupReq( rxParamSetupReq ); }
#define AU915_NEW_CHANNEL_REQ( )                    { return RegionAU915NewChannelReq( newChannelReq ); }
#define AU915_TX_PARAM_SETUP_REQ( )                 { return RegionAU915TxParamSetupReq( txParamSetupReq ); }
#define AU915_DL_CHANNEL_REQ( )                     { return RegionAU915DlChannelReq( dlChannelReq ); }
#define AU915_ALTERNATE_DR( )                       { return RegionAU915AlternateDr( currentDr, type ); }
#define AU915_NEXT_CHANNEL( )                       { return RegionAU915NextChannel( nextChanParams, channel, time, aggregatedTimeOff ); }
#define AU915_CHANNEL_ADD( )                        { return RegionAU915ChannelAdd( channelAdd ); }
#define AU915_CHANNEL_REMOVE( )                     { return RegionAU915ChannelsRemove( channelRemove ); }
#define AU915_APPLY_DR_OFFSET( )                    { return RegionAU915ApplyDrOffset( downlinkDwellTime, dr, drOffset ); }
#define AU915_RX_BEACON_SETUP( )                    { RegionAU915RxBeaconSetup( rxBeaconSetup, outDr ); break; }
// #else
// #define AU915_IS_ACTIVE( )
// #define AU915_GET_PHY_PARAM( )
// #define AU915_SET_BAND_TX_DONE( )
// #define AU915_INIT_DEFAULTS( )
// #define AU915_GET_NVM_CTX( )
// #define AU915_VERIFY( )
// #define AU915_APPLY_CF_LIST( )
// #define AU915_CHAN_MASK_SET( )
// #define AU915_COMPUTE_RX_WINDOW_PARAMETERS( )
// #define AU915_RX_CONFIG( )
// #define AU915_TX_CONFIG( )
// #define AU915_LINK_ADR_REQ( )
// #define AU915_RX_PARAM_SETUP_REQ( )
// #define AU915_NEW_CHANNEL_REQ( )
// #define AU915_TX_PARAM_SETUP_REQ( )
// #define AU915_DL_CHANNEL_REQ( )
// #define AU915_ALTERNATE_DR( )
// #define AU915_NEXT_CHANNEL( )
// #define AU915_CHANNEL_ADD( )
// #define AU915_CHANNEL_REMOVE( )
// #define AU915_APPLY_DR_OFFSET( )
// #define AU915_RX_BEACON_SETUP( )
// #endif

// #ifdef REGION_CN470
#include "RegionCN470.h"
//#define CN470_CASE                                 case LORAMAC_REGION_CN470:
#define CN470_IS_ACTIVE( )                          { return true; }
#define CN470_GET_PHY_PARAM( )                      { return RegionCN470GetPhyParam( getPhy ); }
#define CN470_SET_BAND_TX_DONE( )                   { RegionCN470SetBandTxDone( txDone ); break; }
#define CN470_INIT_DEFAULTS( )                      { RegionCN470InitDefaults( params ); break; }
#define CN470_VERIFY( )                             { return RegionCN470Verify( verify, phyAttribute ); }
#define CN470_APPLY_CF_LIST( )                      { RegionCN470ApplyCFList( applyCFList ); break; }
#define CN470_CHAN_MASK_SET( )                      { return RegionCN470ChanMaskSet( chanMaskSet ); }
#define CN470_COMPUTE_RX_WINDOW_PARAMETERS( )       { RegionCN470ComputeRxWindowParameters( datarate, minRxSymbols, rxError, rxConfigParams ); break; }
#define CN470_RX_CONFIG( )                          { return RegionCN470RxConfig( rxConfig, datarate ); }
#define CN470_TX_CONFIG( )                          { return RegionCN470TxConfig( txConfig, txPower, txTimeOnAir ); }
#define CN470_LINK_ADR_REQ( )                       { return RegionCN470LinkAdrReq( linkAdrReq, drOut, txPowOut, nbRepOut, nbBytesParsed ); }
#define CN470_RX_PARAM_SETUP_REQ( )                 { return RegionCN470RxParamSetupReq( rxParamSetupReq ); }
#define CN470_NEW_CHANNEL_REQ( )                    { return RegionCN470NewChannelReq( newChannelReq ); }
#define CN470_TX_PARAM_SETUP_REQ( )                 { return RegionCN470TxParamSetupReq( txParamSetupReq ); }
#define CN470_DL_CHANNEL_REQ( )                     { return RegionCN470DlChannelReq( dlChannelReq ); }
#define CN470_ALTERNATE_DR( )                       { return RegionCN470AlternateDr( currentDr, type ); }
#define CN470_NEXT_CHANNEL( )                       { return RegionCN470NextChannel( nextChanParams, channel, time, aggregatedTimeOff ); }
#define CN470_CHANNEL_ADD( )                        { return RegionCN470ChannelAdd( channelAdd ); }
#define CN470_CHANNEL_REMOVE( )                     { return RegionCN470ChannelsRemove( channelRemove ); }
#define CN470_APPLY_DR_OFFSET( )                    { return RegionCN470ApplyDrOffset( downlinkDwellTime, dr, drOffset ); }
#define CN470_RX_BEACON_SETUP( )                    { RegionCN470RxBeaconSetup( rxBeaconSetup, outDr ); break; }
// #else
// #define CN470_IS_ACTIVE( )
// #define CN470_GET_PHY_PARAM( )
// #define CN470_SET_BAND_TX_DONE( )
// #define CN470_INIT_DEFAULTS( )
// #define CN470_GET_NVM_CTX( )
// #define CN470_VERIFY( )
// #define CN470_APPLY_CF_LIST( )
// #define CN470_CHAN_MASK_SET( )
// #define CN470_COMPUTE_RX_WINDOW_PARAMETERS( )
// #define CN470_RX_CONFIG( )
// #define CN470_TX_CONFIG( )
// #define CN470_LINK_ADR_REQ( )
// #define CN470_RX_PARAM_SETUP_REQ( )
// #define CN470_NEW_CHANNEL_REQ( )
// #define CN470_TX_PARAM_SETUP_REQ( )
// #define CN470_DL_CHANNEL_REQ( )
// #define CN470_ALTERNATE_DR( )
// #define CN470_NEXT_CHANNEL( )
// #define CN470_CHANNEL_ADD( )
// #define CN470_CHANNEL_REMOVE( )
// #define CN470_APPLY_DR_OFFSET( )
// #define CN470_RX_BEACON_SETUP( )
// #endif

// #ifdef REGION_CN779
#include "RegionCN779.h"
//#define CN779_CASE                                 case LORAMAC_REGION_CN779:
#define CN779_IS_ACTIVE( )                          { return true; }
#define CN779_GET_PHY_PARAM( )                      { return RegionCN779GetPhyParam( getPhy ); }
#define CN779_SET_BAND_TX_DONE( )                   { RegionCN779SetBandTxDone( txDone ); break; }
#define CN779_INIT_DEFAULTS( )                      { RegionCN779InitDefaults( params ); break; }
#define CN779_VERIFY( )                             { return RegionCN779Verify( verify, phyAttribute ); }
#define CN779_APPLY_CF_LIST( )                      { RegionCN779ApplyCFList( applyCFList ); break; }
#define CN779_CHAN_MASK_SET( )                      { return RegionCN779ChanMaskSet( chanMaskSet ); }
#define CN779_COMPUTE_RX_WINDOW_PARAMETERS( )       { RegionCN779ComputeRxWindowParameters( datarate, minRxSymbols, rxError, rxConfigParams ); break; }
#define CN779_RX_CONFIG( )                          { return RegionCN779RxConfig( rxConfig, datarate ); }
#define CN779_TX_CONFIG( )                          { return RegionCN779TxConfig( txConfig, txPower, txTimeOnAir ); }
#define CN779_LINK_ADR_REQ( )                       { return RegionCN779LinkAdrReq( linkAdrReq, drOut, txPowOut, nbRepOut, nbBytesParsed ); }
#define CN779_RX_PARAM_SETUP_REQ( )                 { return RegionCN779RxParamSetupReq( rxParamSetupReq ); }
#define CN779_NEW_CHANNEL_REQ( )                    { return RegionCN779NewChannelReq( newChannelReq ); }
#define CN779_TX_PARAM_SETUP_REQ( )                 { return RegionCN779TxParamSetupReq( txParamSetupReq ); }
#define CN779_DL_CHANNEL_REQ( )                     { return RegionCN779DlChannelReq( dlChannelReq ); }
#define CN779_ALTERNATE_DR( )                       { return RegionCN779AlternateDr( currentDr, type ); }
#define CN779_NEXT_CHANNEL( )                       { return RegionCN779NextChannel( nextChanParams, channel, time, aggregatedTimeOff ); }
#define CN779_CHANNEL_ADD( )                        { return RegionCN779ChannelAdd( channelAdd ); }
#define CN779_CHANNEL_REMOVE( )                     { return RegionCN779ChannelsRemove( channelRemove ); }
#define CN779_APPLY_DR_OFFSET( )                    { return RegionCN779ApplyDrOffset( downlinkDwellTime, dr, drOffset ); }
#define CN779_RX_BEACON_SETUP( )                    { RegionCN779RxBeaconSetup( rxBeaconSetup, outDr ); break; }
// #else
// #define CN779_IS_ACTIVE( )
// #define CN779_GET_PHY_PARAM( )
// #define CN779_SET_BAND_TX_DONE( )
// #define CN779_INIT_DEFAULTS( )
// #define CN779_GET_NVM_CTX( )
// #define CN779_VERIFY( )
// #define CN779_APPLY_CF_LIST( )
// #define CN779_CHAN_MASK_SET( )
// #define CN779_COMPUTE_RX_WINDOW_PARAMETERS( )
// #define CN779_RX_CONFIG( )
// #define CN779_TX_CONFIG( )
// #define CN779_LINK_ADR_REQ( )
// #define CN779_RX_PARAM_SETUP_REQ( )
// #define CN779_NEW_CHANNEL_REQ( )
// #define CN779_TX_PARAM_SETUP_REQ( )
// #define CN779_DL_CHANNEL_REQ( )
// #define CN779_ALTERNATE_DR( )
// #define CN779_NEXT_CHANNEL( )
// #define CN779_CHANNEL_ADD( )
// #define CN779_CHANNEL_REMOVE( )
// #define CN779_APPLY_DR_OFFSET( )
// #define CN779_RX_BEACON_SETUP( )
// #endif

// #ifdef REGION_EU433
#include "RegionEU433.h"
//#define EU433_CASE                                 case LORAMAC_REGION_EU433:
#define EU433_IS_ACTIVE( )                          { return true; }
#define EU433_GET_PHY_PARAM( )                      { return RegionEU433GetPhyParam( getPhy ); }
#define EU433_SET_BAND_TX_DONE( )                   { RegionEU433SetBandTxDone( txDone ); break; }
#define EU433_INIT_DEFAULTS( )                      { RegionEU433InitDefaults( params ); break; }
#define EU433_VERIFY( )                             { return RegionEU433Verify( verify, phyAttribute ); }
#define EU433_APPLY_CF_LIST( )                      { RegionEU433ApplyCFList( applyCFList ); break; }
#define EU433_CHAN_MASK_SET( )                      { return RegionEU433ChanMaskSet( chanMaskSet ); }
#define EU433_COMPUTE_RX_WINDOW_PARAMETERS( )       { RegionEU433ComputeRxWindowParameters( datarate, minRxSymbols, rxError, rxConfigParams ); break; }
#define EU433_RX_CONFIG( )                          { return RegionEU433RxConfig( rxConfig, datarate ); }
#define EU433_TX_CONFIG( )                          { return RegionEU433TxConfig( txConfig, txPower, txTimeOnAir ); }
#define EU433_LINK_ADR_REQ( )                       { return RegionEU433LinkAdrReq( linkAdrReq, drOut, txPowOut, nbRepOut, nbBytesParsed ); }
#define EU433_RX_PARAM_SETUP_REQ( )                 { return RegionEU433RxParamSetupReq( rxParamSetupReq ); }
#define EU433_NEW_CHANNEL_REQ( )                    { return RegionEU433NewChannelReq( newChannelReq ); }
#define EU433_TX_PARAM_SETUP_REQ( )                 { return RegionEU433TxParamSetupReq( txParamSetupReq ); }
#define EU433_DL_CHANNEL_REQ( )                     { return RegionEU433DlChannelReq( dlChannelReq ); }
#define EU433_ALTERNATE_DR( )                       { return RegionEU433AlternateDr( currentDr, type ); }
#define EU433_NEXT_CHANNEL( )                       { return RegionEU433NextChannel( nextChanParams, channel, time, aggregatedTimeOff ); }
#define EU433_CHANNEL_ADD( )                        { return RegionEU433ChannelAdd( channelAdd ); }
#define EU433_CHANNEL_REMOVE( )                     { return RegionEU433ChannelsRemove( channelRemove ); }
#define EU433_APPLY_DR_OFFSET( )                    { return RegionEU433ApplyDrOffset( downlinkDwellTime, dr, drOffset ); }
#define EU433_RX_BEACON_SETUP( )                    { RegionEU433RxBeaconSetup( rxBeaconSetup, outDr ); break; }
// #else
// #define EU433_IS_ACTIVE( )
// #define EU433_GET_PHY_PARAM( )
// #define EU433_SET_BAND_TX_DONE( )
// #define EU433_INIT_DEFAULTS( )
// #define EU433_GET_NVM_CTX( )
// #define EU433_VERIFY( )
// #define EU433_APPLY_CF_LIST( )
// #define EU433_CHAN_MASK_SET( )
// #define EU433_COMPUTE_RX_WINDOW_PARAMETERS( )
// #define EU433_RX_CONFIG( )
// #define EU433_TX_CONFIG( )
// #define EU433_LINK_ADR_REQ( )
// #define EU433_RX_PARAM_SETUP_REQ( )
// #define EU433_NEW_CHANNEL_REQ( )
// #define EU433_TX_PARAM_SETUP_REQ( )
// #define EU433_DL_CHANNEL_REQ( )
// #define EU433_ALTERNATE_DR( )
// #define EU433_NEXT_CHANNEL( )
// #define EU433_CHANNEL_ADD( )
// #define EU433_CHANNEL_REMOVE( )
// #define EU433_APPLY_DR_OFFSET( )
// #define EU433_RX_BEACON_SETUP( )
// #endif

// #ifdef REGION_EU868
#include "RegionEU868.h"
//#define EU868_CASE                                 case LORAMAC_REGION_EU868:
#define EU868_IS_ACTIVE( )                          { return true; }
#define EU868_GET_PHY_PARAM( )                      { return RegionEU868GetPhyParam( getPhy ); }
#define EU868_SET_BAND_TX_DONE( )                   { RegionEU868SetBandTxDone( txDone ); break; }
#define EU868_INIT_DEFAULTS( )                      { RegionEU868InitDefaults( params ); break; }
#define EU868_VERIFY( )                             { return RegionEU868Verify( verify, phyAttribute ); }
#define EU868_APPLY_CF_LIST( )                      { RegionEU868ApplyCFList( applyCFList ); break; }
#define EU868_CHAN_MASK_SET( )                      { return RegionEU868ChanMaskSet( chanMaskSet ); }
#define EU868_COMPUTE_RX_WINDOW_PARAMETERS( )       { RegionEU868ComputeRxWindowParameters( datarate, minRxSymbols, rxError, rxConfigParams ); break; }
#define EU868_RX_CONFIG( )                          { return RegionEU868RxConfig( rxConfig, datarate ); }
#define EU868_TX_CONFIG( )                          { return RegionEU868TxConfig( txConfig, txPower, txTimeOnAir ); }
#define EU868_LINK_ADR_REQ( )                       { return RegionEU868LinkAdrReq( linkAdrReq, drOut, txPowOut, nbRepOut, nbBytesParsed ); }
#define EU868_RX_PARAM_SETUP_REQ( )                 { return RegionEU868RxParamSetupReq( rxParamSetupReq ); }
#define EU868_NEW_CHANNEL_REQ( )                    { return RegionEU868NewChannelReq( newChannelReq ); }
#define EU868_TX_PARAM_SETUP_REQ( )                 { return RegionEU868TxParamSetupReq( txParamSetupReq ); }
#define EU868_DL_CHANNEL_REQ( )                     { return RegionEU868DlChannelReq( dlChannelReq ); }
#define EU868_ALTERNATE_DR( )                       { return RegionEU868AlternateDr( currentDr, type ); }
#define EU868_NEXT_CHANNEL( )                       { return RegionEU868NextChannel( nextChanParams, channel, time, aggregatedTimeOff ); }
#define EU868_CHANNEL_ADD( )                        { return RegionEU868ChannelAdd( channelAdd ); }
#define EU868_CHANNEL_REMOVE( )                     { return RegionEU868ChannelsRemove( channelRemove ); }
#define EU868_APPLY_DR_OFFSET( )                    { return RegionEU868ApplyDrOffset( downlinkDwellTime, dr, drOffset ); }
#define EU868_RX_BEACON_SETUP( )                    { RegionEU868RxBeaconSetup( rxBeaconSetup, outDr ); break; }
// #else
// #define EU868_IS_ACTIVE( )
// #define EU868_GET_PHY_PARAM( )
// #define EU868_SET_BAND_TX_DONE( )
// #define EU868_INIT_DEFAULTS( )
// #define EU868_GET_NVM_CTX( )
// #define EU868_VERIFY( )
// #define EU868_APPLY_CF_LIST( )
// #define EU868_CHAN_MASK_SET( )
// #define EU868_COMPUTE_RX_WINDOW_PARAMETERS( )
// #define EU868_RX_CONFIG( )
// #define EU868_TX_CONFIG( )
// #define EU868_LINK_ADR_REQ( )
// #define EU868_RX_PARAM_SETUP_REQ( )
// #define EU868_NEW_CHANNEL_REQ( )
// #define EU868_TX_PARAM_SETUP_REQ( )
// #define EU868_DL_CHANNEL_REQ( )
// #define EU868_ALTERNATE_DR( )
// #define EU868_NEXT_CHANNEL( )
// #define EU868_CHANNEL_ADD( )
// #define EU868_CHANNEL_REMOVE( )
// #define EU868_APPLY_DR_OFFSET( )
// #define EU868_RX_BEACON_SETUP( )
// #endif

// #ifdef REGION_KR920
#include "RegionKR920.h"
//#define KR920_CASE                                 case LORAMAC_REGION_KR920:
#define KR920_IS_ACTIVE( )                          { return true; }
#define KR920_GET_PHY_PARAM( )                      { return RegionKR920GetPhyParam( getPhy ); }
#define KR920_SET_BAND_TX_DONE( )                   { RegionKR920SetBandTxDone( txDone ); break; }
#define KR920_INIT_DEFAULTS( )                      { RegionKR920InitDefaults( params ); break; }
#define KR920_VERIFY( )                             { return RegionKR920Verify( verify, phyAttribute ); }
#define KR920_APPLY_CF_LIST( )                      { RegionKR920ApplyCFList( applyCFList ); break; }
#define KR920_CHAN_MASK_SET( )                      { return RegionKR920ChanMaskSet( chanMaskSet ); }
#define KR920_COMPUTE_RX_WINDOW_PARAMETERS( )       { RegionKR920ComputeRxWindowParameters( datarate, minRxSymbols, rxError, rxConfigParams ); break; }
#define KR920_RX_CONFIG( )                          { return RegionKR920RxConfig( rxConfig, datarate ); }
#define KR920_TX_CONFIG( )                          { return RegionKR920TxConfig( txConfig, txPower, txTimeOnAir ); }
#define KR920_LINK_ADR_REQ( )                       { return RegionKR920LinkAdrReq( linkAdrReq, drOut, txPowOut, nbRepOut, nbBytesParsed ); }
#define KR920_RX_PARAM_SETUP_REQ( )                 { return RegionKR920RxParamSetupReq( rxParamSetupReq ); }
#define KR920_NEW_CHANNEL_REQ( )                    { return RegionKR920NewChannelReq( newChannelReq ); }
#define KR920_TX_PARAM_SETUP_REQ( )                 { return RegionKR920TxParamSetupReq( txParamSetupReq ); }
#define KR920_DL_CHANNEL_REQ( )                     { return RegionKR920DlChannelReq( dlChannelReq ); }
#define KR920_ALTERNATE_DR( )                       { return RegionKR920AlternateDr( currentDr, type ); }
#define KR920_NEXT_CHANNEL( )                       { return RegionKR920NextChannel( nextChanParams, channel, time, aggregatedTimeOff ); }
#define KR920_CHANNEL_ADD( )                        { return RegionKR920ChannelAdd( channelAdd ); }
#define KR920_CHANNEL_REMOVE( )                     { return RegionKR920ChannelsRemove( channelRemove ); }
#define KR920_APPLY_DR_OFFSET( )                    { return RegionKR920ApplyDrOffset( downlinkDwellTime, dr, drOffset ); }
#define KR920_RX_BEACON_SETUP( )                    { RegionKR920RxBeaconSetup( rxBeaconSetup, outDr ); break; }
// #else
// #define KR920_IS_ACTIVE( )
// #define KR920_GET_PHY_PARAM( )
// #define KR920_SET_BAND_TX_DONE( )
// #define KR920_INIT_DEFAULTS( )
// #define KR920_GET_NVM_CTX( )
// #define KR920_VERIFY( )
// #define KR920_APPLY_CF_LIST( )
// #define KR920_CHAN_MASK_SET( )
// #define KR920_COMPUTE_RX_WINDOW_PARAMETERS( )
// #define KR920_RX_CONFIG( )
// #define KR920_TX_CONFIG( )
// #define KR920_LINK_ADR_REQ( )
// #define KR920_RX_PARAM_SETUP_REQ( )
// #define KR920_NEW_CHANNEL_REQ( )
// #define KR920_TX_PARAM_SETUP_REQ( )
// #define KR920_DL_CHANNEL_REQ( )
// #define KR920_ALTERNATE_DR( )
// #define KR920_NEXT_CHANNEL( )
// #define KR920_CHANNEL_ADD( )
// #define KR920_CHANNEL_REMOVE( )
// #define KR920_APPLY_DR_OFFSET( )
// #define KR920_RX_BEACON_SETUP( )
// #endif

// #ifdef REGION_IN865
#include "RegionIN865.h"
//#define IN865_CASE                                 case LORAMAC_REGION_IN865:
#define IN865_IS_ACTIVE( )                          { return true; }
#define IN865_GET_PHY_PARAM( )                      { return RegionIN865GetPhyParam( getPhy ); }
#define IN865_SET_BAND_TX_DONE( )                   { RegionIN865SetBandTxDone( txDone ); break; }
#define IN865_INIT_DEFAULTS( )                      { RegionIN865InitDefaults( params ); break; }
#define IN865_VERIFY( )                             { return RegionIN865Verify( verify, phyAttribute ); }
#define IN865_APPLY_CF_LIST( )                      { RegionIN865ApplyCFList( applyCFList ); break; }
#define IN865_CHAN_MASK_SET( )                      { return RegionIN865ChanMaskSet( chanMaskSet ); }
#define IN865_COMPUTE_RX_WINDOW_PARAMETERS( )       { RegionIN865ComputeRxWindowParameters( datarate, minRxSymbols, rxError, rxConfigParams ); break; }
#define IN865_RX_CONFIG( )                          { return RegionIN865RxConfig( rxConfig, datarate ); }
#define IN865_TX_CONFIG( )                          { return RegionIN865TxConfig( txConfig, txPower, txTimeOnAir ); }
#define IN865_LINK_ADR_REQ( )                       { return RegionIN865LinkAdrReq( linkAdrReq, drOut, txPowOut, nbRepOut, nbBytesParsed ); }
#define IN865_RX_PARAM_SETUP_REQ( )                 { return RegionIN865RxParamSetupReq( rxParamSetupReq ); }
#define IN865_NEW_CHANNEL_REQ( )                    { return RegionIN865NewChannelReq( newChannelReq ); }
#define IN865_TX_PARAM_SETUP_REQ( )                 { return RegionIN865TxParamSetupReq( txParamSetupReq ); }
#define IN865_DL_CHANNEL_REQ( )                     { return RegionIN865DlChannelReq( dlChannelReq ); }
#define IN865_ALTERNATE_DR( )                       { return RegionIN865AlternateDr( currentDr, type ); }
#define IN865_NEXT_CHANNEL( )                       { return RegionIN865NextChannel( nextChanParams, channel, time, aggregatedTimeOff ); }
#define IN865_CHANNEL_ADD( )                        { return RegionIN865ChannelAdd( channelAdd ); }
#define IN865_CHANNEL_REMOVE( )                     { return RegionIN865ChannelsRemove( channelRemove ); }
#define IN865_APPLY_DR_OFFSET( )                    { return RegionIN865ApplyDrOffset( downlinkDwellTime, dr, drOffset ); }
#define IN865_RX_BEACON_SETUP( )                    { RegionIN865RxBeaconSetup( rxBeaconSetup, outDr ); break; }
// #else
// #define IN865_IS_ACTIVE( )
// #define IN865_GET_PHY_PARAM( )
// #define IN865_SET_BAND_TX_DONE( )
// #define IN865_INIT_DEFAULTS( )
// #define IN865_GET_NVM_CTX( )
// #define IN865_VERIFY( )
// #define IN865_APPLY_CF_LIST( )
// #define IN865_CHAN_MASK_SET( )
// #define IN865_COMPUTE_RX_WINDOW_PARAMETERS( )
// #define IN865_RX_CONFIG( )
// #define IN865_TX_CONFIG( )
// #define IN865_LINK_ADR_REQ( )
// #define IN865_RX_PARAM_SETUP_REQ( )
// #define IN865_NEW_CHANNEL_REQ( )
// #define IN865_TX_PARAM_SETUP_REQ( )
// #define IN865_DL_CHANNEL_REQ( )
// #define IN865_ALTERNATE_DR( )
// #define IN865_NEXT_CHANNEL( )
// #define IN865_CHANNEL_ADD( )
// #define IN865_CHANNEL_REMOVE( )
// #define IN865_APPLY_DR_OFFSET( )
// #define IN865_RX_BEACON_SETUP( )
// #endif

// #ifdef REGION_US915
#include "RegionUS915.h"
//#define US915_CASE                                 case LORAMAC_REGION_US915:
#define US915_IS_ACTIVE( )                          { return true; }
#define US915_GET_PHY_PARAM( )                      { return RegionUS915GetPhyParam( getPhy ); }
#define US915_SET_BAND_TX_DONE( )                   { RegionUS915SetBandTxDone( txDone ); break; }
#define US915_INIT_DEFAULTS( )                      { RegionUS915InitDefaults( params ); break; }
#define US915_VERIFY( )                             { return RegionUS915Verify( verify, phyAttribute ); }
#define US915_APPLY_CF_LIST( )                      { RegionUS915ApplyCFList( applyCFList ); break; }
#define US915_CHAN_MASK_SET( )                      { return RegionUS915ChanMaskSet( chanMaskSet ); }
#define US915_COMPUTE_RX_WINDOW_PARAMETERS( )       { RegionUS915ComputeRxWindowParameters( datarate, minRxSymbols, rxError, rxConfigParams ); break; }
#define US915_RX_CONFIG( )                          { return RegionUS915RxConfig( rxConfig, datarate ); }
#define US915_TX_CONFIG( )                          { return RegionUS915TxConfig( txConfig, txPower, txTimeOnAir ); }
#define US915_LINK_ADR_REQ( )                       { return RegionUS915LinkAdrReq( linkAdrReq, drOut, txPowOut, nbRepOut, nbBytesParsed ); }
#define US915_RX_PARAM_SETUP_REQ( )                 { return RegionUS915RxParamSetupReq( rxParamSetupReq ); }
#define US915_NEW_CHANNEL_REQ( )                    { return RegionUS915NewChannelReq( newChannelReq ); }
#define US915_TX_PARAM_SETUP_REQ( )                 { return RegionUS915TxParamSetupReq( txParamSetupReq ); }
#define US915_DL_CHANNEL_REQ( )                     { return RegionUS915DlChannelReq( dlChannelReq ); }
#define US915_ALTERNATE_DR( )                       { return RegionUS915AlternateDr( currentDr, type ); }
#define US915_NEXT_CHANNEL( )                       { return RegionUS915NextChannel( nextChanParams, channel, time, aggregatedTimeOff ); }
#define US915_CHANNEL_ADD( )                        { return RegionUS915ChannelAdd( channelAdd ); }
#define US915_CHANNEL_REMOVE( )                     { return RegionUS915ChannelsRemove( channelRemove ); }
#define US915_APPLY_DR_OFFSET( )                    { return RegionUS915ApplyDrOffset( downlinkDwellTime, dr, drOffset ); }
#define US915_RX_BEACON_SETUP( )                    { RegionUS915RxBeaconSetup( rxBeaconSetup, outDr ); break; }
// #else
// #define US915_IS_ACTIVE( )
// #define US915_GET_PHY_PARAM( )
// #define US915_SET_BAND_TX_DONE( )
// #define US915_INIT_DEFAULTS( )
// #define US915_GET_NVM_CTX( )
// #define US915_VERIFY( )
// #define US915_APPLY_CF_LIST( )
// #define US915_CHAN_MASK_SET( )
// #define US915_COMPUTE_RX_WINDOW_PARAMETERS( )
// #define US915_RX_CONFIG( )
// #define US915_TX_CONFIG( )
// #define US915_LINK_ADR_REQ( )
// #define US915_RX_PARAM_SETUP_REQ( )
// #define US915_NEW_CHANNEL_REQ( )
// #define US915_TX_PARAM_SETUP_REQ( )
// #define US915_DL_CHANNEL_REQ( )
// #define US915_ALTERNATE_DR( )
// #define US915_NEXT_CHANNEL( )
// #define US915_CHANNEL_ADD( )
// #define US915_CHANNEL_REMOVE( )
// #define US915_APPLY_DR_OFFSET( )
// #define US915_RX_BEACON_SETUP( )
// #endif

// #ifdef REGION_RU864
#include "RegionRU864.h"
//#define RU864_CASE                                 case LORAMAC_REGION_RU864:
#define RU864_IS_ACTIVE( )                          { return true; }
#define RU864_GET_PHY_PARAM( )                      { return RegionRU864GetPhyParam( getPhy ); }
#define RU864_SET_BAND_TX_DONE( )                   { RegionRU864SetBandTxDone( txDone ); break; }
#define RU864_INIT_DEFAULTS( )                      { RegionRU864InitDefaults( params ); break; }
#define RU864_VERIFY( )                             { return RegionRU864Verify( verify, phyAttribute ); }
#define RU864_APPLY_CF_LIST( )                      { RegionRU864ApplyCFList( applyCFList ); break; }
#define RU864_CHAN_MASK_SET( )                      { return RegionRU864ChanMaskSet( chanMaskSet ); }
#define RU864_COMPUTE_RX_WINDOW_PARAMETERS( )       { RegionRU864ComputeRxWindowParameters( datarate, minRxSymbols, rxError, rxConfigParams ); break; }
#define RU864_RX_CONFIG( )                          { return RegionRU864RxConfig( rxConfig, datarate ); }
#define RU864_TX_CONFIG( )                          { return RegionRU864TxConfig( txConfig, txPower, txTimeOnAir ); }
#define RU864_LINK_ADR_REQ( )                       { return RegionRU864LinkAdrReq( linkAdrReq, drOut, txPowOut, nbRepOut, nbBytesParsed ); }
#define RU864_RX_PARAM_SETUP_REQ( )                 { return RegionRU864RxParamSetupReq( rxParamSetupReq ); }
#define RU864_NEW_CHANNEL_REQ( )                    { return RegionRU864NewChannelReq( newChannelReq ); }
#define RU864_TX_PARAM_SETUP_REQ( )                 { return RegionRU864TxParamSetupReq( txParamSetupReq ); }
#define RU864_DL_CHANNEL_REQ( )                     { return RegionRU864DlChannelReq( dlChannelReq ); }
#define RU864_ALTERNATE_DR( )                       { return RegionRU864AlternateDr( currentDr, type ); }
#define RU864_NEXT_CHANNEL( )                       { return RegionRU864NextChannel( nextChanParams, channel, time, aggregatedTimeOff ); }
#define RU864_CHANNEL_ADD( )                        { return RegionRU864ChannelAdd( channelAdd ); }
#define RU864_CHANNEL_REMOVE( )                     { return RegionRU864ChannelsRemove( channelRemove ); }
#define RU864_APPLY_DR_OFFSET( )                    { return RegionRU864ApplyDrOffset( downlinkDwellTime, dr, drOffset ); }
#define RU864_RX_BEACON_SETUP( )                    { RegionRU864RxBeaconSetup( rxBeaconSetup, outDr ); break; }
// #else
// #define RU864_IS_ACTIVE( )
// #define RU864_GET_PHY_PARAM( )
// #define RU864_SET_BAND_TX_DONE( )
// #define RU864_INIT_DEFAULTS( )
// #define RU864_GET_NVM_CTX( )
// #define RU864_VERIFY( )
// #define RU864_APPLY_CF_LIST( )
// #define RU864_CHAN_MASK_SET( )
// #define RU864_COMPUTE_RX_WINDOW_PARAMETERS( )
// #define RU864_RX_CONFIG( )
// #define RU864_TX_CONFIG( )
// #define RU864_LINK_ADR_REQ( )
// #define RU864_RX_PARAM_SETUP_REQ( )
// #define RU864_NEW_CHANNEL_REQ( )
// #define RU864_TX_PARAM_SETUP_REQ( )
// #define RU864_DL_CHANNEL_REQ( )
// #define RU864_ALTERNATE_DR( )
// #define RU864_NEXT_CHANNEL( )
// #define RU864_CHANNEL_ADD( )
// #define RU864_CHANNEL_REMOVE( )
// #define RU864_APPLY_DR_OFFSET( )
// #define RU864_RX_BEACON_SETUP( )
// #endif

bool RegionIsActive( LoRaMacRegion_t region )
{
    switch( region )
    {
        /*!
        * AS band on 923MHz
        */
        case LORAMAC_REGION_AS923:
            printf("AS923 ACTIVE\n");
            AS923_IS_ACTIVE( );
            break;
        /*!
        * Australian band on 915MHz
        */
        case LORAMAC_REGION_AU915:
            printf("AU915 ACTIVE\n");
            AU915_IS_ACTIVE( );
            break;
        /*!
        * Chinese band on 470MHz
        */
        case LORAMAC_REGION_CN470:
            printf("CN470 ACTIVE\n");
            CN470_IS_ACTIVE( );
            break;
        /*!
        * Chinese band on 779MHz
        */
        case LORAMAC_REGION_CN779:
            printf("CN779 ACTIVE\n");
            CN779_IS_ACTIVE( );
            break;
        /*!
        * European band on 433MHz
        */
        case LORAMAC_REGION_EU433:
            printf("EU433 ACTIVE\n");
            EU433_IS_ACTIVE( );
            break;
        /*!
        * European band on 868MHz
        */
        case LORAMAC_REGION_EU868:
            printf("EU868 ACTIVE\n");
            EU868_IS_ACTIVE( );
            break;
        /*!
        * South korean band on 920MHz
        */
        case LORAMAC_REGION_KR920:
            printf("KR920 ACTIVE\n");
            KR920_IS_ACTIVE( );
            break;
        /*!
        * India band on 865MHz
        */
        case LORAMAC_REGION_IN865:
            printf("IN865 ACTIVE\n");
            IN865_IS_ACTIVE( );
            break;
        /*!
        * North american band on 915MHz
        */
        case LORAMAC_REGION_US915:
            printf("US915 ACTIVE\n");
            US915_IS_ACTIVE( );
            break;
        /*!
        * Russia band on 864MHz
        */
        case LORAMAC_REGION_RU864:
            printf("RU864 ACTIVE\n");
            RU864_IS_ACTIVE( );
            break;        
               
        default:
        {
            return false;
        }
    }
}

PhyParam_t RegionGetPhyParam( LoRaMacRegion_t region, GetPhyParams_t* getPhy )
{
    PhyParam_t phyParam = { 0 };
    switch( region )
    {
        /*!
        * AS band on 923MHz
        */
        case LORAMAC_REGION_AS923:
            AS923_GET_PHY_PARAM( );
            break;

        /*!
        * Australian band on 915MHz
        */
        case LORAMAC_REGION_AU915:
            AU915_GET_PHY_PARAM( );
            break;
        /*!
        * Chinese band on 470MHz
        */
        case LORAMAC_REGION_CN470:
            CN470_GET_PHY_PARAM( );
            break;
        /*!
        * Chinese band on 779MHz
        */
        case LORAMAC_REGION_CN779:
            CN779_GET_PHY_PARAM( );
            break;
        /*!
        * European band on 433MHz
        */
        case LORAMAC_REGION_EU433:
            EU433_GET_PHY_PARAM( );
            break;
        /*!
        * European band on 868MHz
        */
        case LORAMAC_REGION_EU868:
            EU868_GET_PHY_PARAM( );
            break;
        /*!
        * South korean band on 920MHz
        */
        case LORAMAC_REGION_KR920:
            KR920_GET_PHY_PARAM( );
            break;
        /*!
        * India band on 865MHz
        */
        case LORAMAC_REGION_IN865:
            IN865_GET_PHY_PARAM( );
            break;
        /*!
        * North american band on 915MHz
        */
        case LORAMAC_REGION_US915:
            US915_GET_PHY_PARAM( );
            break;
        /*!
        * Russia band on 864MHz
        */
        case LORAMAC_REGION_RU864:
            RU864_GET_PHY_PARAM( );
            break;    
        
        
        default:
        {
            return phyParam;
        }
    }
}

void RegionSetBandTxDone( LoRaMacRegion_t region, SetBandTxDoneParams_t* txDone )
{
    switch( region )
    {
        /*!
        * AS band on 923MHz
        */
        case LORAMAC_REGION_AS923:
            AS923_SET_BAND_TX_DONE( );
            break;
        /*!
        * Australian band on 915MHz
        */
        case LORAMAC_REGION_AU915:
            AU915_SET_BAND_TX_DONE( );
            break;
        /*!
        * Chinese band on 470MHz
        */
        case LORAMAC_REGION_CN470:
            CN470_SET_BAND_TX_DONE( );
            break;
        /*!
        * Chinese band on 779MHz
        */
        case LORAMAC_REGION_CN779:
            CN779_SET_BAND_TX_DONE( );
            break;
        /*!
        * European band on 433MHz
        */
        case LORAMAC_REGION_EU433:
            EU433_SET_BAND_TX_DONE( );
            break;
        /*!
        * European band on 868MHz
        */
        case LORAMAC_REGION_EU868:
            EU868_SET_BAND_TX_DONE( );
            break;
        /*!
        * South korean band on 920MHz
        */
        case LORAMAC_REGION_KR920:
            KR920_SET_BAND_TX_DONE( );
            break;
        /*!
        * India band on 865MHz
        */
        case LORAMAC_REGION_IN865:
            IN865_SET_BAND_TX_DONE( );
            break;
        /*!
        * North american band on 915MHz
        */
        case LORAMAC_REGION_US915:
            US915_SET_BAND_TX_DONE( );
            break;
        /*!
        * Russia band on 864MHz
        */
        case LORAMAC_REGION_RU864:
            RU864_SET_BAND_TX_DONE( );
            break;
        
        default:
        {
            return;
        }
    }
}

void RegionInitDefaults( LoRaMacRegion_t region, InitDefaultsParams_t* params )
{
    switch( region )
    {

        /*!
        * AS band on 923MHz
        */
        case LORAMAC_REGION_AS923:
            AS923_INIT_DEFAULTS( );
            break;
        /*!
        * Australian band on 915MHz
        */
        case LORAMAC_REGION_AU915:
            AU915_INIT_DEFAULTS( );
            break;
        /*!
        * Chinese band on 470MHz
        */
        case LORAMAC_REGION_CN470:
            CN470_INIT_DEFAULTS( );
            break;
        /*!
        * Chinese band on 779MHz
        */
        case LORAMAC_REGION_CN779:
            CN779_INIT_DEFAULTS( );
            break;
        /*!
        * European band on 433MHz
        */
        case LORAMAC_REGION_EU433:
            EU433_INIT_DEFAULTS( );
            break;
        /*!
        * European band on 868MHz
        */
        case LORAMAC_REGION_EU868:
            EU868_INIT_DEFAULTS( );
            break;
        /*!
        * South korean band on 920MHz
        */
        case LORAMAC_REGION_KR920:
            KR920_INIT_DEFAULTS( );
            break;
        /*!
        * India band on 865MHz
        */
        case LORAMAC_REGION_IN865:
            IN865_INIT_DEFAULTS( );
            break;
        /*!
        * North american band on 915MHz
        */
        case LORAMAC_REGION_US915:
            US915_INIT_DEFAULTS( );
            break;
        /*!
        * Russia band on 864MHz
        */
        case LORAMAC_REGION_RU864:
            RU864_INIT_DEFAULTS( );
            break;
        
        default:
        {
            break;
        }
    }
}

bool RegionVerify( LoRaMacRegion_t region, VerifyParams_t* verify, PhyAttribute_t phyAttribute )
{
    switch( region )
    {
        /*!
        * AS band on 923MHz
        */
        case LORAMAC_REGION_AS923:
            AS923_VERIFY( );
            break;
        /*!
        * Australian band on 915MHz
        */
        case LORAMAC_REGION_AU915:
            AU915_VERIFY( );
            break;
        /*!
        * Chinese band on 470MHz
        */
        case LORAMAC_REGION_CN470:
            CN470_VERIFY( );
            break;
        /*!
        * Chinese band on 779MHz
        */
        case LORAMAC_REGION_CN779:
            CN779_VERIFY( );
            break;
        /*!
        * European band on 433MHz
        */
        case LORAMAC_REGION_EU433:
            EU433_VERIFY( );
            break;
        /*!
        * European band on 868MHz
        */
        case LORAMAC_REGION_EU868:
            EU868_VERIFY( );
            break;
        /*!
        * South korean band on 920MHz
        */
        case LORAMAC_REGION_KR920:
            KR920_VERIFY( );
            break;
        /*!
        * India band on 865MHz
        */
        case LORAMAC_REGION_IN865:
            IN865_VERIFY( );
            break;
        /*!
        * North american band on 915MHz
        */
        case LORAMAC_REGION_US915:
            US915_VERIFY( );
            break;
        /*!
        * Russia band on 864MHz
        */
        case LORAMAC_REGION_RU864:
            RU864_VERIFY( );
            break;

        default:
        {
            return false;
        }
    }
}

void RegionApplyCFList( LoRaMacRegion_t region, ApplyCFListParams_t* applyCFList )
{
    switch( region )
    {
        /*!
        * AS band on 923MHz
        */
        case LORAMAC_REGION_AS923:
            AS923_APPLY_CF_LIST( );
            break;
        /*!
        * Australian band on 915MHz
        */
        case LORAMAC_REGION_AU915:
            AU915_APPLY_CF_LIST( );
            break;
        /*!
        * Chinese band on 470MHz
        */
        case LORAMAC_REGION_CN470:
            CN470_APPLY_CF_LIST( );
            break;
        /*!
        * Chinese band on 779MHz
        */
        case LORAMAC_REGION_CN779:
            CN779_APPLY_CF_LIST( );
            break;
        /*!
        * European band on 433MHz
        */
        case LORAMAC_REGION_EU433:
            EU433_APPLY_CF_LIST( );
            break;
        /*!
        * European band on 868MHz
        */
        case LORAMAC_REGION_EU868:
            EU868_APPLY_CF_LIST( );
            break;
        /*!
        * South korean band on 920MHz
        */
        case LORAMAC_REGION_KR920:
            KR920_APPLY_CF_LIST( );
            break;
        /*!
        * India band on 865MHz
        */
        case LORAMAC_REGION_IN865:
            IN865_APPLY_CF_LIST( );
            break;
        /*!
        * North american band on 915MHz
        */
        case LORAMAC_REGION_US915:
            US915_APPLY_CF_LIST( );
            break;
        /*!
        * Russia band on 864MHz
        */
        case LORAMAC_REGION_RU864:
            RU864_APPLY_CF_LIST( );
            break;

        default:
        {
            break;
        }
    }
}

bool RegionChanMaskSet( LoRaMacRegion_t region, ChanMaskSetParams_t* chanMaskSet )
{
    switch( region )
    {
        /*!
        * AS band on 923MHz
        */
        case LORAMAC_REGION_AS923:
            AS923_CHAN_MASK_SET( );
            break;
        /*!
        * Australian band on 915MHz
        */
        case LORAMAC_REGION_AU915:
            AU915_CHAN_MASK_SET( );
            break;
        /*!
        * Chinese band on 470MHz
        */
        case LORAMAC_REGION_CN470:
            CN470_CHAN_MASK_SET( );
            break;
        /*!
        * Chinese band on 779MHz
        */
        case LORAMAC_REGION_CN779:
            CN779_CHAN_MASK_SET( );
            break;
        /*!
        * European band on 433MHz
        */
        case LORAMAC_REGION_EU433:
            EU433_CHAN_MASK_SET( );
            break;
        /*!
        * European band on 868MHz
        */
        case LORAMAC_REGION_EU868:
            EU868_CHAN_MASK_SET( );
            break;
        /*!
        * South korean band on 920MHz
        */
        case LORAMAC_REGION_KR920:
            KR920_CHAN_MASK_SET( );
            break;
        /*!
        * India band on 865MHz
        */
        case LORAMAC_REGION_IN865:
            IN865_CHAN_MASK_SET( );
            break;
        /*!
        * North american band on 915MHz
        */
        case LORAMAC_REGION_US915:
            US915_CHAN_MASK_SET( );
            break;
        /*!
        * Russia band on 864MHz
        */
        case LORAMAC_REGION_RU864:
            RU864_CHAN_MASK_SET( );
            break;
        
        default:
        {
            return false;
        }
    }
}

void RegionComputeRxWindowParameters( LoRaMacRegion_t region, int8_t datarate, uint8_t minRxSymbols, uint32_t rxError, RxConfigParams_t *rxConfigParams )
{
    switch( region )
    {
        /*!
        * AS band on 923MHz
        */
        case LORAMAC_REGION_AS923:
            AS923_COMPUTE_RX_WINDOW_PARAMETERS( );
            break;
        /*!
        * Australian band on 915MHz
        */
        case LORAMAC_REGION_AU915:
            AU915_COMPUTE_RX_WINDOW_PARAMETERS( );
            break;
        /*!
        * Chinese band on 470MHz
        */
        case LORAMAC_REGION_CN470:
            CN470_COMPUTE_RX_WINDOW_PARAMETERS( );
            break;
        /*!
        * Chinese band on 779MHz
        */
        case LORAMAC_REGION_CN779:
            CN779_COMPUTE_RX_WINDOW_PARAMETERS( );
            break;
        /*!
        * European band on 433MHz
        */
        LORAMAC_REGION_EU433:
            EU433_COMPUTE_RX_WINDOW_PARAMETERS( );
            break;
        /*!
        * European band on 868MHz
        */
        case LORAMAC_REGION_EU868:
            EU868_COMPUTE_RX_WINDOW_PARAMETERS( );
            break;
        /*!
        * South korean band on 920MHz
        */
        case LORAMAC_REGION_KR920:
            KR920_COMPUTE_RX_WINDOW_PARAMETERS( );
            break;
        /*!
        * India band on 865MHz
        */
        case LORAMAC_REGION_IN865:
            IN865_COMPUTE_RX_WINDOW_PARAMETERS( );
            break;
        /*!
        * North american band on 915MHz
        */
        case LORAMAC_REGION_US915:
            US915_COMPUTE_RX_WINDOW_PARAMETERS( );
            break;
        /*!
        * Russia band on 864MHz
        */
        case LORAMAC_REGION_RU864:
            RU864_COMPUTE_RX_WINDOW_PARAMETERS( );
            break;

        default:
        {
            break;
        }
    }
}

bool RegionRxConfig( LoRaMacRegion_t region, RxConfigParams_t* rxConfig, int8_t* datarate )
{
    switch( region )
    {
        /*!
        * AS band on 923MHz
        */
        case LORAMAC_REGION_AS923:
            AS923_RX_CONFIG( );
            break;
        /*!
        * Australian band on 915MHz
        */
        case LORAMAC_REGION_AU915:
            AU915_RX_CONFIG( );
            break;
        /*!
        * Chinese band on 470MHz
        */
        case LORAMAC_REGION_CN470:
            CN470_RX_CONFIG( );
            break;
        /*!
        * Chinese band on 779MHz
        */
        case LORAMAC_REGION_CN779:
            CN779_RX_CONFIG( );
            break;
        /*!
        * European band on 433MHz
        */
        case LORAMAC_REGION_EU433:
            EU433_RX_CONFIG( );
            break;
        /*!
        * European band on 868MHz
        */
        case LORAMAC_REGION_EU868:
            EU868_RX_CONFIG( );
            break;
        /*!
        * South korean band on 920MHz
        */
        case LORAMAC_REGION_KR920:
            KR920_RX_CONFIG( );
            break;
        /*!
        * India band on 865MHz
        */
        case LORAMAC_REGION_IN865:
            IN865_RX_CONFIG( );
            break;
        /*!
        * North american band on 915MHz
        */
        case LORAMAC_REGION_US915:
            US915_RX_CONFIG( );
            break;
        /*!
        * Russia band on 864MHz
        */
        case LORAMAC_REGION_RU864:
            RU864_RX_CONFIG( );
            break;

        default:
        {
            return false;
        }
    }
}

bool RegionTxConfig( LoRaMacRegion_t region, TxConfigParams_t* txConfig, int8_t* txPower, TimerTime_t* txTimeOnAir )
{
    switch( region )
    {
        /*!
        * AS band on 923MHz
        */
        case LORAMAC_REGION_AS923:
            AS923_TX_CONFIG( );
            break;
        /*!
        * Australian band on 915MHz
        */
        case LORAMAC_REGION_AU915:
            AU915_TX_CONFIG( );
            break;
        /*!
        * Chinese band on 470MHz
        */
        case LORAMAC_REGION_CN470:
            CN470_TX_CONFIG( );
            break;
        /*!
        * Chinese band on 779MHz
        */
        case LORAMAC_REGION_CN779:
            CN779_TX_CONFIG( );
            break;
        /*!
        * European band on 433MHz
        */
        case LORAMAC_REGION_EU433:
            EU433_TX_CONFIG( );
            break;
        /*!
        * European band on 868MHz
        */
        case LORAMAC_REGION_EU868:
            EU868_TX_CONFIG( );
            break;
        /*!
        * South korean band on 920MHz
        */
        case LORAMAC_REGION_KR920:
            KR920_TX_CONFIG( );
            break;
        /*!
        * India band on 865MHz
        */
        case LORAMAC_REGION_IN865:
            IN865_TX_CONFIG( );
            break;
        /*!
        * North american band on 915MHz
        */
        case LORAMAC_REGION_US915:
            US915_TX_CONFIG( );
            break;
        /*!
        * Russia band on 864MHz
        */
        case LORAMAC_REGION_RU864:
            RU864_TX_CONFIG( );
            break;

        default:
        {
            return false;
        }
    }
}

uint8_t RegionLinkAdrReq( LoRaMacRegion_t region, LinkAdrReqParams_t* linkAdrReq, int8_t* drOut, int8_t* txPowOut, uint8_t* nbRepOut, uint8_t* nbBytesParsed )
{
    switch( region )
    {
        /*!
        * AS band on 923MHz
        */
        case LORAMAC_REGION_AS923:
            AS923_LINK_ADR_REQ( );
            break;
        /*!
        * Australian band on 915MHz
        */
        case LORAMAC_REGION_AU915:
            AU915_LINK_ADR_REQ( );
            break;
        /*!
        * Chinese band on 470MHz
        */
        case LORAMAC_REGION_CN470:
            CN470_LINK_ADR_REQ( );
            break;
        /*!
        * Chinese band on 779MHz
        */
        case LORAMAC_REGION_CN779:
            CN779_LINK_ADR_REQ( );
            break;
        /*!
        * European band on 433MHz
        */
        case LORAMAC_REGION_EU433:
            EU433_LINK_ADR_REQ( );
            break;
        /*!
        * European band on 868MHz
        */
        case LORAMAC_REGION_EU868:
            EU868_LINK_ADR_REQ( );
            break;
        /*!
        * South korean band on 920MHz
        */
        case LORAMAC_REGION_KR920:
            KR920_LINK_ADR_REQ( );
            break;
        /*!
        * India band on 865MHz
        */
        case LORAMAC_REGION_IN865:
            IN865_LINK_ADR_REQ( );
            break;
        /*!
        * North american band on 915MHz
        */
        case LORAMAC_REGION_US915:
            US915_LINK_ADR_REQ( );
            break;
        /*!
        * Russia band on 864MHz
        */
        case LORAMAC_REGION_RU864:
            RU864_LINK_ADR_REQ( );
            break;

        default:
        {
            return 0;
        }
    }
}

uint8_t RegionRxParamSetupReq( LoRaMacRegion_t region, RxParamSetupReqParams_t* rxParamSetupReq )
{
    switch( region )
    {
        /*!
        * AS band on 923MHz
        */
        case LORAMAC_REGION_AS923:
            AS923_RX_PARAM_SETUP_REQ( );
            break;
        /*!
        * Australian band on 915MHz
        */
        case LORAMAC_REGION_AU915:
            AU915_RX_PARAM_SETUP_REQ( );
            break;
        /*!
        * Chinese band on 470MHz
        */
        case LORAMAC_REGION_CN470:
            CN470_RX_PARAM_SETUP_REQ( );
            break;
        /*!
        * Chinese band on 779MHz
        */
        case LORAMAC_REGION_CN779:
            CN779_RX_PARAM_SETUP_REQ( );
            break;
        /*!
        * European band on 433MHz
        */
        case LORAMAC_REGION_EU433:
            EU433_RX_PARAM_SETUP_REQ( );
            break;
        /*!
        * European band on 868MHz
        */
        case LORAMAC_REGION_EU868:
            EU868_RX_PARAM_SETUP_REQ( );
            break;
        /*!
        * South korean band on 920MHz
        */
        case LORAMAC_REGION_KR920:
            KR920_RX_PARAM_SETUP_REQ( );
            break;
        /*!
        * India band on 865MHz
        */
        case LORAMAC_REGION_IN865:
            IN865_RX_PARAM_SETUP_REQ( );
            break;
        /*!
        * North american band on 915MHz
        */
        case LORAMAC_REGION_US915:
            US915_RX_PARAM_SETUP_REQ( );
            break;
        /*!
        * Russia band on 864MHz
        */
        case LORAMAC_REGION_RU864:
            RU864_RX_PARAM_SETUP_REQ( );
            break;

        default:
        {
            return 0;
        }
    }
}

int8_t RegionNewChannelReq( LoRaMacRegion_t region, NewChannelReqParams_t* newChannelReq )
{
    switch( region )
    {
        /*!
        * AS band on 923MHz
        */
        case LORAMAC_REGION_AS923:
            AS923_NEW_CHANNEL_REQ( );
            break;
        /*!
        * Australian band on 915MHz
        */
        case LORAMAC_REGION_AU915:
            AU915_NEW_CHANNEL_REQ( );
            break;
        /*!
        * Chinese band on 470MHz
        */
        case LORAMAC_REGION_CN470:
            CN470_NEW_CHANNEL_REQ( );
            break;
        /*!
        * Chinese band on 779MHz
        */
        case LORAMAC_REGION_CN779:
            CN779_NEW_CHANNEL_REQ( );
            break;
        /*!
        * European band on 433MHz
        */
        case LORAMAC_REGION_EU433:
            EU433_NEW_CHANNEL_REQ( );
            break;
        /*!
        * European band on 868MHz
        */
        case LORAMAC_REGION_EU868:
            EU868_NEW_CHANNEL_REQ( );
            break;
        /*!
        * South korean band on 920MHz
        */
        case LORAMAC_REGION_KR920:
            KR920_NEW_CHANNEL_REQ( );
            break;
        /*!
        * India band on 865MHz
        */
        case LORAMAC_REGION_IN865:
            IN865_NEW_CHANNEL_REQ( );
            break;
        /*!
        * North american band on 915MHz
        */
        case LORAMAC_REGION_US915:
            US915_NEW_CHANNEL_REQ( );
            break;
        /*!
        * Russia band on 864MHz
        */
        case LORAMAC_REGION_RU864:
            RU864_NEW_CHANNEL_REQ( );
            break;

        default:
        {
            return 0;
        }
    }
}

int8_t RegionTxParamSetupReq( LoRaMacRegion_t region, TxParamSetupReqParams_t* txParamSetupReq )
{
    switch( region )
    {
        /*!
        * AS band on 923MHz
        */
        case LORAMAC_REGION_AS923:
            AS923_TX_PARAM_SETUP_REQ( );
            break;
        /*!
        * Australian band on 915MHz
        */
        case LORAMAC_REGION_AU915:
            AU915_TX_PARAM_SETUP_REQ( );
            break;
        /*!
        * Chinese band on 470MHz
        */
        case LORAMAC_REGION_CN470:
            CN470_TX_PARAM_SETUP_REQ( );
            break;
        /*!
        * Chinese band on 779MHz
        */
        case LORAMAC_REGION_CN779:
            CN779_TX_PARAM_SETUP_REQ( );
            break;
        /*!
        * European band on 433MHz
        */
        case LORAMAC_REGION_EU433:
            EU433_TX_PARAM_SETUP_REQ( );
            break;
        /*!
        * European band on 868MHz
        */
        case LORAMAC_REGION_EU868:
            EU868_TX_PARAM_SETUP_REQ( );
            break;
        /*!
        * South korean band on 920MHz
        */
        case LORAMAC_REGION_KR920:
            KR920_TX_PARAM_SETUP_REQ( );
            break;
        /*!
        * India band on 865MHz
        */
        case LORAMAC_REGION_IN865:
            IN865_TX_PARAM_SETUP_REQ( );
            break;
        /*!
        * North american band on 915MHz
        */
        case LORAMAC_REGION_US915:
            US915_TX_PARAM_SETUP_REQ( );
            break;
        /*!
        * Russia band on 864MHz
        */
        case LORAMAC_REGION_RU864:
            RU864_TX_PARAM_SETUP_REQ( );
            break;

        default:
        {
            return 0;
        }
    }
}

int8_t RegionDlChannelReq( LoRaMacRegion_t region, DlChannelReqParams_t* dlChannelReq )
{
    switch( region )
    {
        /*!
        * AS band on 923MHz
        */
        case LORAMAC_REGION_AS923:
            AS923_DL_CHANNEL_REQ( );
            break;
        /*!
        * Australian band on 915MHz
        */
        case LORAMAC_REGION_AU915:
            AU915_DL_CHANNEL_REQ( );
            break;
        /*!
        * Chinese band on 470MHz
        */
        case LORAMAC_REGION_CN470:
            CN470_DL_CHANNEL_REQ( );
            break;
        /*!
        * Chinese band on 779MHz
        */
        case LORAMAC_REGION_CN779:
            CN779_DL_CHANNEL_REQ( );
            break;
        /*!
        * European band on 433MHz
        */
        case LORAMAC_REGION_EU433:
            EU433_DL_CHANNEL_REQ( );
            break;
        /*!
        * European band on 868MHz
        */
        case LORAMAC_REGION_EU868:
            EU868_DL_CHANNEL_REQ( );
            break;
        /*!
        * South korean band on 920MHz
        */
        case LORAMAC_REGION_KR920:
            KR920_DL_CHANNEL_REQ( );
            break;
        /*!
        * India band on 865MHz
        */
        case LORAMAC_REGION_IN865:
            IN865_DL_CHANNEL_REQ( );
            break;
        /*!
        * North american band on 915MHz
        */
        case LORAMAC_REGION_US915:
            US915_DL_CHANNEL_REQ( );
            break;
        /*!
        * Russia band on 864MHz
        */
        case LORAMAC_REGION_RU864:
            RU864_DL_CHANNEL_REQ( );
            break;

        default:
        {
            return 0;
        }
    }
}

int8_t RegionAlternateDr( LoRaMacRegion_t region, int8_t currentDr, AlternateDrType_t type )
{
    switch( region )
    {
        /*!
        * AS band on 923MHz
        */
        case LORAMAC_REGION_AS923:
            AS923_ALTERNATE_DR( );
            break;
        /*!
        * Australian band on 915MHz
        */
        case LORAMAC_REGION_AU915:
            AU915_ALTERNATE_DR( );
            break;
        /*!
        * Chinese band on 470MHz
        */
        case LORAMAC_REGION_CN470:
            CN470_ALTERNATE_DR( );
            break;
        /*!
        * Chinese band on 779MHz
        */
        case LORAMAC_REGION_CN779:
            CN779_ALTERNATE_DR( );
            break;
        /*!
        * European band on 433MHz
        */
        case LORAMAC_REGION_EU433:
            EU433_ALTERNATE_DR( );
            break;
        /*!
        * European band on 868MHz
        */
        case LORAMAC_REGION_EU868:
            EU868_ALTERNATE_DR( );
            break;
        /*!
        * South korean band on 920MHz
        */
        case LORAMAC_REGION_KR920:
            KR920_ALTERNATE_DR( );
            break;
        /*!
        * India band on 865MHz
        */
        case LORAMAC_REGION_IN865:
            IN865_ALTERNATE_DR( );
            break;
        /*!
        * North american band on 915MHz
        */
        case LORAMAC_REGION_US915:
            US915_ALTERNATE_DR( );
            break;
        /*!
        * Russia band on 864MHz
        */
        case LORAMAC_REGION_RU864:
            RU864_ALTERNATE_DR( );
            break;

        default:
        {
            return 0;
        }
    }
}

LoRaMacStatus_t RegionNextChannel( LoRaMacRegion_t region, NextChanParams_t* nextChanParams, uint8_t* channel, TimerTime_t* time, TimerTime_t* aggregatedTimeOff )
{
    switch( region )
    {
        /*!
        * AS band on 923MHz
        */
        case LORAMAC_REGION_AS923:
            AS923_NEXT_CHANNEL( );
            break;
        /*!
        * Australian band on 915MHz
        */
        case LORAMAC_REGION_AU915:
            AU915_NEXT_CHANNEL( );
            break;
        /*!
        * Chinese band on 470MHz
        */
        case LORAMAC_REGION_CN470:
            CN470_NEXT_CHANNEL( );
            break;
        /*!
        * Chinese band on 779MHz
        */
        case LORAMAC_REGION_CN779:
            CN779_NEXT_CHANNEL( );
            break;
        /*!
        * European band on 433MHz
        */
        case LORAMAC_REGION_EU433:
            EU433_NEXT_CHANNEL( );
            break;
        /*!
        * European band on 868MHz
        */
        case LORAMAC_REGION_EU868:
            EU868_NEXT_CHANNEL( );
            break;
        /*!
        * South korean band on 920MHz
        */
        case LORAMAC_REGION_KR920:
            KR920_NEXT_CHANNEL( );
            break;
        /*!
        * India band on 865MHz
        */
        case LORAMAC_REGION_IN865:
            IN865_NEXT_CHANNEL( );
            break;
        /*!
        * North american band on 915MHz
        */
        case LORAMAC_REGION_US915:
            US915_NEXT_CHANNEL( );
            break;
        /*!
        * Russia band on 864MHz
        */
        case LORAMAC_REGION_RU864:
            RU864_NEXT_CHANNEL( );
            break;

        default:
        {
            return LORAMAC_STATUS_REGION_NOT_SUPPORTED;
        }
    }
}

LoRaMacStatus_t RegionChannelAdd( LoRaMacRegion_t region, ChannelAddParams_t* channelAdd )
{
    switch( region )
    {
        /*!
        * AS band on 923MHz
        */
        case LORAMAC_REGION_AS923:
            AS923_CHANNEL_ADD( );
            break;
        /*!
        * Australian band on 915MHz
        */
        case LORAMAC_REGION_AU915:
            AU915_CHANNEL_ADD( );
            break;
        /*!
        * Chinese band on 470MHz
        */
        case LORAMAC_REGION_CN470:
            CN470_CHANNEL_ADD( );
            break;
        /*!
        * Chinese band on 779MHz
        */
        case LORAMAC_REGION_CN779:
            CN779_CHANNEL_ADD( );
            break;
        /*!
        * European band on 433MHz
        */
        case LORAMAC_REGION_EU433:
            EU433_CHANNEL_ADD( );
            break;
        /*!
        * European band on 868MHz
        */
        case LORAMAC_REGION_EU868:
            EU868_CHANNEL_ADD( );
            break;
        /*!
        * South korean band on 920MHz
        */
        case LORAMAC_REGION_KR920:
            KR920_CHANNEL_ADD( );
            break;
        /*!
        * India band on 865MHz
        */
        case LORAMAC_REGION_IN865:
            IN865_CHANNEL_ADD( );
            break;
        /*!
        * North american band on 915MHz
        */
        case LORAMAC_REGION_US915:
            US915_CHANNEL_ADD( );
            break;
        /*!
        * Russia band on 864MHz
        */
        case LORAMAC_REGION_RU864:
            RU864_CHANNEL_ADD( );
            break;

        default:
        {
            return LORAMAC_STATUS_PARAMETER_INVALID;
        }
    }
}

bool RegionChannelsRemove( LoRaMacRegion_t region, ChannelRemoveParams_t* channelRemove )
{
    switch( region )
    {
        /*!
        * AS band on 923MHz
        */
        case LORAMAC_REGION_AS923:
            AS923_CHANNEL_REMOVE( );
            break;
        /*!
        * Australian band on 915MHz
        */
        case LORAMAC_REGION_AU915:
            AU915_CHANNEL_REMOVE( );
            break;
        /*!
        * Chinese band on 470MHz
        */
        case LORAMAC_REGION_CN470:
            CN470_CHANNEL_REMOVE( );
            break;
        /*!
        * Chinese band on 779MHz
        */
        case LORAMAC_REGION_CN779:
            CN779_CHANNEL_REMOVE( );
            break;
        /*!
        * European band on 433MHz
        */
        case LORAMAC_REGION_EU433:
            EU433_CHANNEL_REMOVE( );
            break;
        /*!
        * European band on 868MHz
        */
        case LORAMAC_REGION_EU868:
            EU868_CHANNEL_REMOVE( );
            break;
        /*!
        * South korean band on 920MHz
        */
        case LORAMAC_REGION_KR920:
            KR920_CHANNEL_REMOVE( );
            break;
        /*!
        * India band on 865MHz
        */
        case LORAMAC_REGION_IN865:
            IN865_CHANNEL_REMOVE( );
            break;
        /*!
        * North american band on 915MHz
        */
        case LORAMAC_REGION_US915:
            US915_CHANNEL_REMOVE( );
            break;
        /*!
        * Russia band on 864MHz
        */
        case LORAMAC_REGION_RU864:
            RU864_CHANNEL_REMOVE( );
            break;

        default:
        {
            return false;
        }
    }
}

uint8_t RegionApplyDrOffset( LoRaMacRegion_t region, uint8_t downlinkDwellTime, int8_t dr, int8_t drOffset )
{
    switch( region )
    {
        /*!
        * AS band on 923MHz
        */
        case LORAMAC_REGION_AS923:
            AS923_APPLY_DR_OFFSET( );
            break;
        /*!
        * Australian band on 915MHz
        */
        case LORAMAC_REGION_AU915:
            AU915_APPLY_DR_OFFSET( );
            break;
        /*!
        * Chinese band on 470MHz
        */
        case LORAMAC_REGION_CN470:
            CN470_APPLY_DR_OFFSET( );
            break;
        /*!
        * Chinese band on 779MHz
        */
        case LORAMAC_REGION_CN779:
            CN779_APPLY_DR_OFFSET( );
            break;
        /*!
        * European band on 433MHz
        */
        case LORAMAC_REGION_EU433:
            EU433_APPLY_DR_OFFSET( );
            break;
        /*!
        * European band on 868MHz
        */
        case LORAMAC_REGION_EU868:
            EU868_APPLY_DR_OFFSET( );
            break;
        /*!
        * South korean band on 920MHz
        */
        case LORAMAC_REGION_KR920:
            KR920_APPLY_DR_OFFSET( );
            break;
        /*!
        * India band on 865MHz
        */
        case LORAMAC_REGION_IN865:
            IN865_APPLY_DR_OFFSET( );
            break;
        /*!
        * North american band on 915MHz
        */
        case LORAMAC_REGION_US915:
            US915_APPLY_DR_OFFSET( );
            break;
        /*!
        * Russia band on 864MHz
        */
        case LORAMAC_REGION_RU864:
            RU864_APPLY_DR_OFFSET( );
            break;

        default:
        {
            return dr;
        }
    }
}

void RegionRxBeaconSetup( LoRaMacRegion_t region, RxBeaconSetup_t* rxBeaconSetup, uint8_t* outDr )
{
    switch( region )
    {
        /*!
        * AS band on 923MHz
        */
        case LORAMAC_REGION_AS923:
            AS923_RX_BEACON_SETUP( );
            break;
        /*!
        * Australian band on 915MHz
        */
        case LORAMAC_REGION_AU915:
            AU915_RX_BEACON_SETUP( );
            break;
        /*!
        * Chinese band on 470MHz
        */
        case LORAMAC_REGION_CN470:
            CN470_RX_BEACON_SETUP( );
            break;
        /*!
        * Chinese band on 779MHz
        */
        case LORAMAC_REGION_CN779:
            CN779_RX_BEACON_SETUP( );
            break;
        /*!
        * European band on 433MHz
        */
        case LORAMAC_REGION_EU433:
            EU433_RX_BEACON_SETUP( );
            break;
        /*!
        * European band on 868MHz
        */
        case LORAMAC_REGION_EU868:
            EU868_RX_BEACON_SETUP( );
            break;
        /*!
        * South korean band on 920MHz
        */
        case LORAMAC_REGION_KR920:
            KR920_RX_BEACON_SETUP( );
            break;
        /*!
        * India band on 865MHz
        */
        case LORAMAC_REGION_IN865:
            IN865_RX_BEACON_SETUP( );
            break;
        /*!
        * North american band on 915MHz
        */
        case LORAMAC_REGION_US915:
            US915_RX_BEACON_SETUP( );
            break;
        /*!
        * Russia band on 864MHz
        */
        case LORAMAC_REGION_RU864:
            RU864_RX_BEACON_SETUP( );
            break;

        default:
        {
            break;
        }
    }
}

Version_t RegionGetVersion( void )
{
    Version_t version;

    version.Value = REGION_VERSION;

    return version;
}

