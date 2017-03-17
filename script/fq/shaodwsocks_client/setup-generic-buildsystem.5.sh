#!/bin/sh
#
# Script to setup the environment for openvpn-build/generic and openvpn-build/windows-nsis 

BUILD_DEPS="mingw-w64 man2html dos2unix nsis unzip wget curl autoconf libtool"
OSSLSIGNCODE_DEPS="libssl-dev libcurl4-openssl-dev build-essential"
OSSLSIGNCODE_URL="http://sourceforge.net/projects/osslsigncode/files/latest/download"
OSSLSIGNCODE_PACKAGE="osslsigncode-latest.tar.gz"
OPENVPN_BUILD_URL="https://github.com/OpenVPN/openvpn-build.git"
MINGW_PACKAGES="mingw-w64 mingw-w64-common mingw-w64-i686-dev mingw-w64-x86-64-dev"
NSIS_PACKAGES="nsis nsis-common nsis-doc nsis-pluginapi"
GIT_PKG="git"

check_if_root() {
    if ! [ `whoami` = "root" ]; then
            echo "ERROR: you must run this script as root!"
            exit 1
    fi
}

usage() {

    echo "Usage: setup-generic-buildsystem.sh <oscodename>"
    echo
    echo "Parameter <oscodename> is:"
    echo
    echo "    trusty  (Ubuntu 14.04)"
    echo
    echo "Example:"
    echo
    echo "   setup-generic-buildsystem.sh trusty"
    echo
    exit 1
}

install_packages() {

    # We need patched .deb files fix a few serious issues:
    #
    # 1) On Ubuntu 12.04-14.04 the stock nsis version does not support long strings
    #    required when extremely longs paths are used (Trac #465). In addition
    #    the MinGW headers require patching since OpenVPN 2.3.8 or so.
    #
    wget -O - https://swupdate.openvpn.net/repos/repo-public.gpg|apt-key add -
    echo "deb http://build.openvpn.net/apt $OSCODENAME main" > /etc/apt/sources.list.d/build.openvpn.net.list
    apt-get update
    #echo $BUILD_DEPS $GIT_PKG $GNUEABI_PKG $MINGW_PACKAGES $NSIS_PACKAGES
    apt-get -y install $BUILD_DEPS $GIT_PKG $GNUEABI_PKG $MINGW_PACKAGES $NSIS_PACKAGES
}

# osslsigncode is required for signing the binaries and installers
install_osslsigncode() {
    apt-get -y install $OSSLSIGNCODE_DEPS
    curl -L $OSSLSIGNCODE_URL > $OSSLSIGNCODE_PACKAGE
    tar -zxf $OSSLSIGNCODE_PACKAGE
    cd osslsigncode-*
    ./configure
    make
    make install
    cd ..
}

clone_openvpn_build() {
    if ! [ -d "openvpn-build" ]; then
        git clone $OPENVPN_BUILD_URL
    fi
}

# Main script

export OSCODENAME=$1

if [ "$OSCODENAME" = "trusty" ]; then
    GNUEABI_PKG="gcc-4.7-arm-linux-gnueabi"
else
    echo "ERROR: unknown oscodename"
    echo
    usage
fi

check_if_root
install_packages
install_osslsigncode
clone_openvpn_build
