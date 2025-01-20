#!/usr/bin/env bash

# PCB project
# git clone https://github.com/kata0510/ploopyco/madromys.git

# QMK install and compile
# python3 -m pip install qmk
# cd ~/
# qmk setup

# sudo cp -f ~/qmk_firmware/util/udev/50-qmk.rules /etc/udev/rules.d/
rm -rf ~/qmk_firmware/keyboards/ploopyco/madromys/keymaps/charles/
mkdir -p ~/qmk_firmware/keyboards/ploopyco/madromys/keymaps/charles
cp keymap.c ~/qmk_firmware/keyboards/ploopyco/madromys/keymaps/charles/
cp -R ../charybids/maccel/ ~/qmk_firmware/keyboards/ploopyco/madromys/keymaps/charles/
cp config.h ~/qmk_firmware/keyboards/ploopyco/madromys/
cp rules.mk ~/qmk_firmware/keyboards/ploopyco/madromys/keymaps/charles/
cd ~/qmk_firmware/keyboards/ploopyco/madromys/keymaps/charles
# qmk flash -kb ploopyco/madromys/v2/splinky_3 -km charles -bl uf2-split-right
qmk flash -kb ploopyco/madromys -km charles
