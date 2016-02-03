#include "dxfw-tests.h"

/* TEST HELPERS */
void dxfwSetupAnyWmCreateExpectations() {
  expect_any(RegisterRawInputDevices, pRawInputDevices);
  expect_any(RegisterRawInputDevices, uiNumDevices);
  expect_any(RegisterRawInputDevices, cbSize);
  will_return(RegisterRawInputDevices, TRUE);
}

void dxfwSetupAnyWindowCreateExpectations(int id) {
  RECT r = { 0, 0, 100, 200 };
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

  dxfwSetupAnyWmCreateExpectations();

  expect_value(ShowWindow, hWnd, (HWND)id);
  expect_any(ShowWindow, nCmdShow);
  will_return(ShowWindow, TRUE);

  expect_value(UpdateWindow, hWnd, (HWND)id);
  will_return(UpdateWindow, TRUE);
}

dxfwSetupAnyWindowDestroyExpectations(int id) {
  expect_value(DestroyWindow, hWnd, (HWND)id);
  will_return(DestroyWindow, TRUE);
}

wchar_t* dxfwTestsUtf8ToWchar(const char* utf8) {
  int length = MultiByteToWideChar(CP_UTF8, 0, utf8, -1, NULL, 0);
  wchar_t* output_buffer = (wchar_t*)malloc(length * sizeof(wchar_t));
  MultiByteToWideChar(CP_UTF8, 0, utf8, -1, output_buffer, length);
  return output_buffer;
}

/* SETUP & TEARDOWN */
int dxfwTestSetup(void **state) {
  int mock_setup_error = dxfwTestsOsMocksSetup();
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
  dxfwSetErrorCallback(NULL);
  dxfwTerminate();

  int mock_teardown_error = dxfwTestsOsMocksTeardown();
  if (mock_teardown_error) {
    return 1;
  }

  return 0;
}

/* MOCKS */
void dxfwTestErrorCallbackMock(dxfwError error) {
  check_expected(error);
}
