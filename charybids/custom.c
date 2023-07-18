#include QMK_KEYBOARD_H

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
