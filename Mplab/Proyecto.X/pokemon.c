#include "pokemon.h"
#include "sprites.h"
#include "mcc_generated_files/delay.h"
#include "mcc_generated_files/ext_int.h"
#include "mcc_generated_files/tmr2.h"
#include <math.h>
#include <stdio.h>

// Constante para traducir el tipo de pokemon a string
const char *pokemon_type_translator[] = {
    "FUEGO",
    "AGUA",
    "ELECTRICIDAD",
    "NORMAL"
};

int game_stage = -1;
int button_pressed = -1;
Character player;

// Funcion que dibuja la pantalla de inicio
void Game_Start(){
    
    unsigned short index;
    
    t6963c_writeByte(CMD, t6963c_CMD_DisplayOff);

    //Dibuja el titulo del juego
    t6963c_writeCmd2(t6963c_CMD_AddressPointerSet, t6963c_GraphicAddress & 0xff, ((t6963c_GraphicAddress >> 8) & 0xff));
    for( index = 0; index < t6963c_rows*t6963c_columns*SizeChar; index++){
        t6963c_writeCmd1(t6963c_CMD_WriteIncrement, pokemon_home[index]);
        if(index%1000 == 0){
            printf(".");
        }
    }
    game_stage = SCREEN_START;
    t6963c_writeByte(CMD, t6963c_CMD_TextOffGraphicsOn);
}

// Funcion que muestra el mapa y la animacion del mapa mediante paginacion
void Show_Map(){
    
    unsigned short index;
    
    t6963c_writeByte(CMD, t6963c_CMD_DisplayOff);
    
    // Dibuja el mapa del juego
    t6963c_writeCmd2(t6963c_CMD_AddressPointerSet, t6963c_GraphicAddress & 0xff, ((t6963c_GraphicAddress >> 8) & 0xff));
    for( index = 0; index < (map_height)*t6963c_columns; index++){
        t6963c_writeCmd1(t6963c_CMD_WriteIncrement, pokemon_map[index]);
        if(index%1000 == 0){
            printf(".");
        }
    }
    Pokeball_render(18,3);
    Pokeball_render(18,25);
    t6963c_writeByte(CMD, t6963c_CMD_TextOffGraphicsOn);
    
    // Paginacion del mapa
    printf("\r\r  Bienvenido al Pueblo Paleta \r");
    int new_address;
    for( index = 1; index <= map_height-screen_height; index++ ){
        new_address = t6963c_GraphicAddress+index*t6963c_columns; 
        //printf("\r%d\r", index);
        t6963c_writeCmd2(t6963c_CMD_GraphicHomeAddressSet, (new_address) & 0xff, ((new_address >> 8) & 0xff));
        DELAY_milliseconds(25);
    }
    
    Character_render(FIRST_RENDER);
    t6963c_writeCmd2(t6963c_CMD_GraphicHomeAddressSet, t6963c_GraphicAddress & 0xff, ((t6963c_GraphicAddress >> 8) & 0xff)); // graphic home address
    
    printf("\r  Elige un pokemon en la parte baja del pueblo\r");
    printf("\r  Para moverte utiliza:");
    printf("\r  A: Subir");
    printf("\r  B: Bajar");
    printf("\r  C: Izquierda");
    printf("\r  D: Derecha");
    
    printf("\r\r  La Pokebola izquierda contiene a Charmander");
    printf("\r  La Pokebola derecha contiene a Squirtle");
    printf("\r\r  Presiona start cuando estes encima de la Pokebola\r  para seleccionarla\r");
}

// Funcion que muestra al jugador en la parte superior de la pantalla luego de la animacion
void Character_render(int render_time){
    int initial_address,address;
    int index,j,iteration=0;
    if (render_time == FIRST_RENDER){
        player.row = pos_inicial_row;
        player.column = pos_inicial_column;
    }
    initial_address = player.row*t6963c_columns*SizeChar + player.column;
    for(index=0; index<player_size_height*SizeChar; index++){
        address = initial_address + index*t6963c_columns;
        t6963c_writeCmd2(t6963c_CMD_AddressPointerSet, address & 0xff, ((address >> 8) & 0xff));
        for (j=0; j<player_size_width; j++){            
            t6963c_writeCmd1(t6963c_CMD_WriteIncrement, character_16_16[iteration]);
            iteration++;
        }
    }
}

