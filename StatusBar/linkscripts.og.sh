#!/bin/bash

#
# Link Scrips
#

# link dwmblocks scripts from project dir to ~/.local/bin/

existingscripts=$(ls ~/.local/bin/ | grep "sb")

read -n 1 -r -p "This script will overwrite files of the same name, Are you sure? [y/N] " response
case $response in
    [yY])
	ln -sf ~/.config/mf-dwmblocks/scripts/sb-* ~/.local/bin/StatusBar/ &&
	    echo -e "\nFiles linked"
        ;;
    *)
	echo -e "\nDoing nothing"
	exit 0
        ;;
esac
