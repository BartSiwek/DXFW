#include <dxfw/dxfw.h>

int main(int argc, char* *argv) {
  if (!dxfwInitialize())
    return -1;

  dxfwWindow* window = dxfwCreateWindow(640, 480, "Hello World");

  if (!window)
  {
    dxfwTerminate();
    return -1;
  }

  while (!dxfwShouldClose(window))
  {
    // Render here

    // Swap buffers

    dxfwPollOsEvents();
  }

  dxfwTerminate();
  return 0;
}
