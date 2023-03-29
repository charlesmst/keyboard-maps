
layer_state_t layer_state_set_user(layer_state_t state) {
  return update_tri_layer_state(state, 3, 2, 6);
}

bool encoder_update_user(uint8_t index, bool clockwise) {
    if (index == 0) { /* First encoder */
        if (clockwise) {
            tap_code_delay(KC_VOLU, 10);
        } else {
            tap_code_delay(KC_VOLD, 10);
        }
    } else if (index == 1) { /* Second encoder */
        if (clockwise) {
            tap_code_delay(KC_PGUP, 10);
        } else {
            tap_code_delay(KC_PGDN, 10);
        }
    }
    return false;
}

void keyboard_post_init_user(void) {
  wait_ms(2000);
  oled_init(OLED_ROTATION_0);
}
