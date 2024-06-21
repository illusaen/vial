// Copyright 2023 QMK
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H
// #include "oneshot.h"
#include "swapper.h"

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
    OS_SHFT = SAFE_RANGE,
    OS_CTRL,
    OS_ALT,
    OS_CMD,

    SW_WIN, // Switch to next window         (cmd-tab)
};

// enum combo_events {
//     ENTER,
//     COMMENT,
// };

// const uint16_t PROGMEM enter_combo[] = {KC_SPACE, KC_A, COMBO_END};
// const uint16_t PROGMEM comment_combo[] = {KC_SPACE, KC_S, COMBO_END};

// combo_t key_combos[COMBO_COUNT] = {
//   [ENTER] = COMBO(enter_combo, KC_ENT),
//   [COMMENT] = COMBO(comment_combo, KC_SLSH),
// };

// void process_combo_event(uint16_t combo_index, bool pressed) {
//   switch(combo_index) {
//     case COMMENT:
//       if (pressed) {
//         tap_code16(LCTL(KC_SLSH));
//       }
//       break;
//   }
// }

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
     /*
      * ┌───┬───┬───┬───┬───┐       ┌───┬───┬───┬───┬───┐
      * │ Q │ W │ E │ p │ b │       │ j │ l │ u │ y │ ; │
      * ├───┼───┼───┼───┼───┤       ├───┼───┼───┼───┼───┤
      * │ A │ r │ s │ t │ G │       │ m │ n │ e │ i │ o │
      * ├───┼───┼───┼───┼───┤       ├───┼───┼───┼───┼───┤
      * │ Z │ X │ C │ d │ v │       │ k │ h │ , │ . │ / │
      * └───┴───┴───┴───┴───┘       └───┴───┴───┴───┴───┘
      *           ┌───┐                   ┌───┐
      *           │GUI├───┐           ┌───┤Alt│
      *           └───┤Bsp├───┐   ┌───┤Ent├───┘
      *               └───┤   │   │   ├───┘
      *                   └───┘   └───┘
      */
    [DEF] = LAYOUT_split_3x5_3(
        KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,
        KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_L,    KC_L,    KC_SCLN,
        KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_QUOT,
                                LA_NUM,  KC_SPC, LA_NAV,  LA_SYM, KC_SPC, KC_BSPC
    ),

    [COL] = LAYOUT_split_3x5_3(
        KC_Q,    KC_W,    KC_F,    KC_P,    KC_B,    KC_J,    KC_L,    KC_U,    KC_Y,    KC_QUOT,
        KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_M,    KC_N,    KC_E,    KC_I,    KC_O,
        KC_Z,    KC_X,    KC_C,    KC_D,    KC_V,    KC_K,    KC_H,    KC_COMM, KC_DOT,  KC_SCLN,
                                LA_NUM,  KC_SPC, LA_NAV,  LA_SYM, KC_SPC, KC_BSPC
    ),

    [NAV] = LAYOUT_split_3x5_3(
        KC_ESC,  SW_WIN,  C(KC_C), C(KC_V), KC_TAB,                             S(KC_TAB), KC_HOME, KC_UP,   KC_END,  KC_DEL,
        OSM(MOD_LSFT), OSM(MOD_LGUI), OSM(MOD_LALT), OSM(MOD_LCTL), C(KC_SPC),  CW_TOGG,   KC_LEFT, KC_DOWN, KC_RGHT, KC_ENT,
        OSM(MOD_LSFT|MOD_LCTL), C(KC_W), C(KC_S), C(KC_X), C(KC_Z),                            C(KC_Y),   KC_PGDN, KC_VOLU, KC_PGUP, KC_VOLD,
                                _______, _______, _______, _______, _______, _______
    ),

    [SYM] = LAYOUT_split_3x5_3(
        KC_ESC,  KC_LBRC, KC_LCBR, KC_LPRN, KC_TILD, KC_CIRC, KC_RPRN, KC_RCBR, KC_RBRC, KC_GRV,
        KC_MINS, KC_ASTR, KC_EQL,  KC_UNDS, KC_DLR,  KC_HASH, OSM(MOD_LCTL), OSM(MOD_LALT), OSM(MOD_LGUI), OSM(MOD_LSFT),
        KC_PLUS, KC_PIPE, KC_AT,   KC_SLSH, KC_PERC, KC_BSPC, KC_BSLS, KC_AMPR, KC_QUES, KC_EXLM,
                                _______, _______, _______, _______, _______, _______
    ),

    [NUM] = LAYOUT_split_3x5_3(
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                            KC_DOT,    KC_1,    KC_2,    KC_3,    KC_DEL,
        OSM(MOD_LSFT), OSM(MOD_LGUI), OSM(MOD_LALT), OSM(MOD_LCTL), XXXXXXX,    KC_EQL,    KC_4,    KC_5,    KC_6,    KC_0,
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                            KC_SLSH,   KC_7,    KC_8,    KC_9,    KC_MINS,
                                _______, _______, _______, _______, _______, _______
    ),

    [MIS] = LAYOUT_split_3x5_3(
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, DF(DEF), DF(COL), XXXXXXX, XXXXXXX,
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, CG_SWAP, CG_NORM, XXXXXXX, XXXXXXX,
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, KC_VOLU, KC_VOLD, XXXXXXX, XXXXXXX,
                                _______, _______, _______, _______, _______, _______
    ),
};

