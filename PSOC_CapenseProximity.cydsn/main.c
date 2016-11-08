/*******************************************************************************
* File Name: main.c
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

#include <main.h>

/*'deviceConnected' flag is used by application to know whether a Central device  
* has been connected. This is updated in BLE event callback */
extern uint8 deviceConnected;

/*'startNotification' flag is set when the central device writes to CCC (Client  
* Characteristic Configuration) of the CapSense proximity characteristic to 
* enable notifications */
extern uint8 startNotification;	

/* 'restartAdvertisement' flag is used to restart advertisement */
extern uint8 restartAdvertisement;

/* 'initializeCapSenseBaseline' flag is used to call the function once that initializes 
* all CapSense baseline. The baseline is initialized when the first advertisement 
* is started. This is done so that any external disturbance while powering the kit does 
* not infliuence the baseline value, that may cause wrong readings. */
uint8 initializeCapSenseBaseline = TRUE;

/*******************************************************************************
* Function Name: main
********************************************************************************
* Summary:
*        System entrance point. This calls the BLE routine functions as well as
* function for handling CapSense Proximity changes
*
* Parameters:
*  void
*
* Return:
*  int
*

*******************************************************************************/
int main()
{
    /* This function will initialize the system resources such as BLE and CapSense */
	InitializeSystem();
	
    for(;;)
    {
		/*Process Event callback to handle BLE events. The events generated and 
		* used for this application are inside the 'CustomEventHandler' routine*/
		CyBle_ProcessEvents();
		
		/* Function to handle LED status depending on BLE state */
		HandleStatusLED();
		
		/* Handle proximity data and CCCD value update only if BLE device is connected */
		if(TRUE == deviceConnected) 
		{	
			/* When the Client Characteristic Configuration descriptor (CCCD) is written
			* by Central device for enabling/disabling notifications, then the same
			* descriptor value has to be explicitly updated in application so that
			* it reflects the correct value when the descriptor is read */
			UpdateNotificationCCCD();
			
			/* If CapSense Initialize Baseline API has not been called yet, call the
			* API and reset the flag. */
			if(initializeCapSenseBaseline)
			{
				/* Reset 'initializeCapSenseBaseline' flag*/
				initializeCapSenseBaseline = FALSE;
				
				/* Initialize all CapSense Baseline */
				CapSense_InitializeAllBaselines();
			}
			
			/* Send proximity data when notifications are enabled */
			if(startNotification & CCCD_NTF_BIT_MASK)
			{
				/*Check for CapSense proximity change and report to BLE central device*/
				HandleCapSenseProximity();
			}
		}

		if(restartAdvertisement)
		{
			/* Reset 'restartAdvertisement' flag*/
			restartAdvertisement = FALSE;
			
			/* Start Advertisement and enter Discoverable mode*/
			CyBle_GappStartAdvertisement(CYBLE_ADVERTISING_FAST);	
		}
    }	/* This is end of for(;;) */
}

/*******************************************************************************
* Function Name: InitializeSystem
********************************************************************************
* Summary:
*        Starts the components and initializes CapSense baselines
*
* Parameters:
*  void
*
* Return:
*  void
*

*******************************************************************************/
void InitializeSystem(void)
{
	/* Enable global interrupt mask */
	CyGlobalIntEnable;

	/* Start BLE component and register the CustomEventHandler function. This 
	* function exposes the events from BLE component for application use */
	CyBle_Start(CustomEventHandler);					
	
	/* Set drive mode of the status LED pin to Strong*/
	Status_LED_SetDriveMode(Status_LED_DM_STRONG);	
	
	#ifdef CAPSENSE_ENABLED
	/* Enable the proximity widget explicitly and start CapSense component. 
	* Initialization of the baseline is done when the first connection 
	* happens  */
	CapSense_EnableWidget(CapSense_PROXIMITYSENSOR0__PROX);
	CapSense_Start();
	#endif
		
}

/*******************************************************************************
* Function Name: HandleCapSenseProximity
********************************************************************************
* Summary:
*       Read the proximity data from the sensor and update the BLE
*		custom notification value.
*
* Parameters:
*  void
*
* Return:
*  void
*
*******************************************************************************/
void HandleCapSenseProximity(void)
{
	#ifdef CAPSENSE_ENABLED
	/* Static variable used as counter for reading the new proximity value */
	static uint16 proxCounter = TRUE;			
				
	/* 'proximityValue' stores the proximity value read from CapSense component */
	uint8 proximityValue;	
		
	/* Check if proxCounter has reached its counting value */
	if(FALSE == (--proxCounter))			
	{
		/* Set 'proxCounter' to the PROX_COUNTER_VALUE. This counter prevents sending 
		   of large number of CapSense proximity data to BLE Central device */
		proxCounter = PROX_COUNTER_VALUE;

		/* Scan the Proximity Widget */
		CapSense_ScanWidget(CapSense_PROXIMITYSENSOR0__PROX);				
		
		/* Wait for CapSense scanning to be complete. This could take about 5 ms */
		while(CapSense_IsBusy())
		{
		}
		
		/* Update CapSense Baseline */
		CapSense_UpdateEnabledBaselines();			

		/* Get the Diffcount between proximity raw data and baseline */
		proximityValue = CapSense_GetDiffCountData(CapSense_PROXIMITYSENSOR0__PROX);
		
		/* Send the proximity data to BLE central device by notifications*/
		SendDataOverCapSenseNotification(proximityValue);
			
	}
	#endif
}

/* [] END OF FILE */
