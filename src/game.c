#include "game.h"
#include <string.h>

void gdi_pixels(HDC hdc)
{
  PIXELFORMATDESCRIPTOR pfd;
  memset(&pfd, 0, sizeof(pfd));
  pfd.nSize      = sizeof(PIXELFORMATDESCRIPTOR);
  pfd.nVersion   = 1;
  pfd.dwFlags    = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL;
  pfd.iPixelType = PFD_TYPE_RGBA;
  pfd.cColorBits = 32;
  int pf = ChoosePixelFormat(hdc, &pfd);
  SetPixelFormat(hdc, pf, &pfd);
}
void game_init(struct game_struct *ptr)
{
  _game_init(ptr);
  ptr->buffer     = _mem_alloc( 4 * ptr->width * ptr->height );
  ptr->is_running = true;
  ptr->last_tick  = GetTickCount();

  ptr->info.bmiHeader.biSize        = sizeof(BITMAPINFOHEADER);
  ptr->info.bmiHeader.biWidth       = ptr->width;
  ptr->info.bmiHeader.biHeight      = -ptr->height;
  ptr->info.bmiHeader.biPlanes      = 1;
  ptr->info.bmiHeader.biBitCount    = 32;
  ptr->info.bmiHeader.biCompression = BI_RGB;
  ptr->ctrl_active                  = 0;
  ptr->texts = NULL;
  ptr->last  = NULL;
  for (int x = 0; x < NUM_KEYS; x++)
  {
    ptr->game_keys[0] = 0;
  }
}
void game_end(struct game_struct *ptr)
{
  _game_end(ptr);
  _mem_free(ptr->buffer);
}
unsigned long long game_tick(struct game_struct *ptr)
{
  unsigned long long new_tick = GetTickCount();
  unsigned long long old_tick = ptr->last_tick;
  ptr->last_tick              = new_tick;
  return new_tick - old_tick;
}
void game_process(struct game_struct* ptr, unsigned long long dt)
{
  _game_update(dt);
}

void game_pixel_set(U32 x, U32 y, U32 color)
{
  U32 *buffer = (U32 *)globals.buffer;
  buffer[y * globals.width + x] = color;
}
void game_pixel_mix(U32 x, U32 y, U32 color, U8 opacity)
{
  double per   = opacity/255.0f;
  U32 *buffer  = (U32 *)globals.buffer;
  U32 original = buffer[y * globals.width + x];
  U8  red1     = (original >> 16) & 255;
  U8  green1   = (original >> 8) & 255;
  U8  blue1    = original & 255;
  U8  red2     = (color >> 16) & 255;
  U8  green2   = (color >> 8) & 255;
  U8  blue2    = color & 255;
  U8  red3     = (U8)((red1 * (1.0f - per)) + (red2 * (per)));
  U8  green3   = (U8)((green1 * (1.0f - per)) + (green2 * (per)));
  U8  blue3    = (U8)((blue1 * (1.0f - per)) + (blue2 * (per)));
  U32 output   = (red3 << 16) | (green3 << 8) | blue3;
  buffer[y * globals.width + x] = output;
}

void debug_msg(const char* str)
{
  SetWindowTextA(globals.window, str);
}

void text_screen(const char* str, int x, int y, U32 color)
{
  struct text_node *text =_mem_alloc(sizeof(struct text_node));
  text->string = _mem_alloc(strlen(str)+1);
  strcpy(text->string, str);
  text->x      = x;
  text->y      = y;
  text->next   = NULL;
  text->color  = color;
  if (globals.texts == NULL)
  {
    globals.texts = text;
    globals.last  = text;
  }
  else
  {
    globals.last->next = text;
    globals.last = globals.last->next;
  }
}

void game_kill(void)
{
  globals.is_running = false;
}

unsigned short game_height()
{
  return globals.height;
}
unsigned short game_width()
{
  return globals.width;
}

void game_height_set(unsigned short val)
{
  globals.height = val;
}
void game_width_set(unsigned short val)
{
  globals.width = val;
}

char game_key(char code)
{
  return globals.game_keys[code];
}

// Controller functions

char controller_get_button(int index, short flag)
{
  if (globals.ctrl_active & (1 << index))
  {
    return globals.ctrls[index].flags & flag;
  }
  else
  {
    return 0;
  }
}

short controller_get_axis(int index, short axis)
{
  if (globals.ctrl_active & (1 << index))
  {
    switch (axis)
    {
      case GAME_CTRL_LEFT_X:
        return globals.ctrls[index].thumb_left_x;
      case GAME_CTRL_LEFT_Y:
        return globals.ctrls[index].thumb_left_y;
      case GAME_CTRL_RIGHT_X:
        return globals.ctrls[index].thumb_right_x;
      case GAME_CTRL_RIGHT_Y:
        return globals.ctrls[index].thumb_right_y;
      default:
        return 0;
    }
  }
  else
  {
    return 0;
  }
}
char controller_get_trigger(int index, short trigger)
{
  if (globals.ctrl_active & (1 << index))
  {
    switch (trigger)
    {
      case GAME_CTRL_TRIGGER_LEFT:
        return globals.ctrls[index].trigger_left;
      case GAME_CTRL_TRIGGER_RIGHT:
        return globals.ctrls[index].trigger_right;
      default:
        return 0;
    }
  }
  else
  {
    return 0;
  }
}
