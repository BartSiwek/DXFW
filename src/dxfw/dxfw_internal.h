#ifndef DXFW_DXFW_INTERNAL_H_
#define DXFW_DXFW_INTERNAL_H_

#include <stdint.h>
#include <stdbool.h>

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

/* STRUCTS */
struct dxfwWindow {
  // Settings
  bool m_should_close_;

  // Events
  dxfw_on_should_close_changed m_on_should_close_changed_;
  dxfw_on_mouse_button m_on_mouse_button_;
  dxfw_on_mouse_move m_on_mouse_move_;
  dxfw_on_mouse_wheel m_on_mouse_wheel_;
  dxfw_on_keyboard m_on_keyboard_;

  // OS Specific stuff
  HWND m_handle_;
  DWORD m_style_;

  // List pointer
  struct dxfwWindow* m_next_;
};

/* MEMORY MANAGEMENT */
void* dxfwAlloc(size_t size);
void dxfwDealloc(void* ptr);

/* HELPER FUNCTIONS */
WCHAR* dxfwUtf8ToWchar(const char* input);

/* INIT & TERMINATE */
void dxfwTerminateWindowHandling();

/* WINDOW MANAGEMENT INTERNALS */
struct dxfwWindow* dxfwFindWindow(HWND hwnd);

/* WINDOW EVENT MANAGEMENT INTERNALS */
void dxfwFireWindowClosedEvent(HWND hwnd);
void dxfwFireMouseEvent(HWND hwnd, dxfwMouseButton button, dxfwMouseButtonAction action, LPARAM lparam);
void dxfwFireMouseMoveEvent(HWND hwnd, LPARAM lparam);
void dxfwFireMouseWheelEvent(HWND hwnd, WPARAM wparam, LPARAM lparam);
LPARAM dxfwFireKeyboardEvent(HWND hwnd, LPARAM lparam);

dxfwVirtualKeyCode dxfwGetKeyCode(USHORT windows_key_code);
void dxfwRegisterKeyDown(dxfwVirtualKeyCode key_code);
void dxfwRegisterKeyUp(dxfwVirtualKeyCode key_code);

#endif  // DXFW_DXFW_INTERNAL_H_