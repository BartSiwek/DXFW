#include <dxfw/dxfw.h>

int main(int argc, char* *argv) {
  if (!dxfw::Initialize())
    return -1;

  dxfw::WindowDescriptor desc;
  desc.Caption = "Hello World";
  desc.Width = 640;
  desc.Height = 480;

  dxfw::Window* window = dxfw::Window::Create(desc);

  if (!window)
  {
    dxfw::Terminate();
    return -1;
  }

  while (!window->ShouldClose())
  {
    // Render here

    // Swap buffers

    dxfw::PollOsEvents();
  }

  dxfw::Terminate();
  return 0;
}

