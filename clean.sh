#!/bin/bash
source "lib/fasada/_config_template/screen.ini"
echo -e $COLOR1"All non-needed files in '" `pwd` "' will be cleaned"$COLOR2
echo -e "Press Ctrl-C in 4 seconds!"$NORMCO
sleep 4
rm -f service.log
rm -f .qtcreator.err
rm -f *.*~
rm -f *.cmake
rm -f CMakeCache.txt
rm -f CMakeLists.txt.user
rm -f Makefile
rm -R -f CMakeFiles
echo -e $COLOR2"Also qtcreator projects!?!?\nPress Ctrl-C in 4 seconds!"$NORMCO
sleep 4
rm -R -f build-*
