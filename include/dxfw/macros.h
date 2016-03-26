#ifndef DXFW_MACROS_H_
#define DXFW_MACROS_H_

#ifdef __cplusplus
extern "C" {
#endif

#if defined(DEBUG) | defined(_DEBUG)
#ifndef DXFW_ERROR_TRACE
#define DXFW_ERROR_TRACE(FILE, LINE, MSG_BOX, ERROR) dxfwTraceError(FILE, LINE, MSG_BOX, ERROR)
#endif
#else
#ifndef DXFW_ERROR_TRACE
#define DXFW_ERROR_TRACE(FILE, LINE, MSG_BOX, ERROR) (void)(FILE); (void)(LINE); (void)(MSG_BOX); (void)(ERROR)
#endif
#endif

#if defined(DEBUG) | defined(_DEBUG)
#ifndef DXFW_DIRECTX_TRACE
#define DXFW_DIRECTX_TRACE(FILE, LINE, MSG_BOX, HR) dxfwTraceHResult(FILE, LINE, MSG_BOX, HR)
#endif
#else
#ifndef DXFW_DIRECTX_TRACE
#define DXFW_DIRECTX_TRACE(FILE, LINE, MSG_BOX, HR) (void)(FILE); (void)(LINE); (void)(MSG_BOX); (void)(HR)
#endif
#endif

#if defined(DEBUG) | defined(_DEBUG)
#ifndef DXFW_TRACE
#define DXFW_TRACE(FILE, LINE, MSG_BOX, MSG, ...) dxfwTraceMessage(FILE, LINE, MSG_BOX, MSG, __VA_ARGS__)
#endif
#else
#ifndef DXFW_TRACE
#define DXFW_TRACE(FILE, LINE, MSG_BOX, MSG, ...) (void)(FILE); (void)(LINE); (void)(MSG_BOX); (void)(MSG); (void)(__VA_ARGS__)
#endif
#endif

#ifdef __cplusplus
}
#endif

#endif  // DXFW_MACROS_H_
