#ifndef DXFW_DXFW_INTERNAL_MACROS_H_
#define DXFW_DXFW_INTERNAL_MACROS_H_

#define DXFW_CHECK_IF_INITIALIZED()                   \
    if (!g_state_.m_initialized_) {                   \
        dxfwReportError(DXFW_ERROR_NOT_INITIALIZED);  \
        return;                                       \
    }

#define DXFW_CHECK_IF_INITIALIZED_AND_RETURN(x)       \
    if (!g_state_.m_initialized_) {                   \
        dxfwReportError(DXFW_ERROR_NOT_INITIALIZED);  \
        return x;                                     \
    }

#endif  // DXFW_DXFW_INTERNAL_MACROS_H_
