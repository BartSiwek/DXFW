#ifndef DXFW_MACROS_H_
#define DXFW_MACROS_H_

#ifdef __cplusplus
extern "C" {
#endif

#if defined(DEBUG) | defined(_DEBUG)

#ifndef DXFW_ERROR_TRACE
#define DXFW_ERROR_TRACE(FILE, LINE, MSG_BOX, ERROR) dxfwTraceError(FILE, LINE, MSG_BOX, NULL, NULL, ERROR)
#endif  // DXFW_ERROR_TRACE

#ifndef DXFW_ERROR_TRACE_WITH_ALLOC
#define DXFW_ERROR_TRACE_WITH_ALLOC(FILE, LINE, MSG_BOX, ALLOC, DEALLOC, ERROR) dxfwTraceError(FILE, LINE, MSG_BOX, ALLOC, DEALLOC, ERROR)
#endif  // DXFW_ERROR_TRACE_WITH_ALLOC

#ifndef DXFW_DIRECTX_TRACE
#define DXFW_DIRECTX_TRACE(FILE, LINE, MSG_BOX, HR) dxfwTraceHResult(FILE, LINE, MSG_BOX, NULL, NULL, HR)
#endif  // DXFW_DIRECTX_TRACE

#ifndef DXFW_DIRECTX_TRACE_WITH_ALLOC
#define DXFW_DIRECTX_TRACE_WITH_ALLOC(FILE, LINE, MSG_BOX, ALLOC, DEALLOC, HR) dxfwTraceHResult(FILE, LINE, MSG_BOX, ALLOC, DEALLOC, HR)
#endif  // DXFW_DIRECTX_TRACE_WITH_ALLOC

#ifndef DXFW_TRACE
#define DXFW_TRACE(FILE, LINE, MSG_BOX, MSG, ...) dxfwTraceMessage(FILE, LINE, MSG_BOX, NULL, NULL, MSG, __VA_ARGS__)
#endif  // DXFW_TRACE

#ifndef DXFW_TRACE_WITH_ALLOC
#define DXFW_TRACE_WITH_ALLOC(FILE, LINE, MSG_BOX, ALLOC, DEALLOC, MSG, ...) dxfwTraceMessage(FILE, LINE, MSG_BOX, ALLOC, DEALLOC, MSG, __VA_ARGS__)
#endif  // DXFW_TRACE_WITH_ALLOC

#else

#ifndef DXFW_ERROR_TRACE
#define DXFW_ERROR_TRACE(FILE, LINE, MSG_BOX, ERROR) (void)(FILE); (void)(LINE); (void)(MSG_BOX); (void)(ERROR)
#endif  // DXFW_ERROR_TRACE

#ifndef DXFW_ERROR_TRACE_WITH_ALLOC
#define DXFW_ERROR_TRACE_WITH_ALLOC(FILE, LINE, MSG_BOX, ALLOC, DEALLOC, ERROR) (void)(FILE); (void)(LINE); (void)(MSG_BOX); (void)(ALLOC); (void)(DEALLOC); (void)(ERROR)
#endif  // DXFW_ERROR_TRACE_WITH_ALLOC

#ifndef DXFW_DIRECTX_TRACE
#define DXFW_DIRECTX_TRACE(FILE, LINE, MSG_BOX, HR) (void)(FILE); (void)(LINE); (void)(MSG_BOX); (void)(HR)
#endif  // DXFW_DIRECTX_TRACE

#ifndef DXFW_DIRECTX_TRACE_WITH_ALLOC
#define DXFW_DIRECTX_TRACE_WITH_ALLOC(FILE, LINE, MSG_BOX, ALLOC, DEALLOC, HR) (void)(FILE); (void)(LINE); (void)(MSG_BOX); (void)(ALLOC); (void)(DEALLOC); (void)(HR)
#endif  // DXFW_DIRECTX_TRACE_WITH_ALLOC

#ifndef DXFW_TRACE
#define DXFW_TRACE(FILE, LINE, MSG_BOX, MSG, ...) (void)(FILE); (void)(LINE); (void)(MSG_BOX); (void)(MSG); (void)((void)0, __VA_ARGS__)
#endif  // DXFW_TRACE

#ifndef DXFW_TRACE_WITH_ALLOC
#define DXFW_TRACE_WITH_ALLOC(FILE, LINE, MSG_BOX, ALLOC, DEALLOC, MSG, ...) (void)(FILE); (void)(LINE); (void)(MSG_BOX); (void)(ALLOC); (void)(DEALLOC); (void)(MSG); (void)((void)0, __VA_ARGS__)
#endif  // DXFW_TRACE_WITH_ALLOC

#endif  // defined(DEBUG) | defined(_DEBUG)

#ifdef __cplusplus
}
#endif

#endif  // DXFW_MACROS_H_
