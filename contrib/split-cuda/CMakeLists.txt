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

        utils/mtcp_util.c
        utils/procmapsutils.c
        utils/trampoline_setup.cpp
        utils/utils.cpp
        autogen/opengl_wrappers.cpp lh/opengl-lh-if.c log_and_replay/uh_logging.cpp
        autogen/opengl_replay.cpp)

target_include_directories(split-cuda PRIVATE utils common log_and_replay uh ../../include lh ../../jalib autogen)
