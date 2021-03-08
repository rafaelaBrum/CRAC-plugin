#!/bin/bash
if [ ! -d "CRAC-plugin_code_paper/" ]; then
    echo " ----------------------------
  Cloning repository
 ----------------------------"
    git clone https://github.com/rafaelaBrum/CRAC-plugin.git --branch code_paper --single-branch CRAC-plugin_code_paper/
fi
if [ ! -d "CRAC-plugin_code_paper/bin/" ]; then
    echo " ----------------------------
  Compiling DMTCP and CRAC
 ----------------------------"
    cd CRAC-plugin_code_paper
    ./configure
    make -j4
    cd contrib/split-cuda
    make -j4
    cd ~/
fi
if [ -z $LD_LIBRARY_PATH ]; then
    echo " ----------------------------
  Updating LD_LIBRARY_PATH and PATH env variables
 ----------------------------"
    export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/home/ubuntu/CRAC-plugin_code_paper/contrib/split-cuda/:/usr/local/cuda-10.0/lib64
    export PATH=$PATH:/home/ubuntu/CRAC-plugin_code_paper/contrib/split-cuda:/home/ubuntu/CRAC-plugin_code_paper/bin:/usr/local/cuda-10.0/bin
fi
