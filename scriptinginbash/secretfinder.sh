#!/bin/bash

###################################
#				  #
#	 Jonathan Torres	  #
#	 October 8, 2019	  #
#				  #
###################################

# creates a secretfiles directory
mkdir secretfiles

# changes directory to secretfiles
cd secretfiles

# copies grep.tar from location to the current directory
# cp /home/CSCI2467/labs/misc/grep.tar .

# extracts the contents of grep.tar
tar -xf ../grep.tar

# searches for the string 'SECRET AUTH CODE' within all of the files in secretfiles
# the result is redirected into authcode.txt
grep 'SECRET AUTH CODE' * > authcode.txt

# removes all 1000 files, then prints out to screen authcode.txt
rm f*== | rm grep.tar | echo "Found the secret auth code! Saved to authcode.txt"
