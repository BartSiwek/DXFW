#include "dxfw-tests.h"

/* HELPERS */
void dxfwSetupWindowCreateExpectations(int id, uint32_t width, uint32_t height, const wchar_t* title) {
  RECT r = { 0, 0, width, height };
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

  dxfwSetupAnyWmCreateExpectations();

  expect_value(ShowWindow, hWnd, (HWND)id);
  expect_value(ShowWindow, nCmdShow, SW_SHOWNORMAL);
  will_return(ShowWindow, TRUE);

  expect_value(UpdateWindow, hWnd, (HWND)id);
  will_return(UpdateWindow, TRUE);
}

void dxfwTestsRunSuccessfulWindowCreationTest(int id, uint32_t width, uint32_t height, const char* title) {
  // Create
  wchar_t* title_wide = dxfwTestsUtf8ToWchar(title);
  dxfwSetupWindowCreateExpectations(id, width, height, title_wide);
  struct dxfwWindow* w = dxfwCreateWindow(width, height, title);

  // Destroy
  dxfwSetupWindowDestroyExpectations(id);
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

  dxfwSetupWindowDestroyExpectations(id);
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

void dxfwGetWindowSizeTest(void** state) {
  // Test const
  const int WINDOW_ID = 10;
  const uint32_t WINDOW_WIDTH = 110;
  const uint32_t WINDOW_HEIGHT = 210;
  const char* WINDOW_NAME = "dxfwGetWindowSizeTest";

  // Create
  wchar_t* title_wide = dxfwTestsUtf8ToWchar(WINDOW_NAME);
  dxfwSetupWindowCreateExpectations(WINDOW_ID, WINDOW_WIDTH, WINDOW_HEIGHT, title_wide);
  struct dxfwWindow* w = dxfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_NAME);

  RECT client_rect = {
    .left = 510,
    .top = 610,
    .right = 710,
    .bottom = 810
  };
  expect_value(GetClientRect, hWnd, (HWND)WINDOW_ID);
  will_return(GetClientRect, client_rect.left);
  will_return(GetClientRect, client_rect.top);
  will_return(GetClientRect, client_rect.right);
  will_return(GetClientRect, client_rect.bottom);
  will_return(GetClientRect, TRUE);

  uint32_t width;
  uint32_t height;
  dxfwGetWindowSize(w, &width, &height);

  // Destroy
  dxfwSetupWindowDestroyExpectations(WINDOW_ID);
  dxfwDestroyWindow(w);

  // Clean up
  free(title_wide);
}

void dxfwSetWindowSizeTest(void** state) {
  // Test const
  const int WINDOW_ID = 11;
  const uint32_t WINDOW_WIDTH = 111;
  const uint32_t WINDOW_HEIGHT = 211;
  const char* WINDOW_NAME = "dxfwSetWindowSizeTest";
  const uint32_t NEW_WINDOW_WIDTH = 311;
  const uint32_t NEW_WINDOW_HEIGHT = 411;
  const int32_t WINDOW_STYLE = 0;

  // Create
  wchar_t* title_wide = dxfwTestsUtf8ToWchar(WINDOW_NAME);
  dxfwSetupWindowCreateExpectations(WINDOW_ID, WINDOW_WIDTH, WINDOW_HEIGHT, title_wide);
  struct dxfwWindow* w = dxfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_NAME);

  RECT client_rect = { 
    .left = 511, 
    .top = 611, 
    .right = 711, 
    .bottom = 811 
  };
  expect_value(GetClientRect, hWnd, (HWND)WINDOW_ID);
  will_return(GetClientRect, client_rect.left);
  will_return(GetClientRect, client_rect.top);
  will_return(GetClientRect, client_rect.right);
  will_return(GetClientRect, client_rect.bottom);
  will_return(GetClientRect, TRUE);

  expect_value(GetWindowLongW, hWnd, (HWND)WINDOW_ID);
  expect_value(GetWindowLongW, nIndex, GWL_STYLE);
  will_return(GetWindowLongW, (LONG)WINDOW_STYLE);
  
  RECT new_client_rect = { 
    .left = client_rect.left, 
    .top = client_rect.top, 
    .right = client_rect.left + NEW_WINDOW_WIDTH,
    .bottom = client_rect.top + NEW_WINDOW_HEIGHT
  };
  expect_value(AdjustWindowRect, lpRect->left, new_client_rect.left);
  expect_value(AdjustWindowRect, lpRect->top, new_client_rect.top);
  expect_value(AdjustWindowRect, lpRect->right, new_client_rect.right);
  expect_value(AdjustWindowRect, lpRect->bottom, new_client_rect.bottom);
  expect_value(AdjustWindowRect, dwStyle, (LONG)WINDOW_STYLE);
  expect_value(AdjustWindowRect, bMenu, FALSE);
  will_return(AdjustWindowRect, &new_client_rect);
  will_return(AdjustWindowRect, (BOOL)TRUE);

  HWND insert_after = HWND_TOPMOST;  // We need this so the predefined value can narrow correctly
  expect_value(SetWindowPos, hWnd, (HWND)WINDOW_ID);
  expect_value(SetWindowPos, hWndInsertAfter, insert_after);
  expect_value(SetWindowPos, X, new_client_rect.left);
  expect_value(SetWindowPos, Y, new_client_rect.top);
  expect_value(SetWindowPos, cx, new_client_rect.right - new_client_rect.left);
  expect_value(SetWindowPos, cy, new_client_rect.bottom - new_client_rect.top);
  expect_value(SetWindowPos, uFlags, SWP_NOCOPYBITS);
  will_return(SetWindowPos, TRUE);

  dxfwSetWindowSize(w, NEW_WINDOW_WIDTH, NEW_WINDOW_HEIGHT);

  // Destroy
  dxfwSetupWindowDestroyExpectations(WINDOW_ID);
  dxfwDestroyWindow(w);

  // Clean up
  free(title_wide);
}



// void dxfwPollOsEvents();
// bool dxfwShouldWindowClose(struct dxfwWindow* window);
// dxfw_on_should_close_changed dxfwSetShouldCloseChangedCallback(struct dxfwWindow* window, dxfw_on_should_close_changed callback);
