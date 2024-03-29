/**
 * @file spectre.h
 * @author Justin Siao (JSiao)
 * All game-specific logic shall be done in here and its appropriate 
 * Source file.
 */

#pragma once
#ifndef _SPECTRE_H
#define _SPECTRE_H

#include "game.h"
#include "tools.h"

#define S_X 0
#define S_Y 1
#define SPRITE_SIZE 24
#define NUMBER_OF_MAPS 5

// structures ------------------------------------------------------------------

enum game_states
{
  STATE_PLAY = 0,
  STATE_HELP,
  STATE_ANIM,
  STATE_RETRY,
  TOTAL_STATES
};

enum SPRITE_TYPES
{
  TYPE_RUNE,
  TYPE_SPECTRE
};

enum direction
{
  DIR_DISABLED = 0,
  DIR_UP,
  DIR_DOWN,
  DIR_LEFT,
  DIR_RIGHT
};

struct player
{
  double pos[2];
  double dir[2];
};

struct sprite
{
  double   pos[2];
  char     visible;
  char     type;
  char     rendered;
  char     direction;
  char   (*sprite)[SPRITE_SIZE][SPRITE_SIZE];
  U8 opacity;
  int active;
};

struct spectre_vars
{
  char   (*map)[24][24];
  struct   player player;
  struct   sprite sprite_list[4];
  int      sprite_stack[4];
  U32      pallette[10];
  double   plane[2];
  int      score;
  double  *pixel_buffer;
  enum game_states state;
  double   anim;
  void (*key_update[TOTAL_STATES])(struct spectre_vars*, U64);
  void (*logic_update[TOTAL_STATES])(struct spectre_vars*, U64);
  void (*video_update[TOTAL_STATES])(struct spectre_vars*, U64);
  void (*audio_update[TOTAL_STATES])(struct spectre_vars*, U64);
  void (*animation)(struct spectre_vars*, U64);
}; 

extern struct spectre_vars spectre_globals;

// Data ------------------------------------------------------------

