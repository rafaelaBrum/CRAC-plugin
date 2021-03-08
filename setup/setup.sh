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
if [ ! -f "cuda_10.0.130_410.48_linux.run" ]; then
    echo " ----------------------------
   Downloading CUDA install file
 ----------------------------"
    wget https://developer.nvidia.com/compute/cuda/10.0/Prod/local_installers/cuda_10.0.130_410.48_linux
fi
if [ ! -f "cuda_10.0.130.1_linux.run" ]; then
    echo " ----------------------------
   Downloading CUDA patch install file
 ----------------------------"
    wget http://developer.download.nvidia.com/compute/cuda/10.0/Prod/patches/1/cuda_10.0.130.1_linux.run
fi

echo " ----------------------------
   Installing CUDA driver e toolkit (10.0)
 ----------------------------"
sudo sh cuda_10.0.130_410.48_linux.run
sudo sh cuda_10.0.130.1_linux.run
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

