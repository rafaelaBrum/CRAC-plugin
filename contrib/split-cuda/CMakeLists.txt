cmake_minimum_required(VERSION 3.10)

project(split-cuda)

add_executable(split-cuda
        lh/cuda-lh-if.c
        lh/custom-loader.cpp
        lh/kernel-loader.cpp
        lh/mem-restore.c
        lh/mmap-wrapper.cpp
        lh/replay_cuda_calls.cpp
        lh/sbrk-wrapper.cpp

        uh/cuda-plugin.cpp
        uh/switch_context.cpp
        uh/upper-half-wrappers.cpp

        log_and_replay/log_cuda_api.cpp

        utils/mtcp_util.c
        utils/procmapsutils.c
        utils/trampoline_setup.cpp
        utils/utils.cpp
        autogen/opengl_wrappers.cpp lh/opengl-lh-if.c )

target_include_directories(split-cuda PRIVATE utils common log_and_replay uh ../../include lh ../../jalib autogen)