static char _maps[NUMBER_OF_MAPS][24][24] =
{
  {
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
    {1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1},
    {1,0,1,0,1,0,1,1,1,1,1,1,1,1,1,1,1,1,0,1,0,1,0,1},
    {1,0,1,0,1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1},
    {1,0,1,0,1,0,1,1,1,1,1,1,1,1,0,1,0,1,0,1,0,1,1,1},
    {1,0,1,0,1,0,1,0,0,0,0,0,0,0,0,0,0,1,0,1,0,0,0,1},
    {1,0,1,0,1,0,1,0,1,0,1,1,1,1,0,1,0,1,0,1,0,1,0,1},
    {1,0,1,0,1,0,1,0,1,0,1,0,0,0,0,1,0,1,0,1,0,1,0,1},
    {1,0,1,0,0,0,0,0,1,0,1,0,0,0,0,1,0,1,0,1,0,1,0,1},
    {1,0,1,1,1,1,1,0,1,0,1,0,0,0,0,1,0,0,0,1,0,1,0,1},
    {1,0,1,0,0,0,0,0,1,0,1,0,1,1,0,0,0,0,0,1,0,1,0,1},
    {1,0,1,0,1,0,1,1,1,0,0,0,0,0,0,0,0,0,0,1,0,1,0,1},
    {1,0,1,0,1,0,0,0,1,0,0,0,0,1,1,1,1,1,0,1,0,1,0,1},
    {1,0,1,0,1,1,1,0,1,0,1,0,1,1,0,0,0,0,0,1,0,1,0,1},
    {1,0,1,0,1,0,0,0,1,0,0,0,0,1,0,1,1,1,0,1,0,1,0,1},
    {1,0,1,0,1,0,1,1,1,0,0,1,0,1,0,0,0,0,0,1,0,1,0,1},
    {1,0,1,0,1,0,0,0,0,0,0,1,0,0,0,1,0,1,0,1,0,0,0,1},
    {1,0,1,0,1,0,1,1,1,1,1,1,1,1,1,1,0,1,0,1,0,1,1,1},
    {1,0,1,0,1,0,1,0,0,0,0,0,0,0,0,0,0,1,0,1,0,0,0,1},
    {1,0,1,0,1,0,1,0,1,1,1,1,1,1,1,1,1,1,0,1,0,1,0,1},
    {1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1},
    {1,0,1,0,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1},
    {1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
  },
  {
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1},
    {1,0,1,0,1,0,1,1,1,1,1,1,1,1,1,1,1,1,0,1,0,1,0,1},
    {1,0,1,0,1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1},
    {1,0,1,0,1,0,1,1,1,1,1,1,1,1,0,1,0,1,0,1,0,1,1,1},
    {1,0,1,0,1,0,1,0,0,0,0,0,0,0,0,0,0,1,0,1,0,0,0,1},
    {1,0,1,0,0,0,1,0,1,0,1,1,1,1,0,1,0,1,0,1,0,1,0,1},
    {1,0,1,1,1,1,1,0,1,0,1,0,0,0,0,1,0,1,0,1,0,1,0,1},
    {1,0,0,0,0,0,0,0,1,0,1,0,1,1,1,1,0,1,0,1,0,1,0,1},
    {1,1,1,1,1,1,1,0,1,0,1,0,0,0,0,1,0,0,0,1,0,1,0,1},
    {1,0,0,0,0,0,0,0,1,0,1,1,1,1,0,1,1,1,0,1,0,1,0,1},
    {1,0,1,0,1,0,1,1,1,0,0,0,0,1,0,0,0,0,0,1,0,1,0,1},
    {1,0,1,0,1,0,0,0,1,0,0,0,0,1,1,1,1,1,0,1,0,1,0,1},
    {1,0,1,0,1,1,1,0,1,0,1,0,1,1,0,0,0,0,0,1,0,1,0,1},
    {1,0,1,0,1,0,0,0,1,0,0,0,0,1,0,1,1,1,0,1,0,1,0,1},
    {1,0,1,0,1,0,1,1,1,0,0,1,0,1,0,0,0,0,0,1,0,1,0,1},
    {1,0,1,0,1,0,0,0,0,0,0,1,0,0,0,1,0,1,0,1,0,0,0,1},
    {1,0,1,0,1,0,1,1,1,1,1,1,1,1,1,1,0,1,0,1,0,1,1,1},
    {1,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,0,0,0,1},
    {1,0,1,0,1,0,1,1,1,1,1,1,1,1,1,1,1,1,0,1,0,1,0,1},
    {1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1},
    {1,0,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
  },
  {
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,0,1},
    {1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,1},
    {1,0,1,0,1,0,1,1,1,1,1,1,1,1,0,1,0,1,0,1,0,1,1,1},
    {1,0,1,0,1,0,1,0,0,0,0,0,0,0,0,0,0,1,0,1,0,0,0,1},
    {1,0,1,0,0,0,1,0,1,0,1,1,1,1,0,1,0,1,0,1,0,1,0,1},
    {1,0,1,1,1,1,1,0,1,0,1,0,0,0,0,1,0,1,0,1,0,1,0,1},
    {1,0,0,0,0,0,0,0,1,0,1,0,1,1,1,1,0,1,0,1,0,1,0,1},
    {1,1,1,1,1,1,1,0,1,0,1,0,0,0,0,1,0,0,0,0,0,1,0,1},
    {1,0,0,0,0,0,0,0,0,0,1,1,0,1,0,1,1,1,0,1,1,1,0,1},
    {1,0,1,0,1,0,1,1,1,1,1,1,0,1,0,0,0,0,0,1,0,0,0,1},
    {1,0,1,0,1,0,0,0,0,0,0,0,0,1,1,1,1,1,0,1,0,1,0,1},
    {1,0,1,0,1,1,1,0,1,1,1,0,1,1,0,0,0,0,0,0,0,1,0,1},
    {1,0,1,0,1,0,0,0,1,0,0,0,0,1,0,1,1,1,0,1,0,1,0,1},
    {1,0,1,0,1,0,1,1,1,0,0,1,0,1,0,0,0,0,0,1,0,1,0,1},
    {1,0,1,0,1,0,0,0,0,0,0,1,0,0,0,1,0,1,0,1,0,0,0,1},
    {1,0,1,0,1,0,1,0,1,1,1,1,1,1,1,1,0,1,0,1,1,1,1,1},
    {1,0,1,0,1,0,1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,1},
    {1,0,1,0,1,0,1,1,1,1,1,1,1,1,1,1,1,1,0,1,0,1,0,1},
    {1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,0,1},
    {1,0,1,0,1,1,1,1,1,1,1,1,1,0,1,0,1,1,1,1,0,1,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,1},
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
  },
  {
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
    {1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1},
    {1,0,1,0,1,0,1,1,1,1,1,1,1,1,1,1,1,1,0,1,0,1,0,1},
    {1,0,1,0,1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1},
    {1,0,1,0,1,0,1,1,1,1,1,1,1,1,0,1,0,1,0,1,0,1,1,1},
    {1,0,1,0,1,0,1,0,0,0,0,0,0,0,0,0,0,1,0,1,0,0,0,1},
    {1,0,1,0,1,0,1,0,1,1,1,1,1,1,0,1,0,1,0,1,0,1,0,1},
    {1,0,1,0,1,0,1,0,0,0,1,0,0,0,0,1,0,1,0,1,0,1,0,1},
    {1,0,1,0,0,0,0,0,1,0,1,0,0,0,0,1,0,1,0,1,0,1,0,1},
    {1,0,1,1,1,1,1,1,1,0,1,0,0,0,0,1,0,0,0,1,0,1,0,1},
    {1,0,0,0,0,0,0,0,0,0,1,1,1,1,0,1,0,1,0,1,0,1,0,1},
    {1,0,1,0,1,0,1,1,1,0,1,0,0,0,0,0,0,1,0,1,0,1,0,1},
    {1,0,1,0,1,0,0,0,1,0,1,0,0,1,1,1,1,1,0,1,0,1,0,1},
    {1,0,1,0,1,1,1,0,1,0,1,0,1,1,0,0,0,0,0,1,0,1,0,1},
    {1,0,1,0,1,0,0,0,1,0,0,0,0,1,0,1,1,1,0,1,0,1,0,1},
    {1,0,1,0,1,0,1,1,1,0,0,1,0,1,0,1,0,0,0,1,0,1,0,1},
    {1,0,1,0,1,0,0,0,0,0,0,1,0,0,0,1,0,1,0,1,0,0,0,1},
    {1,0,1,0,1,1,1,1,0,1,1,1,1,1,0,1,0,1,0,1,1,1,1,1},
    {1,0,1,0,0,0,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,0,0,1},
    {1,0,1,1,1,1,1,0,1,1,0,1,0,1,1,1,1,1,0,1,0,1,0,1},
    {1,0,1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1,0,1},
    {1,0,1,0,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1},
    {1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
  },
  {
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,1,1},
    {1,0,1,1,0,1,1,1,1,1,1,1,1,1,1,0,1,0,1,1,0,0,0,1},
    {1,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,1,0,1,0,0,1,0,1},
    {1,0,1,1,1,1,0,1,1,1,1,0,1,0,1,0,0,0,0,0,1,0,0,1},
    {1,0,1,0,0,0,0,0,0,0,1,0,1,0,1,0,1,1,1,1,1,0,1,1},
    {1,0,1,0,1,1,0,1,1,0,1,0,1,0,1,0,0,0,0,0,0,0,0,1},
    {1,0,1,0,0,1,0,1,0,0,1,0,1,0,1,0,1,1,1,1,0,1,0,1},
    {1,0,1,1,0,1,0,1,0,0,1,0,1,0,1,0,1,0,0,1,0,1,0,1},
    {1,0,0,0,0,1,0,0,0,0,1,0,1,0,1,0,0,0,0,0,0,1,0,1},
    {1,0,1,1,1,1,0,1,1,1,1,0,1,0,1,1,1,1,1,1,1,1,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,1},
    {1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,1,0,0,0,1,1,1,0,1,0,1,0,1,1,1,1,1,1,1,1,0,1},
    {1,0,1,1,1,1,1,1,0,0,1,0,1,0,1,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,1,0,1,0,1,0,0,0,0,0,0,0,0,1},
    {1,1,1,1,1,0,1,1,1,0,1,0,1,0,1,0,1,1,1,1,1,1,0,1},
    {1,0,0,0,1,0,1,0,0,0,1,0,1,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,1,1,1,0,1,1,1,0,1},
    {1,0,1,0,0,0,0,0,0,0,1,0,1,0,0,0,0,1,0,1,0,0,0,1},
    {1,0,1,0,1,0,1,1,1,0,1,0,1,1,1,1,0,1,0,1,0,0,0,1},
    {1,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,1},
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
  }
};

