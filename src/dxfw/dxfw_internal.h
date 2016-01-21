#ifndef DXFW_DXFW_INTERNAL_H_
#define DXFW_DXFW_INTERNAL_H_

#include "dxfw/dxfw.h"

/* GLOBAL INTERNALS */
void* dxfwAlloc(size_t size);
void dxfwDealloc(void* ptr);

/* WINDOW MANAGEMENT INTERNALS INTERNALS */
void dxfwTerminateWindowHandling();

#endif  // DXFW_DXFW_INTERNAL_H_
