#include QMK_KEYBOARD_H

#define _TILING 5
#define _MOUSE 8
#define _NAV 3

layer_state_t layer_state_set_user(layer_state_t state) {
  return update_tri_layer_state(state, 3, 2, 6);
}

bool get_hold_on_other_key_press(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {
  // Capture all mod-tap keycodes.
  case QK_MOD_TAP ... QK_MOD_TAP_MAX:
    if (keycode == RSFT_T(KC_ENT)) {
      // aka enable IGNORE_MOD_TAP_INTERRUPT for LCTL_T(KC_A).
      //
      // Enable HOLD_ON_OTHER_KEY_PRESS for every other mod-tap keycode.
      return true;
    } else {
      // Disable HOLD_ON_OTHER_KEY_PRESS for LCTL_T(KC_A)
      return false;
    }
  default:
    return false;
  }
}




#include "os_detection.h"
enum custom_keycodes {
  USR_UNDO = SAFE_RANGE,
  USR_CUT,
  USR_COPY,
  USR_PASTE,
  USR_PRINT,
  PREVWIN,
  NEXTWIN,
  MISSION_CONTROL,
};

static bool alt_tab_registered = false;

bool is_apple(void) {
  os_variant_t host_os = detected_host_os();
  return host_os == OS_MACOS || host_os == OS_IOS;
}

void process_platform_combo(uint16_t keycode, keyrecord_t *record) {
  uint16_t keycode_to_press = KC_NO;
  if (is_apple()) {
    switch (keycode) {
    case USR_UNDO:
      keycode_to_press = G(KC_Z);
      break;
    case USR_CUT:
      keycode_to_press = G(KC_X);
      break;
    case USR_COPY:
      keycode_to_press = G(KC_C);
      break;
    case USR_PASTE:
      keycode_to_press = G(KC_V);
      break;
    case USR_PRINT:
      keycode_to_press = SGUI(KC_4);
      break;
      // I use amethyst for this on mac, this is the shortcut
    case NEXTWIN:
      keycode_to_press = LSA(KC_W);
      break;
    case PREVWIN:
      keycode_to_press = LSA(KC_S);
      break;
    case MISSION_CONTROL:
      keycode_to_press = C(KC_UP);
      break;
    }
  } else {
    switch (keycode) {
    case USR_UNDO:
      keycode_to_press = C(KC_Z);
      break;
    case USR_CUT:
      keycode_to_press = C(KC_X);
      break;
    case USR_COPY:
      keycode_to_press = C(KC_C);
      break;
    case USR_PASTE:
      keycode_to_press = C(KC_V);
      break;
    case USR_PRINT:
      keycode_to_press = KC_PSCR;
      break;
    case NEXTWIN:
    case PREVWIN:
      if (record->event.pressed) {
        if (!alt_tab_registered && IS_LAYER_ON(_TILING)) {
          register_code(KC_LALT);
          alt_tab_registered = true;
        }
      }
      keycode_to_press = keycode == NEXTWIN ? KC_TAB : S(KC_TAB);
      break;
    case MISSION_CONTROL:
      keycode_to_press = KC_LGUI;
      break;
    }
  }
  if (record->event.pressed) {
    register_code16(keycode_to_press);
  } else {
    unregister_code16(keycode_to_press);
  }
}


bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {
  case USR_UNDO:
  case USR_CUT:
  case USR_COPY:
  case USR_PASTE:
  case USR_PRINT:
  case NEXTWIN:
  case PREVWIN:
  case MISSION_CONTROL:
    process_platform_combo(keycode, record);
    return false;
  }

  return true;
}

void matrix_scan_user(void) {
  if (alt_tab_registered) {
    if (IS_LAYER_OFF(_TILING)) {
      unregister_code(KC_LALT);
      alt_tab_registered = false;
    }

  }

  // Disable sniping and dragscroll if the mouse layer is off
  if((charybdis_get_pointer_dragscroll_enabled() || charybdis_get_pointer_sniping_enabled()) && (IS_LAYER_OFF(_MOUSE) && IS_LAYER_OFF(_NAV))){
    charybdis_set_pointer_dragscroll_enabled(false);
    charybdis_set_pointer_sniping_enabled(false);
  }
}

bool rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) {
    uint8_t layer = get_highest_layer(layer_state);
    if (layer == 1 || layer == 4) {
      RGB_MATRIX_INDICATOR_SET_COLOR(20, 255, 0, 0);
      return false;
    }


    if (layer == 8 ) {
      RGB_MATRIX_INDICATOR_SET_COLOR(23, 0, 255, 0);
      RGB_MATRIX_INDICATOR_SET_COLOR(24, 0, 255, 0);
      RGB_MATRIX_INDICATOR_SET_COLOR(29, 0, 255, 0);
      RGB_MATRIX_INDICATOR_SET_COLOR(15, 255, 0, 0);
      return false;
    }
    return false;
}
