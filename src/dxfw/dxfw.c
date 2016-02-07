#include "dxfw/dxfw.h"
#include "dxfw-internal-macros.h"
#include "dxfw-internal.h"

#include <stdlib.h>

/***************************************/
/*                GLOBALS              */
/***************************************/
struct dxfwState g_state_ = {
  .m_initialized_ = false,
  .m_alloc_ = NULL,
  .m_dealloc_ = NULL,
  .m_callbacks_ = {
    .m_error_callback_ = NULL
  },
  .m_timer_resolution_ = 1.0,
  .m_timer_start_ = 0,
  .m_windows_head_ = NULL
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
  if (g_state_.m_initialized_) {
    dxfwReportError(DXFW_ERROR_ALREADY_INITIALIZED);
    return;
  }

  g_state_.m_alloc_ = alloc;
  g_state_.m_dealloc_ = dealloc;
}

bool dxfwInitialize() {
  if (g_state_.m_initialized_) {
    dxfwReportError(DXFW_ERROR_ALREADY_INITIALIZED);
    return false;
  }
  
  if (g_state_.m_alloc_ == NULL) {
    g_state_.m_alloc_ = malloc;
  }
  if (g_state_.m_dealloc_ == NULL) {
    g_state_.m_dealloc_ = free;
  }

  dxfwInitializeTimer();

  g_state_.m_initialized_ = true;
  return true;
}

void dxfwTerminate() {
  DXFW_CHECK_IF_INITIALIZED();

  dxfwTerminateWindowHandling();

  g_state_.m_initialized_ = false;
}

double dxfwGetTime() {
  DXFW_CHECK_IF_INITIALIZED_AND_RETURN(0.0);

  LARGE_INTEGER timestamp;
  QueryPerformanceCounter(&timestamp);
  return (double)(timestamp.QuadPart - g_state_.m_timer_start_) * g_state_.m_timer_resolution_;
}

/***************************************/
/*             INTERNALS               */
/***************************************/
void* dxfwAlloc(size_t size) {
  return (*g_state_.m_alloc_)(size);
}

void dxfwDealloc(void* ptr) {
  (*g_state_.m_dealloc_)(ptr);
}

void dxfwInitializeTimer() {
  LARGE_INTEGER frequency;
  QueryPerformanceFrequency(&frequency);
  g_state_.m_timer_resolution_ = 1.0 / (double)frequency.QuadPart;

  LARGE_INTEGER timestamp;
  QueryPerformanceCounter(&timestamp);
  g_state_.m_timer_start_ = timestamp.QuadPart;
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
