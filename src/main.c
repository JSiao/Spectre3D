#include <windows.h>
#include <stdbool.h>
#include <stdint.h>
#include <time.h>
#include "game.h"

struct game_struct globals;

LRESULT CALLBACK window_process(HWND, UINT, WPARAM, LPARAM);
void draw_picture(HWND);
static void display_buffer(HDC, int, int, struct game_struct*, int, int, int, int);
void gdi_pixels(HDC);

int WinMain( HINSTANCE instance, HINSTANCE prev_instance, LPSTR cmd_line, int show_cmd)
{
  const char class_name[] = "Spectre 3D Tech Demo";
  WNDCLASS wc = {
    .style         = CS_HREDRAW | CS_VREDRAW,
    .hbrBackground = GetSysColorBrush(COLOR_3DFACE),
    .lpfnWndProc   = window_process,
    .hInstance     = instance,
    .lpszClassName = class_name
  };
  globals.is_running = 1;
  RegisterClass(&wc);
  game_init(&globals);
  HWND window = CreateWindowEx(
    0,
    class_name,
    "Spectre 3D Tech Demo",
    WS_OVERLAPPEDWINDOW | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
    CW_USEDEFAULT, CW_USEDEFAULT,
    globals.width, globals.height, 
    NULL,
    NULL,
    instance,
    NULL
  );
  if (!window)
  {
    return 0;
  }
  ShowWindow(window, show_cmd);
  MSG message = { };
  HDC dc = GetDC(window);
  gdi_pixels(dc);
  globals.window = window;
  while (globals.is_running)
  {
    RECT r;
    GetClientRect(window, &r);
    while(PeekMessage(&message, 0, 0, 0, PM_REMOVE))
    {
      TranslateMessage(&message);
      DispatchMessage(&message);
    }
    unsigned long long tick = game_tick(&globals);
    game_process(&globals, tick);
    display_buffer(dc, r.right, r.bottom, &globals, 0, 0, 0, 0);
  }
  game_end(&globals);
  return 0;
}

LRESULT CALLBACK window_process(HWND window, UINT u_message, WPARAM w_param, LPARAM l_param)
{
  switch (u_message)
  {
  case WM_PAINT:
    draw_picture(window);
    break;
  case WM_SIZE:
    PostMessage(window, WM_PAINT, 0, 0);
    break;
  case WM_DESTROY:
    PostQuitMessage(0);
    globals.is_running = 0;
    return 0;
  case WM_CHAR:
    switch(w_param)
    {
      case 27:
        PostQuitMessage(0);
        globals.is_running = 0;
        return 0;
    }
  case WM_KEYDOWN:
    for (int key_val = 0; key_val < NUM_KEYS; key_val++)
    {
      if ((GetAsyncKeyState(key_val) & (1<<15)))
      {
        globals.game_keys[key_val] = 1;
      }
    }
    break;
  case WM_KEYUP:
    for (int key_val = 0; key_val < NUM_KEYS; key_val++)
    {
      if (!((GetAsyncKeyState(key_val) & (1<<15))))
      {
        globals.game_keys[key_val] = 0;
      }
    }
    break;

  case WM_QUIT:
    globals.is_running = 0;
    break;
  }
  return DefWindowProc(window, u_message, w_param, l_param);
}

void draw_picture(HWND window)
{
  PAINTSTRUCT ps;
  RECT r;
  GetClientRect(window, &r);
  if (r.bottom == 0)
  {
    return;
  }
  HDC hdc = BeginPaint(window, &ps);
  display_buffer(hdc, r.right, r.bottom, &globals, 0, 0, 0, 0);
  unsigned long max = globals.width * globals.height;
  EndPaint(window, &ps);
}

void display_buffer(HDC dc, int w_width, int w_height, struct game_struct* ptr, int x, int y, int width, int height)
{
  StretchDIBits(dc,
    x, y, w_width, w_height,
    x, y, ptr->width, ptr->height,
    ptr->buffer, &ptr->info,
    DIB_RGB_COLORS, SRCCOPY
  );
  while (globals.texts)
  {
    SetBkMode(dc, TRANSPARENT);
    SetTextColor(dc, globals.texts->color);
    TextOut(dc, globals.texts->x, globals.texts->y, globals.texts->string, strlen(globals.texts->string));
    struct text_node *temp = globals.texts;
    globals.texts = globals.texts->next;
    _mem_free(temp->string);
    _mem_free(temp);
  }
  globals.last = NULL;
}
