// SPDX-License-Identifier: MIT
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.

#ifndef _AIEBU_COMMON_UID_MD5_H_
#define _AIEBU_COMMON_UID_MD5_H_

#ifdef _WIN32
#include "windows/uid_md5.h"
#else
#include "linux/uid_md5.h"
#endif

#endif //_AIEBU_COMMON_UID_MD5_H_
