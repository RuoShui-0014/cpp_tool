#pragma once

#if defined(OS_WIN)

  #if defined(BUILDING_SHARED)
    #define BASE_EXPORT __declspec(dllexport)
  #elif defined(USING_SHARED)
    #define BASE_EXPORT __declspec(dllimport)
  #else
    #define BASE_EXPORT
  #endif

#else

  #define BASE_EXPORT

#endif
