#!/bin/bash

echo " ----------------------------
   Updating package repositories
 ----------------------------"
sudo apt update
echo " ----------------------------
   Upgrading package repositories
 ----------------------------"
sudo apt upgrade
echo " ----------------------------
   Installing build-essential package
 ----------------------------"
sudo apt install build-essential
if [ ! -f "cuda_10.2.89_440.33.01_linux.run" ]; then
    echo " ----------------------------
   Downloading CUDA install file
 ----------------------------"
    wget http://developer.download.nvidia.com/compute/cuda/10.2/Prod/local_installers/cuda_10.2.89_440.33.01_linux.run
fi
echo " ----------------------------
   Installing CUDA driver e toolkit (10.2)
 ----------------------------"
sudo sh cuda_10.2.89_440.33.01_linux.run
echo " ----------------------------
   Installing gcc-8 and g++-8 package
 ----------------------------"
sudo apt install gcc-8 g++-8
echo " ----------------------------
   Installing libc6-dbg package
 ----------------------------"
sudo apt install libc6-dbg
echo " ----------------------------
   Installing python package
 ----------------------------"
sudo apt install python

