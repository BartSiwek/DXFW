#include <dxfw/dxfw.h>

void errorCallback(dxfwError error) {
  switch(error) {
   case DXFW_ERROR_NONE: break;
   case DXFW_ERROR_UTF8_CONVERSION: break;
   case DXFW_ERROR_MAX: break;
   default: break;
  }
}

int main(int argc, char* *argv) {
  if (!dxfwInitialize()) {
    return -1;
  }

  auto window = dxfwCreateWindow(640, 480, "Hello World");

  if (!window) {
    dxfwTerminate();
    return -1;
  }

  while (!dxfwShouldWindowClose(window)) {
    // Render here & swap buffers
    dxfwPollOsEvents();
  }

  dxfwTerminate();
  return 0;
}
