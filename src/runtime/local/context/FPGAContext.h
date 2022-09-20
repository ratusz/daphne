/*
 * Copyright 2021 The DAPHNE Consortium
 *
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

#pragma once

//#include <api/cli/DaphneUserConfig.h>
#include "IContext.h"
//#include <runtime/local/kernels/CUDA/HostUtils.h>
//#include <cublasLt.h>
//#include "AOCLUtils/aocl_utils.h"
//#include "CL/opencl.h"
#include <cassert>
#include <iostream>
#include <memory>

class FPGAContext : public IContext {
    int device_id = -1;
    size_t mem_budget = 0;


    explicit FPGAContext(int id) : device_id(id) { }
public:
    FPGAContext() = delete;
    FPGAContext(const FPGAContext&) = delete;
    FPGAContext& operator=(const FPGAContext&) = delete;
    ~FPGAContext() = default;

    void destroy() override;
    static std::unique_ptr<IContext> createFpgaContext(int id);

//    [[nodiscard]] cublasHandle_t getCublasHandle() const { return cublas_handle; }
//    [[nodiscard]] cusparseHandle_t getCusparseHandle() const { return cusparse_handle; }

  //  [[nodiscard]] const cudaDeviceProp* getDeviceProperties() const { return &device_properties; }


//    size_t getMemBudget() { return mem_budget; }


private:
    void init();
};
