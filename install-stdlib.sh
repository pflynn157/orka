#!/bin/bash

./build-stdlib.sh

if [ "$EUID" -ne 0 ] ; then
    echo "Error: Please run as root."
    exit 1
fi

LIB_INSTALL=/usr/local/lib/orka
INCLUDE_INSTALL=/usr/local/include/orka

if [[ ! -d $LIB_INSTALL ]] ; then
    sudo mkdir -p $LIB_INSTALL
fi

if [[ ! -d $INCLUDE_INSTALL ]] ; then
    sudo mkdir -p $INCLUDE_INSTALL
fi

sudo cp build/occ_start.o $LIB_INSTALL
sudo cp build/liborka_corelib.a /usr/lib
sudo cp build/stdlib/liborka.so /usr/lib

sudo cp -r stdlib/include/* $INCLUDE_INSTALL

sudo ldconfig

echo "Done"

