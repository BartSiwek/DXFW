#include "dxfw-tests.h"

/* HELPERS */
void dxfwTestRunSuccessfulWindowCreationTest(uintptr_t id, uint32_t width, uint32_t height, const char* title) {
  // Create
  wchar_t* title_wide = dxfwTestUtf8ToWchar(title);
  dxfwSetupWindowCreateExpectations(id, width, height, title_wide);
  struct dxfwWindow* w = dxfwCreateWindow(width, height, title);

  // Destroy
  dxfwTestSetupWindowDestroyExpectations(id);
  dxfwDestroyWindow(w);

  // Clean up
  free(title_wide);
}

/* MOCKS */
void dxfwTestOnShouldCloseCallbackMock(struct dxfwWindow* window, bool should_close) {
  check_expected(window);
  check_expected(should_close);
}

/* TESTS */
void dxfwCreateDestroyWindowTest(void** state) {
  DXFW_TEST_UNUSED(state);

  // Test const
  const uintptr_t WINDOW_ID = 2;
  const uint32_t WINDOW_WIDTH = 102;
  const uint32_t WINDOW_HEIGHT = 202;
  const char* WINDOW_NAME = "dxfwCreateDestroyWindowTest";

  dxfwTestRunSuccessfulWindowCreationTest(WINDOW_ID, WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_NAME);
}

void dxfwCreateWindowWithZeroWidthTest(void** state) {
  DXFW_TEST_UNUSED(state);

  const uint32_t WINDOW_WIDTH = 0;
  const uint32_t WINDOW_HEIGHT = 203;
  const char* WINDOW_NAME = "dxfwCreateWindowWithZeroWidthTest";

  expect_value(dxfwTestErrorCallbackMock, error, DXFW_ERROR_INVALID_WINDOW_SIZE);

  struct dxfwWindow* w = dxfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_NAME);
  assert_null(w);
}


void dxfwCreateWindowWithZeroHeightTest(void** state) {
  DXFW_TEST_UNUSED(state);

  const uint32_t WINDOW_WIDTH = 105;
  const uint32_t WINDOW_HEIGHT = 0;
  const char* WINDOW_NAME = "dxfwCreateWindowWithZeroHeightTest";

  expect_value(dxfwTestErrorCallbackMock, error, DXFW_ERROR_INVALID_WINDOW_SIZE);

  struct dxfwWindow* w = dxfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_NAME);
  assert_null(w);
}

void dxfwCreateWindowWithEmptyTitleTest(void** state) {
  DXFW_TEST_UNUSED(state);

  // Test const
  const uintptr_t WINDOW_ID = 7;
  const uint32_t WINDOW_WIDTH = 107;
  const uint32_t WINDOW_HEIGHT = 207;
  const char* WINDOW_NAME = "";

  dxfwTestRunSuccessfulWindowCreationTest(WINDOW_ID, WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_NAME);
}

void dxfwCreateWindowWithNullTitleTest(void** state) {
  DXFW_TEST_UNUSED(state);

  // Test const
  const uint32_t WINDOW_WIDTH = 107;
  const uint32_t WINDOW_HEIGHT = 207;
  const char* WINDOW_NAME = NULL;

  expect_value(dxfwTestErrorCallbackMock, error, DXFW_ERROR_INVALID_ARGUMENT);
  dxfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_NAME);
}

void dxfwGetHandleTest(void** state) {
  struct dxfwTestSingleWindowTestData* data = (struct dxfwTestSingleWindowTestData*)(*state);
  assert_ptr_equal(data->m_window_id_, dxfwGetHandle(data->m_window_));
}

void dxfwSetAndGetUserDataTest(void** state) {
  struct dxfwTestSingleWindowTestData* data = (struct dxfwTestSingleWindowTestData*)(*state);

  uintptr_t DATA = 789;

  assert_ptr_equal(NULL, dxfwGetWindowUserData(data->m_window_));

  dxfwSetWindowUserData(data->m_window_, (void*)DATA);
  assert_ptr_equal((void*)DATA, dxfwGetWindowUserData(data->m_window_));

  dxfwSetWindowUserData(data->m_window_, NULL);
  assert_ptr_equal(NULL, dxfwGetWindowUserData(data->m_window_));
}

