#ifndef DXFW_DXFW_INTERNAL_STRUCTS_H_
#define DXFW_DXFW_INTERNAL_STRUCTS_H_

#include <stdbool.h>

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

#include "dxfw/dxfw.h"

/* TYPEDEFS */

/* STRUCTS */
struct dxfwWindow {
  // State
  bool m_should_close_;
  void* m_user_data_;

  // Events
  dxfw_on_should_close_changed m_on_should_close_changed_;
  dxfw_on_size_changed m_on_size_changed_;
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

struct dxfwState {
  bool m_initialized_;

  dxfw_alloc_function m_alloc_;
  dxfw_dealloc_function m_dealloc_;

  struct {
    dxfw_on_error m_error_callback_;
  } m_callbacks_;

  double m_timer_resolution_;
  int64_t m_timer_start_;

  struct dxfwWindow* m_windows_head_;
};

/* GLOBAL VARIABLES */
extern struct dxfwState g_state_;

#endif  // DXFW_DXFW_INTERNAL_STRUCTS_H_
