
/**
  EXT_INT Generated Driver File 

  @Company:
    Microchip Technology Inc.

  @File Name:
    ext_int.c

  @Summary
    This is the generated driver implementation file for the EXT_INT 
    driver using PIC24 / dsPIC33 / PIC32MM MCUs

  @Description:
    This source file provides implementations for driver APIs for EXT_INT. 
    Generation Information : 
        Product Revision  :  PIC24 / dsPIC33 / PIC32MM MCUs - 1.170.0
        Device            :  PIC24FJ128GA010
    The generated drivers are tested against the following:
        Compiler          :  XC16 v1.61
        MPLAB             :  MPLAB X v5.45
*/

/*
    (c) 2020 Microchip Technology Inc. and its subsidiaries. You may use this
    software and any derivatives exclusively with Microchip products.

    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
    WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
    PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION
    WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION.

    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
    BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
    FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
    ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
    THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.

    MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE
    TERMS.
*/

/**
   Section: Includes
 */

#include "ext_int.h"
#include "../pokemon.h"

//***User Area Begin->code: Add External Interrupt handler specific headers 

//***User Area End->code: Add External Interrupt handler specific headers

/**
   Section: External Interrupt Handlers
*/
 
 void __attribute__ ((weak)) EX_INT0_CallBack(void)
{
    // Add your custom callback code here
    switch(game_stage){
        case SCREEN_START:
            game_stage = SCREEN_MAP;
            break;
        case SCREEN_MAP:
            button_pressed = BUTTON_START;
            break;
        case SCREEN_BATTLE:
            break;
    }
}

/**
  Interrupt Handler for EX_INT0 - INT0
*/
void __attribute__ ( ( interrupt, no_auto_psv ) ) _INT0Interrupt(void)
{
    //***User Area Begin->code: INT0 - External Interrupt 0***
	
	EX_INT0_CallBack();
    
	//***User Area End->code: INT0 - External Interrupt 0***
    EX_INT0_InterruptFlagClear();
}
 void __attribute__ ((weak)) EX_INT1_CallBack(void)
{
    // Add your custom callback code here
    switch(game_stage){
        case SCREEN_START:
            break;
        case SCREEN_MAP:
            button_pressed = BUTTON_A;
            break;
        case SCREEN_BATTLE:
            button_pressed = BUTTON_A-1;
            break;
    }
}

/**
  Interrupt Handler for EX_INT1 - INT1
*/
void __attribute__ ( ( interrupt, no_auto_psv ) ) _INT1Interrupt(void)
{
    //***User Area Begin->code: INT1 - External Interrupt 1***
	
	EX_INT1_CallBack();
    
	//***User Area End->code: INT1 - External Interrupt 1***
    EX_INT1_InterruptFlagClear();
}
 void __attribute__ ((weak)) EX_INT2_CallBack(void)
{
    // Add your custom callback code here
    switch(game_stage){
        case SCREEN_START:
            break;
        case SCREEN_MAP:
            button_pressed = BUTTON_B;
            break;
        case SCREEN_BATTLE:
            button_pressed = BUTTON_B-1;
            break;
    }
}

/**
  Interrupt Handler for EX_INT2 - INT2
*/
void __attribute__ ( ( interrupt, no_auto_psv ) ) _INT2Interrupt(void)
{
    //***User Area Begin->code: INT2 - External Interrupt 2***
	
	EX_INT2_CallBack();
    
	//***User Area End->code: INT2 - External Interrupt 2***
    EX_INT2_InterruptFlagClear();
}
 void __attribute__ ((weak)) EX_INT3_CallBack(void)
{
    // Add your custom callback code here
    switch(game_stage){
        case SCREEN_START:
            break;
        case SCREEN_MAP:
            button_pressed = BUTTON_C;
            break;
        case SCREEN_BATTLE:
            button_pressed = BUTTON_C-1;
            break;
    }
}

/**
  Interrupt Handler for EX_INT3 - INT3
*/
void __attribute__ ( ( interrupt, no_auto_psv ) ) _INT3Interrupt(void)
{
    //***User Area Begin->code: INT3 - External Interrupt 3***
	
	EX_INT3_CallBack();
    
	//***User Area End->code: INT3 - External Interrupt 3***
    EX_INT3_InterruptFlagClear();
}
 void __attribute__ ((weak)) EX_INT4_CallBack(void)
{
    // Add your custom callback code here
    switch(game_stage){
        case SCREEN_START:
            break;
        case SCREEN_MAP:
            button_pressed = BUTTON_D;
            break;
        case SCREEN_BATTLE:
            button_pressed = BUTTON_D-1;
            break;
    }
}

/**
  Interrupt Handler for EX_INT4 - INT4
*/
void __attribute__ ( ( interrupt, no_auto_psv ) ) _INT4Interrupt(void)
{
    //***User Area Begin->code: INT4 - External Interrupt 4***
	
	EX_INT4_CallBack();
	//***User Area End->code: INT4 - External Interrupt 4***
    EX_INT4_InterruptFlagClear();
}
/**
    Section: External Interrupt Initializers
 */
/**
    void EXT_INT_Initialize(void)

    Initializer for the following external interrupts
    INT0
    INT1
    INT2
    INT3
    INT4
*/
void EXT_INT_Initialize(void)
{
    /*******
     * INT0
     * Clear the interrupt flag
     * Set the external interrupt edge detect
     * Enable the interrupt, if enabled in the UI. 
     ********/
    EX_INT0_InterruptFlagClear();   
    EX_INT0_PositiveEdgeSet();
    /*******
     * INT1
     * Clear the interrupt flag
     * Set the external interrupt edge detect
     * Enable the interrupt, if enabled in the UI. 
     ********/
    EX_INT1_InterruptFlagClear();   
    EX_INT1_PositiveEdgeSet();
    /*******
     * INT2
     * Clear the interrupt flag
     * Set the external interrupt edge detect
     * Enable the interrupt, if enabled in the UI. 
     ********/
    EX_INT2_InterruptFlagClear();   
    EX_INT2_PositiveEdgeSet();
    /*******
     * INT3
     * Clear the interrupt flag
     * Set the external interrupt edge detect
     * Enable the interrupt, if enabled in the UI. 
     ********/
    EX_INT3_InterruptFlagClear();   
    EX_INT3_PositiveEdgeSet();
    /*******
     * INT4
     * Clear the interrupt flag
     * Set the external interrupt edge detect
     * Enable the interrupt, if enabled in the UI. 
     ********/
    EX_INT4_InterruptFlagClear();   
    EX_INT4_PositiveEdgeSet();
}

void interrupt_enable_all(){
    EX_INT0_InterruptFlagClear();
    EX_INT1_InterruptFlagClear();
    EX_INT2_InterruptFlagClear();
    EX_INT3_InterruptFlagClear();
    EX_INT4_InterruptFlagClear();
    EX_INT0_InterruptEnable();
    EX_INT1_InterruptEnable();
    EX_INT2_InterruptEnable();
    EX_INT3_InterruptEnable();
    EX_INT4_InterruptEnable();
}

void interrupt_disable_all(){
    EX_INT0_InterruptDisable();
    EX_INT1_InterruptDisable();
    EX_INT2_InterruptDisable();
    EX_INT3_InterruptDisable();
    EX_INT4_InterruptDisable();
}