static char wall_sprite[16][16] =
{
  {1,1,1,1,1,2,3,3,2,1,1,1,1,2,1,1},
  {1,1,1,1,1,1,2,1,3,2,1,1,2,3,2,1},
  {1,1,1,1,1,2,3,1,1,3,2,2,3,1,2,1},
  {2,1,1,1,2,3,1,1,1,1,3,3,1,1,3,2},
  {3,2,2,2,3,3,1,1,1,1,1,1,1,1,1,3},
  {2,3,3,3,3,3,3,2,2,1,1,1,1,1,2,2},
  {2,2,3,1,3,2,2,3,3,2,2,1,2,2,1,1},
  {1,2,1,1,2,1,1,1,2,3,3,2,3,2,1,1},
  {1,2,1,1,2,1,1,1,2,3,3,2,3,2,1,1},
  {1,2,1,1,2,1,1,2,3,2,1,1,2,3,3,2},
  {2,3,1,1,3,2,2,3,2,1,1,2,3,3,2,3},
  {3,2,1,1,2,3,3,2,2,2,2,3,2,2,1,2},
  {2,3,2,2,3,2,3,3,2,2,3,2,1,1,1,1},
  {1,2,3,2,1,1,2,2,3,3,2,1,1,1,1,1},
  {1,2,3,2,1,1,1,1,2,3,2,1,1,1,1,1},
  {2,3,3,2,1,1,1,1,2,3,3,2,1,1,1,1}
};

