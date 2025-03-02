// SPDX-License-Identifier: MIT
// Copyright (C) 2025, Advanced Micro Devices, Inc. All rights reserved.
#ifndef AIEBU_DETAIL_CONFIG_H
#define AIEBU_DETAIL_CONFIG_H

//------------------Enable dynamic linking on windows-------------------------//

#ifdef _WIN32
# ifndef AIEBU_STATIC_BUILD
#  ifdef AIEBU_API_SOURCE
#   define AIEBU_API_EXPORT __declspec(dllexport)
#  else
#   define AIEBU_API_EXPORT __declspec(dllimport)
#  endif
# endif
#endif
#ifdef __linux__
# ifdef AIEBU_API_SOURCE
#  define AIEBU_API_EXPORT __attribute__ ((visibility("default")))
# else
#  define AIEBU_API_EXPORT
# endif
#endif

#ifndef AIEBU_API_EXPORT
# define AIEBU_API_EXPORT
#endif

#endif