// Funcion que dibuja las Pokebolas en la parte inferior del mapa
void Pokeball_render(int row, int col){
    int initial_address,address;
    int index,j,iteration=0;
    initial_address = row*t6963c_columns*SizeChar + col;
    for(index=0; index<player_size_height*SizeChar; index++){
        address = initial_address + index*t6963c_columns;
        t6963c_writeCmd2(t6963c_CMD_AddressPointerSet, address & 0xff, ((address >> 8) & 0xff));
        for (j=0; j<player_size_width; j++){            
            t6963c_writeCmd1(t6963c_CMD_WriteIncrement, pokeball_16_16[iteration]);
            iteration++;
        }
    }
}

// Funcion que evita que el jugador sobreescriba secciones del mapa parandose en puntos ocupados por objetos
void CheckColision(int* move, int row, int col){
    
    int initial_address,address,value_byte,mover = 1;
    int index,j;
    initial_address = row*t6963c_columns*SizeChar + col;
    for(index=0; index<player_size_height*SizeChar; index++){
        address = initial_address + index*t6963c_columns;
        for (j=0; j<player_size_width; j++){            
            value_byte = pokemon_map[address+j];
            if (value_byte != 0x00){
                mover = 0;
            }
        }
    }
    *move = mover;
}

// Funcion que elimina al personaje a medida que se mueve
void Delete_old_character(int row, int col){
    int initial_address,address;
    int index,j;
    initial_address = row*t6963c_columns*SizeChar + col;
    for(index=0; index<player_size_height*SizeChar; index++){
        address = initial_address + index*t6963c_columns;
        t6963c_writeCmd2(t6963c_CMD_AddressPointerSet, address & 0xff, ((address >> 8) & 0xff));
        for (j=0; j<player_size_width; j++){            
            t6963c_writeCmd1(t6963c_CMD_WriteIncrement, 0x00);
        }
    }
}

// Funcion donde el personaje interactua con el mapa hasta seleccionar la pokebola
int Play_Map(){
    int selection;
    int move;
    int new_row, new_col, row, col;
    int new_address = t6963c_GraphicAddress;
    int stayInMap = 1;
    
    while(stayInMap){
        interrupt_enable_all();
        while(button_pressed == -1);
        interrupt_disable_all();
        switch(button_pressed){
            case BUTTON_START:
                if (player.row == 16 && player.column == 3){
                    selection = CHARMANDER;
                    stayInMap = 0;
                }
                else if (player.row == 16 && player.column == 25){
                    selection = SQUIRTLE;
                    stayInMap = 0;
                }
                row = 0;
                col = 0;
                break;
            case BUTTON_A:
                row = -1;
                col = 0;
                break;
            case BUTTON_B:
                row = 1;
                col = 0;
                break;
            case BUTTON_C:
                row = 0;
                col = -1;
                break;
            case BUTTON_D:
                row = 0;
                col = 1;
                break;
        }
        new_row = player.row + row;
        new_col = player.column + col;
        if (button_pressed != BUTTON_START && stayInMap && new_row >= 0 && new_row < map_height/SizeChar - player_size_height && new_col >= 0 && new_col < t6963c_columns - player_size_width/2){
            CheckColision(&move, new_row, new_col);
            if((new_row == 17)&&(new_col == 3 || new_col == 4 || new_col == 25 || new_col == 24)){
                move = 0;
            }
            if(move){
                Delete_old_character(player.row, player.column);
                player.row = new_row;
                player.column = new_col;
                if (new_row >= 8 && new_row < 15 && (button_pressed == BUTTON_A || button_pressed == BUTTON_B)){
                    new_address += row*t6963c_columns*SizeChar;
                    t6963c_writeCmd2(t6963c_CMD_GraphicHomeAddressSet, (new_address) & 0xff, ((new_address >> 8) & 0xff));
                }
                Character_render(UPDATE_RENDER);
            }
        }
        button_pressed = -1;
    }
    
    if (selection == CHARMANDER){
        printf("\r  Ha seleccionado a Charmander como su nuevo pokemon");
    }
    else if (selection == SQUIRTLE){
        printf("\r  Ha seleccionado a Squirtle como su nuevo pokemon");
    }
    
    game_stage = SCREEN_BATTLE;
    return selection;
}

