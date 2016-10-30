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

/* MACROS */
#define DXFW_TEST_UNUSED(x) (void)(x)

/* STRUCTS */
struct dxfwTestSingleWindowTestData {
  uintptr_t m_window_id_;
  struct dxfwWindow* m_window_;
  wchar_t* m_window_name_wide_;
};

/* TEST HELPERS */
wchar_t* dxfwTestUtf8ToWchar(const char* utf8);
void dxfwTestSetupAnyWmCreateExpectations();
void dxfwTestSetupAnyWindowCreateExpectations(uintptr_t id);
void dxfwSetupWindowCreateExpectations(uintptr_t id, uint32_t width, uint32_t height, const wchar_t* title);
void dxfwTestSetupWindowDestroyExpectations(uintptr_t id);
void dxfwTestSetupPeekMessage(HWND hwnd, UINT msg, LPARAM lparam, WPARAM wparam);
void dxfwTestSetupLastPeekMessage();
void dxfwSetupDefWindowProc(HWND hwnd, UINT msg, LPARAM lparam, WPARAM wparam);

/* SETUP & TEARDOWN */
int dxfwNotInitializedTestSetup(void **state);
int dxfwNotInitializedTestTeardown(void **state);
int dxfwInitializedTestSetup(void **state);
int dxfwInitializedTestTeardown(void **state);
int dxfwSingleWindowTestSetup(void **state);
int dxfwSingleWindowTestTeardown(void **state);

/* MOCKS */
void dxfwTestErrorCallbackMock(dxfwError error);

/* SETUP & TEARDOWN - OS */
int dxfwTestOsMocksSetup();
int dxfwTestOsMocksTeardown();

/* TESTS */
void dxfwInitTest(void **state);
void dxfwDoubleInitTest(void **state);
void dxfwNoInitTest(void **state);

void dxfwMemoryTest(void **state);
void dxfwSetAllocNullArguments(void **state);

void dxfwNotInitializedTest(void **state);
void dxfwGetTimeTest(void **state);
void dxfwTraceWithEmprtyArgumentListTest(void **state);

void dxfwCreateDestroyWindowTest(void **state);
void dxfwCreateWindowWithZeroWidthTest(void** state);
void dxfwCreateWindowWithZeroHeightTest(void** state);
void dxfwCreateWindowWithEmptyTitleTest(void** state);
void dxfwCreateWindowWithNullTitleTest(void** state);

void dxfwDestroyNullWindowTest(void** state);

void dxfwGetHandleTest(void** state);
void dxfwGetHandleForNullTest(void** state);

void dxfwSetAndGetUserDataTest(void** state);
void dxfwSetNullWindowUserData(void** state);
void dxfwGetNullWindowUserData(void** state);

void dxfwSetWindowCaptionTest(void** state);
void dxfwSetEmptyWindowCaptionTest(void** state);
void dxfwSetNullWindowCaptionTest(void** state);
void dxfwSetWindowNullCaptionTest(void** state);

void dxfwGetWindowSizeTest(void** state);
void dxfwGetWindowSizeNullWindowTest(void** state);
void dxfwGetWindowSizeNullWidthTest(void** state);
void dxfwGetWindowSizeNullHeightTest(void** state);

void dxfwSetWindowSizeTest(void** state);
void dxfwSetWindowSizeToZeroWidthTest(void** state);
void dxfwSetWindowSizeToZeroHeightTest(void** state);
void dxfwSetNullWindowSizeTest(void** state);

void dxfwShouldWindowCloseTest(void** state);
void dxfwShouldWindowCloseNullWindowTest(void** state);
void dxfwShouldCloseChangedCallbackNullWindowTest(void** state);

void dxfwWindowSizeChangedTest(void** state);
void dxfwWindowMinimizedTest(void** state);
void dxfwWindowMaxShowTest(void** state);
void dxfwWindowMaximizedTest(void** state);
void dxfwWindowMaxHideTest(void** state);
void dxfwSizeChangedCallbackNullWindowTest(void** state);

void dxfwPollOsEventsTest(void** state);

void dxfwTwoWindowTest(void** state);

void dxfwGetKeyStateTest(void** state);
void dxfwGetModifierFlagsTest(void** state);
void dxfwKeyboardCallbackTest(void** state);
void dxfwKeyboardCallbackNullWindowTest(void** state);

void dxfwSetMouseButtonCallbackTest(void** state);
void dxfwSetMouseButtonCallbackNullWindowTest(void** state);

void dxfwSetMouseMoveCallbackTest(void** state);
void dxfwSetMouseMoveCallbackNullWindowTest(void** state);

void dxfwSetMouseWheelCallbackTest(void** state);
void dxfwSetMouseWheelCallbackNullWindowTest(void** state);

#endif  // DXFW_TESTS_DXFW_TESTS_H_
