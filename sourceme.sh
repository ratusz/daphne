#!/usr/bin/env bash
cd /home/mdokter/code/daphne-build-env/ ; source env.sh ; cd -

#initialize Intel FPGA OpenCL environment
source /opt/intel/intelFPGA_pro/21.4/hld/init_opencl.sh
echo $INTELFPGAOCLSDKROOT
export ALTERAOCLSDKROOT=$INTELFPGAOCLSDKROOT

#set up BITSTREAM variable for required FPGA image (can be different different for varius implemented kernels)
export BITSTREAM=/home/pratuszniak/pratuszn/daphne/src/runtime/local/kernels/FPGAOPENCL/gemm.aocx  # SGEMM computational kernel

