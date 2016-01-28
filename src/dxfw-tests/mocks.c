#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#define WINUSERAPI
#include <windows.h>
#endif

WINUSERAPI BOOL WINAPI TranslateMessage(CONST MSG *lpMsg) {
  return TRUE;
}

WINUSERAPI LRESULT WINAPI DispatchMessage(CONST MSG *lpmsg) {
  return 0;
}

WINUSERAPI BOOL WINAPI PeekMessage(LPMSG lpMsg, HWND hWnd, UINT wMsgFilterMin, UINT wMsgFilterMax, UINT wRemoveMsg) {
  return TRUE;
}

WINUSERAPI LRESULT WINAPI DefWindowProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam) {
  return 0;
}

WINUSERAPI ATOM WINAPI RegisterClassEx(CONST WNDCLASSEXW * lpwcx) {
  return 0;
}

WINUSERAPI HWND WINAPI CreateWindowEx(
  DWORD dwExStyle,
  LPCTSTR lpClassName,
  LPCTSTR lpWindowName,
  DWORD dwStyle,
  int x,
  int y,
  int nWidth,
  int nHeight,
  HWND hWndParent,
  HMENU hMenu,
  HINSTANCE hInstance,
  LPVOID lpParam) {
  return (HWND)0;
}

WINUSERAPI BOOL WINAPI ShowWindow(HWND hWnd, int nCmdShow) {
  return TRUE;
}

WINUSERAPI BOOL WINAPI SetWindowPos(
  HWND hWnd,
  HWND hWndInsertAfter,
  int X,
  int Y,
  int cx,
  int cy,
  UINT uFlags) {
  return TRUE;
}

WINUSERAPI BOOL WINAPI UpdateWindow(HWND hWnd) {
  return TRUE;
}

WINUSERAPI BOOL WINAPI SetWindowText(HWND hWnd, LPCTSTR lpString) {
  return TRUE;
}

WINUSERAPI BOOL WINAPI GetClientRect(HWND hWnd, LPRECT lpRect) {
  return TRUE;
}

WINUSERAPI BOOL WINAPI AdjustWindowRect(LPRECT lpRect, DWORD dwStyle, BOOL bMenu) {
  return TRUE;
}

WINUSERAPI LONG WINAPI GetWindowLong(HWND hWnd, int nIndex) {
  return 0;
}

WINUSERAPI HCURSOR WINAPI LoadCursor(HINSTANCE hInstance, LPCTSTR lpCursorName) {
  return (HCURSOR)0;
}

WINUSERAPI HICON WINAPI LoadIcon(HINSTANCE hInstance, LPCTSTR lpIconName) {
  return (HICON)0;
}

WINUSERAPI BOOL WINAPI RegisterRawInputDevices(PCRAWINPUTDEVICE pRawInputDevices, UINT uiNumDevices, UINT cbSize) {
  return TRUE;
}

WINUSERAPI HWND WINAPI GetActiveWindow() {
  return (HWND)0;
}

WINUSERAPI UINT WINAPI GetRawInputData(HRAWINPUT hRawInput, UINT uiCommand, LPVOID pData, PUINT pcbSize, UINT cbSizeHeader) {
  return 0;
}
