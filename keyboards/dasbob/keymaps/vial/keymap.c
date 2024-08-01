// Copyright 2023 QMK
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H
#include "features/oneshot.h"
#include "features/swapper.h"
#include "features/select_word.h"

#define LA_SYM MO(SYM)
#define LA_NAV MO(NAV)
#define LA_NUM MO(NUM)

enum layers {
    DEF,
    COL,
    NAV,
    SYM,
    NUM,
    MIS
};

enum keycodes {
    // Custom oneshot mod implementation with no timers.
    OS_SHFT = QK_KB_0,
    OS_CTRL,
    OS_ALT,
    OS_CMD,

    SW_WIN, // Switch to next window         (cmd-tab)
    CAPS_WD,

    OS_LINUX,
    OS_MAC,
    OS_WIN,

    CUT,
    COPY,
    PASTE,
    UNDO,
    REDO,
    RUN,
    SELWORD,
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [DEF] = LAYOUT_split_3x5_3(
        KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,          KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,
        KC_A,    KC_S,    KC_D,    KC_F,    KC_G,          KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN,
        KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,          KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_QUOT,
                            LA_NUM,  KC_SPC, LA_NAV,    LA_SYM, KC_SPC, KC_BSPC
    ),

    [COL] = LAYOUT_split_3x5_3(
        KC_Q,    KC_W,    KC_F,    KC_P,    KC_B,         KC_J,    KC_L,    KC_U,    KC_Y,    KC_QUOT,
        KC_A,    KC_S,    KC_D,    KC_F,    KC_G,         KC_M,    KC_N,    KC_E,    KC_I,    KC_O,
        KC_Z,    KC_X,    KC_C,    KC_D,    KC_V,         KC_K,    KC_H,    KC_COMM, KC_DOT,  KC_SCLN,
                            LA_NUM,  KC_SPC, LA_NAV,    LA_SYM, KC_SPC, KC_BSPC
    ),

    [NAV] = LAYOUT_split_3x5_3(
        KC_ESC,  CUT,     COPY,    PASTE,   KC_TAB,       S(KC_TAB), KC_HOME, KC_UP,   KC_END,  KC_DEL,
        OS_SHFT, OS_CTRL, OS_ALT,  OS_CMD,  RUN,          CAPS_WD,   KC_LEFT, KC_DOWN, KC_RGHT, KC_ENT,
        SELWORD, XXXXXXX, XXXXXXX, SW_WIN,  UNDO,         REDO,      KC_PGDN, XXXXXXX, KC_PGUP, XXXXXXX,
                            _______, _______, _______,  _______, _______, _______
    ),

    [SYM] = LAYOUT_split_3x5_3(
        KC_ESC,  KC_LBRC, KC_LCBR, KC_LPRN, KC_TILD,      KC_CIRC, KC_RPRN, KC_RCBR, KC_RBRC, KC_GRV,
        KC_MINS, KC_ASTR, KC_EQL,  KC_UNDS, KC_DLR,       KC_HASH, OS_CMD,  OS_ALT,  OS_CTRL, OS_SHFT,
        KC_PLUS, KC_PIPE, KC_AT,   KC_SLSH, KC_PERC,      KC_BSPC, KC_BSLS, KC_AMPR, KC_QUES, KC_EXLM,
                            _______, _______, _______,  _______, _______, _______
    ),

    [NUM] = LAYOUT_split_3x5_3(
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,      KC_DOT,  KC_1,    KC_2,    KC_3,    KC_DEL,
        OS_SHFT, OS_CTRL, OS_ALT,  OS_CMD,  XXXXXXX,      KC_EQL,  KC_4,    KC_5,    KC_6,    KC_0,
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,      KC_SLSH, KC_7,    KC_8,    KC_9,    KC_MINS,
                            _______, _______, _______,  _______, _______, _______
    ),

    [MIS] = LAYOUT_split_3x5_3(
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,      XXXXXXX, DF(DEF),  DF(COL), XXXXXXX, XXXXXXX,
        XXXXXXX, XXXXXXX, KC_VOLD, KC_VOLU, XXXXXXX,      XXXXXXX, OS_LINUX, OS_MAC,  OS_WIN,  XXXXXXX,
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,      XXXXXXX, CG_SWAP,  CG_NORM, XXXXXXX, XXXXXXX,
                            _______, _______, _______,  _______, _______, _______
    ),
};

