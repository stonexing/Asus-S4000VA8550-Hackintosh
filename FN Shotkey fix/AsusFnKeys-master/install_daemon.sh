#!/bin/bash
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null && pwd )"

launchctl unload /Library/LaunchAgents/com.hieplpvip.AsusFnKeysDaemon.plist 2>/dev/null

sudo cp $DIR/AsusFnKeysDaemon /usr/bin
sudo chmod 755 /usr/bin/AsusFnKeysDaemon
sudo chown root:wheel /usr/bin/AsusFnKeysDaemon

sudo cp $DIR/com.hieplpvip.AsusFnKeysDaemon.plist /Library/LaunchAgents
sudo chmod 644 /Library/LaunchAgents/com.hieplpvip.AsusFnKeysDaemon.plist
sudo chown root:wheel /Library/LaunchAgents/com.hieplpvip.AsusFnKeysDaemon.plist

launchctl load /Library/LaunchAgents/com.hieplpvip.AsusFnKeysDaemon.plist
