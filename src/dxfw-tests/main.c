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
    cmocka_unit_test_setup_teardown(dxfwSetWindowCaptionTest, dxfwTestSetup, dxfwTestTeardown),
    cmocka_unit_test_setup_teardown(dxfwSetEmptyWindowCaptionTest, dxfwTestSetup, dxfwTestTeardown),
    cmocka_unit_test_setup_teardown(dxfwGetWindowSizeTest, dxfwTestSetup, dxfwTestTeardown),
    cmocka_unit_test_setup_teardown(dxfwSetWindowSizeTest, dxfwTestSetup, dxfwTestTeardown),
    cmocka_unit_test_setup_teardown(dxfwSetWindowSizeToZeroWidthTest, dxfwTestSetup, dxfwTestTeardown),
    cmocka_unit_test_setup_teardown(dxfwSetWindowSizeToZeroHeightTest, dxfwTestSetup, dxfwTestTeardown),
    cmocka_unit_test_setup_teardown(dxfwShouldWindowCloseTest, dxfwTestSetup, dxfwTestTeardown),
    cmocka_unit_test_setup_teardown(dxfwPollOsEventsTest, dxfwTestSetup, dxfwTestTeardown),
  };

  return cmocka_run_group_tests(tests, NULL, NULL);
}
