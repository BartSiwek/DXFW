#include "dxfw/dxfw.h"

#include <cstdlib>

#include <windows.h>

namespace dxfw {

bool g_initialized_ = false;
AllocFunction g_alloc_ = malloc;
DeallocFunction g_dealloc_ = free;

bool Initialize(AllocFunction alloc, DeallocFunction dealloc) {
  if (g_initialized_) {
    return false;
  }

  g_alloc_ = alloc;
  g_dealloc_ = dealloc;

  return true;
}

void Terminate() {
}

void PollOsEvents() {
  MSG msg;

  while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
    if (msg.message == WM_QUIT) {
      break;
    }
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }
}

}  // namespace dxfw
