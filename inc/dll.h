#pragma once

#ifndef _DLL_H
#define _DLL_H
 
#include <xinput.h>


#define X_INPUT_GET_STATE(name) DWORD WINAPI name(DWORD dwUserIndex, XINPUT_STATE *pState)
#define X_INPUT_SET_STATE(name) DWORD WINAPI name(DWORD dwUserIndex, XINPUT_VIBRATION *pState)

typedef X_INPUT_GET_STATE(x_input_get_state);
typedef X_INPUT_SET_STATE(x_input_set_state);

extern x_input_get_state *XInputGetState_;
extern x_input_set_state *XInputSetState_;

#define XInputGetState XInputGetState_
#define XInputSetState XInputSetState_

static void win32_load_x_input(void);

#endif
