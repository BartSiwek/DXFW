#include "dxfw-tests.h"

/* Mock window class registry */
struct dxfwMockWindowClass {
  WNDPROC m_wnd_proc_;
  LPWSTR m_class_;
  struct dxfwMockWindowClass* m_next_;
};

static struct dxfwMockWindowClass* g_mock_window_class_head_ = NULL;

struct dxfwMockWindowClass* dxfwTestGetWindowClass(LPCWSTR window_class_name) {
  struct dxfwMockWindowClass* ptr = g_mock_window_class_head_;
  while (ptr != NULL && wcscmp(ptr->m_class_, window_class_name) != 0) {
    ptr = ptr->m_next_;
  }
  return ptr;
}

void dxfwTestAddWindowClass(CONST WNDCLASSEXW* window_class) {
  struct dxfwMockWindowClass* ptr = dxfwTestGetWindowClass(window_class->lpszClassName);
  if (ptr == NULL) {
    struct dxfwMockWindowClass* mock_window_class = (struct dxfwMockWindowClass*)malloc(sizeof(struct dxfwMockWindowClass));
    memset(mock_window_class, 0, sizeof(struct dxfwMockWindowClass));
   
    mock_window_class->m_class_ = _wcsdup(window_class->lpszClassName);
    mock_window_class->m_wnd_proc_ = window_class->lpfnWndProc;

    mock_window_class->m_next_ = g_mock_window_class_head_;
    g_mock_window_class_head_ = mock_window_class;
  }
}

void dxfwTestClearWindowClasses() {
  struct dxfwMockWindowClass* ptr = g_mock_window_class_head_;
  g_mock_window_class_head_ = NULL;

  while (ptr != NULL) {
    struct dxfwMockWindowClass* next = ptr->m_next_;
    free(ptr->m_class_);
    free(ptr);
    ptr = next;
  }
}

/* Mock window registry */
struct dxfwMockWindow {
  HWND m_hwnd_;
  WNDPROC m_window_procedure_;
  struct dxfwMockWindow* m_next_;
};

static struct dxfwMockWindow* g_mock_window_head_ = NULL;

struct dxfwMockWindow* dxfwTestGetWindow(HWND hwnd) {
  struct dxfwMockWindow* ptr = g_mock_window_head_;
  while (ptr != NULL && ptr->m_hwnd_ != hwnd) {
    ptr = ptr->m_next_;
  }
  return ptr;
}

void dxfwTestAddWindow(HWND hwnd, WNDPROC window_procedure) {
  struct dxfwMockWindow* ptr = dxfwTestGetWindow(hwnd);
  if (ptr == NULL) {
    struct dxfwMockWindow* mock_window = (struct dxfwMockWindow*)malloc(sizeof(struct dxfwMockWindow));
    memset(mock_window, 0, sizeof(struct dxfwMockWindow));

    mock_window->m_hwnd_ = hwnd;
    mock_window->m_window_procedure_ = window_procedure;

    mock_window->m_next_ = g_mock_window_head_;
    g_mock_window_head_ = mock_window;
  }
}

void dxfwTestRemoveWindow(HWND hwnd) {
  struct dxfwMockWindow** current = &g_mock_window_head_;
  while (*current != NULL && (*current)->m_hwnd_ != hwnd) {
    current = &(*current)->m_next_;
  }
  if (*current != NULL) {
    struct dxfwMockWindow* window = *current;
    *current = window->m_next_;
    free(window);
  }
}

void dxfwTestClearWindows() {
  struct dxfwMockWindow* ptr = g_mock_window_head_;
  g_mock_window_head_ = NULL;

  while (ptr != NULL) {
    struct dxfwMockWindow* next = ptr->m_next_;
    free(ptr);
    ptr = next;
  }
}

/* Setup teardown */
int dxfwTestOsMocksSetup() {
  if (g_mock_window_class_head_ != NULL) {
    return 1;
  }
  if (g_mock_window_head_ != NULL) {
    return 1;
  }
  return 0;
}

int dxfwTestOsMocksTeardown() {
  dxfwTestClearWindows();
  dxfwTestClearWindowClasses();
  return 0;
}

/* Windows OS mocks */
WINUSERAPI BOOL WINAPI PeekMessage(LPMSG lpMsg, HWND hWnd, UINT wMsgFilterMin, UINT wMsgFilterMax, UINT wRemoveMsg) {
  lpMsg->hwnd = (HWND)mock();
  lpMsg->message = (UINT)mock();
  lpMsg->lParam = (LPARAM)mock();
  lpMsg->wParam = (WPARAM)mock();
  lpMsg->time = (DWORD)mock();
  lpMsg->pt.x = (LONG)mock();
  lpMsg->pt.y = (LONG)mock();

  (void)hWnd;
  (void)wMsgFilterMin;
  (void)wMsgFilterMax;
  (void)wRemoveMsg;

  return (BOOL)mock();
}

WINUSERAPI BOOL WINAPI TranslateMessage(CONST MSG *lpMsg) {
  // This does nothing when it comes to mocking
  return TRUE;
}

