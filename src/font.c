#include "font.h"
#include <stdio.h>
#include "font_assets.h"

void font_putchar(int x, int y, U8 _letter, U32 color)
{
  char letter = _get_letter(_letter);
  for (int i = 0; i < CHAR_HEIGHT; i++)
  {
    for (int j = 0; j < CHAR_WIDTH; j++)
    {
      if (_font_letters[letter][i][j])
      {
        game_pixel_set(x+j, y+i, color);
      }
    }
  }
}

void font_puts(const char *str, int x, int y, U32 color)
{
  int len = strlen(str);
  for (int i = 0; i < len; i++)
  {
    font_putchar(x + 9*i, y, str[i], color);
  }
}

char _get_letter(char val)
{
  switch(val)
  {
  case ' ':
    return 0;
  case 'A':
    return 1;
  case 'B':
    return 2;
  case 'C':
    return 3;
  case 'D':
    return 4;
  case 'E':
    return 5;
  case 'F':
    return 6;
  case 'G':
    return 7;
  case 'H':
    return 8;
  case 'I':
    return 9;
  case 'J':
    return 10;
  case 'K':
    return 11;
  case 'L':
    return 12;
  case 'M':
    return 13;
  case 'N':
    return 14;
  case 'O':
    return 15;
  case 'P':
    return 16;
  case 'Q':
    return 17;
  case 'R':
    return 18;
  case 'S':
    return 19;
  case 'T':
    return 20;
  case 'U':
    return 21;
  case 'V':
    return 22;
  case 'W':
    return 23;
  case 'X':
    return 24;
  case 'Y':
    return 25;
  case 'Z':
    return 26;
  case '0':
    return 27;
  case '1':
    return 28;
  case '2':
    return 29;
  case '3':
    return 30;
  case '4':
    return 31;
  case '5':
    return 32;
  case '6':
    return 33;
  case '7':
    return 34;
  case '8':
    return 35;
  case '9':
    return 36;
  case ':':
    return 37;
  case '/':
    return 38;
  case '(':
    return 39;
  case ')':
    return 40;
  case '.':
    return 41;
  case '!':
    return 42;
  case '?':
    return 43;
  case '&':
    return 44;
  case '-':
    return 45;
  case 'a':
    return 46;
  case 'b':
    return 47;
  case 'c':
    return 48;
  case 'd':
    return 49;
  case 'e':
    return 50;
  case 'f':
    return 51;
  case 'g':
    return 52;
  case 'h':
    return 53;
  case 'i':
    return 54;
  case 'j':
    return 55;
  case 'k':
    return 56;
  case 'l':
    return 57;
  case 'm':
    return 58;
  case 'n':
    return 59;
  case 'o':
    return 60;
  case 'p':
    return 61;
  case 'q':
    return 62;
  case 'r':
    return 63;
  case 's':
    return 64;
  case 't':
    return 65;
  case 'u':
    return 66;
  case 'v':
    return 67;
  case 'w':
    return 68;
  case 'x':
    return 69;
  case 'y':
    return 70;
  case 'z':
    return 71;
  case '[':
    return 72;
  case ']':
    return 73;
  default:
    return 0;
  }
}
