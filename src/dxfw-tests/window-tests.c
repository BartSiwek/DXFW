#include "dxfw-tests.h"

void dxfwCreateDestroyWindowTest(void** state) {
  // Test const
  const int WINDOW_ID = 2;
  const int WINDOW_WIDTH = 102;
  const int WINDOW_HEIGHT = 202;
  const char* WINDOW_NAME = "dxfwCreateDestroyWindowTest";
  wchar_t* WINDOW_NAME_WIDE = dxfwTestsUtf8ToWchar(WINDOW_NAME);

  // Create
  RECT r = { 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT };
  expect_value(AdjustWindowRect, lpRect->left, 0);
  expect_value(AdjustWindowRect, lpRect->top, 0);
  expect_value(AdjustWindowRect, lpRect->right, WINDOW_WIDTH);
  expect_value(AdjustWindowRect, lpRect->bottom, WINDOW_HEIGHT);
  expect_value(AdjustWindowRect, dwStyle, WS_OVERLAPPEDWINDOW | WS_VISIBLE);
  expect_value(AdjustWindowRect, bMenu, FALSE);
  will_return(AdjustWindowRect, &r);
  will_return(AdjustWindowRect, TRUE);

  expect_string(CreateWindowExW, lpWindowName, WINDOW_NAME_WIDE);
  expect_value(CreateWindowExW, x, CW_USEDEFAULT);
  expect_value(CreateWindowExW, y, CW_USEDEFAULT);
  expect_value(CreateWindowExW, nWidth, WINDOW_WIDTH);
  expect_value(CreateWindowExW, nHeight, WINDOW_HEIGHT);
  will_return(CreateWindowExW, (HWND)WINDOW_ID);

  dxfwSetupAnyWmCreateExpectations();

  expect_value(ShowWindow, hWnd, (HWND)WINDOW_ID);
  expect_value(ShowWindow, nCmdShow, SW_SHOWNORMAL);
  will_return(ShowWindow, TRUE);

  expect_value(UpdateWindow, hWnd, (HWND)WINDOW_ID);
  will_return(UpdateWindow, TRUE);

  struct dxfwWindow* w = dxfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_NAME);

  // Destroy
  expect_value(DestroyWindow, hWnd, (HWND)WINDOW_ID);
  will_return(DestroyWindow, TRUE);

  dxfwDestroyWindow(w);

  // Clean up
  free(WINDOW_NAME_WIDE);
}

// void dxfwSetWindowCaption(struct dxfwWindow* window, const char* caption);
// void dxfwGetWindowSize(struct dxfwWindow* window, uint32_t* width, uint32_t* height);
// void dxfwSetWindowSize(struct dxfwWindow* window, uint32_t width, uint32_t height);
// bool dxfwShouldWindowClose(struct dxfwWindow* window);
// void dxfwPollOsEvents();
// dxfw_on_should_close_changed dxfwSetShouldCloseChangedCallback(struct dxfwWindow* window, dxfw_on_should_close_changed callback);
