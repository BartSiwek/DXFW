#include "dxfw-tests.h"

/* MOCKS */
void* dxfwMallocMock(size_t size) {
  check_expected(size);
  return malloc(size);
}

void dxfwFreeMock(void* ptr) {
  check_expected(ptr);
}

/* TESTS */
void dxfwInitTest(void **state) {
  dxfwSetErrorCallback(dxfwTestErrorCallbackMock);  // Make sure no errors are reported

  assert_true(dxfwInitialize());
  dxfwTerminate();

  dxfwSetErrorCallback(NULL);
}

void dxfwDoubleInitTest(void **state) {
  dxfwSetErrorCallback(dxfwTestErrorCallbackMock);
  assert_true(dxfwInitialize());

  expect_value(dxfwTestErrorCallbackMock, error, DXFW_ERROR_ALREADY_INITIALIZED);
  assert_false(dxfwInitialize());

  dxfwTerminate();
  dxfwSetErrorCallback(NULL);
}

void dxfwNoInitTest(void **state) {
  dxfwSetErrorCallback(dxfwTestErrorCallbackMock);

  expect_value(dxfwTestErrorCallbackMock, error, DXFW_ERROR_NOT_INITIALIZED);
  dxfwTerminate();

  dxfwSetErrorCallback(NULL);
}

void dxfwSettingMemoryAfterInitTest(void **state) {
  dxfwSetErrorCallback(dxfwTestErrorCallbackMock);

  assert_true(dxfwInitialize());

  expect_value(dxfwTestErrorCallbackMock, error, DXFW_ERROR_ALREADY_INITIALIZED);
  dxfwSetAlloc(dxfwMallocMock, dxfwFreeMock);

  dxfwTerminate();
  dxfwSetErrorCallback(NULL);
}

void dxfwMemoryTest(void **state) {
  assert_int_equal(0, dxfwTestsOsMocksSetup());

  dxfwSetErrorCallback(dxfwTestErrorCallbackMock);  // Make sure no errors are reported
  dxfwSetAlloc(dxfwMallocMock, dxfwFreeMock);
  assert_true(dxfwInitialize());

  dxfwSetupAnyWindowCreateExpectations(1);

  expect_any_count(dxfwMallocMock, size, 2);
  expect_any(dxfwFreeMock, ptr);
  struct dxfwWindow* w = dxfwCreateWindow(100, 100, "dxfwMemoryTest");  // Should allocate

  dxfwSetupWindowDestroyExpectations(1);

  expect_value(dxfwFreeMock, ptr, (void*)w);
  dxfwDestroyWindow(w);  // Should deallocate

  dxfwTerminate();
  dxfwSetAlloc(malloc, free);
  dxfwSetErrorCallback(NULL);

  assert_int_equal(0, dxfwTestsOsMocksTeardown());
}

void dxfwGetTimeTest(void **state) {
  double timestamp1 = dxfwGetTime();
  Sleep(10);
  double timestamp2 = dxfwGetTime();
  Sleep(20);
  double timestamp3 = dxfwGetTime();

  assert_true(timestamp2 - timestamp1 > 0.01);
  assert_true(timestamp3 - timestamp2 > 0.02);
}
