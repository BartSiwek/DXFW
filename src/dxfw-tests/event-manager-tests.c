#include "dxfw-tests.h"

/* HLPERS */
void dxfwTestFireMouseButton(int window_id, dxfwMouseButton button, dxfwMouseButtonAction action, int16_t x, int16_t y) {
  UINT msg = 0;
  if (button == DXFW_LEFT_MOUSE_BUTTON && action == DXFW_MOUSE_BUTTON_DOWN) {
    msg = WM_LBUTTONDOWN;
  } else if (button == DXFW_LEFT_MOUSE_BUTTON && action == DXFW_MOUSE_BUTTON_UP) {
    msg = WM_LBUTTONUP;
  } else if (button == DXFW_RIGHT_MOUSE_BUTTON && action == DXFW_MOUSE_BUTTON_DOWN) {
    msg = WM_RBUTTONDOWN;
  } else if (button == DXFW_RIGHT_MOUSE_BUTTON && action == DXFW_MOUSE_BUTTON_UP) {
    msg = WM_RBUTTONUP;
  } else if (button == DXFW_MIDDLE_MOUSE_BUTTON && action == DXFW_MOUSE_BUTTON_DOWN) {
    msg = WM_MBUTTONDOWN;
  } else if (button == DXFW_MIDDLE_MOUSE_BUTTON && action == DXFW_MOUSE_BUTTON_UP) {
    msg = WM_MBUTTONUP;
  } else {
    assert_true(false);
  }

  LPARAM lparam = (((LPARAM)x) & 0xffff) | (((LPARAM)y) << 16);

  dxfwTestSetupPeekMessage((HWND)window_id, msg, lparam, 0);
  dxfwSetupDefWindowProc((HWND)window_id, msg, lparam, 0);

  dxfwTestSetupLastPeekMessage();

  dxfwPollOsEvents();
}

void dxfwTestRunMouseButtonTest(struct dxfwTestSingleWindowTestData* data, dxfwMouseButton button, dxfwMouseButtonAction action, int16_t x, int16_t y) {
  expect_value(dxfwTestOnMouseButtonCallbackMock, window, data->m_window_);
  expect_value(dxfwTestOnMouseButtonCallbackMock, button, button);
  expect_value(dxfwTestOnMouseButtonCallbackMock, action, action);
  expect_value(dxfwTestOnMouseButtonCallbackMock, x, x);
  expect_value(dxfwTestOnMouseButtonCallbackMock, y, y);
  dxfwTestFireMouseButton(data->m_window_id_, button, action, x, y);
}

void dxfwTestFireMouseMove(int window_id, int16_t x, int16_t y) {
  LPARAM lparam = (((LPARAM)x) & 0xffff) | (((LPARAM)y) << 16);

  dxfwTestSetupPeekMessage((HWND)window_id, WM_MOUSEMOVE, lparam, 0);
  dxfwSetupDefWindowProc((HWND)window_id, WM_MOUSEMOVE, lparam, 0);

  dxfwTestSetupLastPeekMessage();

  dxfwPollOsEvents();
}

void dxfwTestRunMouseMoveTest(struct dxfwTestSingleWindowTestData* data, int16_t x, int16_t y) {
  expect_value(dxfwTestOnMouseMoveCallbackMock, window, data->m_window_);
  expect_value(dxfwTestOnMouseMoveCallbackMock, x, x);
  expect_value(dxfwTestOnMouseMoveCallbackMock, y, y);
  dxfwTestFireMouseMove(data->m_window_id_, x, y);
}

void dxfwTestFireMouseWheel(int window_id, int16_t x, int16_t y, int16_t delta) {
  LPARAM lparam = (((LPARAM)x) & 0xffff) | (((LPARAM)y) << 16);
  WPARAM wparam = (((LPARAM)delta) << 16) | 0x0000;

  dxfwTestSetupPeekMessage((HWND)window_id, WM_MOUSEWHEEL, lparam, wparam);
  dxfwSetupDefWindowProc((HWND)window_id, WM_MOUSEWHEEL, lparam, wparam);

  dxfwTestSetupLastPeekMessage();

  dxfwPollOsEvents();
}

