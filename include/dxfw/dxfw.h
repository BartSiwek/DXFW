#ifndef DXFW_DXFW_H_
#define DXFW_DXFW_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

/* MEMORY MANAGEMENT */
typedef void*(*dxfw_alloc_function)(size_t);
typedef void(*dxfw_dealloc_function)(void*);
void dxfwSetAlloc(dxfw_alloc_function alloc, dxfw_dealloc_function dealloc);

/* INIT & TERMINATE */
bool dxfwInitialize();
void dxfwTerminate();

/* ERROR HANDLING */
typedef enum {
  DXFW_NO_ERROR,
  DXFW_UTF8_CONVERSION_ERROR,
  DXFW_MAX_ERRORS,
} dxfwError;
typedef void(*dxfw_on_error)(dxfwError);
dxfw_on_error dxfwSetErrorCallback(dxfw_on_error callback);

/* TIME MANAGEMENT */
double dxfwGetTime();

/* WINDOW MANAGEMENT */
struct dxfwWindow;
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
typedef enum {
  DXFW_LEFT_MOUSE_BUTTON,
  DXFW_MIDDLE_MOUSE_BUTTON,
  DXFW_RIGHT_MOUSE_BUTTON
} dxfwMouseButton;

typedef enum {
  DXFW_MOUSE_BUTTON_UP,
  DXFW_MOUSE_BUTTON_DOWN
} dxfwMouseButtonAction;

typedef void(*dxfw_on_mouse_button)(struct dxfwWindow*, dxfwMouseButton button, dxfwMouseButtonAction action, int32_t x, int32_t y);
dxfw_on_mouse_button dxfwSetMouseButtonCallback(struct dxfwWindow* window, dxfw_on_mouse_button callback);

typedef void(*dxfw_on_mouse_move)(struct dxfwWindow*, int32_t x, int32_t y);
dxfw_on_mouse_move dxfwSetMouseMoveCallback(struct dxfwWindow* window, dxfw_on_mouse_move callback);

typedef void(*dxfw_on_mouse_wheel)(struct dxfwWindow*, int32_t x, int32_t y, int32_t delta);
dxfw_on_mouse_wheel dxfwSetMouseWheelCallback(struct dxfwWindow* window, dxfw_on_mouse_wheel callback);