WINUSERAPI LRESULT WINAPI DispatchMessageW(CONST MSG *lpmsg) {
  struct dxfwMockWindow* mock_window = dxfwTestGetWindow(lpmsg->hwnd);
  assert_non_null(mock_window);
  return (*mock_window->m_window_procedure_)(lpmsg->hwnd, lpmsg->message, lpmsg->wParam, lpmsg->lParam); 
}

WINUSERAPI LRESULT WINAPI DefWindowProcW(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam) {
  check_expected(hWnd);
  check_expected(Msg);
  check_expected(wParam);
  check_expected(lParam);

  return (LRESULT)mock();
}

WINUSERAPI ATOM WINAPI RegisterClassExW(CONST WNDCLASSEXW* lpwcx) {
  dxfwTestAddWindowClass(lpwcx);
  return 0;
}

WINUSERAPI HWND WINAPI CreateWindowExW(DWORD dwExStyle, LPCTSTR lpClassName, LPCTSTR lpWindowName, DWORD dwStyle,
                                       int x, int y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu,
                                       HINSTANCE hInstance, LPVOID lpParam) {
  check_expected(lpWindowName);
  check_expected(x);
  check_expected(y);
  check_expected(nWidth);
  check_expected(nHeight);

  HWND hwnd = (HWND)mock();
  struct dxfwMockWindowClass* mock_window_class = dxfwTestGetWindowClass(lpClassName);
  dxfwTestAddWindow(hwnd, mock_window_class->m_wnd_proc_);

  (*mock_window_class->m_wnd_proc_)(hwnd, WM_CREATE, 0, 0);

  return hwnd;
}

WINUSERAPI BOOL WINAPI DestroyWindow(HWND hWnd) {
  check_expected(hWnd);
  dxfwTestRemoveWindow(hWnd);
  return (BOOL)mock();
}

WINUSERAPI BOOL WINAPI ShowWindow(HWND hWnd, int nCmdShow) {
  check_expected(hWnd);
  check_expected(nCmdShow);

  return (BOOL)mock();
}

WINUSERAPI BOOL WINAPI SetWindowPos(HWND hWnd, HWND hWndInsertAfter, int X, int Y, int cx, int cy, UINT uFlags) {
  check_expected(hWnd);
  check_expected(hWndInsertAfter);
  check_expected(X);
  check_expected(Y);
  check_expected(cx);
  check_expected(cy);
  check_expected(uFlags);

  return (BOOL)mock();
}

WINUSERAPI BOOL WINAPI UpdateWindow(HWND hWnd) {
  check_expected(hWnd);

  return (BOOL)mock();
}

WINUSERAPI BOOL WINAPI SetWindowTextW(HWND hWnd, LPCTSTR lpString) {
  check_expected(hWnd);
  check_expected(lpString);

  return (BOOL)mock();
}

WINUSERAPI BOOL WINAPI GetClientRect(HWND hWnd, LPRECT lpRect) {
  check_expected(hWnd);
  lpRect->left = (LONG)mock();
  lpRect->top = (LONG)mock();
  lpRect->right = (LONG)mock();
  lpRect->bottom = (LONG)mock();

  return (BOOL)mock();
}

WINUSERAPI BOOL WINAPI AdjustWindowRect(LPRECT lpRect, DWORD dwStyle, BOOL bMenu) {
  if (lpRect != NULL) {
    check_expected(lpRect->left);
    check_expected(lpRect->top);
    check_expected(lpRect->right);
    check_expected(lpRect->bottom);
  } else {
    check_expected(lpRect);
  }

  check_expected(dwStyle);
  check_expected(bMenu);

  lpRect = mock_ptr_type(LPRECT);

  return (BOOL)mock();
}

WINUSERAPI LONG WINAPI GetWindowLongW(HWND hWnd, int nIndex) {
  check_expected(hWnd);
  check_expected(nIndex);
  return (LONG)mock();
}

WINUSERAPI HCURSOR WINAPI LoadCursorW(HINSTANCE hInstance, LPCTSTR lpCursorName) {
  // Nothing
  return (HCURSOR)0;
}

WINUSERAPI HICON WINAPI LoadIconW(HINSTANCE hInstance, LPCTSTR lpIconName) {
  // Nothing
  return (HICON)0;
}

WINUSERAPI BOOL WINAPI RegisterRawInputDevices(PCRAWINPUTDEVICE pRawInputDevices, UINT uiNumDevices, UINT cbSize) {
  check_expected(pRawInputDevices);
  check_expected(uiNumDevices);
  check_expected(cbSize);

  return (BOOL)mock();
}

WINUSERAPI HWND WINAPI GetActiveWindow() {
  return (HWND)mock();
}

WINUSERAPI UINT WINAPI GetRawInputData(HRAWINPUT hRawInput, UINT uiCommand, LPVOID pData, PUINT pcbSize, UINT cbSizeHeader) {
  LPRAWINPUT raw_input = (LPRAWINPUT)pData;

  check_expected(hRawInput);
  check_expected(uiCommand);
  raw_input->header.dwType = (DWORD)mock();
  raw_input->data.keyboard.VKey = (USHORT)mock();
  raw_input->data.keyboard.Flags = (USHORT)mock();
  *pcbSize = sizeof(RAWINPUT);
  check_expected(cbSizeHeader);

  return (UINT)mock();
}
