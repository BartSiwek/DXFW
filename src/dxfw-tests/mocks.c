#include "dxfw-tests.h"

/* TEST HELPERS */
void dxfwTestSetupAnyWmCreateExpectations() {
  expect_any(RegisterRawInputDevices, pRawInputDevices);
  expect_any(RegisterRawInputDevices, uiNumDevices);
  expect_any(RegisterRawInputDevices, cbSize);
  will_return(RegisterRawInputDevices, TRUE);
}

void dxfwTestSetupAnyWindowCreateExpectations(uintptr_t id) {
  RECT r = { 0, 0, 1, 1 };
  expect_any(AdjustWindowRect, lpRect->left);
  expect_any(AdjustWindowRect, lpRect->top);
  expect_any(AdjustWindowRect, lpRect->right);
  expect_any(AdjustWindowRect, lpRect->bottom);
  expect_any(AdjustWindowRect, dwStyle);
  expect_any(AdjustWindowRect, bMenu);
  will_return(AdjustWindowRect, &r);
  will_return(AdjustWindowRect, TRUE);

  expect_any(CreateWindowExW, lpWindowName);
  expect_any(CreateWindowExW, x);
  expect_any(CreateWindowExW, y);
  expect_any(CreateWindowExW, nWidth);
  expect_any(CreateWindowExW, nHeight);
  will_return(CreateWindowExW, (HWND)id);

  dxfwTestSetupAnyWmCreateExpectations();

  expect_value(ShowWindow, hWnd, (HWND)id);
  expect_any(ShowWindow, nCmdShow);
  will_return(ShowWindow, TRUE);

  expect_value(UpdateWindow, hWnd, (HWND)id);
  will_return(UpdateWindow, TRUE);
}

void dxfwSetupWindowCreateExpectations(uintptr_t id, uint32_t width, uint32_t height, const wchar_t* title) {
  RECT r;
  r.left = 0;
  r.top = 0;
  r.right = width;
  r.bottom = height;

  expect_value(AdjustWindowRect, lpRect->left, 0);
  expect_value(AdjustWindowRect, lpRect->top, 0);
  expect_value(AdjustWindowRect, lpRect->right, width);
  expect_value(AdjustWindowRect, lpRect->bottom, height);
  expect_value(AdjustWindowRect, dwStyle, WS_OVERLAPPEDWINDOW | WS_VISIBLE);
  expect_value(AdjustWindowRect, bMenu, FALSE);
  will_return(AdjustWindowRect, &r);
  will_return(AdjustWindowRect, TRUE);

  expect_string(CreateWindowExW, lpWindowName, title);
  expect_value(CreateWindowExW, x, CW_USEDEFAULT);
  expect_value(CreateWindowExW, y, CW_USEDEFAULT);
  expect_value(CreateWindowExW, nWidth, width);
  expect_value(CreateWindowExW, nHeight, height);
  will_return(CreateWindowExW, (HWND)id);

  dxfwTestSetupAnyWmCreateExpectations();

  expect_value(ShowWindow, hWnd, (HWND)id);
  expect_value(ShowWindow, nCmdShow, SW_SHOWNORMAL);
  will_return(ShowWindow, TRUE);

  expect_value(UpdateWindow, hWnd, (HWND)id);
  will_return(UpdateWindow, TRUE);
}

void dxfwTestSetupWindowDestroyExpectations(uintptr_t id) {
  expect_value(DestroyWindow, hWnd, (HWND)id);
  will_return(DestroyWindow, TRUE);
}

void dxfwTestSetupPeekMessage(HWND hwnd, UINT msg, LPARAM lparam, WPARAM wparam) {
  will_return(PeekMessageW, hwnd);
  will_return(PeekMessageW, msg);
  will_return(PeekMessageW, lparam);
  will_return(PeekMessageW, wparam);
  will_return(PeekMessageW, 0);  // time
  will_return(PeekMessageW, 0);  // pt.x
  will_return(PeekMessageW, 0);  // pt.y
  will_return(PeekMessageW, TRUE);
}

