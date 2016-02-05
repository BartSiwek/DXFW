#ifndef DXFW_DXFW_H_
#define DXFW_DXFW_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

#include <dxfw/enums.h>

/* TYPEDEFS */
typedef void*(*dxfw_alloc_function)(size_t);
typedef void(*dxfw_dealloc_function)(void*);
typedef void(*dxfw_on_error)(dxfwError);
typedef void(*dxfw_on_should_close_changed)(struct dxfwWindow*, bool);
typedef void(*dxfw_on_mouse_button)(struct dxfwWindow*, dxfwMouseButton, dxfwMouseButtonAction, int32_t, int32_t);
typedef void(*dxfw_on_mouse_move)(struct dxfwWindow*, int32_t, int32_t);
typedef void(*dxfw_on_mouse_wheel)(struct dxfwWindow*, int32_t, int32_t, int32_t);
typedef void(*dxfw_on_keyboard)(struct dxfwWindow*, dxfwVirtualKeyCode, dxfwVirtualKeyModifiers, dxfwVirtualKeyState, dxfwVirtualKeyState);

/* MEMORY MANAGEMENT */
void dxfwSetAlloc(dxfw_alloc_function alloc, dxfw_dealloc_function dealloc);

/* INIT & TERMINATE */
bool dxfwInitialize();
void dxfwTerminate();

/* ERROR HANDLING */
dxfw_on_error dxfwSetErrorCallback(dxfw_on_error callback);

/* TIME MANAGEMENT */
double dxfwGetTime();

/* WINDOW MANAGEMENT */
struct dxfwWindow* dxfwCreateWindow(uint32_t width, uint32_t height, const char* caption);
void dxfwDestroyWindow(struct dxfwWindow* window);

/* WINDOW STATE */
void dxfwSetWindowCaption(struct dxfwWindow* window, const char* caption);
void dxfwGetWindowSize(struct dxfwWindow* window, uint32_t* width, uint32_t* height); 
void dxfwSetWindowSize(struct dxfwWindow* window, uint32_t width, uint32_t height);
bool dxfwShouldWindowClose(struct dxfwWindow* window);

/* EVENT MANEGEMENT */
void dxfwPollOsEvents();

/* SHOULD CLOSE EVENT */
dxfw_on_should_close_changed dxfwSetShouldCloseChangedCallback(struct dxfwWindow* window, dxfw_on_should_close_changed callback);

/* MOUSE EVENTS */
dxfw_on_mouse_button dxfwSetMouseButtonCallback(struct dxfwWindow* window, dxfw_on_mouse_button callback);
dxfw_on_mouse_move dxfwSetMouseMoveCallback(struct dxfwWindow* window, dxfw_on_mouse_move callback);
dxfw_on_mouse_wheel dxfwSetMouseWheelCallback(struct dxfwWindow* window, dxfw_on_mouse_wheel callback);

/* KEYBOARD EVENTS */
dxfwVirtualKeyState dxfwGetKeyState(dxfwVirtualKeyCode key_code);
dxfwVirtualKeyState dxfwGetPreviousKeyState(dxfwVirtualKeyCode key_code);
dxfwVirtualKeyModifiers dxfwGetModifierFlags();
dxfw_on_keyboard dxfwSetKeyboardCallback(struct dxfwWindow* window, dxfw_on_keyboard callback);

#ifdef __cplusplus
}
#endif

#endif  // DXFW_DXFW_H_
