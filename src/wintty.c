#include "wintty.h"

DWORD win32_tty_get_console(FILE* f) {
	if (f == stdin) {
		return STD_INPUT_HANDLE;
	}
	if (f == stdout) {
		return STD_OUTPUT_HANDLE;
	}
	if (f == stderr) {
		return STD_ERROR_HANDLE;
	}
	return -1;
}

HANDLE win32_tty_get_tty(FILE* f) {
	DWORD h = win32_tty_get_console(f);
	return h == -1 ? NULL : GetStdHandle(h);
}

DWORD win32_tty_is_tty(FILE* f) {
	return win32_tty_get_tty(f) == NULL ? -1 : 0;
}

DWORD win32_tty_get_mode(FILE* f, DWORD* mode) {
	HANDLE h = win32_tty_get_tty(f);
	return h == NULL ? -1 : GetConsoleMode(h, mode);
}

DWORD win32_tty_get_screen_info(FILE* f, CONSOLE_SCREEN_BUFFER_INFO* info) {
	HANDLE h = win32_tty_get_tty(f);
	return h == NULL ? -1 : GetConsoleScreenBufferInfo(h, info);
}
