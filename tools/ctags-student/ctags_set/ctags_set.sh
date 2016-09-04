#!/bin/bash 
#版本日期 2014年10月30日

rm -rf ctags-5.8
tar xvf ctags-5.8.tar.gz
cd ctags-5.8 
./configure
make
sudo make install
cd ../
rm -rf ./ctags-5.8

cd /usr/include/ 
ctags -R
sudo echo   "set tags+=/usr/include/tags" >> ~/.vimrc

echo -e "\n"
ctags --version
echo -e "\n"
