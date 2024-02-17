#!/usr/bin/env bash

# PCB project
# git clone https://github.com/kata0510/bastardkb/charybdis/3x5.git

# QMK install and compile
# python3 -m pip install qmk
# cd ~/
# qmk setup

# sudo cp -f ~/qmk_firmware/util/udev/50-qmk.rules /etc/udev/rules.d/
rm -rf ~/qmk_firmware/keyboards/bastardkb/charybdis/3x5/keymaps/charles/
mkdir -p ~/qmk_firmware/keyboards/bastardkb/charybdis/3x5/keymaps/charles
cp charles_keymap.json ~/qmk_firmware/keyboards/bastardkb/charybdis/3x5/keymaps/charles/
cp custom.c ~/qmk_firmware/keyboards/bastardkb/charybdis/3x5/keymaps/charles/
cp config.h ~/qmk_firmware/keyboards/bastardkb/charybdis/3x5/
cp rules.mk ~/qmk_firmware/keyboards/bastardkb/charybdis/3x5/keymaps/charles/
cd ~/qmk_firmware/keyboards/bastardkb/charybdis/3x5/keymaps/charles
qmk json2c charles_keymap.json > keymap_generated.c
rm keymap.c
touch keymap.c
cat custom.c >> keymap.c
cat keymap_generated.c >> keymap.c
qmk flash -kb bastardkb/charybdis/3x5/v2/splinky_3 -km charles -bl uf2-split-right
# qmk flash -kb bastardkb/charybdis/3x5/v2/splinky_3 -km charles -bl uf2-split-left
