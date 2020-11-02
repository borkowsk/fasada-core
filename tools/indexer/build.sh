#./bin/bash
# Part of Fasada Toolbox
#
# Fasada-core is an open source shareware
# Please contact the author about licensing
# 2019 - 2020 (c) Wojciech Borkowski wborkowsk@gmail.com

# You may need to install libmagic before
#
#    sudo apt-get install -y libmagic-dev #https://zoomadmin.com/HowToInstall/UbuntuPackage/libmagic-dev
#
# Compilation:
UMAGIC="-DUSE_MAGIC"
LMAGIC="-lmagic"
g++ -std=c++11 -Wall -pedantic $UMAGIC indexer_main.cpp $LMAGIC -lboost_system -lboost_filesystem \
&& echo -e "\nCompiled\n" \
&& ./a.out $1 $2 \
&& echo -e "\nReady\n"
if [ $? -eq 0 ]; 
then
    mv a.out indexer
fi
