#include <string>

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

#include <dxfw/dxfw.h>

void ErrorCallback(dxfwError error) {
  std::wstring error_message = std::to_wstring(error);
  MessageBox(NULL, error_message.c_str(), L"DXFW Error", MB_OK);
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

  while (!dxfwShouldWindowClose(window)) {
    // Render here & swap buffers
    dxfwPollOsEvents();
  }

  dxfwDestroyWindow(window);
  dxfwTerminate();
  return 0;
}
