#include "dxfw-tests.h"

void errorCallbackMock(dxfwError error) {
  check_expected(error);
}

int dxfwTestSetup(void **state) {
  int mock_setup_error = dxfwTestsMocksSetup();
  if (mock_setup_error) {
    return 1;
  }

  bool initialize_ok = dxfwInitialize();
  if (!initialize_ok) {
    return 1;
  }

  dxfwSetErrorCallback(errorCallbackMock);

  return 0;
}

int dxfwTestTeardown(void **state) {
  dxfwSetErrorCallback(NULL);
  dxfwTerminate();

  int mock_teardown_error = dxfwTestsMocksTeardown();
  if (mock_teardown_error) {
    return 1;
  }

  return 0;
}

void dxfwSetupAnyWmCreateExpectations() {
  expect_any(RegisterRawInputDevices, pRawInputDevices);
  expect_any(RegisterRawInputDevices, uiNumDevices);
  expect_any(RegisterRawInputDevices, cbSize);
  will_return(RegisterRawInputDevices, TRUE);
}

void dxfwSetupAnyWindowCreateExpectations() {
  will_return(CreateWindowExW, (HWND)1);

  dxfwSetupAnyWmCreateExpectations();

  RECT r = { 0, 0, 107, 107 };
  expect_any(AdjustWindowRect, lpRect);
  expect_any(AdjustWindowRect, dwStyle);
  expect_any(AdjustWindowRect, bMenu);
  will_return(AdjustWindowRect, &r);
  will_return(AdjustWindowRect, TRUE);

  expect_any(ShowWindow, hWnd);
  expect_any(ShowWindow, nCmdShow);
  will_return(ShowWindow, TRUE);

  expect_any(UpdateWindow, hWnd);
  will_return(UpdateWindow, TRUE);
}

void dxfwInitTest(void **state) {
  assert_true(dxfwInitialize());
  dxfwTerminate();
}

void dxfwDoubleInitTest(void **state) {
  assert_true(dxfwInitialize());
  assert_false(dxfwInitialize());
  dxfwTerminate();
}

void dxfwNoInitTest(void **state) {
  dxfwTerminate();
}

void* dxfwMallocMock(size_t size) {
  check_expected(size);
  return malloc(size);
}

void dxfwFreeMock(void* ptr) {
  check_expected(ptr);
}

void dxfwMemoryTest(void **state) {
  dxfwSetAlloc(dxfwMallocMock, dxfwFreeMock);

  dxfwSetupAnyWindowCreateExpectations();

  expect_any_count(dxfwMallocMock, size, 2);
  expect_any(dxfwFreeMock, ptr);
  struct dxfwWindow* w = dxfwCreateWindow(100, 100, "dxfwMemoryTest");  // Should allocate

  expect_value(dxfwFreeMock, ptr, (void*)w);
  dxfwDestroyWindow(w);  // Should deallocate
}