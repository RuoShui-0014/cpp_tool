#pragma once

#ifdef OS_WIN

#ifdef BUILDING_SHARED
#define BASE_EXPORT __declspec(dllexport)
#elifdef  USING_SHARED
#define BASE_EXPORT __declspec(dllimport)
#else
#define BASE_EXPORT
#endif

#else

#define BASE_EXPORT

#endif
