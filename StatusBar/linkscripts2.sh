#!/bin/bash

#
# Link Scrips
#

# link dwmblocks scripts from project dir to ~/.local/bin/

# scriptdir=$HOME'/Code/sh/StatusBar'
# destination=$HOME'/.local.bin'

scriptdir=$HOME'/Code/Test/DIR1'
destination=$HOME'/Code/Test/DIR2'


# existingscripts=$(ls "$destination" | grep "sb")
existingscripts=$(find "$destination" -type f -name "sb*")

read -n 1 -r -p "Replace[y], Query[i], Cancel[N] " response
case $response in

    [iI])
	for file in "$scriptdir"/*; do
	    filename=$(basename "$file")
	    # Check if the file also exists in DIR2
	    if [ -f "$destination/$filename" ]; then
		echo -e "\nDo you want to replace $filename in DIR1 with $filename from DIR2? [y/N]"
		read -r response
		case $response in
		    [Yy]*)
			# Replace the file
			cp "$scriptdir/$filename" "$destination/$filename"
			echo "File $filename replaced."
			;;
		    *)
			echo "Skipping $filename."
			;;
		esac
	    fi
	done
	;;
    [yY])
	ln -sf ~/.config/mf-dwmblocks/scripts/sb-* ~/.local/bin/StatusBar/ &&
	    echo -e "\nFiles linked"
        ;;
    *)
	echo -e "\nDoing nothing"
	exit 0
        ;;
esac
