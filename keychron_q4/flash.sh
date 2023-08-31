#!/usr/bin/env bash

# PCB project
# git clone https://github.com/kata0510/Lily58.git

# QMK install and compile
# python3 -m pip install qmk
# cd ~/
# qmk setup

rm -rf ~/personal/keychron/qmk_firmware/keyboards/keychron/q4/ansi_v2/keymaps/charles
mkdir -p ~/personal/keychron/qmk_firmware/keyboards/keychron/q4/ansi_v2/keymaps/charles/
cp keymap.json ~/personal/keychron/qmk_firmware/keyboards/keychron/q4/ansi_v2/keymaps/charles/
cp custom.c ~/personal/keychron/qmk_firmware/keyboards/keychron/q4/ansi_v2/keymaps/charles/
cp rules.mk ~/personal/keychron/qmk_firmware/keyboards/keychron/q4/ansi_v2/keymaps/charles/
cd ~/personal/keychron/qmk_firmware/keyboards/keychron/q4/ansi_v2/keymaps/charles
qmk json2c keymap.json > keymap_generated.c
rm keymap.c
rm keymap.json
touch keymap.c
cat custom.c >> keymap.c
cat keymap_generated.c >> keymap.c
cd ~/personal/keychron/qmk_firmware
make clean
make keychron/q4/ansi_v2:charles:flash
