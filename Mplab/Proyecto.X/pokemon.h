/* 
 * File:   pokemon.h
 * Author: Diego Negrette
 *
 * Created on July 22, 2021, 9:28 PM
 */

#ifndef POKEMON_H
#define	POKEMON_H

#include "t6963c.h"

#ifdef	__cplusplus
extern "C" {
#endif

#define map_height           188 //px
#define player_size_height   2 //Bytes
#define player_size_width    2 // Bytes
#define pokemon_size_height  8 //Bytes
#define pokemon_size_width   8 // Bytes    
#define pos_inicial_row      0
#define pos_inicial_column   15
#define flag_debug           0 // Habilitar para ver mensajes de debug en el juego    

extern int game_stage;
extern int button_pressed;

enum game_stage_enum {
    SCREEN_START = 0,
    SCREEN_MAP,
    SCREEN_BATTLE,
};

enum button_pressed_enum {
    BUTTON_START = 0,
    BUTTON_A,
    BUTTON_B,
    BUTTON_C,
    BUTTON_D,
};

enum render_enum {
    FIRST_RENDER = 0,
    UPDATE_RENDER,
};

enum pokemon_enum {
    CHARMANDER = 0,
    SQUIRTLE,
};

enum stats_enum {
    VIDA = 0,
    ATAQUE,
    DEFENSA,
    VICTORIA,
    DERROTA
};

enum players_enum {
    COMPUTER = 0,
    PLAYER,
};
    
typedef struct {
    int row;
    int column;
}Character;

enum pokemon_type{
    FIRE = 0,
    WATER,
    ELECTRIC,
    NORMAL,
};

typedef struct {
    char *name;
    int pp;
    int type;
    float power;
}Move;

typedef struct {
    char *name;
    int type;
    float level;
    float attack;
    float defense;
    float hp;
    float current_hp;
    Move movimiento[4];
}Pokemon;

typedef struct{
    char *player;
    char *computer;
    int qty_moves;
    char *winner;
    char *first_attack;
}Game_stats;

void Game_Start();
void Show_Map();
void Character_render(int);
void Pokeball_render(int, int);
void CheckColision(int*, int, int);
void Delete_old_character(int, int);
int Play_Map();
void Pokemon_init(Pokemon*, char*, int, float, float, float, float);
void Load_Pokemon(Pokemon*, Pokemon*, int);
void Render_pokemon(const unsigned char*, int, int);
void Pokemon_Stats(int, int, float, int);
void Show_pokemons(Pokemon*,Pokemon*,int);
int random_num_generator();
float calculate_damage(float, float, float, float, float, float, int);
void apply_move(Pokemon*, Pokemon*, int);
void Battle_loop(Pokemon*, Pokemon*);

#ifdef	__cplusplus
}
#endif

#endif	/* POKEMON_H */

