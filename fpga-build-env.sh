#!/usr/bin/env bash
export JAVA_HOME=$INSTALL_PREFIX/jdk-11.0.15+10
export PATH=$JAVA_HOME/bin:$(pwd)/installed/bin:$PATH
export LD_LIBRARY_PATH=$(pwd)/installed/lib:$(pwd)/installed/lib64:$LD_LIBRARY_PATH

#initialize Intel FPGA OpenCL environment

export QUARTUSDIR=/opt/intel/intelFPGA_pro/21.4

source $QUARTUSDIR/hld/init_opencl.sh
echo $INTELFPGAOCLSDKROOT
export ALTERAOCLSDKROOT=$INTELFPGAOCLSDKROOT

#set up BITSTREAM variable for required FPGA image (can be different different for varius implemented kernels)
export BITSTREAM=src/runtime/local/kernels/FPGAOPENCL/gemm.aocx  # SGEMM computational kernel

