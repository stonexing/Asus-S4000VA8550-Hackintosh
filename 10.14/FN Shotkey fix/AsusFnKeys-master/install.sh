#!/bin/bash

sudo sh build.sh
sudo sh mount_efi.sh
sudo rm -rf /Volumes/EFI/EFI/CLOVER/kexts/Other/AsusFnKeys.kext
sudo cp -a ./build/Debug/AsusFnKeys.kext /Volumes/EFI/EFI/CLOVER/kexts/Other
sudo sh ./build/Debug/install_daemon.sh
echo "Installed to EFI"
