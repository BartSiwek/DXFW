#include <string>

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

#include <dxfw/dxfw.h>

void ErrorCallback(dxfwError error) {
  DXFW_ERROR_TRACE(__FILE__, __LINE__, true, error);
}

void ResizeCallback(dxfwWindow* /* window */, uint32_t width, uint32_t height) {
  DXFW_TRACE(__FILE__, __LINE__, false, "New size %d x %d.", width, height);
}

int main(int /* argc */, char** /* argv */) {
  if (!dxfwInitialize()) {
    return -1;
  }
  dxfwSetErrorCallback(ErrorCallback);

  auto window = dxfwCreateWindow(640, 480, "Hello Windows");

  if (!window) {
    dxfwTerminate();
    return -1;
  }

  dxfwSetSizeChangedCallback(window, ResizeCallback);

  while (!dxfwShouldWindowClose(window)) {
    // Render here & swap buffers
    dxfwPollOsEvents();
  }

  dxfwDestroyWindow(window);
  dxfwTerminate();
  return 0;
}
