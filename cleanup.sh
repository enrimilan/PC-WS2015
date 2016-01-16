#!/bin/sh

SRC_DIR="src"
PLOT_DIR="plots"


echo -n "Removing compiled programs, logs and plots... "
rm -f $PLOT_DIR/*.png
cd $SRC_DIR
make clean > /dev/null
echo "Done!"