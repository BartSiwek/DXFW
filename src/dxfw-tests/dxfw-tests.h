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

int dxfwTestsMocksSetup();
int dxfwTestsMocksTeardown();

void errorCallbackMock(dxfwError error);
int dxfwTestSetup(void **state);
int dxfwTestTeardown(void **state);

void dxfwSetupAnyWindowCreateExpectations();
void dxfwSetupAnyWmCreateExpectations();

void dxfwInitTest(void **state);
void dxfwDoubleInitTest(void **state);
void dxfwNoInitTest(void **state);
void dxfwMemoryTest(void **state);

#endif  // DXFW_TESTS_DXFW_TESTS_H_