/* KEYBOARD EVENTS */
typedef enum {
  DXFW_KEY_NONE,

  DXFW_KEY_A,
  DXFW_KEY_B,
  DXFW_KEY_C,
  DXFW_KEY_D,
  DXFW_KEY_E,
  DXFW_KEY_F,
  DXFW_KEY_G,
  DXFW_KEY_H,
  DXFW_KEY_I,
  DXFW_KEY_J,
  DXFW_KEY_K,
  DXFW_KEY_L,
  DXFW_KEY_M,
  DXFW_KEY_N,
  DXFW_KEY_O,
  DXFW_KEY_P,
  DXFW_KEY_Q,
  DXFW_KEY_R,
  DXFW_KEY_S,
  DXFW_KEY_T,
  DXFW_KEY_U,
  DXFW_KEY_V,
  DXFW_KEY_W,
  DXFW_KEY_X,
  DXFW_KEY_Y,
  DXFW_KEY_Z,

  DXFW_KEY_0,
  DXFW_KEY_1,
  DXFW_KEY_2,
  DXFW_KEY_3,
  DXFW_KEY_4,
  DXFW_KEY_5,
  DXFW_KEY_6,
  DXFW_KEY_7,
  DXFW_KEY_8,
  DXFW_KEY_9,

  DXFW_KEY_ESCAPE,

  DXFW_KEY_F1,
  DXFW_KEY_F2,
  DXFW_KEY_F3,
  DXFW_KEY_F4,
  DXFW_KEY_F5,
  DXFW_KEY_F6,
  DXFW_KEY_F7,
  DXFW_KEY_F8,
  DXFW_KEY_F9,
  DXFW_KEY_F10,
  DXFW_KEY_F11,
  DXFW_KEY_F12,

  DXFW_KEY_TILDE,
  DXFW_KEY_MINUS,
  DXFW_KEY_EQUALS,
  DXFW_KEY_BACKSPACE,
  DXFW_KEY_TAB,
  DXFW_KEY_OPENBRACE,
  DXFW_KEY_CLOSEBRACE,
  DXFW_KEY_ENTER,
  DXFW_KEY_SEMICOLON,
  DXFW_KEY_QUOTE,
  DXFW_KEY_BACKSLASH,
  DXFW_KEY_COMMA,
  DXFW_KEY_DOT,
  DXFW_KEY_SLASH,
  DXFW_KEY_SPACE,

  DXFW_KEY_INSERT,
  DXFW_KEY_DELETE,
  DXFW_KEY_HOME,
  DXFW_KEY_END,
  DXFW_KEY_PGUP,
  DXFW_KEY_PGDN,

  DXFW_KEY_LEFT,
  DXFW_KEY_RIGHT,
  DXFW_KEY_UP,
  DXFW_KEY_DOWN,

  DXFW_KEY_NUMPAD_0,
  DXFW_KEY_NUMPAD_1,
  DXFW_KEY_NUMPAD_2,
  DXFW_KEY_NUMPAD_3,
  DXFW_KEY_NUMPAD_4,
  DXFW_KEY_NUMPAD_5,
  DXFW_KEY_NUMPAD_6,
  DXFW_KEY_NUMPAD_7,
  DXFW_KEY_NUMPAD_8,
  DXFW_KEY_NUMPAD_9,

  DXFW_KEY_NUMPAD_SLASH,
  DXFW_KEY_NUMPAD_ASTERISK,
  DXFW_KEY_NUMPAD_MINUS,
  DXFW_KEY_NUMPAD_PLUS,
  DXFW_KEY_NUMPAD_DECIMAL,
  DXFW_KEY_NUMPAD_ENTER,

  DXFW_KEY_LSHIFT,
  DXFW_KEY_RSHIFT,
  DXFW_KEY_LCTRL,
  DXFW_KEY_RCTRL,
  DXFW_KEY_ALT,
  DXFW_KEY_LWIN,
  DXFW_KEY_RWIN,

  DXFW_KEY_PRINTSCREEN,
  DXFW_KEY_SCROLLLOCK,
  DXFW_KEY_PAUSE,
  DXFW_KEY_NUMLOCK,
  DXFW_KEY_CAPSLOCK,

  DXFW_KEY_COUNT,
} dxfwVirtualKeyCode;

typedef enum {
  DXFW_KEY_MODIFIER_NONE = 0x0000,
  DXFW_KEY_MODIFIER_SHIFT = 0x0001,
  DXFW_KEY_MODIFIER_CTRL = 0x0002,
  DXFW_KEY_MODIFIER_ALT = 0x0004,
  DXFW_KEY_MODIFIER_LWIN = 0x0008,
  DXFW_KEY_MODIFIER_RWIN = 0x0010,
} dxfwVirtualKeyModifiers;

typedef enum {
  DXFW_KEY_STATE_UP = 0,
  DXFW_KEY_STATE_DOWN = 1,
} dxfwVirtualKeyState;

dxfwVirtualKeyState dxfwGetKeyState(dxfwVirtualKeyCode key_code);
dxfwVirtualKeyState dxfwGetPreviousKeyState(dxfwVirtualKeyCode key_code);
dxfwVirtualKeyModifiers dxfwGetModifierFlags();

typedef void(*dxfw_on_keyboard)(struct dxfwWindow*, dxfwVirtualKeyCode key, dxfwVirtualKeyModifiers modifiers, dxfwVirtualKeyState state, dxfwVirtualKeyState previous_state);
dxfw_on_keyboard dxfwSetKeyboardCallback(struct dxfwWindow* window, dxfw_on_keyboard callback);

#ifdef __cplusplus
}
#endif

#endif  // DXFW_DXFW_H_
