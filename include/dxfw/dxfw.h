#ifndef DXFW_DXFW_H_
#define DXFW_DXFW_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

#include <dxfw/enums.h>

/* MEMORY MANAGEMENT */
typedef void*(*dxfw_alloc_function)(size_t);
typedef void(*dxfw_dealloc_function)(void*);
void dxfwSetAlloc(dxfw_alloc_function alloc, dxfw_dealloc_function dealloc);

/* INIT & TERMINATE */
bool dxfwInitialize();
void dxfwTerminate();

/* ERROR HANDLING */
typedef void(*dxfw_on_error)(dxfwError);
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
typedef void(*dxfw_on_should_close_changed)(struct dxfwWindow*, bool should_close);
dxfw_on_should_close_changed dxfwSetShouldCloseChangedCallback(struct dxfwWindow* window, dxfw_on_should_close_changed callback);

/* MOUSE EVENTS */
typedef void(*dxfw_on_mouse_button)(struct dxfwWindow*, dxfwMouseButton button, dxfwMouseButtonAction action, int32_t x, int32_t y);
dxfw_on_mouse_button dxfwSetMouseButtonCallback(struct dxfwWindow* window, dxfw_on_mouse_button callback);

typedef void(*dxfw_on_mouse_move)(struct dxfwWindow*, int32_t x, int32_t y);
dxfw_on_mouse_move dxfwSetMouseMoveCallback(struct dxfwWindow* window, dxfw_on_mouse_move callback);

typedef void(*dxfw_on_mouse_wheel)(struct dxfwWindow*, int32_t x, int32_t y, int32_t delta);
dxfw_on_mouse_wheel dxfwSetMouseWheelCallback(struct dxfwWindow* window, dxfw_on_mouse_wheel callback);

/* KEYBOARD EVENTS */
dxfwVirtualKeyState dxfwGetKeyState(dxfwVirtualKeyCode key_code);
dxfwVirtualKeyState dxfwGetPreviousKeyState(dxfwVirtualKeyCode key_code);
dxfwVirtualKeyModifiers dxfwGetModifierFlags();

typedef void(*dxfw_on_keyboard)(struct dxfwWindow*, dxfwVirtualKeyCode key, dxfwVirtualKeyModifiers modifiers, dxfwVirtualKeyState state, dxfwVirtualKeyState previous_state);
dxfw_on_keyboard dxfwSetKeyboardCallback(struct dxfwWindow* window, dxfw_on_keyboard callback);

#ifdef __cplusplus
}
#endif

#endif  // DXFW_DXFW_H_
