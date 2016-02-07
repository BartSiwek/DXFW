#include "dxfw/dxfw.h"
#include "dxfw-internal-macros.h"
#include "dxfw-internal.h"

/***************************************/
/*           PUBLIC INTERFACE          */
/***************************************/
dxfw_on_error dxfwSetErrorCallback(dxfw_on_error callback) {
  // Allow this callback to be set even if not initialized
  dxfw_on_error prev = g_state_.m_callbacks_.m_error_callback_;
  g_state_.m_callbacks_.m_error_callback_ = callback;
  return prev;
}

dxfw_on_should_close_changed dxfwSetShouldCloseChangedCallback(struct dxfwWindow* window, dxfw_on_should_close_changed callback) {
  DXFW_CHECK_IF_INITIALIZED_AND_RETURN(NULL);

  dxfw_on_should_close_changed prev = window->m_on_should_close_changed_;
  window->m_on_should_close_changed_ = callback;
  return prev;
}

dxfw_on_mouse_button dxfwSetMouseButtonCallback(struct dxfwWindow* window, dxfw_on_mouse_button callback) {
  DXFW_CHECK_IF_INITIALIZED_AND_RETURN(NULL);

  dxfw_on_mouse_button prev = window->m_on_mouse_button_;
  window->m_on_mouse_button_ = callback;
  return prev;
}

dxfw_on_mouse_move dxfwSetMouseMoveCallback(struct dxfwWindow* window, dxfw_on_mouse_move callback) {
  DXFW_CHECK_IF_INITIALIZED_AND_RETURN(NULL);

  dxfw_on_mouse_move prev = window->m_on_mouse_move_;
  window->m_on_mouse_move_ = callback;
  return prev;
}

dxfw_on_mouse_wheel dxfwSetMouseWheelCallback(struct dxfwWindow* window, dxfw_on_mouse_wheel callback) {
  DXFW_CHECK_IF_INITIALIZED_AND_RETURN(NULL);

  dxfw_on_mouse_wheel prev = window->m_on_mouse_wheel_;
  window->m_on_mouse_wheel_ = callback;
  return prev;
}

dxfw_on_keyboard dxfwSetKeyboardCallback(struct dxfwWindow* window, dxfw_on_keyboard callback) {
  DXFW_CHECK_IF_INITIALIZED_AND_RETURN(NULL);

  dxfw_on_keyboard prev = window->m_on_keyboard_;
  window->m_on_keyboard_ = callback;
  return prev;
}

/***************************************/
/*             INTERNALS               */
/***************************************/
void dxfwReportError(dxfwError error) {
  if (g_state_.m_callbacks_.m_error_callback_ != NULL) {
    (*g_state_.m_callbacks_.m_error_callback_)(error);
  }
}

void dxfwFireWindowClosedEvent(HWND hwnd) {
  struct dxfwWindow* window = dxfwFindWindow(hwnd);
  if (window != NULL) {
    window->m_should_close_ = true;
    if (window->m_on_should_close_changed_ != NULL) {
      (*window->m_on_should_close_changed_)(window, window->m_should_close_);
    }
  }
}

void dxfwFireMouseEvent(HWND hwnd, dxfwMouseButton button, dxfwMouseButtonAction action, LPARAM lparam) {
  struct dxfwWindow* window = dxfwFindWindow(hwnd);
  if (window != NULL && window->m_on_mouse_button_ != NULL) {
    int16_t x = (0xffff & lparam);
    int16_t y = ((0xffff0000 & lparam) >> 16);

    (*window->m_on_mouse_button_)(window, button, action, x, y);
  }
}

void dxfwFireMouseMoveEvent(HWND hwnd, LPARAM lparam) {
  struct dxfwWindow* window = dxfwFindWindow(hwnd);
  if (window != NULL && window->m_on_mouse_move_ != NULL) {
    int16_t x = (0xffff & lparam);
    int16_t y = ((0xffff0000 & lparam) >> 16);

    (*window->m_on_mouse_move_)(window, x, y);
  }
}

void dxfwFireMouseWheelEvent(HWND hwnd, WPARAM wparam, LPARAM lparam) {
  struct dxfwWindow* window = dxfwFindWindow(hwnd);
  if (window != NULL && window->m_on_mouse_wheel_) {
    int16_t x = (0xffff & lparam);
    int16_t y = ((0xffff0000 & lparam) >> 16);
    int16_t delta = (int16_t)((0xffff0000 & wparam) >> 16);

    (*window->m_on_mouse_wheel_)(window, x, y, delta);
  }
}

LPARAM dxfwFireKeyboardEvent(HWND hWnd, LPARAM lparam) {
  DXFW_UNUSED(hWnd);

  RAWINPUT ri;
  UINT size = sizeof(RAWINPUT);
  GetRawInputData((HRAWINPUT)lparam, RID_INPUT, &ri, &size, sizeof(RAWINPUTHEADER));
  if (ri.header.dwType == RIM_TYPEKEYBOARD) {
    dxfwVirtualKeyCode key_code = dxfwGetKeyCode(ri.data.keyboard.VKey);

    dxfwVirtualKeyState state;
    if ((ri.data.keyboard.Flags & RI_KEY_BREAK) != 0) {
      // Key is UP
      dxfwRegisterKeyUp(key_code);
      state = DXFW_KEY_STATE_UP;
    }
    else {
      // Key is DOWN
      dxfwRegisterKeyDown(key_code);
      state = DXFW_KEY_STATE_DOWN;
    }

    dxfwVirtualKeyModifiers modifier_flags = dxfwGetModifierFlags();
    dxfwVirtualKeyState previous_state = dxfwGetPreviousKeyState(key_code);

    HWND active_hwnd = GetActiveWindow();
    struct dxfwWindow* window = dxfwFindWindow(active_hwnd);
    if (window != NULL && window->m_on_keyboard_) {
      (*window->m_on_keyboard_)(window, key_code, modifier_flags, state, previous_state);
    }

    return 0;  // Return 0 to mark the event as handled
  }
  return 1;  // Return 1 to mark the event as NOT handled
}