void dxfwTestSetupLastPeekMessage() {
  will_return(PeekMessageW, 0);  // hwnd
  will_return(PeekMessageW, 0);  // msg
  will_return(PeekMessageW, 0);  // lparam
  will_return(PeekMessageW, 0);  // wparam
  will_return(PeekMessageW, 0);  // time
  will_return(PeekMessageW, 0);  // pt.x
  will_return(PeekMessageW, 0);  // pt.y
  will_return(PeekMessageW, FALSE);
}

void dxfwSetupDefWindowProc(HWND hwnd, UINT msg, LPARAM lparam, WPARAM wparam) {
  expect_value(DefWindowProcW, hWnd, hwnd);
  expect_value(DefWindowProcW, Msg, msg);
  expect_value(DefWindowProcW, lParam, lparam);
  expect_value(DefWindowProcW, wParam, wparam);
  will_return(DefWindowProcW, (LPARAM)0);
}

wchar_t* dxfwTestUtf8ToWchar(const char* utf8) {
  int length = MultiByteToWideChar(CP_UTF8, 0, utf8, -1, NULL, 0);
  wchar_t* output_buffer = (wchar_t*)malloc(length * sizeof(wchar_t));
  MultiByteToWideChar(CP_UTF8, 0, utf8, -1, output_buffer, length);
  return output_buffer;
}

/* SETUP & TEARDOWN */
int dxfwTestSetup(void **state) {
  DXFW_TEST_UNUSED(state);

  int mock_setup_error = dxfwTestOsMocksSetup();
  if (mock_setup_error) {
    return 1;
  }

  bool initialize_ok = dxfwInitialize();
  if (!initialize_ok) {
    return 1;
  }

  dxfwSetErrorCallback(dxfwTestErrorCallbackMock);

  return 0;
}

int dxfwTestTeardown(void **state) {
  DXFW_TEST_UNUSED(state);

  dxfwSetErrorCallback(NULL);
  dxfwTerminate();

  int mock_teardown_error = dxfwTestOsMocksTeardown();
  if (mock_teardown_error) {
    return 1;
  }

  return 0;
}

int dxfwSingleWindowTestSetup(void **state) {
  int base_result = dxfwTestSetup(state);
  if (base_result != 0) {
    return base_result;
  }

  const uintptr_t WINDOW_ID = 1;
  const uint32_t WINDOW_WIDTH = 101;
  const uint32_t WINDOW_HEIGHT = 201;
  const char* WINDOW_TITLE = "dxfwSingleWindowTestInstance";

  wchar_t* title_wide = dxfwTestUtf8ToWchar(WINDOW_TITLE);
  dxfwSetupWindowCreateExpectations(WINDOW_ID, WINDOW_WIDTH, WINDOW_HEIGHT, title_wide);
  struct dxfwWindow* w = dxfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE);

  struct dxfwTestSingleWindowTestData* data = (struct dxfwTestSingleWindowTestData*)malloc(sizeof(struct dxfwTestSingleWindowTestData));
  data->m_window_id_ = WINDOW_ID;
  data->m_window_ = w;
  data->m_window_name_wide_ = title_wide;

  *state = data;

  return 0;
}

int dxfwSingleWindowTestTeardown(void **state) {
  struct dxfwTestSingleWindowTestData* data = (struct dxfwTestSingleWindowTestData*)(*state);

  dxfwTestSetupWindowDestroyExpectations(data->m_window_id_);
  dxfwDestroyWindow(data->m_window_);

  free(data->m_window_name_wide_);

  int base_result = dxfwTestTeardown(state);
  if (base_result != 0) {
    return base_result;
  }

  return 0;
}

/* MOCKS */
void dxfwTestErrorCallbackMock(dxfwError error) {
  check_expected(error);
}
