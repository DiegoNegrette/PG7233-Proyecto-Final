/**
  Generated main.c file from MPLAB Code Configurator

  @Company
    Microchip Technology Inc.

  @File Name
    main.c

  @Summary
    This is the generated main.c using PIC24 / dsPIC33 / PIC32MM MCUs.

  @Description
    This source file provides main entry point for system initialization and application code development.
    Generation Information :
        Product Revision  :  PIC24 / dsPIC33 / PIC32MM MCUs - 1.170.0
        Device            :  PIC24FJ128GA010
    The generated drivers are tested against the following:
        Compiler          :  XC16 v1.61
        MPLAB 	          :  MPLAB X v5.45
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
  Section: Included Files
*/
#include "mcc_generated_files/system.h"
#include "pokemon.h"
#include "t6963c.h"
#include "mcc_generated_files/ext_int.h"
#include <stdio.h>

/*
                         Main application
 */
int main(void)
{
    Pokemon player;
    Pokemon computer;
    // initialize the device
    SYSTEM_Initialize();
    int pokemon_select;
    t6963c_init();
    printf("  * USB PG7233 *\r");
    printf("  * Diego Negrette 19-91277 *\r\r");
    printf("  Iniciando el juego");
    Game_Start();
    printf("\r\r  * PRESIONE START *\r\r");
    interrupt_enable_all();
    while(game_stage == SCREEN_START);
    interrupt_disable_all();
    printf("  Cargando el mapa");
    Show_Map();
    interrupt_enable_all();
    pokemon_select = Play_Map();
    interrupt_disable_all();
    printf("\r\r  Cargando batalla");
    Load_Pokemon(&player,&computer,pokemon_select);
    Show_pokemons(&player,&computer,pokemon_select);
    Battle_loop(&player, &computer);
    
    while (1)
    {
        // Add your application code
    }

    return 1;
}
/**
 End of File
*/

