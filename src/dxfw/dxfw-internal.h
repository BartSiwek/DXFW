#ifndef DXFW_DXFW_INTERNAL_H_
#define DXFW_DXFW_INTERNAL_H_

#include <stdint.h>
#include <stdbool.h>

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

#include "dxfw/dxfw.h"

#include "dxfw-internal-structs.h"

/* MEMORY MANAGEMENT */
void* dxfwAlloc(size_t size);
void dxfwDealloc(void* ptr);

/* HELPER FUNCTIONS */
wchar_t* dxfwUtf8ToWchar(const char* input, dxfw_alloc_function alloc, dxfw_dealloc_function dealloc);
char* dxfwWcharToUtf8(wchar_t* input, dxfw_alloc_function alloc, dxfw_dealloc_function dealloc);

/* INIT & TERMINATE */
void dxfwTerminateWindowHandling();

/* WINDOW MANAGEMENT */
struct dxfwWindow* dxfwFindWindow(HWND hwnd);

/* EVENT MANAGEMENT */
void dxfwReportError(dxfwError error);
void dxfwFireWindowClosedEvent(HWND hwnd);
void dxfwFireSizeChangedEvent(HWND hwnd, LPARAM lparam);
void dxfwFireMouseEvent(HWND hwnd, dxfwMouseButton button, dxfwMouseButtonAction action, LPARAM lparam);
void dxfwFireMouseMoveEvent(HWND hwnd, LPARAM lparam);
void dxfwFireMouseWheelEvent(HWND hwnd, WPARAM wparam, LPARAM lparam);
LPARAM dxfwFireKeyboardEvent(HWND hwnd, LPARAM lparam);

/* KEYBOARD */
dxfwVirtualKeyCode dxfwGetKeyCode(USHORT windows_key_code);
void dxfwRegisterKeyDown(dxfwVirtualKeyCode key_code);
void dxfwRegisterKeyUp(dxfwVirtualKeyCode key_code);

#endif  // DXFW_DXFW_INTERNAL_H_
