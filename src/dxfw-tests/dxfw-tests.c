#include "dxfw-tests.h"

/* HELPERS */
void dxfwTestRunAllocationTest(uintptr_t id) {
  dxfwTestSetupAnyWindowCreateExpectations(id);

  expect_any(dxfwTestMallocMock, size);  // Window
  expect_any(dxfwTestMallocMock, size);  // UTF 8 caption
  expect_any(dxfwTestFreeMock, ptr);         // UTF 8 caption
  struct dxfwWindow* w = dxfwCreateWindow(100, 100, "dxfwTestCreateAndDestroyWindow");

  dxfwTestSetupWindowDestroyExpectations(id);

  expect_value(dxfwTestFreeMock, ptr, (void*)w);  // Window
  dxfwDestroyWindow(w);
}

/* MOCKS */
void* dxfwTestMallocMock(size_t size) {
  check_expected(size);
  return malloc(size);
}

void dxfwTestFreeMock(void* ptr) {
  check_expected(ptr);
}

/* TESTS */
void dxfwInitTest(void **state) {
  DXFW_TEST_UNUSED(state);

  dxfwSetErrorCallback(dxfwTestErrorCallbackMock);  // Make sure no errors are reported

  assert_true(dxfwInitialize());
  dxfwTerminate();

  dxfwSetErrorCallback(NULL);
}

void dxfwDoubleInitTest(void **state) {
  DXFW_TEST_UNUSED(state);

  dxfwSetErrorCallback(dxfwTestErrorCallbackMock);
  assert_true(dxfwInitialize());

  expect_value(dxfwTestErrorCallbackMock, error, DXFW_ERROR_ALREADY_INITIALIZED);
  assert_false(dxfwInitialize());

  dxfwTerminate();
  dxfwSetErrorCallback(NULL);
}

void dxfwNoInitTest(void **state) {
  DXFW_TEST_UNUSED(state);

  dxfwSetErrorCallback(dxfwTestErrorCallbackMock);

  expect_value(dxfwTestErrorCallbackMock, error, DXFW_ERROR_NOT_INITIALIZED);
  dxfwTerminate();

  dxfwSetErrorCallback(NULL);
}

void dxfwSettingMemoryAfterInitTest(void **state) {
  DXFW_TEST_UNUSED(state);

  dxfwSetErrorCallback(dxfwTestErrorCallbackMock);

  assert_true(dxfwInitialize());

  expect_value(dxfwTestErrorCallbackMock, error, DXFW_ERROR_ALREADY_INITIALIZED);
  dxfwSetAlloc(dxfwTestMallocMock, dxfwTestFreeMock);

  dxfwTerminate();
  dxfwSetErrorCallback(NULL);
}

void dxfwMemoryTest(void **state) {
  DXFW_TEST_UNUSED(state);

  assert_int_equal(0, dxfwTestOsMocksSetup());

  dxfwSetErrorCallback(dxfwTestErrorCallbackMock);  // Make sure no errors are reported
  dxfwSetAlloc(dxfwTestMallocMock, dxfwTestFreeMock);
  assert_true(dxfwInitialize());

  dxfwTestSetupAnyWindowCreateExpectations(1);

  expect_any(dxfwTestMallocMock, size);  // Window
  expect_any(dxfwTestMallocMock, size);  // UTF 8 caption
  expect_any(dxfwTestFreeMock, ptr);         // UTF 8 caption
  struct dxfwWindow* w = dxfwCreateWindow(100, 100, "dxfwMemoryTest");

  dxfwTestSetupWindowDestroyExpectations(1);

  expect_value(dxfwTestFreeMock, ptr, (void*)w);  // Window
  dxfwDestroyWindow(w);

  dxfwTerminate();
  dxfwSetAlloc(malloc, free);
  dxfwSetErrorCallback(NULL);

  assert_int_equal(0, dxfwTestOsMocksTeardown());
}

