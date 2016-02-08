#ifndef DXFW_DXFW_INTERNAL_MACROS_H_
#define DXFW_DXFW_INTERNAL_MACROS_H_

#define DXFW_UNUSED(x) (void)(x)

#define DXFW_CHECK_IF_INITIALIZED()                                               \
    if (!g_state_.m_initialized_) {                                               \
        dxfwReportError(DXFW_ERROR_NOT_INITIALIZED);                              \
        return;                                                                   \
    }

#define DXFW_CHECK_IF_INITIALIZED_AND_RETURN(x)                                   \
    if (!g_state_.m_initialized_) {                                               \
        dxfwReportError(DXFW_ERROR_NOT_INITIALIZED);                              \
        return x;                                                                 \
    }

#define DXFW_CHECK_ARGUMENT_NOT_EQUAL(argument, value)                            \
    if ((argument) == (value)) {                                                  \
        dxfwReportError(DXFW_ERROR_INVALID_ARGUMENT);                             \
        return;                                                                   \
    }

#define DXFW_CHECK_ARGUMENT_NOT_EQUAL_AND_RETURN(argument, value, return_value)  \
    if ((argument) == (value)) {                                                 \
        dxfwReportError(DXFW_ERROR_INVALID_ARGUMENT);                            \
        return return_value;                                                     \
    }

#endif  // DXFW_DXFW_INTERNAL_MACROS_H_