void RunSetCaptionTest(uintptr_t id, uint32_t width, uint32_t height, const char* title, const char* new_title) {
  dxfwTestSetupAnyWindowCreateExpectations(id);
  struct dxfwWindow* w = dxfwCreateWindow(width, height, title);

  wchar_t* new_title_wide = dxfwTestUtf8ToWchar(new_title);
  expect_value(SetWindowTextW, hWnd, (HWND)id);
  expect_string(SetWindowTextW, lpString, new_title_wide);
  will_return(SetWindowTextW, TRUE);
  dxfwSetWindowCaption(w, new_title);

  dxfwTestSetupWindowDestroyExpectations(id);
  dxfwDestroyWindow(w);

  free(new_title_wide);
}

void dxfwSetWindowCaptionTest(void** state) {
  DXFW_TEST_UNUSED(state);

  // Test const
  const uintptr_t WINDOW_ID = 8;
  const uint32_t WINDOW_WIDTH = 108;
  const uint32_t WINDOW_HEIGHT = 208;
  const char* WINDOW_NAME = "dxfwSetWindowCaptionTest";
  const char* WINDOW_NAME_NEW = "dxfwSetWindowCaptionTestNew";
  
  RunSetCaptionTest(WINDOW_ID, WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_NAME, WINDOW_NAME_NEW);
}

void dxfwSetEmptyWindowCaptionTest(void** state) {
  DXFW_TEST_UNUSED(state);

  // Test const
  const uintptr_t WINDOW_ID = 9;
  const uint32_t WINDOW_WIDTH = 109;
  const uint32_t WINDOW_HEIGHT = 209;
  const char* WINDOW_NAME = "dxfwSetEmptyWindowCaptionTest";
  const char* WINDOW_NAME_NEW = "";

  RunSetCaptionTest(WINDOW_ID, WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_NAME, WINDOW_NAME_NEW);
}

void dxfwGetWindowSizeTest(void** state) {
  struct dxfwTestSingleWindowTestData* data = (struct dxfwTestSingleWindowTestData*)(*state);

  // Test
  RECT client_rect = {
    .left = 510,
    .top = 610,
    .right = 710,
    .bottom = 810
  };
  expect_value(GetClientRect, hWnd, (HWND)data->m_window_id_);
  will_return(GetClientRect, client_rect.left);
  will_return(GetClientRect, client_rect.top);
  will_return(GetClientRect, client_rect.right);
  will_return(GetClientRect, client_rect.bottom);
  will_return(GetClientRect, TRUE);

  uint32_t width;
  uint32_t height;
  dxfwGetWindowSize(data->m_window_, &width, &height);

  assert_int_equal(client_rect.right - client_rect.left, width);
  assert_int_equal(client_rect.bottom - client_rect.top, height);
}

void dxfwSetWindowSizeTest(void** state) {
  struct dxfwTestSingleWindowTestData* data = (struct dxfwTestSingleWindowTestData*)(*state);

  // Test const
  const uint32_t NEW_WINDOW_WIDTH = 311;
  const uint32_t NEW_WINDOW_HEIGHT = 411;
  const int32_t WINDOW_STYLE = 0;

  // Test
  RECT client_rect = { 
    .left = 511, 
    .top = 611, 
    .right = 711, 
    .bottom = 811 
  };
  expect_value(GetClientRect, hWnd, (HWND)data->m_window_id_);
  will_return(GetClientRect, client_rect.left);
  will_return(GetClientRect, client_rect.top);
  will_return(GetClientRect, client_rect.right);
  will_return(GetClientRect, client_rect.bottom);
  will_return(GetClientRect, TRUE);

  expect_value(GetWindowLongW, hWnd, (HWND)data->m_window_id_);
  expect_value(GetWindowLongW, nIndex, GWL_STYLE);
  will_return(GetWindowLongW, (LONG)WINDOW_STYLE);
  
  RECT new_client_rect;
  new_client_rect.left = client_rect.left;
  new_client_rect.top = client_rect.top;
  new_client_rect.right = client_rect.left + NEW_WINDOW_WIDTH;
  new_client_rect.bottom = client_rect.top + NEW_WINDOW_HEIGHT;
  
  expect_value(AdjustWindowRect, lpRect->left, new_client_rect.left);
  expect_value(AdjustWindowRect, lpRect->top, new_client_rect.top);
  expect_value(AdjustWindowRect, lpRect->right, new_client_rect.right);
  expect_value(AdjustWindowRect, lpRect->bottom, new_client_rect.bottom);
  expect_value(AdjustWindowRect, dwStyle, (LONG)WINDOW_STYLE);
  expect_value(AdjustWindowRect, bMenu, FALSE);
  will_return(AdjustWindowRect, &new_client_rect);
  will_return(AdjustWindowRect, (BOOL)TRUE);

  HWND insert_after = HWND_TOPMOST;  // We need this so the predefined value can narrow correctly
  expect_value(SetWindowPos, hWnd, (HWND)data->m_window_id_);
  expect_value(SetWindowPos, hWndInsertAfter, insert_after);
  expect_value(SetWindowPos, X, new_client_rect.left);
  expect_value(SetWindowPos, Y, new_client_rect.top);
  expect_value(SetWindowPos, cx, new_client_rect.right - new_client_rect.left);
  expect_value(SetWindowPos, cy, new_client_rect.bottom - new_client_rect.top);
  expect_value(SetWindowPos, uFlags, SWP_NOCOPYBITS);
  will_return(SetWindowPos, TRUE);

  dxfwSetWindowSize(data->m_window_, NEW_WINDOW_WIDTH, NEW_WINDOW_HEIGHT);
}

