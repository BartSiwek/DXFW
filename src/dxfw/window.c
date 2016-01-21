#include "dxfw/dxfw.h"
#include "dxfw_internal.h"

#include <stdint.h>

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

/* WINDOW STRUCT */
struct dxfwWindow {
  struct dxfwWindow* m_next_;
  HWND m_handle_;
  DWORD m_style_;
  const char* m_caption_;
  uint32_t m_width_;
  uint32_t m_height_;
  bool m_should_close_;
};

/* FORWARDS */
LRESULT CALLBACK dxfwInternalWindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

/* GLOBALS */
struct dxfwWindow* g_head_ = NULL;

/* INIT & TERMINATE*/
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

  window->m_next_ = g_head_;
  window->m_handle_ = handle;
  window->m_style_ = style;
  window->m_caption_ = caption;
  window->m_width_ = width;
  window->m_height_ = height;
  window->m_should_close_ = false;

  g_head_ = window;

  // Show window
  ShowWindow(handle, SW_SHOWNORMAL);
  UpdateWindow(handle);

  // Done
  return window;
}

bool dxfwShouldClose(struct dxfwWindow* window) {
  return window->m_should_close_;
}

/* WINDOW INTERNALS */
LRESULT CALLBACK dxfwInternalWindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
  return DefWindowProc(hwnd, msg, wparam, lparam);
}

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

/*
HWND Window::GetHandle() const {
  return m_hwnd_;
}

void Window::SetSize(uint32_t width, uint32_t height) {
  SetWidth(width);
  SetHeight(height);
}

void Window::SetPosition(int32_t left, int32_t top) {
  m_left_ = left;
  m_top_ = top;
}

uint32_t Window::GetWidth() const {
  return m_width_;
}

uint32_t Window::GetHeight() const {
  return m_height_;
}

int32_t Window::GetLeft() const {
  return m_left_;
}

int32_t Window::GetTop() const {
  return m_top_;
}

void Window::SetWidth(uint32_t width) {
  m_width_ = width;
}

void Window::SetHeight(uint32_t height) {
  m_height_ = height;
}

void Window::SetCaption(const char* caption) {
  m_caption_ = caption;
}

const char* Window::GetCaption() const {
  return m_caption_;
}
*/
