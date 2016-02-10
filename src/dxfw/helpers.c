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
void dxfwDoTrace(const TCHAR* message, const TCHAR* title, bool show_msg_box);
TCHAR* dxfwHResultToString(HRESULT hr);
TCHAR* dxfwFormatMessage(const TCHAR* format, DWORD_PTR* arguments);

/***************************************/
/*           PUBLIC INTERFACE          */
/***************************************/
const char* dxfwErrorToString(dxfwError error) {
  return g_error_to_string_[error];
}

void dxfwTraceError(const char* file, int line, dxfwError error, bool show_msg_box) {
  const char* error_message = dxfwErrorToString(error);

  DWORD_PTR arguments[3];
  arguments[0] = (DWORD_PTR)file;
  arguments[1] = (DWORD_PTR)line;
  arguments[2] = (DWORD_PTR)error_message;
  TCHAR* full_message = dxfwFormatMessage(TEXT("%1!S!(%2!lu!): %3!S!."), arguments);

  dxfwDoTrace(full_message, TEXT("DXFW Error Trace"), show_msg_box);

  LocalFree(full_message);
}

void dxfwTraceHResult(const char* file, int line, HRESULT hr, bool show_msg_box) {
  TCHAR* hresult_message = dxfwHResultToString(hr);

  DWORD_PTR arguments[3];
  arguments[0] = (DWORD_PTR)file;
  arguments[1] = (DWORD_PTR)line;
  arguments[2] = (DWORD_PTR)hresult_message;
  TCHAR* full_message = dxfwFormatMessage(TEXT("%1!S!(%2!lu!): %3!s!."), arguments);

  dxfwDoTrace(full_message, TEXT("DXFW DirectX Trace"), show_msg_box);

  LocalFree(full_message);
  LocalFree(hresult_message);
}

/***************************************/
/*             INTERNALS               */
/***************************************/
void dxfwDoTrace(const TCHAR* message, const TCHAR* title, bool show_msg_box) {
  OutputDebugString(message);
  if (show_msg_box) {
    MessageBox(NULL, message, title, MB_OK);
  }

}

TCHAR* dxfwHResultToString(HRESULT hr) {
  TCHAR* message = NULL;
  FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
    NULL,
    hr,
    MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
    (LPTSTR)&message,
    0,
    NULL);
  return message;
}

TCHAR* dxfwFormatMessage(const TCHAR* format, DWORD_PTR* arguments) {
  TCHAR* message = NULL;
  FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_STRING | FORMAT_MESSAGE_ARGUMENT_ARRAY,
    format,
    0,
    MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
    (LPTSTR)&message,
    0,
    (va_list*)arguments);
  return message;
}