void dxfwSetWindowSizeToZeroWidthTest(void** state) {
  struct dxfwTestSingleWindowTestData* data = (struct dxfwTestSingleWindowTestData*)(*state);

  // Test const
  const uint32_t NEW_WINDOW_WIDTH = 0;
  const uint32_t NEW_WINDOW_HEIGHT = 412;

  expect_value(dxfwTestErrorCallbackMock, error, DXFW_ERROR_INVALID_WINDOW_SIZE);

  dxfwSetWindowSize(data->m_window_, NEW_WINDOW_WIDTH, NEW_WINDOW_HEIGHT);
}

void dxfwSetWindowSizeToZeroHeightTest(void** state) {
  struct dxfwTestSingleWindowTestData* data = (struct dxfwTestSingleWindowTestData*)(*state);

  // Test const
  const uint32_t NEW_WINDOW_WIDTH = 313;
  const uint32_t NEW_WINDOW_HEIGHT = 0;

  expect_value(dxfwTestErrorCallbackMock, error, DXFW_ERROR_INVALID_WINDOW_SIZE);

  dxfwSetWindowSize(data->m_window_, NEW_WINDOW_WIDTH, NEW_WINDOW_HEIGHT);
}

void dxfwShouldWindowCloseTest(void** state) {
  struct dxfwTestSingleWindowTestData* data = (struct dxfwTestSingleWindowTestData*)(*state);

  // Test const
  const UINT MESSAGE = WM_CLOSE;
  const LPARAM LPARAM_VALUE = 0;
  const WPARAM WPARAM_VALUE = 0;

  // Tets if we should close
  assert_false(dxfwShouldWindowClose(data->m_window_));

  // Set callback
  assert_null(dxfwSetShouldCloseChangedCallback(data->m_window_, dxfwTestOnShouldCloseCallbackMock));
  expect_value(dxfwTestOnShouldCloseCallbackMock, window, data->m_window_);
  expect_value(dxfwTestOnShouldCloseCallbackMock, should_close, true);

  // Setup OS message flow
  dxfwTestSetupPeekMessage((HWND)data->m_window_id_, MESSAGE, LPARAM_VALUE, WPARAM_VALUE);
  dxfwSetupDefWindowProc((HWND)data->m_window_id_, MESSAGE, LPARAM_VALUE, WPARAM_VALUE);
  dxfwTestSetupLastPeekMessage();

  // Trigger
  dxfwPollOsEvents();

  // Tets if we should close
  assert_true(dxfwShouldWindowClose(data->m_window_));
}

