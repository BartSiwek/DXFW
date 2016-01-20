#ifndef DXFW_WINDOW_H_
#define DXFW_WINDOW_H_

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <windows.h>
#include <cstdint>

namespace dxfw {

struct WindowDescriptor {
  WindowDescriptor() {}

  const char* Caption = "";
  uint32_t Width = 640;
  uint32_t Height = 480;
  int32_t Left = 100;
  int32_t Top = 100;
};

class Window {
 public:
  static Window* Create(const WindowDescriptor& descriptor);

  HWND GetHandle() const;

  void SetSize(uint32_t width, uint32_t height);
  void SetPosition(int32_t left, int32_t top);

  uint32_t GetWidth() const;
  uint32_t GetHeight() const;
  int32_t GetLeft() const;
  int32_t GetTop() const;

  void SetWidth(uint32_t width);
  void SetHeight(uint32_t height);

  void SetCaption(const char* caption);
  const char* GetCaption() const;

  bool ShouldClose() const;

 private:
  Window(const WindowDescriptor& descriptor);
  ~Window();

  Window(const Window& other) = delete;
  Window(Window&& other) = delete;
  Window& operator=(const Window& other) = delete;
  Window& operator=(Window&& other) = delete;

  HWND m_hwnd_;
  DWORD m_style_;
  const char* m_caption_;
  uint32_t m_width_;
  uint32_t m_height_;
  int32_t m_left_;
  int32_t m_top_;
};

}  // namespace dxfw

#endif  // DXFW_WINDOW_H_