static char rune[SPRITE_SIZE][SPRITE_SIZE] =
{
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,4,4,0,4,4,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,4,4,0,0,4,4,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,4,4,0,0,0,4,4,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,4,4,0,0,0,4,4,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,4,4,0,0,4,4,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,4,4,0,4,4,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
};

static char spectre[SPRITE_SIZE][SPRITE_SIZE] =
{
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,5,5,5,5,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,5,5,5,5,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,5,5,5,5,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,5,5,5,5,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,5,5,5,5,5,5,5,5,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,5,5,5,5,5,5,5,5,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,5,5,5,5,5,5,5,5,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,5,5,5,5,5,5,5,5,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,5,5,5,5,5,5,5,5,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,5,5,5,5,5,5,5,5,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,5,5,5,5,5,5,5,5,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,5,5,5,5,5,5,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,5,5,5,5,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,5,5,5,5,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,5,5,5,5,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,5,5,5,5,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,5,5,5,5,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,5,5,5,5,5,5,0,0,0,0,0,0,0,0,0}
};


// functions -------------------------------------------------------------------

void video_init(struct spectre_vars*);
void spectre_end(void);
U32 pallette_val(U8, U8, U8);


void play_key_update(struct spectre_vars*, U64 tick);
void play_logic_update(struct spectre_vars*, U64 tick);
void play_video_update(struct spectre_vars*, U64 tick);
void play_audio_update(struct spectre_vars*, U64 tick);

void help_key_update(struct spectre_vars*, U64 tick);
void help_logic_update(struct spectre_vars*, U64 tick);
void help_video_update(struct spectre_vars*, U64 tick);
void help_audio_update(struct spectre_vars*, U64 tick);

void anim_key_update(struct spectre_vars*, U64 tick);
void anim_logic_update(struct spectre_vars*, U64 tick);
void anim_video_update(struct spectre_vars*, U64 tick);
void anim_audio_update(struct spectre_vars*, U64 tick);

void retry_key_update(struct spectre_vars*, U64 tick);
void retry_logic_update(struct spectre_vars*, U64 tick);
void retry_video_update(struct spectre_vars*, U64 tick);
void retry_audio_update(struct spectre_vars*, U64 tick);

void reset_game();

//Logic Functions
U8 collide(const struct player *player, const struct sprite *sprite);

//Video Functions
void video_render_wall(struct spectre_vars*);
void video_render_line(struct spectre_vars*, int, int, int, int, int, int);

U8 video_render_sprite(struct spectre_vars*, int);
void video_sprite_line(struct spectre_vars*, int, int, int, int, int, char (*)[SPRITE_SIZE][SPRITE_SIZE], int, U8);

void sort_sprites(struct spectre_vars*);
double distances(const struct sprite *, const struct player *);

void teleport(struct spectre_vars *);

void move_sprite(struct spectre_vars *, int, U64);

void animation1(struct spectre_vars *, U64);
void animation2(struct spectre_vars *, U64);

#endif
