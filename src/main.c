#include <windows.h>
#include <stdbool.h>
#include <stdint.h>
#include <time.h>
#include "game.h"
#include <stdio.h>
#include <xinput.h>

struct game_struct globals;

X_INPUT_GET_STATE(XInputGetStateStub)
{
  return 0;
}
X_INPUT_SET_STATE(XInputSetStateStub)
{
  return 0;
}

x_input_get_state *XInputGetState_ = XInputGetStateStub;
x_input_set_state *XInputSetState_ = XInputSetStateStub;
static void win32_load_x_input(void)
{
  HMODULE x_input_module = LoadLibrary("xinput1_3.dll");
  if (x_input_module)
  {
    XInputGetState = (x_input_get_state *)GetProcAddress(x_input_module, "XInputGetState");
    XInputSetState = (x_input_set_state *)GetProcAddress(x_input_module, "XInputSetState");
  }
}

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
  win32_load_x_input();
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
    // TODO(jsiao): Should we poll more freq?
    for(int ctrl_index = 0; ctrl_index < XUSER_MAX_COUNT; ctrl_index++)
    {
      XINPUT_STATE ctrl_state;
      if(XInputGetState(ctrl_index,&ctrl_state) == ERROR_SUCCESS)
      {
        globals.ctrl_active |= 1 << ctrl_index;
        XINPUT_GAMEPAD *game_pad = &ctrl_state.Gamepad;
 
        globals.ctrls[ctrl_index].flags         = game_pad->wButtons;
        globals.ctrls[ctrl_index].trigger_left  = game_pad->bLeftTrigger;
        globals.ctrls[ctrl_index].trigger_right = game_pad->bRightTrigger;
        globals.ctrls[ctrl_index].thumb_left_x  = game_pad->sThumbLX;
        globals.ctrls[ctrl_index].thumb_left_y  = game_pad->sThumbLY;
        globals.ctrls[ctrl_index].thumb_right_x = game_pad->sThumbRX;
        globals.ctrls[ctrl_index].thumb_right_y = game_pad->sThumbRY;

      }
      else
      {
        globals.ctrl_active &= ~(1 << ctrl_index);
      }
    }
    unsigned long long tick = game_tick(&globals);
    game_process(&globals, tick);
    display_buffer(dc, r.right, r.bottom, &globals, 0, 0, 0, 0);
    double new_tick = tick/(double) CLOCKS_PER_SEC;
  }
  game_end(&globals);
  return 0;
}

LRESULT CALLBACK window_process(HWND window, UINT u_message, WPARAM w_param, LPARAM l_param)
{
  switch (u_message)
  {
  case WM_PAINT:
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
