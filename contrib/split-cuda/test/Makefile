# NOTE: Update the following variables for your system
CC=gcc
CXX=g++
NVCC=/usr/local/cuda-10.0/bin/nvcc
LD=${CXX}
CUDA_INCLUDE_PATH=/usr/local/cuda-10.0/include/
CRAC_FOLDER=/home/rafaela/CRAC-plugin_code_paper/contrib/split-cuda/


# Wrapper library against which the target application will be linked.
WRAPPER_LIB=cuda_wrappers

INCLUDE_FLAGS= 
WARNING_FLAGS=-Wall -Wno-deprecated-declarations -Werror
NVCC_FLAGS=-ccbin=/usr/bin/g++-7 -Xcompiler '-g3 -O0 $(WARNING_FLAGS)'

FILES=simple_addition/simple_addition malloc_across_kernel/malloc_across_kernel \
      read_device_var/read_device_var read_uvm_var/read_uvm_var \
      rw_device_var/rw_device_var rw_dev_array/rw_dev_array \
      texture_test/texture_test uvm_var/uvm_var uvm_args/uvm_args \
      uvm_output_args/uvm_output_args uvm_var_managed/uvm_var_managed \
      texture_object_test/texture_object_test target
# cudaCreateChannelDesc_test cuda_plus_mpi

OBJS=$(addsuffix .o, ${FILES})

#TESTS_DUMMY=$(addsuffix .dummy.exe, ${FILES})
TESTS=$(addsuffix .exe, ${FILES})
#LDFLAGS_DUMMY=-L.. -lcuda_wrappers

default: ${TESTS} ${TESTS_DUMMY}

%.o: %.c
	${NVCC} ${INCLUDE_FLAGS} ${NVCC_FLAGS} -c $< -o $@

%.o: %.c
	${NVCC} ${INCLUDE_FLAGS} ${NVCC_FLAGS} -c $< -o $@

%.o: %.cu
	${NVCC} ${INCLUDE_FLAGS} ${NVCC_FLAGS} -c $< -o $@

%.exe: %.o
	${LD} $< -o $@ -L${CRAC_FOLDER} -l${WRAPPER_LIB}

%.exe: %.cu
	$(NVCC) ${INCLUDE_FLAGS} ${NVCC_FLAGS} --cudart shared -o $@ $<

%.dummy.exe: %.o
	$(LD) -o $@ $< ${LDFLAGS_DUMMY}

tidy_simple_addition:
	rm -f simple_addition/ckpt_*.dmtcp simple_addition/dmtcp_restart_script* \
	simple_addition/dmtcp-shared-memory.* simple_addition/lhInfo* \
	simple_addition/uhInfo* simple_addition/dmtcp-test-typescript.tmp \
	simple_addition/core*
	rm -rf simple_addition/ckpt_*

tidy_malloc_across_kernel: tidy_simple_addition
	rm -f malloc_across_kernel/ckpt_*.dmtcp malloc_across_kernel/lhInfo* \
	malloc_across_kernel/dmtcp_restart_script* malloc_across_kernel/uhInfo* \
	malloc_across_kernel/dmtcp-shared-memory.* \
	malloc_across_kernel/dmtcp-test-typescript.tmp malloc_across_kernel/core*
	rm -rf malloc_across_kernel/ckpt_*

tidy_read_device_var: tidy_malloc_across_kernel
	rm -f read_device_var/ckpt_*.dmtcp read_device_var/dmtcp_restart_script* \
	read_device_var/dmtcp-shared-memory.* read_device_var/core* \
	read_device_var/dmtcp-test-typescript.tmp read_device_var/lhInfo* \
	read_device_var/uhInfo*
	rm -rf read_device_var/ckpt_*

tidy_read_uvm_var: tidy_read_device_var
	rm -f read_uvm_var/ckpt_*.dmtcp read_uvm_var/dmtcp_restart_script* \
	read_uvm_var/dmtcp-shared-memory.* read_uvm_var/dmtcp-test-typescript.tmp \
	read_uvm_var/core* read_uvm_var/lhInfo* read_uvm_var/uhInfo*
	rm -rf read_uvm_var/ckpt_*

