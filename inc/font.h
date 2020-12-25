#include "game.h"

#define NUM_OF_CHARS 74
#define CHAR_HEIGHT  12
#define CHAR_WIDTH   8

extern const char _font_letters[NUM_OF_CHARS][CHAR_HEIGHT][CHAR_WIDTH];

char _get_letter(char);
void font_putchar(int, int, U8, U32);
void font_puts(const char*, int, int, U32);
