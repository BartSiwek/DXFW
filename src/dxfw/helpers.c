#include "dxfw/dxfw.h"
#include "dxfw-internal-macros.h"
#include "dxfw-internal.h"

#include <stdlib.h>
#include <stdio.h>

/***************************************/
/*                GLOBALS              */
/***************************************/
const char* g_error_to_string_[DXFW_ERROR_MAX] = {
  "No error",                           // DXFW_ERROR_NONE = 0
  "Invalid argument",                   // DXFW_ERROR_INVALID_ARGUMENT = 1
  "DXFW has already been initialized",  // DXFW_ERROR_ALREADY_INITIALIZED = 2
  "DXFW has not been initialized",      // DXFW_ERROR_NOT_INITIALIZED = 3
  "UTF8 conversion error",              // DXFW_ERROR_UTF8_CONVERSION = 4
  "Invalid window size",                // DXFW_ERROR_INVALID_WINDOW_SIZE = 5
  // DXFW_ERROR_MAX
};

/***************************************/
/*               FORWARDS              */
/***************************************/
void dxfwDoTrace(const wchar_t* message, bool show_msg_box);
wchar_t* dxfwFormatHResult(HRESULT hr);
wchar_t* dxfwFormatMessage(const wchar_t* format, ...);
wchar_t* dxfwFormatMessageWithArgumentList(const wchar_t* format, va_list arguments);

/***************************************/
/*           PUBLIC INTERFACE          */
/***************************************/
const char* dxfwErrorToString(dxfwError error) {
  DXFW_CHECK_IF_INITIALIZED_DONT_REPORT_ERROR_AND_RETURN(NULL);

  return g_error_to_string_[error];
}

char* dxfwHResultToString(HRESULT hr) {
  DXFW_CHECK_IF_INITIALIZED_DONT_REPORT_ERROR_AND_RETURN(NULL);

  wchar_t* message_wide = dxfwFormatHResult(hr);

  char* message = dxfwWcharToUtf8(message_wide);

  LocalFree(message_wide);

  return message;
}

void dxfwTraceMessage(const char* file, int line, bool show_msg_box, const char* message, ...) {
  DXFW_CHECK_IF_INITIALIZED_DONT_REPORT_ERROR();

  wchar_t* message_wide = dxfwUtf8ToWchar(message);

  if (message_wide != NULL) {
    va_list message_args;
    va_start(message_args, message);
    wchar_t* full_message_text = dxfwFormatMessageWithArgumentList(message_wide, message_args);
    va_end(message_args);

    wchar_t* full_message = dxfwFormatMessage(L"%S(%d): %s\n", file, line, full_message_text);

    dxfwDoTrace(full_message, show_msg_box);

    dxfwDealloc(full_message);
    dxfwDealloc(full_message_text);
    dxfwDealloc(message_wide);
  }
}

void dxfwTraceError(const char* file, int line, bool show_msg_box, dxfwError error) {
  DXFW_CHECK_IF_INITIALIZED_DONT_REPORT_ERROR();

  const char* error_message = dxfwErrorToString(error);
  wchar_t* full_message = dxfwFormatMessage(L"%S(%d): %3S.\n", file, line, error_message);

  dxfwDoTrace(full_message, show_msg_box);

  dxfwDealloc(full_message);
}

void dxfwTraceHResult(const char* file, int line, bool show_msg_box, HRESULT hr) {
  DXFW_CHECK_IF_INITIALIZED_DONT_REPORT_ERROR();

  wchar_t* hresult_message = dxfwFormatHResult(hr);
  wchar_t* full_message = dxfwFormatMessage(L"%S(%d): %s", file, line, hresult_message);

  dxfwDoTrace(full_message, show_msg_box);

  dxfwDealloc(full_message);
  LocalFree(hresult_message);
}

/***************************************/
/*             INTERNALS               */
/***************************************/
void dxfwDoTrace(const wchar_t* message, bool show_msg_box) {
  OutputDebugStringW(message);
  if (show_msg_box) {
    MessageBoxW(NULL, message, L"DXFW", MB_OK);
  }

}

wchar_t* dxfwFormatHResult(HRESULT hr) {
  wchar_t* message = NULL;
  FormatMessageW(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
    NULL,
    hr,
    MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
    (LPWSTR)&message,
    0,
    NULL);
  return message;
}

wchar_t* dxfwFormatMessage(const wchar_t* format, ...) {
  va_list arguments;
  va_start(arguments, format);
  wchar_t* message = dxfwFormatMessageWithArgumentList(format, arguments);
  va_end(arguments);
  return message;
}

wchar_t* dxfwFormatMessageWithArgumentList(const wchar_t* format, va_list arguments) {
  int character_count = _vscwprintf(format, arguments) + 1;
  wchar_t* message = dxfwAlloc(character_count * sizeof(wchar_t));
  vswprintf(message, character_count, format, arguments);
  return message;
}
