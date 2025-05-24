#pragma once

#ifdef OS_WIN

#ifdef BUILDING_SHARED
#define BASE_EXPORT __declspec(dllexport)
#elif USING_SHARED
#define BASE_EXPORT __declspec(dllimport)
#endif

#endif