// Funcion que inicializa el valor de los pokemones
void Pokemon_init(Pokemon *pokemonPtr, char *name, int type, float level, float attack, float defense, float hp){
    pokemonPtr->name = name;
    pokemonPtr->type = type;
    pokemonPtr->level = level;
    pokemonPtr->attack = attack;
    pokemonPtr->defense = defense;
    pokemonPtr->hp = hp;
    pokemonPtr->current_hp = hp;
}

// Funcion que carga los pokemones dependiendo de la pokebola elegida
void Load_Pokemon(Pokemon *playerPtr, Pokemon *computerPtr, int pokemon_select){
    if (pokemon_select == CHARMANDER){
        Pokemon_init(playerPtr, "Charmander", FIRE, 1, 52, 43, 39); //HP 39 probare con 100
        playerPtr->movimiento[0].name = "Scratch";
        playerPtr->movimiento[0].pp = 3;
        playerPtr->movimiento[0].type = NORMAL;
        playerPtr->movimiento[0].power = 40;
        playerPtr->movimiento[1].name = "Ember";
        playerPtr->movimiento[1].pp = 2;
        playerPtr->movimiento[1].type = FIRE;
        playerPtr->movimiento[1].power = 40;
        playerPtr->movimiento[2].name = "Fire Fang";
        playerPtr->movimiento[2].pp = 3;
        playerPtr->movimiento[2].type = FIRE;
        playerPtr->movimiento[2].power = 65;
        playerPtr->movimiento[3].name = "Slash";
        playerPtr->movimiento[3].pp = 1;
        playerPtr->movimiento[3].type = NORMAL;
        playerPtr->movimiento[3].power = 70;
    }
    else{
        Pokemon_init(playerPtr, "Squirtle", WATER, 1, 48, 65, 44); //HP 52 probare con 100
        playerPtr->movimiento[0].name = "Tackle";
        playerPtr->movimiento[0].pp = 3;
        playerPtr->movimiento[0].type = NORMAL;
        playerPtr->movimiento[0].power = 40;
        playerPtr->movimiento[1].name = "Water Gun";
        playerPtr->movimiento[1].pp = 2;
        playerPtr->movimiento[1].type = WATER;
        playerPtr->movimiento[1].power = 40;
        playerPtr->movimiento[2].name = "Aqua Tail";
        playerPtr->movimiento[2].pp = 1;
        playerPtr->movimiento[2].type = WATER;
        playerPtr->movimiento[2].power = 90;
        playerPtr->movimiento[3].name = "Bite";
        playerPtr->movimiento[3].pp = 3;
        playerPtr->movimiento[3].type = NORMAL;
        playerPtr->movimiento[3].power = 60;
    }
    
    Pokemon_init(computerPtr, "Pikachu", ELECTRIC, 1, 55, 40, 35); //HP 35 probare con 100
    computerPtr->movimiento[0].name = "Slam";
    computerPtr->movimiento[0].pp = 3;
    computerPtr->movimiento[0].type = NORMAL;
    computerPtr->movimiento[0].power = 90;
    computerPtr->movimiento[1].name = "Spark";
    computerPtr->movimiento[1].pp = 4;
    computerPtr->movimiento[1].type = ELECTRIC;
    computerPtr->movimiento[1].power = 65;
    computerPtr->movimiento[2].name = "Quick attack";
    computerPtr->movimiento[2].pp = 2;
    computerPtr->movimiento[2].type = NORMAL;
    computerPtr->movimiento[2].power = 40;
    computerPtr->movimiento[3].name = "Thunder Shock";
    computerPtr->movimiento[3].pp = 1;
    computerPtr->movimiento[3].type = ELECTRIC;
    computerPtr->movimiento[3].power = 40;
}

// Funcion que dibuja a los pokemones en pantalla
void Render_pokemon(const unsigned char *imgPtr, int row, int col){
    int initial_address,address;
    int index,j;
    initial_address = row*t6963c_columns*SizeChar + col;
    for(index=0; index<pokemon_size_height*SizeChar; index++){
        address = initial_address + index*t6963c_columns;
        t6963c_writeCmd2(t6963c_CMD_AddressPointerSet, address & 0xff, ((address >> 8) & 0xff));
        for (j=0; j<pokemon_size_width; j++){            
            t6963c_writeCmd1(t6963c_CMD_WriteIncrement, *imgPtr);
            imgPtr++;
        }
        if(index%1000 == 0){
            printf(".");
        }
    }
}

