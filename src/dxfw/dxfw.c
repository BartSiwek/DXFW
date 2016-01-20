#include "dxfw/dxfw.h"

#include <stdint.h>
#include <stdlib.h>

#include <windows.h>

bool g_initialized_ = false;
dxfw_alloc_function g_alloc_ = malloc;
dxfw_dealloc_function g_dealloc_ = free;

bool dxfwSetAlloc(dxfw_alloc_function alloc, dxfw_dealloc_function dealloc) {
  if (g_initialized_) {
    return false;
  }

  g_alloc_ = alloc;
  g_dealloc_ = dealloc;

  return true;
}

bool dxfwInitialize() {
  if (g_initialized_) {
    return false;
  }

  g_initialized_ = true;
  return true;
}

void dxfwTerminate() {
  if (!g_initialized_) {
    return;
  }

  g_initialized_ = false;
}

void dxfwPollOsEvents() {
  MSG msg;

  while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
    if (msg.message == WM_QUIT) {
      break;
    }
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }
}

