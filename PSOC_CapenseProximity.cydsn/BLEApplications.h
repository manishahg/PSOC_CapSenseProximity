/*******************************************************************************
* File Name: BLEApplications.h
*
* Version: 1.0
*
* Description:
*  This is the source code for the PSoC 4 Capsense Proximity - Setting up a Connection.
*
* Hardware Dependency:
*  CY8CKIT-042 BLE Pioneer Kit
*
********************************************************************************
* Copyright 2014, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

/********************************************************************************
* Contains all macros and function declaration used in the BLEApplications.c 
* file.
********************************************************************************/
#if !defined(BLEAPPLICATIONS_H)
#define BLEAPPLICATIONS_H
#include <project.h>

/*************************Pre-processor directives****************************/
/* 'ENABLE_LOW_POWER_MODE' pre-processor directive enables the low power mode 
* handling in the firmware, ensuring low power consumption during project usage.
* To disable, comment the following #define. 
* If disabled, prevent usage of the project with coin cell */
#define ENABLE_LOW_POWER_MODE
	
/* 'CAPSENSE_ENABLED' pre-processor directive enables CapSense functionality 
* in the firmware. To disable Capsense, comment the following #define as well 
* as disable the CapSense component from the TopDesign */
#define CAPSENSE_ENABLED
/****************************************************************************/
	
/**************************Function Declarations*****************************/
void CustomEventHandler(uint32 event, void * eventParam);
void SendDataOverCapSenseNotification(uint8 proximityValue);
void UpdateNotificationCCCD(void);
void HandleStatusLED(void);
/****************************************************************************/

/***************************Macro Definitions*******************************/
/* Data length of CapSense Proximity data sent over notification */
#define CAPSENSE_NOTIFICATION_DATA_LEN		1

/* Bit mask for notification bit in CCCD (Client Characteristic 
* Configuration Descriptor) written by Client device. */
#define CCCD_NTF_BIT_MASK					0x01

/* Client Characteristic Configuration descriptor data length. This is defined
* as per BLE spec. */
#define CCC_DATA_LEN						2

/* LED Blink rate values for different stages of BLE connection */
#ifdef ENABLE_LOW_POWER_MODE
#define	LED_ADV_BLINK_PERIOD_ON			5
#define LED_ADV_BLINK_PERIOD_OFF		20
#else
#define	LED_ADV_BLINK_PERIOD_ON			10000
#define LED_ADV_BLINK_PERIOD_OFF		15000
#endif

/* Macros for LED ON and OFF values */
#define LED_ON							0x00
#define LED_OFF							0x01
/****************************************************************************/

#endif
/* [] END OF FILE */
