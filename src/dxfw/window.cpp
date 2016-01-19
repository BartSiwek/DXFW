#include "dxfw/window.h"

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <windows.h>
#include <cstdint>


namespace dxfw {

Window::Window()
    : m_hwnd_(nullptr),
      m_style_(WS_OVERLAPPEDWINDOW | WS_VISIBLE),
      m_caption_(""),
      m_width_(640),
      m_height_(480),
      m_left_(0),
      m_top_(0) {
  // TODO
}

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

/*
LRESULT CALLBACK InternalWindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
  if (msg != WM_CREATE) {
    // Messages other than WM_CREATE
    auto ptr = GetWindowLongPtr(hwnd, 0);
    WindowsProcInterface* window_proc = reinterpret_cast<WindowsProcInterface*>(ptr);

    if (window_proc != NULL) {
      return window_proc->WindowProc(hwnd, msg, wparam, lparam);
    }
    return DefWindowProc(hwnd, msg, wparam, lparam);
  }
  else {
    // WM_CREATE message
    auto create_struct = reinterpret_cast<LPCREATESTRUCT>(lparam);
    WindowsProcInterface* window_proc = reinterpret_cast<WindowsProcInterface*>(create_struct->lpCreateParams);

    if (window_proc != NULL) {
      SetWindowLongPtr(hwnd, 0, reinterpret_cast<LONG_PTR>(window_proc));
      return window_proc->WindowProc(hwnd, msg, wparam, lparam);
    }
    else {
      return DefWindowProc(hwnd, msg, wparam, lparam);
    }
  }
}
*/

}  // namespace dxfw
