
/**
 * C library for interfacing a T6963C display with a PIC microcontroller
 * Copyright (C) 2015 Camil Staps <info@camilstaps.nl>

 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 *******************************************************************************
 *
 * File:   t6963c.h
 * Author: Camil Staps <info@camilstaps.nl>
 *
 * C library for interfacing a T6963C LCD display. This library was written with
 * the help of the application note "Writing Software for T6963C based Graphic
 * LCDs", https://www.sparkfun.com/datasheets/LCD/Monochrome/T6963C-AppNote.pdf
 * 
 * This is the generic library. In t6963c_specific.h, you should define which 
 * pins you are using for what function. Also, timer functions should be written
 * in a t6963c_specific.c. The rationale is to separate the generic and the 
 * project-specific code.
 */

#ifndef FCY
#define FCY 32000000UL // 32MHz
#endif

#include <xc.h>
#include "t6963c_specific.h"
#include <libpic30.h>

#ifndef T6963C_H
#define	T6963C_H

#ifdef	__cplusplus
extern "C" {
#endif
        
/**
 * To be written by someone implementing this library.
 * Initialise the timer for the delay functions
 */
//extern inline void t6963c_initTimer(void);
   
/**
 * To be written by someone implementing this library.
 * Start the timer for the delay functions
 */
//extern inline void t6963c_startTimer(void);
   
/**
 * To be written by someone implementing this library.
 * Get the amount of ns (nano seconds) elapsed since starting the timer with
 * t6963c_startTimer().
 */
//extern inline unsigned short t6963c_getTimeNs(void);
   
/**
 * To be written by someone implementing this library.
 * Stop the timer for the delay functions
 */
//extern inline void t6963c_stopTimer(void);

#ifndef t6963c_rst
extern inline void t6963c_rst(unsigned);
extern inline void t6963c_cd(unsigned);
extern inline void t6963c_ce(unsigned);
extern inline void t6963c_rd(unsigned);
extern inline void t6963c_wr(unsigned);
extern inline void t6963c_t_rst(unsigned);
extern inline void t6963c_t_cd(unsigned);
extern inline void t6963c_t_ce(unsigned);
extern inline void t6963c_t_rd(unsigned);
extern inline void t6963c_t_wr(unsigned);

extern inline void t6963c_data(unsigned int);
extern inline void t6963c_t_data(unsigned int);
#endif

#ifndef t6963c_rows
#define t6963c_rows 16
#endif
#ifndef t6963c_columns
#define t6963c_columns 40
#endif

// Text attribute mode definitions
#define t6963c_attr_normal 0x00
#define t6963c_attr_invert 0x05
#define t6963c_attr_inhibit 0x03
#define t6963c_attr_blink 0x08
#define t6963c_attr_bl_invert 0x0d
#define t6963c_attr_bl_inhibit 0x0b

// Definiciones adicionales

// Comandos tomados de la nota de aplicación AN029 y del datasheet del t6963c

#define t6963c_CMD_CursorPointerSet               0b00100001
#define t6963c_CMD_OffsetRegisterSet              0b00100010
#define t6963c_CMD_AddressPointerSet              0b00100100

// Set Control Word Command
#define t6963c_CMD_TextHomeAddressSet             0b01000000
#define t6963c_CMD_TextAreaSet                    0b01000001 // Se debe enviar el número de columnas
#define t6963c_CMD_GraphicHomeAddressSet          0b01000010
#define t6963c_CMD_GraphicAreaSet                 0b01000011 // Se debe enviar el número de columnas

// Mode Set Command
#define t6963c_CMD_TextAttributeModeSet           0b10000100
#define t6963c_CMD_InternalCGROMSet               0b10000000
#define t6963c_CMD_ExternalCGROMSet               0b10001000

// Display Mode Set Command
#define t6963c_CMD_DisplayOff                     0b10010000
#define t6963c_CMD_CursorOnBlinkOff               0b10010010 
#define t6963c_CMD_CursorOnBlinkOn                0b10010011 
#define t6963c_CMD_TextOnGraphicsOff              0b10010100
#define t6963c_CMD_TextOffGraphicsOn              0b10011000
#define t6963c_CMD_TextOnGraphicsOn               0b10011100

// Cursor Pattern Select Command
#define t6963c_CMD_OneLineCursorSet               0b10100000

// Data Auto R/W Commands
#define t6963c_CMD_DataAutoWriteSet               0b10110000
#define t6963c_CMD_DataAutoReadSet                0b10110001
#define t6963c_CMD_AutoReset                      0b10110010

// Data R/W Commands
#define t6963c_CMD_WriteIncrement                 0b11000000
#define t6963c_CMD_ReadIncrement                  0b11000001
#define t6963c_CMD_WriteDecrement                 0b11000010
#define t6963c_CMD_ReadDecrement                  0b11000011
#define t6963c_CMD_WriteUnchanged                 0b11000100
#define t6963c_CMD_ReadUnchanged                  0b11000101

// COMMANDS: BIT SET / RESET
#define t6963c_CMD_BitReset                       0b11110000 // Ultimos 3 bits deben decidir cual de los 8 bits se editará
#define t6963c_CMD_BitSet                         0b11111000

#define CMD                                       1 // Enviar Comando
#define DATA                                      0 // Enviar Data
#define HIGH                                      1
#define LOW                                       0
#define ZERO                                      0x00
#define OFFSET_ASCII                              0x20 // Se debe restar para transformar ASCII a T6963C
#define Delay_ns                                  2 //ns
#define SizeChar                                  0x8

// Definir secciones de memoria
#define t6963c_TextAddress                        0x17FF
#define t6963c_GraphicAddress                     0x0000
#define t6963c_OffsetAddress                      0b0001111

#define screen_width                              240 //px
#define screen_height                             128 //px
        
/**
 * Write a single byte to the LCD
 * @param cd whether this is a command (1) or data (0)
 * @param byte the byte to send
 */
void t6963c_writeByte(unsigned cd, char byte);

/**
 * Write a command with one data byte to the LCD
 * @param cmd the command byte
 * @param data the data byte
 */
void t6963c_writeCmd1(char cmd, char data);

/**
 * Write a command with two data bytes to the LCD
 * @param cmd the command byte
 * @param data1 the first data byte
 * @param data2 the second data byte
 */
void t6963c_writeCmd2(char cmd, char data1, char data2);

/**
 * Start "auto write mode". After this, characters may be written with 
 * t6963c_autoWrite or t6963c_autoWriteChar. No commands can be executed until
 * calling t6963c_stopAutoWrite().
 */
void t6963c_startAutoWrite(void);

/**
 * Finish an "auto write" session.
 * @see t6963c_startAutoWrite
 */
void t6963c_stopAutoWrite(void);

/**
 * Auto write a raw byte to the LCD
 * @param byte the byte
 * @see t6963c_startAutoWrite
 */
void t6963c_autoWrite(char byte);

/**
 * Auto write an ASCII byte to the LCD
 * @param character the character
 * @see t6963c_startAutoWrite
 */
inline void t6963c_autoWriteChar(char character);

/**
 * Write an ASCII string to the LCD
 * @param string
 */
void t6963c_writeString(char* string);

/**
 * Clear the LCD, and set the data address to the top left
 */
void t6963c_clear(void);

/**
 * Initialise the LCD for text attribute mode
 */
void t6963c_init(void);

/**
 * Set the data write address
 * @param row
 * @param column
 */
void t6963c_set_address(unsigned char row, unsigned char column);

/**
 * Set the cursor address
 * @param row
 * @param column
 */
void t6963c_set_cursor_address(unsigned char row, unsigned char column);

/**
 * This function may be used as a callback from a Terminal.update.
 */
//void t6963c_update_terminal(Terminal*);

void t6963c_set_address_for_text(unsigned char row, unsigned char column);

#ifdef	__cplusplus
}
#endif

#endif	/* T6963C_H */

