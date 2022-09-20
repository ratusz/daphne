/*
 * Copyright 2021 The DAPHNE Consortium
 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

//#include "AOCLUtils/aocl_utils.h"
//#include "CL/opencl.h"


void FPGAContext::destroy() {
#ifndef NDEBUG
    std::cout << "Destroying FPGA context..." << std::endl;
#endif
//    CHECK_CUBLAS(cublasDestroy(cublas_handle));
}

void FPGAContext::init() {
//    CHECK_CUDART(cudaSetDevice(device_id));
//    CHECK_CUDART(cudaGetDeviceProperties(&device_properties, device_id));

//    size_t available; size_t total;
//    cudaMemGetInfo(&available, &total);
    // ToDo: make this a user config item
//    float mem_usage = 0.9f;
//    mem_budget = total * mem_usage;

//#ifndef NDEBUG
//    std::cout << "Using CUDA device " << device_id << ": " << device_properties.name  << "\nAvailable mem: "
//            << available << " Total mem: " << total << " using " << mem_usage * 100 << "% thereof -> " << mem_budget
//           << std::endl;
//#endif
//    CHECK_CUBLAS(cublasCreate(&cublas_handle));
//    CHECK_CUSPARSE(cusparseCreate(&cusparse_handle));
//    CHECK_CUDNN(cudnnCreate(&cudnn_handle));
}

std::unique_ptr<IContext> FPGAContext::createFpgaContext(int device_id) {

#ifndef NDEBUG
    std::cout << "creating FPGA context..." << std::endl;
#endif
//    int device_count = -1;
//    CHECK_CUDART(cudaGetDeviceCount(&device_count));

#ifndef NDEBUG
    DPRINTF("\n===== Host-CPU setting up the OpenCL platform and device ======\n\n");
#endif
    // Use this to check the output of each API call
    cl_int status;

    //----------------------------------------------
    // Discover and initialize the platforms
    //----------------------------------------------
    cl_uint numPlatforms = 0;
    cl_platform_id *platforms = NULL;

    // Use clGetPlatformIDs() to retrieve the
    // number of platforms
    status = clGetPlatformIDs(0, NULL, &numPlatforms);
#ifndef NDEBUG
    DPRINTF("Number of platforms = %d\n", numPlatforms);
#endif
 
    // Allocate enough space for each platform
    // platforms = (cl_platform_id*) acl_aligned_malloc (numplatforms * sizeof(cl_platform_id));
    platforms = (cl_platform_id *)malloc(numPlatforms * sizeof(cl_platform_id));

#ifndef NDEBUG
    DPRINTF("Allocated space for Platform\n");
#endif
 
    // Fill in platforms with clGetPlatformIDs()
    status = clGetPlatformIDs(numPlatforms, platforms, NULL);
    CHECK(status);
#ifndef NDEBUG
    DPRINTF("Filled in platforms\n");
#endif
 
    //----------------------------------------------
    // Discover and initialize the devices
    //----------------------------------------------

    cl_uint numDevices = 0;

    // Device info
    char buffer[4096];
    unsigned int buf_uint;
    int device_found = 0;
    const cl_uint maxDevices = 4;
    cl_device_id devices[maxDevices];
#ifndef NDEBUG
    DPRINTF("Initializing IDs\n");
#endif
    for (int i = 0; i < (int)numPlatforms; i++) {
        status = clGetDeviceIDs(platforms[i],
                                CL_DEVICE_TYPE_ALL,
                                maxDevices,
                                devices,
                                &numDevices);

        if (status == CL_SUCCESS) {
            clGetPlatformInfo(platforms[i],
                              CL_PLATFORM_NAME,
                              4096,
                              buffer,
                              NULL);
#if defined(ALTERA_CL)
            if (strstr(buffer, "Altera") != NULL) {
                device_found = 1;
            }
//            DPRINTF("%s\n", buffer);
#elif defined(NVIDIA_CL)
            if (strstr(buffer, "NVIDIA") != NULL) {
                device_found = 1;
            }
#else
            if (strstr(buffer, "Intel") != NULL) {
                device_found = 1;
            }
#endif
#ifndef NDEBUG
            DPRINTF("Platform found : %s\n", buffer);
#endif
	    device_found = 1;
        }
    }

    if (!device_found) {
        DPRINTF("failed to find a OpenCL device\n");
        exit(-1);
    }
#ifndef NDEBUG
    DPRINTF("Total number of devices: %d", numDevices);
    for (unsigned int i = 0; i < numDevices; i++) {
        clGetDeviceInfo(devices[i],
                        CL_DEVICE_NAME,
                        4096,
                        buffer,
                        NULL);
        DPRINTF("\nDevice Name: %s\n", buffer);

        clGetDeviceInfo(devices[i],
                        CL_DEVICE_VENDOR,
                        4096,
                        buffer,
                        NULL);
        DPRINTF("Device Vendor: %s\n", buffer);

        clGetDeviceInfo(devices[i],
                        CL_DEVICE_MAX_COMPUTE_UNITS,
                        sizeof(buf_uint),
                        &buf_uint,
                        NULL);
        DPRINTF("Device Computing Units: %u\n", buf_uint);

        clGetDeviceInfo(devices[i],
                        CL_DEVICE_GLOBAL_MEM_SIZE,
                        sizeof(unsigned long),
                        &buffer,
                        NULL);
        DPRINTF("Global Memory Size: %li\n", *((unsigned long*)buffer));

        clGetDeviceInfo(devices[i],
                        CL_DEVICE_MAX_MEM_ALLOC_SIZE,
                        sizeof(unsigned long),
                        &buffer,
                        NULL);
        DPRINTF("Global Memory Allocation Size: %li\n\n", *((unsigned long*)buffer));
    }
#endif
    //----------------------------------------------
    // Create a context
    //----------------------------------------------
#ifndef NDEBUG
    DPRINTF("\n===== Host-CPU setting up the OpenCL command queues ======\n\n");
#endif
    cl_context context = NULL;

    // Create a context using clCreateContext() and
    // associate it with the device

    context = clCreateContext(
        NULL,
        1,
        devices,
        NULL,
        NULL,
        &status);
    CHECK(status);



    if(device_count < 1) {
        std::cerr << "Not creating requested FPGA context. No FPGA devices available." << std::endl;
        return nullptr;
    }

    if(device_id >= device_count) {
        std::cerr << "Requested device ID " << device_id << " >= device count " << device_count << std::endl;
        return nullptr;
    }

    auto ctx = std::unique_ptr<FPGAContext>(new FPGAContext(device_id));
    ctx->init();
    return ctx;
}

