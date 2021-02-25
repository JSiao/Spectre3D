#pragma once
#ifndef _GAME_H
#define _GAME_H

#include <windows.h>
#include <stdbool.h>
#include <stdint.h>
#include <time.h>

#include "tools.h"

#ifdef __MINGW32__
  #include "dll.h"
#endif // __MINGW32__

#define NUM_KEYS 0xFF
// enums -------------------------------------------------

enum ctrl_input
{
  GAME_CTRL_UP     = 0x0001,
  GAME_CTRL_DOWN   = 0x0002,
  GAME_CTRL_LEFT   = 0x0004,
  GAME_CTRL_RIGHT  = 0x0008,
  GAME_CTRL_START  = 0x0010,
  GAME_CTRL_BACK   = 0x0020,
  GAME_CTRL_LTHUMB = 0x0040,
  GAME_CTRL_RTHUMB = 0x0080,
  GAME_CTRL_LSHLDR = 0x0100,
  GAME_CTRL_RSHLDR = 0x0200,
  GAME_CTRL_A      = 0x1000,
  GAME_CTRL_B      = 0x2000,
  GAME_CTRL_X      = 0x4000,
  GAME_CTRL_Y      = 0x8000
};

enum ctrl_axis
{
  GAME_CTRL_LEFT_X,
  GAME_CTRL_LEFT_Y,
  GAME_CTRL_RIGHT_X,
  GAME_CTRL_RIGHT_Y
};

enum ctrl_triggers
{
  GAME_CTRL_TRIGGER_LEFT,
  GAME_CTRL_TRIGGER_RIGHT
};

// structures --------------------------------------------
struct text_node
{
  char *string;
  int x, y;
  U32 color;
  struct text_node *next;
};

struct game_ctrl
{
  U16   flags;
  U8    trigger_left;
  U8    trigger_right;
  short thumb_left_x;
  short thumb_left_y;
  short thumb_right_x;
  short thumb_right_y;
};


struct game_struct
{
  unsigned short height;
  unsigned short width;
  void *buffer;
  bool is_running;
  unsigned long long last_tick;
  BITMAPINFO info;
  void *assets;
  char game_keys[NUM_KEYS];
  char ctrl_active;
  struct game_ctrl ctrls[4];
  HWND window;
  struct text_node *texts;
  struct text_node *last;
};


// Data --------------------------------------------------

extern struct game_struct globals;

// Functions ---------------------------------------------

//Game Engine functions
void game_init(struct game_struct *);
void game_end(struct game_struct *);
unsigned long long game_tick(struct game_struct *);
void game_process(struct game_struct*, unsigned long long);

unsigned short game_height();
unsigned short game_width();

void game_height_set(unsigned short);
void game_width_set(unsigned short);

//Game Specific 
void _game_init(struct game_struct*);
void _game_update(unsigned long long);
void _game_end(struct game_struct*);

void game_pixel_set(U32, U32, U32);
void game_pixel_mix(U32, U32, U32, U8);

void debug_msg(const char*);
void text_screen(const char*, int, int, U32);

void game_kill(void);

char game_key(char);

// Contoller Input

char  controller_get_button(int, short);
short controller_get_axis(int, short);
char  controller_get_trigger(int, short);

#endif
