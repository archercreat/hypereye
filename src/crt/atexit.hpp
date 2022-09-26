#pragma once

using _PVFV = void(__cdecl*)();
using _PIFV = int (__cdecl*)();

extern "C" int __cdecl atexit(_PVFV const function);
extern "C" int __cdecl doinit(void);
extern "C" int __cdecl doexit(void);
