#pragma once
#include "TimeUtil.h"

#if defined(_DEBUG)
#define LOG(format, ...)\
        do{\
            char _buffer[256];\
            int _res = snprintf(_buffer, std::size(_buffer), "[%.3f]: "##format##"\n", NULLEngine::Core::TimeUtil::GetTime(),__VA_ARGS__);\
            OutputDebugStringA(_buffer);\
        }while(false)

#define ASSERT(condition, format, ...)\
        do{\
            if (!(condition))\
            {\
                LOG("ASSERT! %s(%d)\n"##format, __FILE__, __LINE__, __VA_ARGS__);\
                DebugBreak();\
            }\
        } while (false)

#else
#define LOG(format,...)
#define ASSERT(condition, format, ...) do{(void)(sizeof(condition));}while(false)
#endif