// Funcion que dibuja los stats en la pantalla
void Pokemon_Stats(int render_time, int stat, float value, int column){
        int print_number = 1;
        char num_char[10];
        sprintf(num_char, "%.2d", (int) value);
        
        switch(stat){
            case VIDA:
                if(render_time == FIRST_RENDER){
                    t6963c_set_address_for_text(0, column); 
                    t6963c_startAutoWrite();
                    t6963c_writeString("Vida:");
                    t6963c_stopAutoWrite();
                }
                t6963c_set_address_for_text(0, column+9);
                break;
            case ATAQUE:
                if(render_time == FIRST_RENDER){
                    t6963c_set_address_for_text(1, column); 
                    t6963c_startAutoWrite();
                    t6963c_writeString("Ataque:");
                    t6963c_stopAutoWrite();
                }
                t6963c_set_address_for_text(1, column+9);
                break;
            case DEFENSA:
                if(render_time == FIRST_RENDER){
                    t6963c_set_address_for_text(2, column); 
                    t6963c_startAutoWrite();
                    t6963c_writeString("Defensa:");
                    t6963c_stopAutoWrite();
                }
                t6963c_set_address_for_text(2, column+9);
                break;
            case VICTORIA:
                t6963c_set_address_for_text(8, 10); 
                t6963c_startAutoWrite();
                t6963c_writeString("VICTORIA");
                t6963c_stopAutoWrite();
                print_number = 0;
                break;
            case DERROTA:
                t6963c_set_address_for_text(8, 10); 
                t6963c_startAutoWrite();
                t6963c_writeString("DERROTA");
                t6963c_stopAutoWrite();
                print_number = 0;
                break;
        }
       if(print_number){
           t6963c_startAutoWrite();
           t6963c_writeString(num_char);  
           t6963c_stopAutoWrite();
       }
};

// Funcion que muestra a los pokemones en pantalla mas los stats
void Show_pokemons(Pokemon *playerPtr, Pokemon *computerPtr, int pokemon_select){
    
    // Borra la Pantalla
    int index;
    t6963c_writeByte(CMD, t6963c_CMD_DisplayOff);
    t6963c_writeCmd2(t6963c_CMD_AddressPointerSet, t6963c_GraphicAddress & 0xff, ((t6963c_GraphicAddress >> 8) & 0xff));
    for( index = 0; index < map_height*t6963c_columns; index++){
        t6963c_writeCmd1(t6963c_CMD_WriteIncrement, 0x00 );
        if(index%1000 == 0){
            printf(".");
        }
    }
    
    t6963c_writeCmd2(t6963c_CMD_GraphicHomeAddressSet, t6963c_GraphicAddress & 0xff, ((t6963c_GraphicAddress >> 8) & 0xff)); // graphic home address
    if (pokemon_select == CHARMANDER){
        Render_pokemon(charmander_64_64,6,2);
    }
    else if (pokemon_select == SQUIRTLE){
        Render_pokemon(squirtle_64_64,6,2);
    }
    Render_pokemon(pikachu_64_64,6,20);
    
    Pokemon_Stats(FIRST_RENDER,VIDA,playerPtr->current_hp,0);
    Pokemon_Stats(FIRST_RENDER,ATAQUE,playerPtr->attack,0);
    Pokemon_Stats(FIRST_RENDER,DEFENSA,playerPtr->defense,0);
    Pokemon_Stats(FIRST_RENDER,VIDA,computerPtr->current_hp,17);
    Pokemon_Stats(FIRST_RENDER,ATAQUE,computerPtr->attack,17);
    Pokemon_Stats(FIRST_RENDER,DEFENSA,computerPtr->defense,17);
    
    t6963c_writeByte(CMD, t6963c_CMD_TextOnGraphicsOn);
    
    printf("\r\r  *La batalla ha iniciado*\r");
}

// Funcion que genera un numero aleatorio segun el rango especificado
int random_num_generator(int qty_options){
    int random_num;
    random_num = (TMR2_Counter16BitGet()/(0xFFFF/qty_options));
    return random_num;
}