// bool is_oneshot_cancel_key(uint16_t keycode) {
//     switch (keycode) {
//     case LA_SYM:
//     case LA_NAV:
//         return true;
//     default:
//         return false;
//     }
// }

// bool is_oneshot_ignored_key(uint16_t keycode) {
//     switch (keycode) {
//     case LA_SYM:
//     case LA_NAV:
//     case KC_LSFT:
//     case OS_SHFT:
//     case OS_CTRL:
//     case OS_ALT:
//     case OS_CMD:
//         return true;
//     default:
//         return false;
//     }
// }

bool sw_win_active = false;
// bool is_windows_os = false;

// oneshot_state os_shft_state = os_up_unqueued;
// oneshot_state os_ctrl_state = os_up_unqueued;
// oneshot_state os_alt_state = os_up_unqueued;
// oneshot_state os_cmd_state = os_up_unqueued;

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    update_swapper(
        &sw_win_active, KC_LALT, KC_TAB, SW_WIN,
        keycode, record
    );

    // update_oneshot(
    //     &os_shft_state, KC_LSFT, OS_SHFT,
    //     keycode, record
    // );
    // update_oneshot(
    //     &os_ctrl_state, KC_LCTL, OS_CTRL,
    //     keycode, record
    // );
    // update_oneshot(
    //     &os_alt_state, KC_LALT, OS_ALT,
    //     keycode, record
    // );
    // update_oneshot(
    //     &os_cmd_state, KC_LCMD, OS_CMD,
    //     keycode, record
    // );

    // if (record->event.pressed == OS_MOD1) {
    //     if (is_windows_os) {

    //     }
    // }
    return true;
}

layer_state_t layer_state_set_user(layer_state_t state) {
    return update_tri_layer_state(state, SYM, NAV, MIS);
}

#ifdef OLED_ENABLE

oled_rotation_t oled_init_user(oled_rotation_t rotation) { return OLED_ROTATION_90; }
bool oled_task_user(void) {
    if (is_keyboard_master()) {
        static const char PROGMEM runqmk_logo[] = {0,0,0,0,152,152,152,152,152,152,24,24,24,
        152,152,24,24,24,152,152,24,24,152,152,24,24,24,152,152,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,127,127,13,13,29,63,103,64,0,63,127,96,96,96,127,63,0,0,127,127,7,12,56,
        127,127,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,120,252,134,198,198,252,120,0,0,254,254,
        60,224,224,60,254,254,0,254,254,16,24,124,230,130,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,24,24,25,25,25,25,27,24,24,25,25,24,25,25,24,25,25,24,25,25,24,24,24,24,25,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0};
        oled_write_raw_P(runqmk_logo, sizeof(runqmk_logo));

        led_t led_usb_state = host_keyboard_led_state();
        oled_set_cursor(6, 3);
        oled_write_P(led_usb_state.num_lock    ? PSTR("NUM") : PSTR(""), false);
        oled_set_cursor(6, 2);
        oled_write_P(PSTR("WPM: "), false);
        oled_write(get_u8_str(get_current_wpm(), '0'), false);
        oled_set_cursor(6, 0);
        oled_write_P(PSTR("by GroooveBob"), false);
            oled_set_cursor(6, 1);
            oled_write_P(PSTR("Layer: "), false);

    switch (get_highest_layer(layer_state)) {
        case DEF:
        oled_set_cursor(12, 1);
            oled_write_P(PSTR("Def\n"), false);
            break;
        case COL:
        oled_set_cursor(12, 1);
            oled_write_P(PSTR("Col\n"), false);
            break;
        case SYM:
        oled_set_cursor(12, 1);
            oled_write_P(PSTR("Sym\n"), false);
            break;
        case NAV:
        oled_set_cursor(12, 1);
            oled_write_P(PSTR("Nav\n"), false);
            break;
        case NUM:
        oled_set_cursor(12, 1);
            oled_write_P(PSTR("Num\n"), false);
            break;
        case MIS:
        oled_set_cursor(12, 1);
            oled_write_P(PSTR("Misc\n"), false);
            break;
        default:
            // Or use the write_ln shortcut over adding '\n' to the end of your string
        oled_set_cursor(6, 1);
        oled_write_ln_P(PSTR("Undefined"), false);
    }
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
