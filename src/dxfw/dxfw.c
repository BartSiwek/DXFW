#include "dxfw/dxfw.h"
#include "dxfw_internal.h"

#include <stdlib.h>

/* GLOBAL VARIABLES */
bool g_initialized_ = false;
dxfw_alloc_function g_alloc_ = malloc;
dxfw_dealloc_function g_dealloc_ = free;

/* MEMORY MANAGEMENT */
bool dxfwSetAlloc(dxfw_alloc_function alloc, dxfw_dealloc_function dealloc) {
  if (g_initialized_) {
    return false;
  }

  g_alloc_ = alloc;
  g_dealloc_ = dealloc;

  return true;
}

void* dxfwAlloc(size_t size) {
  return (*g_alloc_)(size);
}

void dxfwDealloc(void* ptr) {
  (*g_dealloc_)(ptr);
}

/* HELPER FUNCTIONS */
WCHAR* dxfwUtf8ToWchar(const char* input) {
  int length = MultiByteToWideChar(CP_UTF8, 0, input, -1, NULL, 0);

  if (length == 0) {
    return NULL;
  }
    
  WCHAR* result = (WCHAR*)dxfwAlloc(length * sizeof(WCHAR));

  int conversion_result = MultiByteToWideChar(CP_UTF8, 0, input, -1, result, length);
  if (conversion_result == 0) {
    dxfwDealloc(result);
    return NULL;
  }

  return result;
}

/* INIT & TERMINATE */
bool dxfwInitialize() {
  if (g_initialized_) {
    return false;
  }

  dxfwTerminateWindowHandling();

  g_initialized_ = true;
  return true;
}

void dxfwTerminate() {
  if (!g_initialized_) {
    return;
  }

  g_initialized_ = false;
}
