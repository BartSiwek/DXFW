#ifndef DXFW_TESTS_DXFW_TESTS_H_
#define DXFW_TESTS_DXFW_TESTS_H_

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <stdlib.h>

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#define WINUSERAPI
#include <windows.h>
#endif

#include <dxfw/dxfw.h>

/* TEST HELPERS */
void dxfwSetupAnyWmCreateExpectations();
void dxfwSetupAnyWindowCreateExpectations(int id);
dxfwSetupAnyWindowDestroyExpectations(int id);
wchar_t* dxfwTestsUtf8ToWchar(const char* utf8);

/* SETUP & TEARDOWN */
int dxfwTestSetup(void **state);
int dxfwTestTeardown(void **state);

/* MOCKS */
void errorCallbackMock(dxfwError error);

/* SETUP & TEARDOWN - OS */
int dxfwTestsOsMocksSetup();
int dxfwTestsOsMocksTeardown();

/* TESTS */
void dxfwInitTest(void **state);
void dxfwDoubleInitTest(void **state);
void dxfwNoInitTest(void **state);
void dxfwMemoryTest(void **state);
void dxfwGetTimeTest(void **state);

void dxfwCreateDestroyWindowTest(void **state);

#endif  // DXFW_TESTS_DXFW_TESTS_H_
