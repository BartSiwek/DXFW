#include "dxfw-tests.h"

int main() {
  const struct CMUnitTest tests[] = {
    cmocka_unit_test(dxfwInitTest),
    cmocka_unit_test(dxfwDoubleInitTest),
    cmocka_unit_test(dxfwNoInitTest),
    cmocka_unit_test(dxfwMemoryTest),
    cmocka_unit_test_setup_teardown(dxfwGetTimeTest, dxfwTestSetup, dxfwTestTeardown),
    cmocka_unit_test_setup_teardown(dxfwCreateDestroyWindowTest, dxfwTestSetup, dxfwTestTeardown),
    cmocka_unit_test_setup_teardown(dxfwCreateWindowWithZeroWidthTest, dxfwTestSetup, dxfwTestTeardown),
    cmocka_unit_test_setup_teardown(dxfwCreateWindowWithZeroHeightTest, dxfwTestSetup, dxfwTestTeardown),
    cmocka_unit_test_setup_teardown(dxfwCreateWindowWithEmptyTitleTest, dxfwTestSetup, dxfwTestTeardown),
    cmocka_unit_test_setup_teardown(dxfwGetHandleTest, dxfwSingleWindowTestSetup, dxfwSingleWindowTestTeardown),
    cmocka_unit_test_setup_teardown(dxfwSetWindowCaptionTest, dxfwTestSetup, dxfwTestTeardown),
    cmocka_unit_test_setup_teardown(dxfwSetEmptyWindowCaptionTest, dxfwTestSetup, dxfwTestTeardown),
    cmocka_unit_test_setup_teardown(dxfwGetWindowSizeTest, dxfwSingleWindowTestSetup, dxfwSingleWindowTestTeardown),
    cmocka_unit_test_setup_teardown(dxfwSetWindowSizeTest, dxfwSingleWindowTestSetup, dxfwSingleWindowTestTeardown),
    cmocka_unit_test_setup_teardown(dxfwSetWindowSizeToZeroWidthTest, dxfwSingleWindowTestSetup, dxfwSingleWindowTestTeardown),
    cmocka_unit_test_setup_teardown(dxfwSetWindowSizeToZeroHeightTest, dxfwSingleWindowTestSetup, dxfwSingleWindowTestTeardown),
    cmocka_unit_test_setup_teardown(dxfwShouldWindowCloseTest, dxfwSingleWindowTestSetup, dxfwSingleWindowTestTeardown),
    cmocka_unit_test_setup_teardown(dxfwPollOsEventsTest, dxfwSingleWindowTestSetup, dxfwSingleWindowTestTeardown),
    cmocka_unit_test_setup_teardown(dxfwTwoWindowTest, dxfwTestSetup, dxfwTestTeardown),
    cmocka_unit_test_setup_teardown(dxfwGetKeyStateTest, dxfwSingleWindowTestSetup, dxfwSingleWindowTestTeardown),
    cmocka_unit_test_setup_teardown(dxfwGetModifierFlagsTest, dxfwSingleWindowTestSetup, dxfwSingleWindowTestTeardown),
    cmocka_unit_test_setup_teardown(dxfwKeyboardCallbackTest, dxfwSingleWindowTestSetup, dxfwSingleWindowTestTeardown),
    cmocka_unit_test_setup_teardown(dxfwSetMouseButtonCallbackTest, dxfwSingleWindowTestSetup, dxfwSingleWindowTestTeardown),
    cmocka_unit_test_setup_teardown(dxfwSetMouseMoveCallbackTest, dxfwSingleWindowTestSetup, dxfwSingleWindowTestTeardown),
    cmocka_unit_test_setup_teardown(dxfwSetMouseWheelCallbackTest, dxfwSingleWindowTestSetup, dxfwSingleWindowTestTeardown),
  };

  return cmocka_run_group_tests(tests, NULL, NULL);
}
