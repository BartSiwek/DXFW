#include "dxfw/dxfw.h"
#include "dxfw_internal.h"

/* SHOULD CLOSE EVENT */
dxfw_on_should_close_changed dxfwSetShouldCloseChangedCallback(struct dxfwWindow* window, dxfw_on_should_close_changed callback) {
  dxfw_on_should_close_changed prev = window->m_on_should_close_changed_;
  window->m_on_should_close_changed_ = callback;
  return prev;
}

void dxfwFireWindowClosedEvent(HWND hwnd) {
  struct dxfwWindow* window = dxfwFindWindow(hwnd);
  if(window != NULL) {
    window->m_should_close_ = true;
    if(window->m_on_should_close_changed_ != NULL) {
      (*window->m_on_should_close_changed_)(window, window->m_should_close_);
    }
  }
}

/* MOUSE EVENTS */
dxfw_on_mouse_button dxfwSetMouseButtonCallback(struct dxfwWindow* window, dxfw_on_mouse_button callback) {
  dxfw_on_mouse_button prev = window->m_on_mouse_button_;
  window->m_on_mouse_button_ = callback;
  return prev;
}

void dxfwFireMouseEvent(HWND hwnd, dxfwMouseButton button, dxfwMouseButtonAction action, LPARAM lparam) {
  struct dxfwWindow* window = dxfwFindWindow(hwnd);
  if (window != NULL && window->m_on_mouse_button_ != NULL) {
    int32_t x = (0xffff & lparam);
    int32_t y = ((0xffff0000 & lparam) >> 16);

    (*window->m_on_mouse_button_)(window, button, action, x, y);
  }
}

dxfw_on_mouse_move dxfwSetMouseMoveCallback(struct dxfwWindow* window, dxfw_on_mouse_move callback) {
  dxfw_on_mouse_move prev = window->m_on_mouse_move_;
  window->m_on_mouse_move_ = callback;
  return prev;
}

void dxfwFireMouseMoveEvent(HWND hwnd, LPARAM lparam) {
  struct dxfwWindow* window = dxfwFindWindow(hwnd);
  if (window != NULL && window->m_on_mouse_move_ != NULL) {
    int32_t x = (0xffff & lparam);
    int32_t y = ((0xffff0000 & lparam) >> 16);

    (*window->m_on_mouse_move_)(window, x, y);
  }
}

dxfw_on_mouse_wheel dxfwSetMouseWheelCallback(struct dxfwWindow* window, dxfw_on_mouse_wheel callback) {
  dxfw_on_mouse_wheel prev = window->m_on_mouse_wheel_;
  window->m_on_mouse_wheel_ = callback;
  return prev;
}

void dxfwFireMouseWheelEvent(HWND hwnd, WPARAM wparam, LPARAM lparam) {
  struct dxfwWindow* window = dxfwFindWindow(hwnd);
  if (window != NULL && window->m_on_mouse_wheel_) {
    int32_t x = (0xffff & lparam);
    int32_t y = ((0xffff0000 & lparam) >> 16);
    int32_t delta = (int32_t)((0xffff0000 & wparam) >> 16);

    (*window->m_on_mouse_wheel_)(window, x, y, delta);
  }
}

/* KEYBOARD EVENTS */
LPARAM dxfwFireKeyboardEvent(HWND hwnd, LPARAM lparam) {
  RAWINPUT ri;
  UINT size = sizeof(RAWINPUT);
  GetRawInputData((HRAWINPUT)lparam, RID_INPUT, &ri, &size, sizeof(RAWINPUTHEADER));
  if (ri.header.dwType == RIM_TYPEKEYBOARD) {
    /*
    VirtualKeyCode key_code = GetKeyCode(ri.data.keyboard.VKey);

    VirtualKeyState state;
    if ((ri.data.keyboard.Flags & RI_KEY_BREAK) != 0) {
      // Key is UP
      RegisterKeyUp(key_code);
      state = KEY_STATE_UP;
    }
    else {
      // Key is DOWN
      RegisterKeyDown(key_code);
      state = KEY_STATE_DOWN;
    }

    VirtualKeyModifierFlags modifier_flags = GetModifierFlags();
    VirtualKeyState previous_state = GetPreviousState(key_code);
    */
    HWND acctive_hwnd = GetActiveWindow();
    struct dxfwWindow* window = dxfwFindWindow(hwnd);
    if (window != NULL) {
      /*
      Elg::DirectX::Events::Event e;
      e.EventType = Elg::DirectX::Events::KEYBOARD_EVENT;
      e.KeyboardEvent.Window = window;
      e.KeyboardEvent.KeyCode = key_code;
      e.KeyboardEvent.ModifierFlags = modifier_flags;
      e.KeyboardEvent.State = state;
      e.KeyboardEvent.PreviousState = previous_state;

      m_event_manager_.FireEvent(&e);
      */
    }

    return 0;  // Return 0 to mark the event as handled
  }
  return 1;  // Return 1 to mark the event as NOT handled
}
