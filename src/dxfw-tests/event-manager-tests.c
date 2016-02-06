#include "dxfw-tests.h"

/* HLPERS */
void dxfwTestFireMouseButton(int window_id, dxfwMouseButton button, dxfwMouseButtonAction action, int32_t x, int32_t y) {
  UINT msg;
  if (button == DXFW_LEFT_MOUSE_BUTTON && action == DXFW_MOUSE_BUTTON_DOWN) {
    msg = WM_LBUTTONDOWN;
  }
  else if (button == DXFW_LEFT_MOUSE_BUTTON && action == DXFW_MOUSE_BUTTON_UP) {
    msg = WM_LBUTTONUP;
  }
  else if (button == DXFW_RIGHT_MOUSE_BUTTON && action == DXFW_MOUSE_BUTTON_DOWN) {
    msg = WM_RBUTTONDOWN;
  }
  else if (button == DXFW_RIGHT_MOUSE_BUTTON && action == DXFW_MOUSE_BUTTON_UP) {
    msg = WM_RBUTTONUP;
  }
  else if (button == DXFW_MIDDLE_MOUSE_BUTTON && action == DXFW_MOUSE_BUTTON_DOWN) {
    msg = WM_MBUTTONDOWN;
  }
  else if (button == DXFW_MIDDLE_MOUSE_BUTTON && action == DXFW_MOUSE_BUTTON_UP) {
    msg = WM_MBUTTONUP;
  }
  else {
    assert_true(false);
  }

  LPARAM lparam = (((LPARAM)x) & 0xffff) | (((LPARAM)y) << 16);

  dxfwTestSetupPeekMessage((HWND)window_id, msg, lparam, 0);

  dxfwSetupDefWindowProc((HWND)window_id, msg, lparam, 0);

  dxfwTestSetupLastPeekMessage();

  dxfwPollOsEvents();
}

/* MOCKS */
void dxfwTestOnMouseButtonCallbackMock(struct dxfwWindow* window, dxfwMouseButton button, dxfwMouseButtonAction action, int32_t x, int32_t y) {
  check_expected(window);
  check_expected(button);
  check_expected(action);
  check_expected(x);
  check_expected(y);
}

/* TESTS */
void dxfwSetMouseButtonCallbackTest(void** state) {
  struct dxfwTestSingleWindowTestData* data = (struct dxfwTestSingleWindowTestData*)(*state);

  // Consts
  const dxfwMouseButton BUTTON = DXFW_LEFT_MOUSE_BUTTON;
  const dxfwMouseButtonAction ACTION = DXFW_MOUSE_BUTTON_DOWN;
  const int32_t X = 501;
  const int32_t Y = 701;

  // Set callback
  assert_null(dxfwSetMouseButtonCallback(data->m_window_, dxfwTestOnMouseButtonCallbackMock));

  // Test
  expect_value(dxfwTestOnMouseButtonCallbackMock, window, data->m_window_);
  expect_value(dxfwTestOnMouseButtonCallbackMock, button, BUTTON);
  expect_value(dxfwTestOnMouseButtonCallbackMock, action, ACTION);
  expect_value(dxfwTestOnMouseButtonCallbackMock, x, X);
  expect_value(dxfwTestOnMouseButtonCallbackMock, y, Y);
  dxfwTestFireMouseButton(data->m_window_id_, BUTTON, ACTION, X, Y);
}

// dxfw_on_mouse_button dxfwSetMouseButtonCallback(struct dxfwWindow* window, dxfw_on_mouse_button callback);
// dxfw_on_mouse_move dxfwSetMouseMoveCallback(struct dxfwWindow* window, dxfw_on_mouse_move callback);
// dxfw_on_mouse_wheel dxfwSetMouseWheelCallback(struct dxfwWindow* window, dxfw_on_mouse_wheel callback);

