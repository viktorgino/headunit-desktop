#!/bin/bash

### Build headunit 

cd /opt

if [ "$COPYFOLDER" == "" ]; then
    if [ -d "headunit-desktop" ]; then
        cd headunit-desktop
        echo "Pulling headunit-desktop and submodules"
        git reset --hard
        git clean -fd
        git pull
        git submodule foreach git reset --hard
        git submodule foreach git clean -fd
        git submodule update --init --recursive
    else
        echo "Cloning headunit-desktop from https://github.com/viktorgino/headunit-desktop.git"
        git clone --recursive --depth 1 -j6 https://github.com/viktorgino/headunit-desktop.git
    fi
fi

cd /opt/headunit-desktop

#Generate protobuf with proto
if [ ! -f /opt/headunit-desktop/headunit/hu/generated.x64/hu.pb.cc ]; then
    echo "Generating Protobuf classes"
    protoc --proto_path=headunit/hu/ --cpp_out=headunit/hu/generated.x64/ headunit/hu/hu.proto
fi
#compile headunit-desktop
if [ "$CLEAN" == 1 ]; then
cat << EOF
__________________________________

Running make clean...

¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯
EOF
    make clean
    if [ "$WELLEIO" == 1 ]; then
        echo "Will be building with welle.io"
        if [ "$DRYRUN" == 0 ]; then
            qmake CONFIG+=welleio -config release 
        fi
    else
        echo "Will be building w/o welle.io"
        if [ "$DRYRUN" == 0 ]; then
            qmake -config release 
        fi
    fi
fi
if [ "$DRYRUN" == 0 ]; then
    make -j4
fi
