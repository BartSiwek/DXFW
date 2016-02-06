#include <iostream>
#include <dxfw/dxfw.h>

void errorCallback(dxfwError error) {
  std::cout << "Error " << error << std::endl;
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
