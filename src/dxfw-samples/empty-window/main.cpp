#include <dxfw/dxfw.h>

int main(int argc, char* *argv) {
  if (!dxfwInitialize())
    return -1;

  dxfwWindow* window = NULL;

  /*
  dxfw::WindowDescriptor desc;
  desc.Caption = "Hello World";
  desc.Width = 640;
  desc.Height = 480;

  dxfw::Window* window = dxfw::Window::Create(desc);
  */

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

