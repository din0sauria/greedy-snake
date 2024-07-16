#!/bin/bash

set -o pipefail
set -o errexit

WK_DIR=$(cd `dirname $0`; pwd)

data_file="$WK_DIR/data/map.txt"
computer="$WK_DIR/bin/computer"
player="$WK_DIR/bin/player"
log_dir="$WK_DIR/log/judge"
ok=""

RED_COLOR='\E[1;31m'
RES='\E[0m'

if [ "$1" == "--visible" ]; then
    ok="true"
else
    echo -e "[TIPS] You could watch the visual process of the game with the additional parameter: ${RED_COLOR}./run.sh --visible${RES}"
    ok="false"
fi

cd $WK_DIR

ulimit -s 524288

if [ ! -d $WK_DIR/bin ]; then
    echo "[INFO] Make directory $WK_DIR/bin ........"
    mkdir $WK_DIR/bin
fi

if [ ! -d $WK_DIR/log ]; then
    echo "[INFO] Make directory $WK_DIR/log ........"
    mkdir $WK_DIR/log
fi

if [ ! -d $WK_DIR/log/judge ]; then
    echo "[INFO] Make directory $WK_DIR/log/judge..."
    mkdir $WK_DIR/log/judge
fi




echo "[INFO] Make project .................."

echo "[INFO] Check computer ................"
make check_computer
./bin/check_computer $data_file
echo "[INFO] Computer pass the check"

echo "[INFO] Check player .................."
make check_player
./bin/check_player $data_file
echo "[INFO] Player pass the check"

echo "[INFO] Make computer ................."
make computer

echo "[INFO] Make player ..................."
make player

echo "[INFO] Game is running ..............."
./bin/judge --data_file="$data_file" --player_red="$computer" --player_blue="$player" --log_dir="$log_dir" --visible="$ok"
echo "[INFO] The game is over"