void dxfwTestRunMouseWheelTest(struct dxfwTestSingleWindowTestData* data, int16_t x, int16_t y, int16_t delta) {
  expect_value(dxfwTestOnMouseWheelCallbackMock, window, data->m_window_);
  expect_value(dxfwTestOnMouseWheelCallbackMock, x, x);
  expect_value(dxfwTestOnMouseWheelCallbackMock, y, y);
  expect_value(dxfwTestOnMouseWheelCallbackMock, delta, delta);
  dxfwTestFireMouseWheel(data->m_window_id_, x, y, delta);
}

/* MOCKS */
void dxfwTestOnMouseButtonCallbackMock(struct dxfwWindow* window, dxfwMouseButton button, dxfwMouseButtonAction action, int16_t x, int16_t y) {
  check_expected(window);
  check_expected(button);
  check_expected(action);
  check_expected(x);
  check_expected(y);
}

void dxfwTestOnMouseMoveCallbackMock(struct dxfwWindow* window, int16_t x, int16_t y) {
  check_expected(window);
  check_expected(x);
  check_expected(y);
}

void dxfwTestOnMouseWheelCallbackMock(struct dxfwWindow* window, int16_t x, int16_t y, int16_t delta) {
  check_expected(window);
  check_expected(x);
  check_expected(y);
  check_expected(delta);
}

/* TESTS */
void dxfwSetMouseButtonCallbackTest(void** state) {
  struct dxfwTestSingleWindowTestData* data = (struct dxfwTestSingleWindowTestData*)(*state);

  // Set callback
  assert_null(dxfwSetMouseButtonCallback(data->m_window_, dxfwTestOnMouseButtonCallbackMock));

  // Test
  dxfwTestRunMouseButtonTest(data, DXFW_LEFT_MOUSE_BUTTON, DXFW_MOUSE_BUTTON_DOWN, 701, 501);
  dxfwTestRunMouseButtonTest(data, DXFW_LEFT_MOUSE_BUTTON, DXFW_MOUSE_BUTTON_UP, -702, 502);
  dxfwTestRunMouseButtonTest(data, DXFW_RIGHT_MOUSE_BUTTON, DXFW_MOUSE_BUTTON_DOWN, 703, 503);
  dxfwTestRunMouseButtonTest(data, DXFW_RIGHT_MOUSE_BUTTON, DXFW_MOUSE_BUTTON_UP, 704, -504);
  dxfwTestRunMouseButtonTest(data, DXFW_MIDDLE_MOUSE_BUTTON, DXFW_MOUSE_BUTTON_DOWN, 705, 505);
  dxfwTestRunMouseButtonTest(data, DXFW_MIDDLE_MOUSE_BUTTON, DXFW_MOUSE_BUTTON_UP, -706, -506);
}

void dxfwSetMouseMoveCallbackTest(void** state) {
  struct dxfwTestSingleWindowTestData* data = (struct dxfwTestSingleWindowTestData*)(*state);

  // Set callback
  assert_null(dxfwSetMouseMoveCallback(data->m_window_, dxfwTestOnMouseMoveCallbackMock));

  // Test
  dxfwTestRunMouseMoveTest(data, 101, 201);
  dxfwTestRunMouseMoveTest(data, -5, 202);
  dxfwTestRunMouseMoveTest(data, 103, -10);
  dxfwTestRunMouseMoveTest(data, -50, -20);
}

void dxfwSetMouseWheelCallbackTest(void** state) {
  struct dxfwTestSingleWindowTestData* data = (struct dxfwTestSingleWindowTestData*)(*state);

  // Set callback
  assert_null(dxfwSetMouseWheelCallback(data->m_window_, dxfwTestOnMouseWheelCallbackMock));

  // Test
  dxfwTestRunMouseWheelTest(data, 101, 201, 301);
  dxfwTestRunMouseWheelTest(data, -5, 202, 302);
  dxfwTestRunMouseWheelTest(data, 103, -10, 303);
  dxfwTestRunMouseWheelTest(data, 104, 204, -15);
  dxfwTestRunMouseWheelTest(data, -20, -25, 305);
  dxfwTestRunMouseWheelTest(data, -30, 206, -35);
  dxfwTestRunMouseWheelTest(data, 107, -40, -45);
  dxfwTestRunMouseWheelTest(data, -50, -55, -60);
}


