#include "dxfw/dxfw.h"
#include "dxfw_internal.h"

/* FORWARDS */
LRESULT CALLBACK dxfwInternalWindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

/* GLOBALS */
struct dxfwWindow* g_head_ = NULL;

/* INIT & TERMINATE */
void dxfwTerminateWindowHandling() {
  while (g_head_ != NULL) {
    struct dxfwWindow* current = g_head_;
    g_head_ = g_head_->m_next_;
    dxfwDealloc(current);
  }
}

/* WINDOW MANAGEMENT */
struct dxfwWindow* dxfwCreateWindow(uint32_t width, uint32_t height, const char* caption) {
  // Setup window class
  WNDCLASSEXA wc;
  memset(&wc, 0, sizeof(wc));

  wc.cbSize = sizeof(WNDCLASSEX);
  wc.style = CS_HREDRAW | CS_VREDRAW;
  wc.lpfnWndProc = dxfwInternalWindowProc;
  wc.cbClsExtra = 0;
  wc.cbWndExtra = 0;
  wc.hInstance = 0;
  wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
  wc.hCursor = LoadCursor(NULL, IDC_ARROW);
  wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 2);
  wc.lpszMenuName = NULL;
  wc.lpszClassName = "DxfwWindow";
  wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

  // Set style
  DWORD style = WS_OVERLAPPEDWINDOW | WS_VISIBLE;

  // Register the class
  RegisterClassExA(&wc);

  // Calculate the size of the window
  RECT rc;
  rc.top = rc.left = 0;
  rc.right = width;
  rc.bottom = height;

  // Adjust the window size for style
  AdjustWindowRect(&rc, style, FALSE);

  // Read back the adjusted values
  int32_t lwidth = rc.right - rc.left;
  int32_t lheight = rc.bottom - rc.top;

  // Create the window
  HWND handle = CreateWindowExA(0, wc.lpszClassName, caption, style, CW_USEDEFAULT, CW_USEDEFAULT,
                                lwidth /* width */, lheight /* height */, NULL, NULL, NULL, NULL);

  // Save window data and add to list
  struct dxfwWindow* window = (struct dxfwWindow*)dxfwAlloc(sizeof(struct dxfwWindow));
  memset(window, 0, sizeof(struct dxfwWindow));

  window->m_should_close_ = false;

  window->m_on_should_close_changed_ = NULL;
  window->m_on_mouse_button_ = NULL;
  window->m_on_mouse_move_ = NULL;
  window->m_on_mouse_wheel_ = NULL;
  window->m_on_keyboard_ = NULL;

  window->m_handle_ = handle;
  window->m_style_ = style;

  window->m_next_ = g_head_;
  g_head_ = window;

  // Show window
  ShowWindow(handle, SW_SHOWNORMAL);
  UpdateWindow(handle);

  // Done
  return window;
}

void dxfwDestroyWindow(struct dxfwWindow* window) {
  struct dxfwWindow** current = &g_head_;
  while(*current != NULL && *current != window) {
    current = &(*current)->m_next_;
  }
  if(*current != NULL) {
    *current = window->m_next_;
  }
  dxfwDealloc(window);
}

/* WINDOW STATE */
bool dxfwShouldClose(struct dxfwWindow* window) {
  return window->m_should_close_;
}

/* EVENT MANEGEMENT */
void dxfwPollOsEvents() {
  MSG msg;

  while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
    if (msg.message == WM_QUIT) {
      break;
    }
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }
}

/* WINDOW MANAGEMENT INTERNALS */
struct dxfwWindow* dxfwFindWindow(HWND hwnd) {
  struct dxfwWindow* ptr = g_head_;
  while (ptr != NULL && ptr->m_handle_ != hwnd) {
    ptr = ptr->m_next_;
  }
  return ptr;
}

/* WINDOW OS INTERNALS */
LRESULT CALLBACK dxfwInternalWindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
  switch (msg) {
    case WM_CREATE: {
      // Register RAW Input
      RAWINPUTDEVICE rid = { 0x01, 6, RIDEV_NOLEGACY | RIDEV_INPUTSINK, hwnd };
      RegisterRawInputDevices(&rid, 1, sizeof(rid));

      // Return 0 to allow the window to proceed in the creation process.
      return 0;
    }
    case WM_PAINT: {
      // This message is handled by the default handler to avoid a repeated sending of the message. This results in
      // the ability to process all pending messages at once without getting stuck in an eternal loop.
      break;
    }
    case WM_CLOSE: {
      // This message is sent when a window or an application should terminate.
      dxfwFireWindowClosedEvent(hwnd);
      break;
    }
    case WM_SIZE: {
      // dxfwFireWindowResizedEvent(hwnd, wparam, lparam);
      break;
    }
    case WM_LBUTTONUP: {
      dxfwFireMouseEvent(hwnd, DXFW_LEFT_MOUSE_BUTTON, DXFW_MOUSE_BUTTON_UP, lparam);
      break;
    }
    case WM_LBUTTONDOWN: {
      dxfwFireMouseEvent(hwnd, DXFW_LEFT_MOUSE_BUTTON, DXFW_MOUSE_BUTTON_DOWN, lparam);
      break;
    }
    case WM_MBUTTONUP: {
      dxfwFireMouseEvent(hwnd, DXFW_MIDDLE_MOUSE_BUTTON, DXFW_MOUSE_BUTTON_UP, lparam);
      break;
    }
    case WM_MBUTTONDOWN: {
      dxfwFireMouseEvent(hwnd, DXFW_MIDDLE_MOUSE_BUTTON, DXFW_MOUSE_BUTTON_DOWN, lparam);
      break;
    }
    case WM_RBUTTONUP: {
      dxfwFireMouseEvent(hwnd, DXFW_RIGHT_MOUSE_BUTTON, DXFW_MOUSE_BUTTON_UP, lparam);
      break;
    }
    case WM_RBUTTONDOWN: {
      dxfwFireMouseEvent(hwnd, DXFW_RIGHT_MOUSE_BUTTON, DXFW_MOUSE_BUTTON_DOWN, lparam);
      break;
    }
    case WM_MOUSEMOVE: {
      dxfwFireMouseMoveEvent(hwnd, lparam);
      break;
    }
    case WM_MOUSEWHEEL: {
      dxfwFireMouseWheelEvent(hwnd, wparam, lparam);
      break;
    }
    case WM_INPUT: {
      return dxfwFireKeyboardEvent(hwnd, lparam);
    }
  }

  return DefWindowProc(hwnd, msg, wparam, lparam);
}
