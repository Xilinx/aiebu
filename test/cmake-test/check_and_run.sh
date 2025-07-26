#!/bin/bash

# SPDX-License-Identifier: Apache-2.0
# Copyright (C) 2025 Advanced Micro Devices, Inc. All rights reserved.

STAGING_DIR="$1"
shift

if [[ ! -d "$STAGING_DIR" ]]; then
  echo "Skipping test because staging dir '$STAGING_DIR' does not exist"
  exit 77
fi

# Run the rest of the command
exec "$@"
