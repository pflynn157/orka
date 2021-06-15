#!/bin/bash

./build-stdlib.sh

LIB_INSTALL=/usr/local/lib/orka
INCLUDE_INSTALL=/usr/local/include/orka

if [[ ! -d $LIB_INSTALL ]] ; then
    sudo mkdir -p $LIB_INSTALL
fi

if [[ ! -d $INCLUDE_INSTALL ]] ; then
    sudo mkdir -p $INCLUDE_INSTALL
fi

sudo cp build/occ_start.o $LIB_INSTALL
sudo cp build/liborka_corelib.a /usr/local/lib

sudo cp -r corelib/include/* $INCLUDE_INSTALL

sudo ldconfig

echo "Done"