// Funcion que calcula el dano del ataque dependiendo de los atributos de ambos pokemones
float calculate_damage(float attacker_level, float attack, float power, float defense, float type_bonus, float type_modifier, int is_crit){
    /*
    attacker_level = attacker's Level
    attack = attacker's Attack or Special
    power = attack Power
    defense = defender's Defense or Special
    type_bonus = same-Type attack bonus (1 or 1.5)
    type_modifier = Type modifiers (40, 20, 10, 5, 2.5, or 0)
    z = a random number between 217 and 255
    is_crit = true or false
    */
    
    float crit_mult = 2; //No crit, hardcoded
    float damage = 0;
    
    if(is_crit){
        crit_mult = 4;
        printf("\r  El ataque ha aplicado critico");
    }
    else{
        if(flag_debug){
            printf("\r  Aplica critico: NO");
        }
    }
    if(flag_debug){
        printf("\r  Dano calculado 1: %.2f",(double) ((((((crit_mult * attacker_level) / 5) + 2) * attack * power)) / defense));
    }    
    damage = floor(((((crit_mult * attacker_level) / 5) + 2) * attack * power) / defense);
    if(flag_debug){
        printf("\r  Dano calculado 1 rounded: %.2f",(double) damage);
        printf("\r  Dano calculado 2: %.2f", (double) damage/50);
    }
    damage = floor(damage / 50) + 2;
    if(flag_debug){
        printf("\r  Dano calculado 2 + 2: %.2f", (double) damage);
        printf("\r  Dano calculado 3: %.2f",(double) (damage*type_bonus));
    }
    damage = floor(damage * type_bonus);
    if(flag_debug){
        printf("\r  Dano calculado 3 rounded %.2f",(double) damage);
        printf("\r  Dano calculado 4: %.2f",(double) (damage*type_modifier));
    }
    damage = floor(damage * type_modifier);
    if(flag_debug){
        printf("\r  Dano calculado 4 rounded %.2f",(double) damage);
    }
    
    /*
    This is how you could compute the minimum and maximum
    damage that this ability will do. Only for reference.
    min_damage = np.floor((damage * 217) / 255)
    max_damage = np.floor((damage * 255) / 255)
    */
    
    int z = 255 - random_num_generator(255-217);
    if(flag_debug){
        printf("\r  Valor aleatorio z calculado %.2f", (double) z);
    }
    if(flag_debug){
        printf("\r  Dano calculado 5: %.2f",(double) ((damage * z) / 255));
    }
    damage = floor((damage * z) / 255);
    printf("\r  El dano realizado es: %.0f", (double)damage);
    
    return damage;
}

// Funcion que aplica los movimientos seleccionados para cada turno de ataque
void apply_move(Pokemon* attacker, Pokemon* defender, int move_number){
    int is_crit = random_num_generator(2);
    float type_bonus = 1;
    float attack, defense, power;
    float attacker_level;
    float damage;
    float type_modifier=1; // No se implementara
    
    printf("\r\r  Atacando: %s", attacker->name);
    printf("\r  Defendiendo: %s", defender->name);
    printf("\r  Ataque seleccionado: %s", attacker->movimiento[move_number].name);
    printf("\r  Tipo: %s", pokemon_type_translator[attacker->movimiento[move_number].type]);
    printf("\r  Power: %.0f", (double) attacker->movimiento[move_number].power);
    printf("\r  PP: %d", attacker->movimiento[move_number].pp);
    
    if(attacker->type == attacker->movimiento[move_number].type){
        type_bonus = 1.5;
        if (flag_debug){
            printf("\r  Aplica bonus por ataque del mismo elemento: SI");
        }
    }
    else{
        if(flag_debug){
            printf("\r  Aplica bonus por ataque del mismo elemento: NO");
        }
    }
    
    attack = attacker->attack;
    defense = defender->defense;
    
    //No se implementaran habilidades especiales
    
    attacker_level = attacker->level;
    
    power = attacker->movimiento[move_number].power;
    if (power == 0){
        power = 1;
    }
    
    damage = calculate_damage(attacker_level, attack, power, defense, type_bonus, type_modifier, is_crit);
    
    defender->current_hp -= damage;
    if (defender->current_hp < 0){
        defender->current_hp = 0;
    }
    attacker->movimiento[move_number].pp -= 1;
}

