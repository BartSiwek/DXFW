#include "dxfw-tests.h"

int main() {
  const struct CMUnitTest tests[] = {
    cmocka_unit_test(dxfwInitTest),
    cmocka_unit_test(dxfwDoubleInitTest),
    cmocka_unit_test(dxfwNoInitTest),
    cmocka_unit_test(dxfwMemoryTest),
    cmocka_unit_test(dxfwSetAllocNullArguments),
    cmocka_unit_test_setup_teardown(dxfwNotInitializedTest, dxfwNotInitializedTestSetup, dxfwNotInitializedTestTeardown),
    cmocka_unit_test_setup_teardown(dxfwGetTimeTest, dxfwInitializedTestSetup, dxfwInitializedTestTeardown),
    cmocka_unit_test_setup_teardown(dxfwCreateDestroyWindowTest, dxfwInitializedTestSetup, dxfwInitializedTestTeardown),
    cmocka_unit_test_setup_teardown(dxfwCreateWindowWithZeroWidthTest, dxfwInitializedTestSetup, dxfwInitializedTestTeardown),
    cmocka_unit_test_setup_teardown(dxfwCreateWindowWithZeroHeightTest, dxfwInitializedTestSetup, dxfwInitializedTestTeardown),
    cmocka_unit_test_setup_teardown(dxfwCreateWindowWithEmptyTitleTest, dxfwInitializedTestSetup, dxfwInitializedTestTeardown),
    cmocka_unit_test_setup_teardown(dxfwCreateWindowWithNullTitleTest, dxfwInitializedTestSetup, dxfwInitializedTestTeardown),
    cmocka_unit_test_setup_teardown(dxfwGetHandleTest, dxfwSingleWindowTestSetup, dxfwSingleWindowTestTeardown),
    cmocka_unit_test_setup_teardown(dxfwSetAndGetUserDataTest, dxfwSingleWindowTestSetup, dxfwSingleWindowTestTeardown),
    cmocka_unit_test_setup_teardown(dxfwSetWindowCaptionTest, dxfwInitializedTestSetup, dxfwInitializedTestTeardown),
    cmocka_unit_test_setup_teardown(dxfwSetEmptyWindowCaptionTest, dxfwInitializedTestSetup, dxfwInitializedTestTeardown),
    cmocka_unit_test_setup_teardown(dxfwGetWindowSizeTest, dxfwSingleWindowTestSetup, dxfwSingleWindowTestTeardown),
    cmocka_unit_test_setup_teardown(dxfwSetWindowSizeTest, dxfwSingleWindowTestSetup, dxfwSingleWindowTestTeardown),
    cmocka_unit_test_setup_teardown(dxfwSetWindowSizeToZeroWidthTest, dxfwSingleWindowTestSetup, dxfwSingleWindowTestTeardown),
    cmocka_unit_test_setup_teardown(dxfwSetWindowSizeToZeroHeightTest, dxfwSingleWindowTestSetup, dxfwSingleWindowTestTeardown),
    cmocka_unit_test_setup_teardown(dxfwShouldWindowCloseTest, dxfwSingleWindowTestSetup, dxfwSingleWindowTestTeardown),
    cmocka_unit_test_setup_teardown(dxfwPollOsEventsTest, dxfwSingleWindowTestSetup, dxfwSingleWindowTestTeardown),
    cmocka_unit_test_setup_teardown(dxfwTwoWindowTest, dxfwInitializedTestSetup, dxfwInitializedTestTeardown),
    cmocka_unit_test_setup_teardown(dxfwGetKeyStateTest, dxfwSingleWindowTestSetup, dxfwSingleWindowTestTeardown),
    cmocka_unit_test_setup_teardown(dxfwGetModifierFlagsTest, dxfwSingleWindowTestSetup, dxfwSingleWindowTestTeardown),
    cmocka_unit_test_setup_teardown(dxfwKeyboardCallbackTest, dxfwSingleWindowTestSetup, dxfwSingleWindowTestTeardown),
    cmocka_unit_test_setup_teardown(dxfwSetMouseButtonCallbackTest, dxfwSingleWindowTestSetup, dxfwSingleWindowTestTeardown),
    cmocka_unit_test_setup_teardown(dxfwSetMouseMoveCallbackTest, dxfwSingleWindowTestSetup, dxfwSingleWindowTestTeardown),
    cmocka_unit_test_setup_teardown(dxfwSetMouseWheelCallbackTest, dxfwSingleWindowTestSetup, dxfwSingleWindowTestTeardown),
  };

  return cmocka_run_group_tests(tests, NULL, NULL);
}

/*
NULL tests TODO:
void dxfwDestroyWindow(struct dxfwWindow* window);  // NUll window
HWND dxfwGetHandle(struct dxfwWindow* window);  // NUll window
void dxfwSetWindowUserData(struct dxfwWindow* window, void* data);  // NUll window
void* dxfwGetWindowUserData(struct dxfwWindow* window);  // NUll window
void dxfwSetWindowCaption(struct dxfwWindow* window, const char* caption);  // NUll window, caption
void dxfwGetWindowSize(struct dxfwWindow* window, uint32_t* width, uint32_t* height);   // NUll window, width, height
void dxfwSetWindowSize(struct dxfwWindow* window, uint32_t width, uint32_t height);   // NUll window
bool dxfwShouldWindowClose(struct dxfwWindow* window);  // NUll window
dxfw_on_should_close_changed dxfwSetShouldCloseChangedCallback(struct dxfwWindow* window, dxfw_on_should_close_changed callback);  // NUll window
dxfw_on_mouse_button dxfwSetMouseButtonCallback(struct dxfwWindow* window, dxfw_on_mouse_button callback);  // NUll window
dxfw_on_mouse_move dxfwSetMouseMoveCallback(struct dxfwWindow* window, dxfw_on_mouse_move callback);  // NUll window
dxfw_on_mouse_wheel dxfwSetMouseWheelCallback(struct dxfwWindow* window, dxfw_on_mouse_wheel callback);  // NUll window
dxfw_on_keyboard dxfwSetKeyboardCallback(struct dxfwWindow* window, dxfw_on_keyboard callback);  // NUll window
*/