void dxfwPollOsEventsTest(void** state) {
  struct dxfwTestSingleWindowTestData* data = (struct dxfwTestSingleWindowTestData*)(*state);

  // Test const
  const UINT MESSAGE = WM_CLOSE;
  const LPARAM LPARAM_VALUE = 0;
  const WPARAM WPARAM_VALUE = 0;

  // Setup OS message flow
  dxfwTestSetupPeekMessage((HWND)data->m_window_id_, MESSAGE, LPARAM_VALUE, WPARAM_VALUE);
  dxfwSetupDefWindowProc((HWND)data->m_window_id_, MESSAGE, LPARAM_VALUE, WPARAM_VALUE);
  dxfwTestSetupLastPeekMessage();

  // Trigger
  dxfwPollOsEvents();
}

void dxfwTwoWindowTest(void** state) {
  DXFW_TEST_UNUSED(state);

  // Test const
  const uintptr_t WINDOW_1_ID = 16;
  const uintptr_t WINDOW_2_ID = 17;
  const uint32_t WINDOW_1_WIDTH = 116;
  const uint32_t WINDOW_1_HEIGHT = 216;
  const uint32_t WINDOW_2_WIDTH = 117;
  const uint32_t WINDOW_2_HEIGHT = 217;
  const char* WINDOW_NAME = "dxfwTwoWindowTest";

  const UINT MESSAGE = WM_CLOSE;
  const LPARAM LPARAM_VALUE = 0;
  const WPARAM WPARAM_VALUE = 0;

  // Create
  wchar_t* title_wide = dxfwTestUtf8ToWchar(WINDOW_NAME);

  dxfwSetupWindowCreateExpectations(WINDOW_1_ID, WINDOW_1_WIDTH, WINDOW_1_HEIGHT, title_wide);
  struct dxfwWindow* w1 = dxfwCreateWindow(WINDOW_1_WIDTH, WINDOW_1_HEIGHT, WINDOW_NAME);
  assert_null(dxfwSetShouldCloseChangedCallback(w1, dxfwTestOnShouldCloseCallbackMock));

  dxfwSetupWindowCreateExpectations(WINDOW_2_ID, WINDOW_2_WIDTH, WINDOW_2_HEIGHT, title_wide);
  struct dxfwWindow* w2 = dxfwCreateWindow(WINDOW_2_WIDTH, WINDOW_2_HEIGHT, WINDOW_NAME);
  assert_null(dxfwSetShouldCloseChangedCallback(w2, dxfwTestOnShouldCloseCallbackMock));

  // Tets if we should close
  assert_false(dxfwShouldWindowClose(w1));
  assert_false(dxfwShouldWindowClose(w2));

  // Setup OS message flow
  dxfwTestSetupPeekMessage((HWND)WINDOW_1_ID, MESSAGE, LPARAM_VALUE, WPARAM_VALUE);
  dxfwSetupDefWindowProc((HWND)WINDOW_1_ID, MESSAGE, LPARAM_VALUE, WPARAM_VALUE);
  dxfwTestSetupLastPeekMessage();

  // Trigger
  expect_value(dxfwTestOnShouldCloseCallbackMock, window, w1);
  expect_value(dxfwTestOnShouldCloseCallbackMock, should_close, true);
  dxfwPollOsEvents();

  // Tets if we should close
  assert_true(dxfwShouldWindowClose(w1));
  assert_false(dxfwShouldWindowClose(w2));

  // Setup OS message flow
  dxfwTestSetupPeekMessage((HWND)WINDOW_2_ID, MESSAGE, LPARAM_VALUE, WPARAM_VALUE);
  dxfwSetupDefWindowProc((HWND)WINDOW_2_ID, MESSAGE, LPARAM_VALUE, WPARAM_VALUE);
  dxfwTestSetupLastPeekMessage();

  // Trigger
  expect_value(dxfwTestOnShouldCloseCallbackMock, window, w2);
  expect_value(dxfwTestOnShouldCloseCallbackMock, should_close, true);
  dxfwPollOsEvents();

  // Tets if we should close
  assert_true(dxfwShouldWindowClose(w1));
  assert_true(dxfwShouldWindowClose(w2));

  // Destroy
  dxfwTestSetupWindowDestroyExpectations(WINDOW_1_ID);
  dxfwDestroyWindow(w1);
  dxfwTestSetupWindowDestroyExpectations(WINDOW_2_ID);
  dxfwDestroyWindow(w2);

  // Clean up
  free(title_wide);
}
