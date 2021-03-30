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
if [ ! -f "cuda-repo-ubuntu1804_10.0.130-1_amd64.deb" ]; then
    echo " ----------------------------
   Downloading CUDA deb file
 ----------------------------"
   wget https://developer.download.nvidia.com/compute/cuda/repos/ubuntu1804/x86_64/cuda-repo-ubuntu1804_10.0.130-1_amd64.deb
fi
echo " ----------------------------
   Installing CUDA driver e toolkit (10.0)
 ----------------------------"
sudo apt-key adv --fetch-keys https://developer.download.nvidia.com/compute/cuda/repos/ubuntu1804/x86_64/7fa2af80.pub && sudo apt update
sudo dpkg -i cuda-repo-ubuntu1804_10.0.130-1_amd64.deb
sudo apt update
sudo apt install -y cuda-10-0
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