void dxfwSetAllocNullArguments(void **state) {
  DXFW_TEST_UNUSED(state);

  dxfwSetErrorCallback(dxfwTestErrorCallbackMock);  // Make sure no errors are reported

  expect_value(dxfwTestErrorCallbackMock, error, DXFW_ERROR_INVALID_ARGUMENT);
  dxfwSetAlloc(NULL, dxfwTestFreeMock);

  expect_value(dxfwTestErrorCallbackMock, error, DXFW_ERROR_INVALID_ARGUMENT);
  dxfwSetAlloc(dxfwTestMallocMock, NULL);

  expect_value(dxfwTestErrorCallbackMock, error, DXFW_ERROR_INVALID_ARGUMENT);
  dxfwSetAlloc(NULL, NULL);
}

void dxfwNotInitializedTest(void **state) {
  DXFW_TEST_UNUSED(state);

  expect_value(dxfwTestErrorCallbackMock, error, DXFW_ERROR_NOT_INITIALIZED);
  dxfwGetTime();

  expect_value(dxfwTestErrorCallbackMock, error, DXFW_ERROR_NOT_INITIALIZED);
  dxfwCreateWindow(100, 100, "dxfwNotInitializedTest");

  expect_value(dxfwTestErrorCallbackMock, error, DXFW_ERROR_NOT_INITIALIZED);
  dxfwDestroyWindow(NULL);

  expect_value(dxfwTestErrorCallbackMock, error, DXFW_ERROR_NOT_INITIALIZED);
  dxfwGetHandle(NULL);

  expect_value(dxfwTestErrorCallbackMock, error, DXFW_ERROR_NOT_INITIALIZED);
  dxfwGetWindowUserData(NULL);

  expect_value(dxfwTestErrorCallbackMock, error, DXFW_ERROR_NOT_INITIALIZED);
  dxfwSetWindowUserData(NULL, NULL);

  expect_value(dxfwTestErrorCallbackMock, error, DXFW_ERROR_NOT_INITIALIZED);
  dxfwSetWindowCaption(NULL, NULL);

  expect_value(dxfwTestErrorCallbackMock, error, DXFW_ERROR_NOT_INITIALIZED);
  dxfwGetWindowSize(NULL, NULL, NULL);

  expect_value(dxfwTestErrorCallbackMock, error, DXFW_ERROR_NOT_INITIALIZED);
  dxfwSetWindowSize(NULL, 100, 100);

  expect_value(dxfwTestErrorCallbackMock, error, DXFW_ERROR_NOT_INITIALIZED);
  dxfwShouldWindowClose(NULL);

  expect_value(dxfwTestErrorCallbackMock, error, DXFW_ERROR_NOT_INITIALIZED);
  dxfwPollOsEvents();
  
  expect_value(dxfwTestErrorCallbackMock, error, DXFW_ERROR_NOT_INITIALIZED);
  dxfwSetShouldCloseChangedCallback(NULL, NULL);

  expect_value(dxfwTestErrorCallbackMock, error, DXFW_ERROR_NOT_INITIALIZED);
  dxfwSetMouseButtonCallback(NULL, NULL);

  expect_value(dxfwTestErrorCallbackMock, error, DXFW_ERROR_NOT_INITIALIZED);
  dxfwSetMouseMoveCallback(NULL, NULL);

  expect_value(dxfwTestErrorCallbackMock, error, DXFW_ERROR_NOT_INITIALIZED);
  dxfwSetMouseWheelCallback(NULL, NULL);

  expect_value(dxfwTestErrorCallbackMock, error, DXFW_ERROR_NOT_INITIALIZED);
  dxfwGetKeyState(DXFW_KEY_A);

  expect_value(dxfwTestErrorCallbackMock, error, DXFW_ERROR_NOT_INITIALIZED);
  dxfwGetPreviousKeyState(DXFW_KEY_A);

  expect_value(dxfwTestErrorCallbackMock, error, DXFW_ERROR_NOT_INITIALIZED);
  dxfwGetModifierFlags();

  expect_value(dxfwTestErrorCallbackMock, error, DXFW_ERROR_NOT_INITIALIZED);
  dxfwSetKeyboardCallback(NULL, NULL);
}

void dxfwGetTimeTest(void **state) {
  DXFW_TEST_UNUSED(state);

  double timestamp1 = dxfwGetTime();
  Sleep(10);
  double timestamp2 = dxfwGetTime();
  Sleep(20);
  double timestamp3 = dxfwGetTime();

  assert_true(timestamp2 - timestamp1 > 0.01);
  assert_true(timestamp3 - timestamp2 > 0.02);
}
