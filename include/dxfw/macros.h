#ifndef DXFW_MACROS_H_
#define DXFW_MACROS_H_

#ifdef __cplusplus
extern "C" {
#endif

#if defined(DEBUG) | defined(_DEBUG)
#ifndef DXFW_ERROR_TRACE
#define DXFW_ERROR_TRACE(FILE, LINE, ERROR, MSG_BOX) dxfwTraceError(FILE, LINE, ERROR, MSG_BOX)
#endif
#else
#ifndef DXFW_ERROR_TRACE
#define DXFW_ERROR_TRACE(FILE, LINE, ERROR, MSG_BOX) (void)(FILE); (void)(LINE); (void)(ERROR); (void)(MSG_BOX)
#endif
#endif

#if defined(DEBUG) | defined(_DEBUG)
#ifndef DXFW_DIRECTX_TRACE
#define DXFW_DIRECTX_TRACE(FILE, LINE, HR, MSG_BOX) dxfwTraceHResult(FILE, LINE, HR, MSG_BOX)
#endif
#else
#ifndef DXFW_DIRECTX_TRACE
#define DXFW_DIRECTX_TRACE(FILE, LINE, HR, MSG_BOX) (void)(FILE); (void)(LINE); (void)(HR); (void)(MSG_BOX)
#endif
#endif

#ifdef __cplusplus
}
#endif

#endif  // DXFW_MACROS_H_
