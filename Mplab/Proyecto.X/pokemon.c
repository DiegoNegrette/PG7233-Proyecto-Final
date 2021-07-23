#include "pokemon.h"
#include "sprites.h"

void Game_Start(){
    
    unsigned short index;
    
    t6963c_writeByte(CMD, t6963c_CMD_DisplayOff);
    t6963c_set_address(0, 0);

    //Dibuja el titulo del juego
    t6963c_writeCmd2(t6963c_CMD_AddressPointerSet, t6963c_GraphicAddress & 0xff, ((t6963c_GraphicAddress >> 8) & 0xff));
    for( index = 0; index < t6963c_rows*t6963c_columns*SizeChar; index++){
        t6963c_writeCmd1(t6963c_CMD_WriteIncrement, pokemon_home[index]);
    }
    
    t6963c_set_address(0, 0); 
    t6963c_startAutoWrite();
    t6963c_writeString("123456789012345678901234567890");
    t6963c_stopAutoWrite();
    t6963c_writeByte(CMD, t6963c_CMD_TextOffGraphicsOn);
/*
    t6963c_set_address(0, 0);    

    //Dibuja el nombres de los programadores
    t6963c_writeCmd2(t6963c_CMD_AddressPointerSet, t6963c_GraphicAddress & 0xff, ((t6963c_GraphicAddress >> 8) & 0xff));
    for( index = 0; index < t6963c_rows*t6963c_columns*SizeChar; index++){
        t6963c_writeCmd1(t6963c_CMD_WriteIncrement, names[index]);
    }  

    t6963c_set_address(0, 0);

    //Borra la pantalla
    t6963c_writeCmd2(t6963c_CMD_AddressPointerSet, t6963c_GraphicAddress & 0xff, ((t6963c_GraphicAddress >> 8) & 0xff));
    for( index = 0; index < t6963c_rows*t6963c_columns*SizeChar; index++){
        t6963c_writeCmd1(t6963c_CMD_WriteIncrement, 0xff );
    }  
*/
 
   
   
};