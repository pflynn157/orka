#!/bin/bash

if [ "$EUID" -ne 0 ] ; then
    echo "Error: Please run as root."
    exit 1
fi

echo "Installing..."

if [[ ! -f build/src/occ ]] ; then
    echo "Error: The compiler does not seem to be built."
    echo ""
    exit 1
fi

./install-stdlib.sh

install ./build/src/occ /usr/local/bin/okcc

echo "Done"

exit 0