// Loop Final donde se seleccionan las habilidades a utilizar y se actualiza en pantalla
// la vida de cada pokemon
void Battle_loop(Pokemon* player, Pokemon* computer){
    
    Pokemon *attacker;
    Pokemon *defender;
    int approve_pp;
    
    int computer_move,move_attacker,move_defender,i;
    
    Game_stats game_info;
    game_info.player = player->name;
    game_info.computer = computer->name;
    game_info.qty_moves = 0;
    
    int start = random_num_generator(2);
    
    if (start == COMPUTER){
        game_info.first_attack = computer->name;
    }
    else{
        game_info.first_attack = player->name;
    }
    
    printf("\r  El primero en atacar sera %s",game_info.first_attack);
    
    while(1){
        approve_pp = 0;
        printf("\r\r  ****************************************************************** ");
        printf("\r\r  * Seleccione una habilidad utilizando\r  A,B,C y D respectivamente* ");
        for (i=0; i<4; i++){
            printf("\r\r  Ataque: %s - Tipo: %s - Power: %.0f - PP: %d", player->movimiento[i].name, pokemon_type_translator[player->movimiento[i].type], (double) player->movimiento[i].power, player->movimiento[i].pp);
        }
        
        printf("\r\r  Las habilidades del oponente: ");
        for (i=0; i<4; i++){
            printf("\r\r  Ataque: %s - Tipo: %s - Power: %.0f - PP: %d", computer->movimiento[i].name, pokemon_type_translator[computer->movimiento[i].type], (double) computer->movimiento[i].power, computer->movimiento[i].pp);
        }
        
        interrupt_enable_all();
        while(approve_pp == 0){
            while(button_pressed == -1);
            if (player->movimiento[button_pressed].pp > 0){
                approve_pp = 1;
            }
            else{
                printf("\r\r  La habilidad %s tiene %d pp, elija otra habilidad",player->movimiento[button_pressed].name,player->movimiento[button_pressed].pp);
                button_pressed = -1;
            }
        }
        approve_pp = 0;
        while(approve_pp == 0){
            computer_move = random_num_generator(4);
            if (computer->movimiento[computer_move].pp > 0){
                approve_pp = 1;
            }
            else{
                printf("\r\r  La habilidad %s elegida por la computadora tiene %d pp, probar otra habilidad",computer->movimiento[computer_move].name,computer->movimiento[computer_move].pp);
            }
        }
        while(button_pressed == -1);
        interrupt_disable_all();
        printf("\r\r  Ha seleccionado %s",player->movimiento[button_pressed].name);
        printf("\r  El oponente ha seleccionado %s",computer->movimiento[computer_move].name);
        
        if(start == COMPUTER){
            attacker = computer;
            defender = player;
            move_attacker = computer_move;
            move_defender = button_pressed;
        }
        else{
            attacker = player;
            defender = computer;
            move_attacker = button_pressed;
            move_defender = computer_move;
        }
        
        game_info.qty_moves += 1;
        apply_move(attacker,defender,move_attacker);
        if (defender->current_hp <= 0){
            game_info.winner = attacker->name;
            break;
        }
        
        //game_info.qty_moves += 1;
        apply_move(defender,attacker,move_defender);
        if (attacker->current_hp <= 0){
            game_info.winner = defender->name;
            break;
        }
        Pokemon_Stats(UPDATE_RENDER,VIDA,player->current_hp,0);
        if(flag_debug){
            printf("\r\r  Vida player: %.0f", (double)player->current_hp);
        }
        Pokemon_Stats(UPDATE_RENDER,VIDA,computer->current_hp,17);
        if(flag_debug){
            printf("\r  Vida computadora: %.0f",(double) computer->current_hp);
        }
        button_pressed = -1;
    }
    
    Pokemon_Stats(UPDATE_RENDER,VIDA,player->current_hp,0);
    Pokemon_Stats(UPDATE_RENDER,VIDA,computer->current_hp,17);
    if(game_info.winner == game_info.player){
        Pokemon_Stats(UPDATE_RENDER,VICTORIA,0,0);
        printf("\r\r  VICTORIA");
    }
    else{
        Pokemon_Stats(UPDATE_RENDER,DERROTA,0,0);
        printf("\r\r  DERROTA");
    }
    
    printf("\r\r  Ganador: %s", game_info.winner);
    printf("\r  Cantidad de movimientos: %d", game_info.qty_moves);
    
    
}