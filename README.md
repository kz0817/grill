# grill
grill is a library including a wide digit integer class and mathematical functions
using it.

# How to build

## Install depending library

### Leaf
    cd `<grill's top directory>`
    git clone git@github.com:kz0817/Leaf.git

or you can pull only the latest revision with `--depth 1`.

    git clone --detph 1 git@github.com:kz0817/Leaf.git


### Other packages

    sudo apt install automake autoconf make g++ m4 libtools autoconf libtool libboost-test-dev

## Build

    ./autogen.sh
    ./configure
    make
