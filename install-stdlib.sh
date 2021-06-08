#!/bin/bash

./build-stdlib.sh

LIB_INSTALL=/usr/local/lib/orka

if [[ ! -d $LIB_INSTALL ]] ; then
    sudo mkdir -p $LIB_INSTALL
fi

sudo cp build/occ_start.o $LIB_INSTALL
sudo cp build/liborka_corelib.a /usr/local/lib

sudo ldconfig

echo "Done"

