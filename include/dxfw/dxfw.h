#ifndef DXFW_DXFW_H_
#define DXFW_DXFW_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

typedef void*(*dxfw_alloc_function)(size_t);
typedef void(*dxfw_dealloc_function)(void*);
bool dxfwSetAlloc(dxfw_alloc_function alloc, dxfw_dealloc_function dealloc);

bool dxfwInitialize();
void dxfwTerminate();

void dxfwPollOsEvents();

struct dxfwWindow;

struct dxfwWindow* dxfwCreateWindow(uint32_t width, uint32_t height, const char* caption);

bool dxfwShouldClose(struct dxfwWindow* window);

#ifdef __cplusplus
}
#endif

#endif  // DXFW_DXFW_H_
