using _PVFV = void(__cdecl*)();
using _PIFV = int (__cdecl*)();

extern "C" int __cdecl atexit(_PVFV const function);

namespace globals
{
void initialize();
void teardown();
};

