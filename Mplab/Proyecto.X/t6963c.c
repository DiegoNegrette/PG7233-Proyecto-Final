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
 */

#include "t6963c.h"

inline void delay_ns(unsigned short ns) {
    __delay32( (unsigned long) (((unsigned long long) ns)*(FCY)/1000000000ULL));
}

void t6963c_writeByte(unsigned cd, char byte) {
    t6963c_cd(cd);
    t6963c_wr(LOW);
	t6963c_data(byte);
    t6963c_ce(LOW);
    delay_ns(Delay_ns);
    t6963c_ce(HIGH);
    t6963c_wr(HIGH);
    delay_ns(Delay_ns);
}

void t6963c_writeCmd1(char cmd, char data) {
    t6963c_writeByte(DATA, data);
    t6963c_writeByte(CMD, cmd);
    delay_ns(Delay_ns);
}

void t6963c_writeCmd2(char cmd, char data1, char data2) {
    t6963c_writeByte(DATA, data1);
    t6963c_writeByte(DATA, data2);
    t6963c_writeByte(CMD, cmd);
    delay_ns(Delay_ns);
}

void t6963c_startAutoWrite(void) {
    t6963c_writeByte(1, 0xb0);
    delay_ns(Delay_ns);
    delay_ns(Delay_ns);
}

void t6963c_stopAutoWrite(void) {
    t6963c_writeByte(1, 0xb2);
    delay_ns(Delay_ns);
    delay_ns(Delay_ns);
}

void t6963c_autoWrite(char byte) {
    t6963c_cd(LOW);
    t6963c_wr(LOW);
    t6963c_data(byte);
    t6963c_ce(LOW);
    delay_ns(Delay_ns);
    t6963c_ce(HIGH);
    t6963c_wr(HIGH);
    delay_ns(Delay_ns);
}

inline void t6963c_autoWriteChar(char byte) {
    t6963c_autoWrite(byte - OFFSET_ASCII);
}

void t6963c_writeString(char* string) {
    t6963c_startAutoWrite();
    for (;*string;string++)
        t6963c_autoWriteChar(*string);
    t6963c_stopAutoWrite();
}

void t6963c_clear(void) {
    unsigned short i;
    t6963c_set_address(0, 0);
    t6963c_startAutoWrite();
    for (i = 0; i < t6963c_rows * t6963c_columns; i++) {
        t6963c_autoWrite(ZERO);
    }
    t6963c_writeCmd2(t6963c_CMD_AddressPointerSet, ZERO, ZERO);
    for (i = 0; i < t6963c_rows * t6963c_columns; i++) {
        t6963c_autoWrite(t6963c_attr_normal);
    }
    t6963c_stopAutoWrite();
    t6963c_set_cursor_address(0, 0);
}

void t6963c_init(void) {
    unsigned short i;
    
    t6963c_t_rst(LOW);
    t6963c_t_cd(LOW);
    t6963c_t_ce(LOW);
    t6963c_t_rd(LOW);
    t6963c_t_wr(LOW);
    t6963c_t_data(ZERO);
    
    t6963c_wr(HIGH);
    t6963c_rd(HIGH);
    t6963c_cd(HIGH);
    t6963c_ce(HIGH);
    
    t6963c_rst(LOW);
    for (i = 0; i < 10; i++)
        delay_ns(Delay_ns);
    t6963c_rst(HIGH);
    
    // Modificado siguiendo la inicializacion recomendada por el datasheet
    
    // Mode Set Command
    t6963c_writeByte(CMD, t6963c_CMD_InternalCGROMSet | 0x1); // Internal ROM, Texto or Gráficos
    // Display Mode Set Command
    t6963c_writeByte(CMD, (t6963c_CMD_TextOnGraphicsOn | t6963c_CMD_CursorOnBlinkOn));// Gráficos y texto encendido. Cursor encendido parpadeando
    t6963c_writeCmd2(t6963c_CMD_OffsetRegisterSet, t6963c_OffsetAddress, ZERO); // Offset Register
    
    t6963c_writeCmd2(t6963c_CMD_TextHomeAddressSet, t6963c_TextAddress & 0xff, ((t6963c_TextAddress >> 8) & 0xff)); // text home address
    t6963c_writeCmd2(t6963c_CMD_TextAreaSet, t6963c_columns, ZERO); // text area set
    t6963c_writeCmd2(t6963c_CMD_GraphicHomeAddressSet, t6963c_GraphicAddress & 0xff, ((t6963c_GraphicAddress >> 8) & 0xff)); // graphic home address
    t6963c_writeCmd2(t6963c_CMD_GraphicAreaSet, t6963c_columns, ZERO); // graphic area set
    
    t6963c_writeByte(CMD, t6963c_CMD_OneLineCursorSet); // 8-line cursor
    
    t6963c_clear();
    
    t6963c_set_address(ZERO, ZERO);
    t6963c_set_cursor_address(ZERO, ZERO);
    
}

void t6963c_set_address(unsigned char row, unsigned char column) {
    unsigned short address = 
            ((unsigned short) row) * ((unsigned short) t6963c_columns) + column;
    t6963c_writeCmd2(t6963c_CMD_AddressPointerSet, address & 0xff, ((address >> 8) & 0xff));
}

void t6963c_set_cursor_address(unsigned char row, unsigned char column) {
    t6963c_writeCmd2(t6963c_CMD_CursorPointerSet, column, row);
}

void t6963c_set_address_for_text(unsigned char row, unsigned char column) {
    unsigned short address = ((unsigned short) row) * ((unsigned short) t6963c_columns) + column;
    t6963c_writeCmd2(t6963c_CMD_AddressPointerSet, (t6963c_TextAddress+address) & 0xff, (((t6963c_TextAddress+address) >> 8) & 0xff));
}