#pragma once
#ifndef _TOOLS_H
#define _TOOLS_H


#ifdef __MINGW32__
  #include <windows.h>
  #define _mem_alloc(X) VirtualAlloc(0, (X), MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE)
  #define _mem_free(X) VirtualFree((X), 0, MEM_RELEASE)
  #define U8  UINT8
  #define U16 UINT16
  #define U32 UINT32
  #define U64 UINT64
#else
  #define _mem_alloc(X) malloc((X))
  #define _mem_free(X) free((X))
  #include <stdlib.h>
  #define U8  uint8_t
  #define U16 uint16_t
  #define U32 uint32_t
  #define U64 uint64_t
#endif // __MINGW32__


#endif // _TOOLS_H