enum operating_system {
    LINUX = 0,
    MAC,
    WIN,
};

enum operating_system active_operating_system = MAC;

bool is_oneshot_cancel_key(uint16_t keycode) {
    switch (keycode) {
    case LA_SYM:
    case LA_NAV:
        return true;
    default:
        return false;
    }
}

bool is_oneshot_ignored_key(uint16_t keycode) {
    switch (keycode) {
    case LA_SYM:
    case LA_NAV:
    case KC_LSFT:
    case OS_SHFT:
    case OS_CTRL:
    case OS_ALT:
    case OS_CMD:
        return true;
    default:
        return false;
    }
}

bool sw_win_active = false;
oneshot_state os_shft_state = os_up_unqueued;
oneshot_state os_ctrl_state = os_up_unqueued;
oneshot_state os_alt_state = os_up_unqueued;
oneshot_state os_cmd_state = os_up_unqueued;

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    uint16_t cmdish = active_operating_system == MAC ? KC_LGUI : KC_LALT;
    uint16_t is_mac = active_operating_system == MAC;
    update_swapper(
        &sw_win_active, cmdish, KC_TAB, SW_WIN,
        keycode, record
    );

    update_oneshot(
        &os_shft_state, KC_LSFT, OS_SHFT,
        keycode, record
    );
    update_oneshot(
        &os_ctrl_state, KC_LCTL, OS_CTRL,
        keycode, record
    );
    update_oneshot(
        &os_alt_state, KC_LALT, OS_ALT,
        keycode, record
    );
    update_oneshot(
        &os_cmd_state, KC_LCMD, OS_CMD,
        keycode, record
    );

    switch (keycode) {
        case CAPS_WD:
            if (record->event.pressed) {
                caps_word_toggle();
            }
            return 0;
        case OS_LINUX:
            if (record->event.pressed) {
                active_operating_system = LINUX;
            }
            return 0;
        case OS_MAC:
            if (record->event.pressed) {
                active_operating_system = MAC;
            }
            return 0;
        case OS_WIN:
            if (record->event.pressed) {
                active_operating_system = WIN;
            }
            return 0;
        case CUT:
            if (record->event.pressed) {
                SEND_STRING(is_mac ? SS_LGUI("x") : SS_LCTL("x"));
            }
            return 0;
        case COPY:
            if (record->event.pressed) {
                SEND_STRING(is_mac ? SS_LGUI("c") : SS_LCTL("c"));
            }
            return 0;
        case PASTE:
            if (record->event.pressed) {
                SEND_STRING(is_mac ? SS_LGUI("v") : SS_LCTL("v"));
            }
            return 0;
        case UNDO:
            if (record->event.pressed) {
                SEND_STRING(is_mac ? SS_LGUI("z") : SS_LCTL("z"));
            }
            return 0;
        case REDO:
            if (record->event.pressed) {
                SEND_STRING(is_mac ? SS_LGUI(SS_LSFT("z")) : SS_LCTL(SS_LSFT("z")));
            }
            return 0;
        case RUN:
            if (record->event.pressed) {
                SEND_STRING(is_mac ? SS_LGUI(" ") : SS_LCTL(" "));
            }
            return 0;
    }

    if (!process_select_word(keycode, record, SELWORD, is_mac)) { return false; }

    return true;
}

layer_state_t layer_state_set_user(layer_state_t state) {
    return update_tri_layer_state(state, SYM, NAV, MIS);
}

#ifdef OLED_ENABLE

oled_rotation_t oled_init_user(oled_rotation_t rotation) {
    return !is_keyboard_master() ? OLED_ROTATION_0 : OLED_ROTATION_270;
}

