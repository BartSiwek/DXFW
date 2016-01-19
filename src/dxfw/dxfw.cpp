#include "dxfw/dxfw.h"

#include <windows.h>

namespace dxfw {

void PollEvents() {
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
