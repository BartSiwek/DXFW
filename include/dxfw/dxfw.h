#ifndef DXFW_DXFW_H_
#define DXFW_DXFW_H_

#include "window.h"

#include <cstdlib>

namespace dxfw {

using AllocFunction = void*(*)(size_t);
using DeallocFunction = void(*)(void*);

bool Initialize(AllocFunction alloc = malloc, DeallocFunction dealloc = free);
void Terminate();

void PollOsEvents();

}  // namespace dxfw

#endif  // DXFW_DXFW_H_
