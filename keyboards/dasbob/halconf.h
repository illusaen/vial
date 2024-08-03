// Copyright 2023 QMK
// SPDX-License-Identifier: GPL-2.0-or-later
#pragma once

#undef HAL_USE_SIO
#define HAL_USE_SIO TRUE

#define HAL_USE_I2C TRUE

#define HAL_USE_PWM TRUE
#define HAL_USE_PAL TRUE

#include_next <halconf.h>
