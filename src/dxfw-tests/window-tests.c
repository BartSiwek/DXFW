#include "dxfw-tests.h"

/* HELPERS */
void dxfwTestsRunSuccessfulWindowCreationTest(int id, uint32_t width, uint32_t height, const char* title) {
  wchar_t* title_wide = dxfwTestsUtf8ToWchar(title);

  // Create
  RECT r = { 0, 0, width, height };
  expect_value(AdjustWindowRect, lpRect->left, 0);
  expect_value(AdjustWindowRect, lpRect->top, 0);
  expect_value(AdjustWindowRect, lpRect->right, width);
  expect_value(AdjustWindowRect, lpRect->bottom, height);
  expect_value(AdjustWindowRect, dwStyle, WS_OVERLAPPEDWINDOW | WS_VISIBLE);
  expect_value(AdjustWindowRect, bMenu, FALSE);
  will_return(AdjustWindowRect, &r);
  will_return(AdjustWindowRect, TRUE);

  expect_string(CreateWindowExW, lpWindowName, title_wide);
  expect_value(CreateWindowExW, x, CW_USEDEFAULT);
  expect_value(CreateWindowExW, y, CW_USEDEFAULT);
  expect_value(CreateWindowExW, nWidth, width);
  expect_value(CreateWindowExW, nHeight, height);
  will_return(CreateWindowExW, (HWND)id);

  dxfwSetupAnyWmCreateExpectations();

  expect_value(ShowWindow, hWnd, (HWND)id);
  expect_value(ShowWindow, nCmdShow, SW_SHOWNORMAL);
  will_return(ShowWindow, TRUE);

  expect_value(UpdateWindow, hWnd, (HWND)id);
  will_return(UpdateWindow, TRUE);

  struct dxfwWindow* w = dxfwCreateWindow(width, height, title);

  // Destroy
  expect_value(DestroyWindow, hWnd, (HWND)id);
  will_return(DestroyWindow, TRUE);

  dxfwDestroyWindow(w);

  // Clean up
  free(title_wide);
}

/* TESTS */
void dxfwCreateDestroyWindowTest(void** state) {
  // Test const
  const int WINDOW_ID = 2;
  const uint32_t WINDOW_WIDTH = 102;
  const uint32_t WINDOW_HEIGHT = 202;
  const char* WINDOW_NAME = "dxfwCreateDestroyWindowTest";

  dxfwTestsRunSuccessfulWindowCreationTest(WINDOW_ID, WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_NAME);
}

void dxfwCreateWindowWithZeroWidthTest(void** state) {
  const uint32_t WINDOW_WIDTH = 0;
  const uint32_t WINDOW_HEIGHT = 203;
  const char* WINDOW_NAME = "dxfwCreateWindowWithZeroWidthTest";

  expect_value(dxfwTestErrorCallbackMock, error, DXFW_ERROR_INVALID_WINDOW_SIZE);

  struct dxfwWindow* w = dxfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_NAME);
  assert_null(w);
}


void dxfwCreateWindowWithZeroHeightTest(void** state) {
  const uint32_t WINDOW_WIDTH = 105;
  const uint32_t WINDOW_HEIGHT = 0;
  const char* WINDOW_NAME = "dxfwCreateWindowWithZeroHeightTest";

  expect_value(dxfwTestErrorCallbackMock, error, DXFW_ERROR_INVALID_WINDOW_SIZE);

  struct dxfwWindow* w = dxfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_NAME);
  assert_null(w);
}

void dxfwCreateWindowWithEmptyTitleTest(void** state) {
  // Test const
  const int WINDOW_ID = 7;
  const uint32_t WINDOW_WIDTH = 107;
  const uint32_t WINDOW_HEIGHT = 207;
  const char* WINDOW_NAME = "";

  dxfwTestsRunSuccessfulWindowCreationTest(WINDOW_ID, WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_NAME);
}

void RunSetCaptionTest(int id, uint32_t width, uint32_t height, const char* title, const char* new_title) {
  dxfwSetupAnyWindowCreateExpectations(id);
  struct dxfwWindow* w = dxfwCreateWindow(width, height, title);

  wchar_t* new_title_wide = dxfwTestsUtf8ToWchar(new_title);
  expect_value(SetWindowTextW, hWnd, (HWND)id);
  expect_string(SetWindowTextW, lpString, new_title_wide);
  will_return(SetWindowTextW, TRUE);
  dxfwSetWindowCaption(w, new_title);

  dxfwSetupAnyWindowDestroyExpectations(id);
  dxfwDestroyWindow(w);

  free(new_title_wide);
}

void dxfwSetWindowCaptionTest(void** state) {
  // Test const
  const int WINDOW_ID = 8;
  const uint32_t WINDOW_WIDTH = 108;
  const uint32_t WINDOW_HEIGHT = 208;
  const char* WINDOW_NAME = "dxfwSetWindowCaptionTest";
  const char* WINDOW_NAME_NEW = "dxfwSetWindowCaptionTestNew";
  
  RunSetCaptionTest(WINDOW_ID, WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_NAME, WINDOW_NAME_NEW);
}

void dxfwSetEmptyWindowCaptionTest(void** state) {
  // Test const
  const int WINDOW_ID = 9;
  const uint32_t WINDOW_WIDTH = 109;
  const uint32_t WINDOW_HEIGHT = 209;
  const char* WINDOW_NAME = "dxfwSetEmptyWindowCaptionTest";
  const char* WINDOW_NAME_NEW = "";

  RunSetCaptionTest(WINDOW_ID, WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_NAME, WINDOW_NAME_NEW);
}

// void dxfwGetWindowSize(struct dxfwWindow* window, uint32_t* width, uint32_t* height);
// void dxfwSetWindowSize(struct dxfwWindow* window, uint32_t width, uint32_t height);

// void dxfwPollOsEvents();
// bool dxfwShouldWindowClose(struct dxfwWindow* window);
// dxfw_on_should_close_changed dxfwSetShouldCloseChangedCallback(struct dxfwWindow* window, dxfw_on_should_close_changed callback);
