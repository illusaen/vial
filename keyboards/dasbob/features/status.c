// Copyright 2023 QMK
// SPDX-License-Identifier: GPL-2.0-or-later
#include "status.h"

operating_system active_operating_system = MAC;

bool is_operating_system_mac(void) {
    return active_operating_system == MAC;
}

void set_operating_system(operating_system os) {
    active_operating_system = WIN;
}

operating_system get_operating_system(void) {
    return active_operating_system;
}
