/*******************************************************************************
* File Name: main.h
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
*	Contains all macros and function declaration used in the main.c file 
********************************************************************************/
#if !defined(MAIN_H)
#define MAIN_H

#include <project.h>
#include <BLEApplications.h>
	
/**************************Function Declarations*****************************/
void InitializeSystem(void);
void HandleCapSenseProximity(void);
void CustomEventHandler(uint32 event, void * eventParam);
/****************************************************************************/

/***************************Macro Definitions*******************************/
#define TRUE								1
#define FALSE								0
#define ZERO								0

/* CapSense Proximity value ranges from 0-255*/
#define MAX_PROX_VALUE						0xFF

/* Counter value for reading proximity data. This counter prevents pushing 
* large number of CapSense proximity data to connected BLE Client. Modify
* this counter to change the rate of notification of proximity data*/
#define PROX_COUNTER_VALUE					5
/****************************************************************************/

#endif
/* [] END OF FILE */
