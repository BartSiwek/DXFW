#ifndef DXFW_DXFW_INTERNAL_STRUCTS_H_
#define DXFW_DXFW_INTERNAL_STRUCTS_H_

#include <stdbool.h>

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

#include "dxfw/dxfw.h"

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

/* GLOBAL VARIABLES */
extern bool g_initialized_;

#endif  // DXFW_DXFW_INTERNAL_STRUCTS_H_
