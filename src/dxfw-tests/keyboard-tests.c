#include "dxfw-tests.h"

/* HELPERS */
void dxfwTestFireKeyDown(uintptr_t window_id, USHORT windows_key_code) {
  const LPARAM LPARAM_VALUE = 7;

  dxfwTestSetupPeekMessage((HWND)window_id, WM_INPUT, LPARAM_VALUE, 0);

  expect_value(GetRawInputData, hRawInput, (HRAWINPUT)LPARAM_VALUE);
  expect_value(GetRawInputData, uiCommand, RID_INPUT);
  will_return(GetRawInputData, RIM_TYPEKEYBOARD);
  will_return(GetRawInputData, windows_key_code);
  will_return(GetRawInputData, 0);  // Down
  expect_value(GetRawInputData, cbSizeHeader, sizeof(RAWINPUTHEADER));
  will_return(GetRawInputData, 0);

  will_return(GetActiveWindow, (HWND)window_id);

  dxfwTestSetupLastPeekMessage();

  dxfwPollOsEvents();
}

void dxfwTestFireKeyUp(uintptr_t window_id, USHORT windows_key_code) {
  const LPARAM LPARAM_VALUE = 11;

  dxfwTestSetupPeekMessage((HWND)window_id, WM_INPUT, LPARAM_VALUE, 0);

  expect_value(GetRawInputData, hRawInput, (HRAWINPUT)LPARAM_VALUE);
  expect_value(GetRawInputData, uiCommand, RID_INPUT);
  will_return(GetRawInputData, RIM_TYPEKEYBOARD);
  will_return(GetRawInputData, windows_key_code);
  will_return(GetRawInputData, RI_KEY_BREAK);  // Up
  expect_value(GetRawInputData, cbSizeHeader, sizeof(RAWINPUTHEADER));
  will_return(GetRawInputData, 0);

  will_return(GetActiveWindow, (HWND)window_id);

  dxfwTestSetupLastPeekMessage();

  dxfwPollOsEvents();
}

/* MOCKS */
void dxfwTestOnKeyboardCallbackMock(struct dxfwWindow* window, dxfwVirtualKeyCode key_code, dxfwVirtualKeyModifiers modifiers, dxfwVirtualKeyState state, dxfwVirtualKeyState prev_state) {
  check_expected(window);
  check_expected(key_code);
  check_expected(modifiers);
  check_expected(state);
  check_expected(prev_state);
}

/* TESTS */
void dxfwGetKeyStateTest(void** state) {
  struct dxfwTestSingleWindowTestData* data = (struct dxfwTestSingleWindowTestData*)(*state);

  // Consts
  const dxfwVirtualKeyCode KEY = DXFW_KEY_D;
  const USHORT WINDOWS_KEY = 0x44;  // D key

  // Test
  assert_int_equal(DXFW_KEY_STATE_UP, dxfwGetKeyState(KEY));
  assert_int_equal(DXFW_KEY_STATE_UP, dxfwGetPreviousKeyState(KEY));

  dxfwTestFireKeyDown(data->m_window_id_, WINDOWS_KEY);

  assert_int_equal(DXFW_KEY_STATE_DOWN, dxfwGetKeyState(KEY));
  assert_int_equal(DXFW_KEY_STATE_UP, dxfwGetPreviousKeyState(KEY));

  dxfwTestFireKeyUp(data->m_window_id_, WINDOWS_KEY);

  assert_int_equal(DXFW_KEY_STATE_UP, dxfwGetKeyState(KEY));
  assert_int_equal(DXFW_KEY_STATE_DOWN, dxfwGetPreviousKeyState(KEY));

  dxfwTestFireKeyUp(data->m_window_id_, WINDOWS_KEY);

  assert_int_equal(DXFW_KEY_STATE_UP, dxfwGetKeyState(KEY));
  assert_int_equal(DXFW_KEY_STATE_UP, dxfwGetPreviousKeyState(KEY));
}