static void oled_write_os_logo(void) {
    // static const char PROGMEM mac_logo[] = {
    //     149,150,10,
    //     181,182,0
    // };
    // static const char PROGMEM windows_logo[] = {
    //     151,152,10,
    //     183,184,0
    // };
    static const char PROGMEM linux_logo[] = {
        153,154,10,
        185,186,0
    };
    switch (active_operating_system) {
        case LINUX:
            oled_write_P(linux_logo, false);
            break;
        case MAC:
            oled_write("MAC", false);
            break;
        case WIN:
            oled_write("WIN", false);
            break;
    }
}

static void oled_write_layer(void) {
    oled_write_P(PSTR("\n\n\nlayer"), false);
    switch (get_highest_layer(layer_state)) {
        case DEF:
            oled_write_ln_P(PSTR("DEF"), false);
            break;
        case COL:
            oled_write_ln_P(PSTR("COL"), false);
            break;
        case SYM:
            oled_write_ln_P(PSTR("SYM"), false);
            break;
        case NAV:
            oled_write_ln_P(PSTR("NAV"), false);
            break;
        case NUM:
            oled_write_ln_P(PSTR("NUM"), false);
            break;
        case MIS:
            oled_write_ln_P(PSTR("MISC"), false);
            break;
        default:
            oled_write_ln_P(PSTR("??"), false);
            break;
    }
    oled_write_ln_P(PSTR(""), false);
}

bool oled_task_user(void) {
    if (is_keyboard_master()) {
        oled_write_layer();

        led_t led_usb_state = host_keyboard_led_state();
        if (led_usb_state.num_lock || led_usb_state.caps_lock) {
            oled_write_P(led_usb_state.num_lock ? PSTR("NUM") : PSTR(""), false);
            oled_write_P(led_usb_state.caps_lock ? PSTR("CAP") : PSTR(""), false);
            oled_write_ln_P(PSTR(""), false);
        }

        oled_write_ln_P(PSTR("WPM"), false);
        oled_write_ln(get_u8_str(get_current_wpm(), '0'), false);
        oled_write_ln_P(PSTR(""), false);

        oled_write_os_logo();
    } else {
        static const char PROGMEM dasbob_logo[] = {
            0,  0,  0,  0,  0,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,  0,  0,  0,  0,  0,128,128,128,192,192,192,192,192,192,192,192,192,192,192,128,128,  0,  0,  0,  0,  0,  0,  0,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,  0,  0,  0,  0,  0,128,128,128,128,128,128,128,128,128,128,128,128,128,  0,  0,  0,128,128,128,128,128,192,192,192,192,192,  0,  0,  0,128,128,128,128,128,128,128,128,128,  0,  0,  0,  0,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,  0,  0,
            0,  0,126,127,255,255,255,255,255,255,255,199,247,255,255,255,255,255,255,255,255,127,252,255,255,255,255,255,255,255,255,255, 15,127,255,255,255,255,255,255,255,255,254,248,  0,126,127,255,255,255,255,255,255,255,199,247,255,255,255,255,255,255,255,255,127,126,255,255,255,255,255,255,255,255,239,239,239,239,207,223,223,223,159, 63,  0,  7, 63,255,255,255,255,255,255,255,254, 14,254,255,255,255,255,255,255,255, 63,241,252,254,255,255,255,255,255,255,255, 31,255,255,255,255,255,255,255,255,255,255,128,  0,
            0,  0,126,255,255,255,255,255,255,255,255,227,255,255,255,255,255,255,255,255,255,  0, 15, 63,127,255,255,255,255,255,255,255,248,255,255,255,255,255,255,255,255,127, 63,  7,  0,126,255,255,255,255,255,255,255,255,227,255,255,255,255,255,255,255,255,255,128,252,253,249,249,251,251,251,251,251,255,255,255,255,255,255,255,255,127, 30,  0,  0,  0,  3, 31,255,255,255,255,255,255,255,255,255,255,255,255,255,255,  7,  0, 63,255,255,255,255,255,255,255,255,255,248,255,255,255,255,255,255,255,255,255,255,255,  0,
            0,  0,  0,  0,  0,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  0,  0,  0,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  0,
        };
        oled_write_raw_P(dasbob_logo, sizeof(dasbob_logo));
    }
    return false;
}
#endif
