#!/usr/bin/env bash

# PCB project
# git clone https://github.com/kata0510/boardsource/lulu.git

# QMK install and compile
# python3 -m pip install qmk
# cd ~/
# qmk setup

sudo cp -f ~/qmk_firmware/util/udev/50-qmk.rules /etc/udev/rules.d/
rm -rf ~/qmk_firmware/keyboards/boardsource/lulu/keymaps/charles/
mkdir -p ~/qmk_firmware/keyboards/boardsource/lulu/keymaps/charles
cp charles_keymap.json ~/qmk_firmware/keyboards/boardsource/lulu/keymaps/charles/
cp custom.c ~/qmk_firmware/keyboards/boardsource/lulu/keymaps/charles/
cp config.h ~/qmk_firmware/keyboards/boardsource/lulu/
cp rules.mk ~/qmk_firmware/keyboards/boardsource/lulu/keymaps/charles/
cd ~/qmk_firmware/keyboards/boardsource/lulu/keymaps/charles
qmk json2c charles_keymap.json > keymap.c
cat custom.c >> keymap.c
cd ~/qmk_firmware
make clean
make boardsource/lulu/avr:charles:flash
