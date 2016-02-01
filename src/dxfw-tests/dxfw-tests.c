#include "dxfw-tests.h"

void errorCallbackMock(dxfwError error) {
  check_expected(error);
}

/*
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
*/

/* MOCKS */
void* dxfwMallocMock(size_t size) {
  check_expected(size);
  return malloc(size);
}

void dxfwFreeMock(void* ptr) {
  check_expected(ptr);
}

/* TESTS */
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
  dxfwSetErrorCallback(errorCallbackMock);  // Make sure no errors are reported

  assert_true(dxfwInitialize());
  dxfwTerminate();

  dxfwSetErrorCallback(NULL);
}

void dxfwDoubleInitTest(void **state) {
  dxfwSetErrorCallback(errorCallbackMock);
  assert_true(dxfwInitialize());

  expect_value(errorCallbackMock, error, DXFW_ERROR_ALREADY_INITIALIZED);
  assert_false(dxfwInitialize());

  dxfwTerminate();
  dxfwSetErrorCallback(NULL);
}

void dxfwNoInitTest(void **state) {
  dxfwSetErrorCallback(errorCallbackMock);

  expect_value(errorCallbackMock, error, DXFW_ERROR_NOT_INITIALIZED);
  dxfwTerminate();

  dxfwSetErrorCallback(NULL);
}

void dxfwSettingMemoryAfterInitTest(void **state) {
  dxfwSetErrorCallback(errorCallbackMock);

  assert_true(dxfwInitialize());

  expect_value(errorCallbackMock, error, DXFW_ERROR_ALREADY_INITIALIZED);
  dxfwSetAlloc(dxfwMallocMock, dxfwFreeMock);

  dxfwTerminate();
  dxfwSetErrorCallback(NULL);
}

void dxfwMemoryTest(void **state) {
  dxfwSetErrorCallback(errorCallbackMock);  // Make sure no errors are reported
  dxfwSetAlloc(dxfwMallocMock, dxfwFreeMock);
  assert_true(dxfwInitialize());

  dxfwSetupAnyWindowCreateExpectations();

  expect_any_count(dxfwMallocMock, size, 2);
  expect_any(dxfwFreeMock, ptr);
  struct dxfwWindow* w = dxfwCreateWindow(100, 100, "dxfwMemoryTest");  // Should allocate

  expect_value(dxfwFreeMock, ptr, (void*)w);
  dxfwDestroyWindow(w);  // Should deallocate

  dxfwTerminate();
  dxfwSetErrorCallback(NULL);
}