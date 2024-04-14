#include <Windows.h>
#include <stdio.h>

DWORD win32_tty_get_console(FILE* f);
HANDLE win32_tty_get_tty(FILE* f);
DWORD win32_tty_is_tty(FILE* f);
DWORD win32_tty_get_mode(FILE* f, DWORD* mode);
DWORD win32_tty_get_screen_info(FILE* f, CONSOLE_SCREEN_BUFFER_INFO* info);
