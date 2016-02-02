#include "dxfw/dxfw.h"
#include "dxfw-internal-macros.h"
#include "dxfw-internal.h"

#include <stdlib.h>

/***************************************/
/*                GLOBALS              */
/***************************************/
struct dxfwState g_state_ = {
  .initialized = false,
  .alloc = malloc,
  .dealloc = free,
  .callbacks = {
    .error_callback = NULL
  },
  .timer_resolution = 1.0,
  .timer_start = 0
};

/***************************************/
/*               FORWARDS              */
/***************************************/
void dxfwInitializeTimer();

/***************************************/
/*           PUBLIC INTERFACE          */
/***************************************/
void dxfwSetAlloc(dxfw_alloc_function alloc, dxfw_dealloc_function dealloc) {
  // This function should be set before initialization as that may allocate memory
  if (g_state_.initialized) {
    dxfwReportError(DXFW_ERROR_ALREADY_INITIALIZED);
    return;
  }

  g_state_.alloc = alloc;
  g_state_.dealloc = dealloc;
}

bool dxfwInitialize() {
  if (g_state_.initialized) {
    dxfwReportError(DXFW_ERROR_ALREADY_INITIALIZED);
    return false;
  }

  dxfwInitializeTimer();

  g_state_.initialized = true;
  return true;
}

void dxfwTerminate() {
  DXFW_CHECK_IF_INITIALIZED();

  dxfwTerminateWindowHandling();

  g_state_.initialized = false;
}

double dxfwGetTime() {
  DXFW_CHECK_IF_INITIALIZED_AND_RETURN(0.0);

  LARGE_INTEGER timestamp;
  QueryPerformanceCounter(&timestamp);
  return (double)(timestamp.QuadPart - g_state_.timer_start) * g_state_.timer_resolution;
}

/***************************************/
/*             INTERNALS               */
/***************************************/
void* dxfwAlloc(size_t size) {
  return (*g_state_.alloc)(size);
}

void dxfwDealloc(void* ptr) {
  (*g_state_.dealloc)(ptr);
}

void dxfwInitializeTimer() {
  LARGE_INTEGER frequency;
  QueryPerformanceFrequency(&frequency);
  g_state_.timer_resolution = 1.0 / (double)frequency.QuadPart;

  LARGE_INTEGER timestamp;
  QueryPerformanceCounter(&timestamp);
  g_state_.timer_start = timestamp.QuadPart;
}

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
