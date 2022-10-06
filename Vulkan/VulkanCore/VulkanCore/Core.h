#pragma once

#include <Core/Log/Log.h>

#define VK_NO_PROTOTYPES
#include <volk.h>

#ifdef NDEBUG
#define VK_CALL(vkFunc) vkFunc
#else
#define VK_CALL(vkFunc) if (vkFunc != VK_SUCCESS) { LOG_ASSERTION(false, "VK_CALL failed.")}
#endif
