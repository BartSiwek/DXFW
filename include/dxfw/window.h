#ifndef DXFW_WINDOW_H_
#define DXFW_WINDOW_H_

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <windows.h>
#include <cstdint>

namespace dxfw {

class Window {
 public:
  Window();
  ~Window();

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

 private:
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
