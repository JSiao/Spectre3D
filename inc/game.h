#pragma once
#ifndef _GAME_H
#define _GAME_H

#include <windows.h>
#include <stdbool.h>
#include <stdint.h>
#include <time.h>

#include "tools.h"

#define NUM_KEYS 0xFF

// structures --------------------------------------------
struct text_node
{
  char *string;
  int x, y;
  U32 color;
  struct text_node *next;
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

#endif
