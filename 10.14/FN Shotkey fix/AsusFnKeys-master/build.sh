#!/bin/bash

sudo rm -rf build
sudo xcodebuild -alltargets -configuration Debug
sudo xcodebuild -alltargets -configuration Release

sudo cp ./AsusFnKeysDaemon/com.hieplpvip.AsusFnKeysDaemon.plist ./build/Debug
sudo cp ./AsusFnKeysDaemon/com.hieplpvip.AsusFnKeysDaemon.plist ./build/Release

sudo cp ./install_daemon.sh ./build/Debug
sudo cp ./install_daemon.sh ./build/Release

sudo chown -R root:wheel ./build/Debug/AsusFnKeys.kext
sudo chmod -R 755 ./build/Debug/AsusFnKeys.kext
sudo chown -R root:wheel ./build/Release/AsusFnKeys.kext
sudo chmod -R 755 ./build/Release/AsusFnKeys.kext