tidy_rw_device_var: tidy_read_uvm_var
	rm -f rw_device_var/ckpt_*.dmtcp rw_device_var/dmtcp_restart_script* \
	rw_device_var/dmtcp-shared-memory.* rw_device_var/dmtcp-test-typescript.tmp \
	rw_device_var/core* rw_device_var/lhInfo* rw_device_var/uhInfo*
	rm -rf rw_device_var/ckpt_*

tidy_rw_dev_array: tidy_rw_device_var
	rm -f rw_dev_array/ckpt_*.dmtcp rw_dev_array/dmtcp_restart_script* \
	rw_dev_array/dmtcp-shared-memory.* rw_dev_array/dmtcp-test-typescript.tmp \
	rw_dev_array/core* rw_dev_array/lhInfo* rw_dev_array/uhInfo*
	rm -rf rw_dev_array/ckpt_*

tidy_texture_test: tidy_rw_dev_array
	rm -f texture_test/ckpt_*.dmtcp texture_test/dmtcp_restart_script* \
	texture_test/dmtcp-shared-memory.* texture_test/dmtcp-test-typescript.tmp \
	texture_test/core* texture_test/lhInfo* texture_test/uhInfo*
	rm -rf texture_test/ckpt_*

tidy_uvm_var: tidy_texture_test
	rm -f uvm_var/ckpt_*.dmtcp uvm_var/dmtcp_restart_script* \
	uvm_var/dmtcp-shared-memory.* uvm_var/dmtcp-test-typescript.tmp \
	uvm_var/core* uvm_var/lhInfo* uvm_var/uhInfo*
	rm -rf uvm_var/ckpt_*

tidy_uvm_args: tidy_uvm_var
	rm -f uvm_args/ckpt_*.dmtcp uvm_args/dmtcp_restart_script* \
	uvm_args/dmtcp-shared-memory.* uvm_args/dmtcp-test-typescript.tmp \
	uvm_args/core* uvm_args/lhInfo* uvm_args/uhInfo*
	rm -rf uvm_args/ckpt_*

tidy_uvm_output_args: tidy_uvm_args
	rm -f uvm_output_args/ckpt_*.dmtcp uvm_output_args/dmtcp_restart_script* \
	uvm_output_args/dmtcp-shared-memory.* uvm_output_args/core* \
	uvm_output_args/dmtcp-test-typescript.tmp uvm_output_args/lhInfo* \
	uvm_output_args/uhInfo*
	rm -rf uvm_output_args/ckpt_*

tidy_uvm_var_managed: tidy_uvm_output_args
	rm -f uvm_var_managed/ckpt_*.dmtcp uvm_var_managed/dmtcp_restart_script* \
	uvm_var_managed/dmtcp-shared-memory.* uvm_var_managed/core* \
	uvm_var_managed/dmtcp-test-typescript.tmp uvm_var_managed/lhInfo* \
	uvm_var_managed/uhInfo*
	rm -rf uvm_var_managed/ckpt_*

tidy_texture_object_test: tidy_uvm_var_managed
	rm -f texture_object_test/ckpt_*.dmtcp texture_object_test/core* \
	texture_object_test/dmtcp_restart_script* texture_object_test/lhInfo* \
	texture_object_test/dmtcp-shared-memory.* texture_object_test/uhInfo* \
	texture_object_test/dmtcp-test-typescript.tmp
	rm -rf texture_object_test/ckpt_*

tidy: tidy_texture_object_test
	rm -f ckpt_*.dmtcp dmtcp_restart_script* \
	dmtcp-shared-memory.* dmtcp-test-typescript.tmp core* \
	lhInfo* uhInfo*
	rm -rf ckpt_*

clean: tidy
	rm -f $(TESTS) ${OBJS} ${TESTS_DUMMY} *.pyc *.so
