#include "dxfw/dxfw.h"
#include "dxfw-internal-macros.h"
#include "dxfw-internal.h"

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
wchar_t* dxfwFormatMessage(const wchar_t* format, DWORD_PTR* arguments);

/***************************************/
/*           PUBLIC INTERFACE          */
/***************************************/
const char* dxfwErrorToString(dxfwError error) {
  return g_error_to_string_[error];
}

char* dxfwHResultToString(HRESULT hr) {
  wchar_t* message_wide = dxfwFormatHResult(hr);

  char* message = dxfwWcharToUtf8(message_wide);

  LocalFree(message_wide);

  return message;
}

void dxfwTraceMessage(const char* file, int line, const char* message, bool show_msg_box) {
  wchar_t* message_wide = dxfwUtf8ToWchar(message);

  if (message_wide != NULL) {
    DWORD_PTR arguments[3];
    arguments[0] = (DWORD_PTR)file;
    arguments[1] = (DWORD_PTR)line;
    arguments[2] = (DWORD_PTR)message_wide;
    wchar_t* full_message = dxfwFormatMessage(L"%1!S!(%2!lu!): %3!s!.", arguments);

    dxfwDoTrace(full_message, show_msg_box);

    dxfwDealloc(message_wide);
  }
}

void dxfwTraceError(const char* file, int line, dxfwError error, bool show_msg_box) {
  const char* error_message = dxfwErrorToString(error);

  DWORD_PTR arguments[3];
  arguments[0] = (DWORD_PTR)file;
  arguments[1] = (DWORD_PTR)line;
  arguments[2] = (DWORD_PTR)error_message;
  wchar_t* full_message = dxfwFormatMessage(L"%1!S!(%2!lu!): %3!S!.", arguments);

  dxfwDoTrace(full_message, show_msg_box);

  LocalFree(full_message);
}

void dxfwTraceHResult(const char* file, int line, HRESULT hr, bool show_msg_box) {
  wchar_t* hresult_message = dxfwFormatHResult(hr);

  DWORD_PTR arguments[3];
  arguments[0] = (DWORD_PTR)file;
  arguments[1] = (DWORD_PTR)line;
  arguments[2] = (DWORD_PTR)hresult_message;
  wchar_t* full_message = dxfwFormatMessage(L"%1!S!(%2!lu!): %3!s!.", arguments);

  dxfwDoTrace(full_message, show_msg_box);

  LocalFree(full_message);
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

wchar_t* dxfwFormatMessage(const wchar_t* format, DWORD_PTR* arguments) {
  wchar_t* message = NULL;
  FormatMessageW(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_STRING | FORMAT_MESSAGE_ARGUMENT_ARRAY,
    format,
    0,
    MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
    (LPWSTR)&message,
    0,
    (va_list*)arguments);
  return message;
}