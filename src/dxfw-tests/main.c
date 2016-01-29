#include "dxfw-tests.h"

int main() {
  const struct CMUnitTest tests[] = {
    cmocka_unit_test(dxfwInitTest),
    cmocka_unit_test(dxfwDoubleInitTest),
    cmocka_unit_test(dxfwNoInitTest),
    cmocka_unit_test_setup_teardown(dxfwMemoryTest, dxfwTestSetup, dxfwTestTeardown)
  };

  return cmocka_run_group_tests(tests, NULL, NULL);
}
