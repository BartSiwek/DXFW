#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#include <dxfw/dxfw.h>

/* A test case that does nothing and succeeds. */
static void init_test(void **state) {
  dxfwInitialize();

  struct dxfwWindow* window = dxfwCreateWindow(640, 480, "Test");

  dxfwTerminate();
}

int run_dxfw_tests() {
  const struct CMUnitTest tests[] = {
    cmocka_unit_test(init_test),
  };

  return cmocka_run_group_tests(tests, NULL, NULL);
}
