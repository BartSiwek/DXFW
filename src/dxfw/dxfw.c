#include "dxfw/dxfw.h"
#include "dxfw_internal.h"

#include <stdlib.h>

/* GLOBAL VARIABLES */
bool g_initialized_ = false;
dxfw_alloc_function g_alloc_ = malloc;
dxfw_dealloc_function g_dealloc_ = free;
double g_timer_resolution_ = 1.0;
int64_t g_timer_start_ = 0;
dxfw_on_error g_error_callback_ = NULL;

/* MEMORY MANAGEMENT */
void dxfwSetAlloc(dxfw_alloc_function alloc, dxfw_dealloc_function dealloc) {
  if (g_initialized_) {
    return;
  }

  g_alloc_ = alloc;
  g_dealloc_ = dealloc;
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
void dxfwInitializeTimer() {
  LARGE_INTEGER frequency;
  QueryPerformanceFrequency(&frequency);
  g_timer_resolution_ = 1.0 / (double)frequency.QuadPart;

  LARGE_INTEGER timestamp;
  QueryPerformanceCounter(&timestamp);
  g_timer_start_ = timestamp.QuadPart;
}

bool dxfwInitialize() {
  if (g_initialized_) {
    return false;
  }

  dxfwInitializeTimer();

  g_initialized_ = true;
  return true;
}

void dxfwTerminate() {
  if (!g_initialized_) {
    return;
  }

  dxfwTerminateWindowHandling();

  g_initialized_ = false;
}

/* ERROR HANDLING */
dxfw_on_error dxfwSetErrorCallback(dxfw_on_error callback) {
  dxfw_on_error prev = g_error_callback_;
  g_error_callback_ = callback;
  return prev;
}

void dxfwReportError(dxfwError error) {
  if(g_error_callback_ != NULL) {
    (*g_error_callback_)(error);
  }
}

/* TIME MANAGEMENT */
double dxfwGetTime() {
  LARGE_INTEGER timestamp;
  QueryPerformanceCounter(&timestamp);
  return (double)(timestamp.QuadPart - g_timer_start_) * g_timer_resolution_;
}