void dxfwGetModifierFlagsTest(void** state) {
  struct dxfwTestSingleWindowTestData* data = (struct dxfwTestSingleWindowTestData*)(*state);

  // Consts
  const USHORT CONTROL_WINDOWS_KEY = VK_LCONTROL;
  const USHORT SHIFT_WINDOWS_KEY = VK_RSHIFT;

  // Test
  assert_int_equal(DXFW_KEY_MODIFIER_NONE, dxfwGetModifierFlags());

  dxfwTestFireKeyDown(data->m_window_id_, CONTROL_WINDOWS_KEY);

  assert_int_equal(DXFW_KEY_MODIFIER_CTRL, dxfwGetModifierFlags());

  dxfwTestFireKeyDown(data->m_window_id_, SHIFT_WINDOWS_KEY);

  assert_int_equal(DXFW_KEY_MODIFIER_CTRL | DXFW_KEY_MODIFIER_SHIFT, dxfwGetModifierFlags());

  dxfwTestFireKeyUp(data->m_window_id_, CONTROL_WINDOWS_KEY);

  assert_int_equal(DXFW_KEY_MODIFIER_SHIFT, dxfwGetModifierFlags());

  dxfwTestFireKeyUp(data->m_window_id_, SHIFT_WINDOWS_KEY);

  assert_int_equal(DXFW_KEY_MODIFIER_NONE, dxfwGetModifierFlags());
}

void dxfwKeyboardCallbackTest(void** state) {
  struct dxfwTestSingleWindowTestData* data = (struct dxfwTestSingleWindowTestData*)(*state);

  // Consts
  const dxfwVirtualKeyCode CONTROL_KEY = DXFW_KEY_LCTRL;
  const USHORT CONTROL_WINDOWS_KEY = VK_LCONTROL;
  const dxfwVirtualKeyCode D_KEY = DXFW_KEY_D;
  const USHORT D_WINDOWS_KEY = 0x44;  // D key

  // Set callback
  assert_null(dxfwSetKeyboardCallback(data->m_window_, dxfwTestOnKeyboardCallbackMock));

  // Test
  expect_value(dxfwTestOnKeyboardCallbackMock, window, data->m_window_);
  expect_value(dxfwTestOnKeyboardCallbackMock, key_code, CONTROL_KEY);
  expect_value(dxfwTestOnKeyboardCallbackMock, modifiers, DXFW_KEY_MODIFIER_CTRL);  // Modifiers are registered immidiatelly
  expect_value(dxfwTestOnKeyboardCallbackMock, state, DXFW_KEY_STATE_DOWN);
  expect_value(dxfwTestOnKeyboardCallbackMock, prev_state, DXFW_KEY_STATE_UP);
  dxfwTestFireKeyDown(data->m_window_id_, CONTROL_WINDOWS_KEY);

  expect_value(dxfwTestOnKeyboardCallbackMock, window, data->m_window_);
  expect_value(dxfwTestOnKeyboardCallbackMock, key_code, D_KEY);
  expect_value(dxfwTestOnKeyboardCallbackMock, modifiers, DXFW_KEY_MODIFIER_CTRL);
  expect_value(dxfwTestOnKeyboardCallbackMock, state, DXFW_KEY_STATE_DOWN);
  expect_value(dxfwTestOnKeyboardCallbackMock, prev_state, DXFW_KEY_STATE_UP);
  dxfwTestFireKeyDown(data->m_window_id_, D_WINDOWS_KEY);
  
  expect_value(dxfwTestOnKeyboardCallbackMock, window, data->m_window_);
  expect_value(dxfwTestOnKeyboardCallbackMock, key_code, CONTROL_KEY);
  expect_value(dxfwTestOnKeyboardCallbackMock, modifiers, DXFW_KEY_MODIFIER_NONE);  // Modifiers are registered immidiatelly
  expect_value(dxfwTestOnKeyboardCallbackMock, state, DXFW_KEY_STATE_UP);
  expect_value(dxfwTestOnKeyboardCallbackMock, prev_state, DXFW_KEY_STATE_DOWN);
  dxfwTestFireKeyUp(data->m_window_id_, CONTROL_WINDOWS_KEY);

  expect_value(dxfwTestOnKeyboardCallbackMock, window, data->m_window_);
  expect_value(dxfwTestOnKeyboardCallbackMock, key_code, D_KEY);
  expect_value(dxfwTestOnKeyboardCallbackMock, modifiers, DXFW_KEY_MODIFIER_NONE);
  expect_value(dxfwTestOnKeyboardCallbackMock, state, DXFW_KEY_STATE_UP);
  expect_value(dxfwTestOnKeyboardCallbackMock, prev_state, DXFW_KEY_STATE_DOWN);
  dxfwTestFireKeyUp(data->m_window_id_, D_WINDOWS_KEY);
}
