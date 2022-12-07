#THIS FILE IS AUTO GENERATED FROM THE TEMPLATE! DO NOT CHANGE!
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_VERSION 1)
cmake_minimum_required(VERSION 3.23)

# specify cross-compilers and tools
set(CMAKE_C_COMPILER arm-none-eabi-gcc)
set(CMAKE_CXX_COMPILER arm-none-eabi-g++)
set(CMAKE_ASM_COMPILER  arm-none-eabi-gcc)
set(CMAKE_AR arm-none-eabi-ar)
set(CMAKE_OBJCOPY arm-none-eabi-objcopy)
set(CMAKE_OBJDUMP arm-none-eabi-objdump)
set(SIZE arm-none-eabi-size)
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

# project settings
project(l475tflite C CXX ASM)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 11)

#Uncomment for hardware floating point
#add_compile_definitions(ARM_MATH_CM4;ARM_MATH_MATRIX_CHECK;ARM_MATH_ROUNDING)
#add_compile_options(-mfloat-abi=hard -mfpu=fpv4-sp-d16)
#add_link_options(-mfloat-abi=hard -mfpu=fpv4-sp-d16)

#Uncomment for software floating point
#add_compile_options(-mfloat-abi=soft)

add_compile_options(-mcpu=cortex-m4 -mthumb -mthumb-interwork)
add_compile_options(-ffunction-sections -fdata-sections -fno-common -fmessage-length=0)

# uncomment to mitigate c++17 absolute addresses warnings
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-register")

# Enable assembler files preprocessing
add_compile_options($<$<COMPILE_LANGUAGE:ASM>:-x$<SEMICOLON>assembler-with-cpp>)

if ("${CMAKE_BUILD_TYPE}" STREQUAL "Release")
    message(STATUS "Maximum optimization for speed")
    add_compile_options(-Ofast)
elseif ("${CMAKE_BUILD_TYPE}" STREQUAL "RelWithDebInfo")
    message(STATUS "Maximum optimization for speed, debug info included")
    add_compile_options(-Ofast -g)
elseif ("${CMAKE_BUILD_TYPE}" STREQUAL "MinSizeRel")
    message(STATUS "Maximum optimization for size")
    add_compile_options(-Os)
else ()
    message(STATUS "Minimal optimization, debug info included")
    add_compile_options(-Og -g)
endif ()

include_directories(Core/Inc X-CUBE-MEMS1/Target USB_DEVICE/App USB_DEVICE/Target Drivers/BSP/B-L475E-IOT01A1 Drivers/STM32L4xx_HAL_Driver/Inc Drivers/STM32L4xx_HAL_Driver/Inc/Legacy Middlewares/ST/STM32_USB_Device_Library/Core/Inc Middlewares/ST/STM32_USB_Device_Library/Class/HID/Inc Drivers/CMSIS/Device/ST/STM32L4xx/Include Drivers/CMSIS/Include Drivers/BSP/Components/lsm6dsl)

add_definitions(
	-DDEBUG
       	-DUSE_HAL_DRIVER
       	-DSTM32L475xx
	-DTF_LITE_DISABLE_X86_NEON
        -DCMSIS_NN
        -DTF_LITE_USE_CTIME
        -DUSE_HAL_DRIVER
        -DTF_LITE_MCU_DEBUG_LOG
        -DNDEBUG
)

file(GLOB_RECURSE SOURCES "Core/*.*" "Middlewares/*.*" "X-CUBE-MEMS1/*.*" "Drivers/*.*" "USB_DEVICE/*.*")

set(LINKER_SCRIPT ${CMAKE_SOURCE_DIR}/STM32L475VGTX_FLASH.ld)

add_link_options(-Wl,-gc-sections,--print-memory-usage,-Map=${PROJECT_BINARY_DIR}/${PROJECT_NAME}.map)
add_link_options(-mcpu=cortex-m4 -mthumb -mthumb-interwork)
add_link_options(-T ${LINKER_SCRIPT})

include_directories(
        .
        tensorflow/lite
        tensorflow/lite/c
        tensorflow/lite/core
        tensorflow/lite/kernels
        tensorflow/lite/kernels/internal
        tensorflow/lite/kernels/internal/optimized
        tensorflow/lite/kernels/internal/reference
        tensorflow/lite/kernels/internal/reference/integer_ops
        tensorflow/lite/micro
        tensorflow/lite/micro/kernels
        tensorflow/lite/micro/kernels/cmsis_nn
        tensorflow/lite/micro/kernels/memory_planer
        tensorflow/lite/micro/kernels/testing
        tensorflow/lite/micro/tools/make/downloads
        tensorflow/lite/micro/tools/make/downloads/cmsis
        tensorflow/lite/micro/tools/make/downloads/cmsis/CMSIS
        tensorflow/lite/micro/tools/make/downloads/cmsis/CMSIS/Core/Include
        tensorflow/lite/micro/tools/make/downloads/cmsis/CMSIS/DSP/Include
        tensorflow/lite/micro/tools/make/downloads/cmsis/CMSIS/DSP/Include/dsp
        tensorflow/lite/micro/tools/make/downloads/cmsis/CMSIS/NN/Include
        tensorflow/lite/schema
        third_party/flatbuffers/include
        third_party/gemmlowp
        third_party/ruy
)

set(TENSORFLOW
        "tensorflow/lite/micro/all_ops_resolver.cc"
        "tensorflow/lite/micro/debug_log.cc"
        "tensorflow/lite/micro/flatbuffer_utils.cc"
        "tensorflow/lite/micro/memory_helpers.cc"
        "tensorflow/lite/micro/micro_allocator.cc"
        "tensorflow/lite/micro/micro_error_reporter.cc"
        "tensorflow/lite/micro/micro_graph.cc"
        "tensorflow/lite/micro/micro_interpreter.cc"
        "tensorflow/lite/micro/micro_profiler.cc"
        "tensorflow/lite/micro/micro_resource_variable.cc"
        "tensorflow/lite/micro/micro_string.cc"
        "tensorflow/lite/micro/micro_time.cc"
        "tensorflow/lite/micro/micro_utils.cc"
        "tensorflow/lite/micro/mock_micro_graph.cc"
        "tensorflow/lite/micro/recording_micro_allocator.cc"
        "tensorflow/lite/micro/recording_simple_memory_allocator.cc"
        "tensorflow/lite/micro/simple_memory_allocator.cc"
        "tensorflow/lite/micro/system_setup.cc"
        "tensorflow/lite/micro/test_helpers.cc"
        "tensorflow/lite/micro/memory_planner/greedy_memory_planner.cc"
        "tensorflow/lite/micro/memory_planner/linear_memory_planner.cc"
        "tensorflow/lite/micro/memory_planner/non_persistent_buffer_planner_shim.cc"
        "tensorflow/lite/kernels/kernel_util.cc"
        "tensorflow/lite/kernels/internal/reference/portable_tensor_utils.cc"
        "tensorflow/lite/kernels/internal/quantization_util.cc"
        "tensorflow/lite/core/api/error_reporter.cc"
        "tensorflow/lite/core/api/tensor_utils.cc"
        "tensorflow/lite/core/api/flatbuffer_conversions.cc"
        "tensorflow/lite/core/api/op_resolver.cc"
        "tensorflow/lite/schema/schema_utils.cc"
        "tensorflow/lite/c/common.c"
        "tensorflow/lite/micro/tools/make/downloads/cmsis/CMSIS/NN/Source/ActivationFunctions/arm_nn_activations_q15.c"
        "tensorflow/lite/micro/tools/make/downloads/cmsis/CMSIS/NN/Source/ActivationFunctions/arm_nn_activations_q7.c"
        "tensorflow/lite/micro/tools/make/downloads/cmsis/CMSIS/NN/Source/ActivationFunctions/arm_relu6_s8.c"
        "tensorflow/lite/micro/tools/make/downloads/cmsis/CMSIS/NN/Source/ActivationFunctions/arm_relu_q15.c"
        "tensorflow/lite/micro/tools/make/downloads/cmsis/CMSIS/NN/Source/ActivationFunctions/arm_relu_q7.c"
        "tensorflow/lite/micro/tools/make/downloads/cmsis/CMSIS/NN/Source/BasicMathFunctions/arm_elementwise_add_s8.c"
        "tensorflow/lite/micro/tools/make/downloads/cmsis/CMSIS/NN/Source/BasicMathFunctions/arm_elementwise_mul_s8.c"
        "tensorflow/lite/micro/tools/make/downloads/cmsis/CMSIS/NN/Source/ConcatenationFunctions/arm_concatenation_s8_w.c"
        "tensorflow/lite/micro/tools/make/downloads/cmsis/CMSIS/NN/Source/ConcatenationFunctions/arm_concatenation_s8_x.c"
        "tensorflow/lite/micro/tools/make/downloads/cmsis/CMSIS/NN/Source/ConcatenationFunctions/arm_concatenation_s8_y.c"
        "tensorflow/lite/micro/tools/make/downloads/cmsis/CMSIS/NN/Source/ConcatenationFunctions/arm_concatenation_s8_z.c"
        "tensorflow/lite/micro/tools/make/downloads/cmsis/CMSIS/NN/Source/ConvolutionFunctions/arm_convolve_1_x_n_s8.c"
        "tensorflow/lite/micro/tools/make/downloads/cmsis/CMSIS/NN/Source/ConvolutionFunctions/arm_convolve_1x1_HWC_q7_fast_nonsquare.c"
        "tensorflow/lite/micro/tools/make/downloads/cmsis/CMSIS/NN/Source/ConvolutionFunctions/arm_convolve_1x1_s8_fast.c"
        "tensorflow/lite/micro/tools/make/downloads/cmsis/CMSIS/NN/Source/ConvolutionFunctions/arm_convolve_HWC_q15_basic.c"
        "tensorflow/lite/micro/tools/make/downloads/cmsis/CMSIS/NN/Source/ConvolutionFunctions/arm_convolve_HWC_q15_fast.c"
        "tensorflow/lite/micro/tools/make/downloads/cmsis/CMSIS/NN/Source/ConvolutionFunctions/arm_convolve_HWC_q15_fast_nonsquare.c"
        "tensorflow/lite/micro/tools/make/downloads/cmsis/CMSIS/NN/Source/ConvolutionFunctions/arm_convolve_HWC_q7_RGB.c"
        "tensorflow/lite/micro/tools/make/downloads/cmsis/CMSIS/NN/Source/ConvolutionFunctions/arm_convolve_HWC_q7_basic.c"
        "tensorflow/lite/micro/tools/make/downloads/cmsis/CMSIS/NN/Source/ConvolutionFunctions/arm_convolve_HWC_q7_basic_nonsquare.c"
        "tensorflow/lite/micro/tools/make/downloads/cmsis/CMSIS/NN/Source/ConvolutionFunctions/arm_convolve_HWC_q7_fast.c"
        "tensorflow/lite/micro/tools/make/downloads/cmsis/CMSIS/NN/Source/ConvolutionFunctions/arm_convolve_HWC_q7_fast_nonsquare.c"
        "tensorflow/lite/micro/tools/make/downloads/cmsis/CMSIS/NN/Source/ConvolutionFunctions/arm_convolve_fast_s16.c"
        "tensorflow/lite/micro/tools/make/downloads/cmsis/CMSIS/NN/Source/ConvolutionFunctions/arm_convolve_s16.c"
        "tensorflow/lite/micro/tools/make/downloads/cmsis/CMSIS/NN/Source/ConvolutionFunctions/arm_convolve_s8.c"
        "tensorflow/lite/micro/tools/make/downloads/cmsis/CMSIS/NN/Source/ConvolutionFunctions/arm_convolve_wrapper_s16.c"
        "tensorflow/lite/micro/tools/make/downloads/cmsis/CMSIS/NN/Source/ConvolutionFunctions/arm_convolve_wrapper_s8.c"
        "tensorflow/lite/micro/tools/make/downloads/cmsis/CMSIS/NN/Source/ConvolutionFunctions/arm_depthwise_conv_3x3_s8.c"
        "tensorflow/lite/micro/tools/make/downloads/cmsis/CMSIS/NN/Source/ConvolutionFunctions/arm_depthwise_conv_s8.c"
        "tensorflow/lite/micro/tools/make/downloads/cmsis/CMSIS/NN/Source/ConvolutionFunctions/arm_depthwise_conv_s8_opt.c"
        "tensorflow/lite/micro/tools/make/downloads/cmsis/CMSIS/NN/Source/ConvolutionFunctions/arm_depthwise_conv_u8_basic_ver1.c"
        "tensorflow/lite/micro/tools/make/downloads/cmsis/CMSIS/NN/Source/ConvolutionFunctions/arm_depthwise_conv_wrapper_s8.c"
        "tensorflow/lite/micro/tools/make/downloads/cmsis/CMSIS/NN/Source/ConvolutionFunctions/arm_depthwise_separable_conv_HWC_q7.c"
        "tensorflow/lite/micro/tools/make/downloads/cmsis/CMSIS/NN/Source/ConvolutionFunctions/arm_depthwise_separable_conv_HWC_q7_nonsquare.c"
        "tensorflow/lite/micro/tools/make/downloads/cmsis/CMSIS/NN/Source/ConvolutionFunctions/arm_nn_depthwise_conv_s8_core.c"
        "tensorflow/lite/micro/tools/make/downloads/cmsis/CMSIS/NN/Source/ConvolutionFunctions/arm_nn_mat_mult_kernel_q7_q15.c"
        "tensorflow/lite/micro/tools/make/downloads/cmsis/CMSIS/NN/Source/ConvolutionFunctions/arm_nn_mat_mult_kernel_q7_q15_reordered.c"
        "tensorflow/lite/micro/tools/make/downloads/cmsis/CMSIS/NN/Source/ConvolutionFunctions/arm_nn_mat_mult_kernel_s8_s16.c"
        "tensorflow/lite/micro/tools/make/downloads/cmsis/CMSIS/NN/Source/ConvolutionFunctions/arm_nn_mat_mult_kernel_s8_s16_reordered.c"
        "tensorflow/lite/micro/tools/make/downloads/cmsis/CMSIS/NN/Source/ConvolutionFunctions/arm_nn_mat_mult_s8.c"
        "tensorflow/lite/micro/tools/make/downloads/cmsis/CMSIS/NN/Source/FullyConnectedFunctions/arm_fully_connected_mat_q7_vec_q15.c"
        "tensorflow/lite/micro/tools/make/downloads/cmsis/CMSIS/NN/Source/FullyConnectedFunctions/arm_fully_connected_mat_q7_vec_q15_opt.c"
        "tensorflow/lite/micro/tools/make/downloads/cmsis/CMSIS/NN/Source/FullyConnectedFunctions/arm_fully_connected_q15.c"
        "tensorflow/lite/micro/tools/make/downloads/cmsis/CMSIS/NN/Source/FullyConnectedFunctions/arm_fully_connected_q15_opt.c"
        "tensorflow/lite/micro/tools/make/downloads/cmsis/CMSIS/NN/Source/FullyConnectedFunctions/arm_fully_connected_q7.c"
        "tensorflow/lite/micro/tools/make/downloads/cmsis/CMSIS/NN/Source/FullyConnectedFunctions/arm_fully_connected_q7_opt.c"
        "tensorflow/lite/micro/tools/make/downloads/cmsis/CMSIS/NN/Source/FullyConnectedFunctions/arm_fully_connected_s16.c"
        "tensorflow/lite/micro/tools/make/downloads/cmsis/CMSIS/NN/Source/FullyConnectedFunctions/arm_fully_connected_s8.c"
        "tensorflow/lite/micro/tools/make/downloads/cmsis/CMSIS/NN/Source/NNSupportFunctions/arm_nn_accumulate_q7_to_q15.c"
        "tensorflow/lite/micro/tools/make/downloads/cmsis/CMSIS/NN/Source/NNSupportFunctions/arm_nn_add_q7.c"
        "tensorflow/lite/micro/tools/make/downloads/cmsis/CMSIS/NN/Source/NNSupportFunctions/arm_nn_depthwise_conv_nt_t_padded_s8.c"
        "tensorflow/lite/micro/tools/make/downloads/cmsis/CMSIS/NN/Source/NNSupportFunctions/arm_nn_depthwise_conv_nt_t_s8.c"
        "tensorflow/lite/micro/tools/make/downloads/cmsis/CMSIS/NN/Source/NNSupportFunctions/arm_nn_mat_mul_core_1x_s8.c"
        "tensorflow/lite/micro/tools/make/downloads/cmsis/CMSIS/NN/Source/NNSupportFunctions/arm_nn_mat_mul_core_4x_s8.c"
        "tensorflow/lite/micro/tools/make/downloads/cmsis/CMSIS/NN/Source/NNSupportFunctions/arm_nn_mat_mul_kernel_s16.c"
        "tensorflow/lite/micro/tools/make/downloads/cmsis/CMSIS/NN/Source/NNSupportFunctions/arm_nn_mat_mult_nt_t_s8.c"
        "tensorflow/lite/micro/tools/make/downloads/cmsis/CMSIS/NN/Source/NNSupportFunctions/arm_nn_mult_q15.c"
        "tensorflow/lite/micro/tools/make/downloads/cmsis/CMSIS/NN/Source/NNSupportFunctions/arm_nn_mult_q7.c"
        "tensorflow/lite/micro/tools/make/downloads/cmsis/CMSIS/NN/Source/NNSupportFunctions/arm_nn_vec_mat_mult_t_s16.c"
        "tensorflow/lite/micro/tools/make/downloads/cmsis/CMSIS/NN/Source/NNSupportFunctions/arm_nn_vec_mat_mult_t_s8.c"
        "tensorflow/lite/micro/tools/make/downloads/cmsis/CMSIS/NN/Source/NNSupportFunctions/arm_nn_vec_mat_mult_t_svdf_s8.c"
        "tensorflow/lite/micro/tools/make/downloads/cmsis/CMSIS/NN/Source/NNSupportFunctions/arm_nntables.c"
        "tensorflow/lite/micro/tools/make/downloads/cmsis/CMSIS/NN/Source/NNSupportFunctions/arm_q7_to_q15_no_shift.c"
        "tensorflow/lite/micro/tools/make/downloads/cmsis/CMSIS/NN/Source/NNSupportFunctions/arm_q7_to_q15_reordered_no_shift.c"
        "tensorflow/lite/micro/tools/make/downloads/cmsis/CMSIS/NN/Source/NNSupportFunctions/arm_q7_to_q15_reordered_with_offset.c"
        "tensorflow/lite/micro/tools/make/downloads/cmsis/CMSIS/NN/Source/NNSupportFunctions/arm_q7_to_q15_with_offset.c"
        "tensorflow/lite/micro/tools/make/downloads/cmsis/CMSIS/NN/Source/PoolingFunctions/arm_avgpool_s8.c"
        "tensorflow/lite/micro/tools/make/downloads/cmsis/CMSIS/NN/Source/PoolingFunctions/arm_max_pool_s8.c"
        "tensorflow/lite/micro/tools/make/downloads/cmsis/CMSIS/NN/Source/PoolingFunctions/arm_pool_q7_HWC.c"
        "tensorflow/lite/micro/tools/make/downloads/cmsis/CMSIS/NN/Source/ReshapeFunctions/arm_reshape_s8.c"
        "tensorflow/lite/micro/tools/make/downloads/cmsis/CMSIS/NN/Source/SVDFunctions/arm_svdf_s8.c"
        "tensorflow/lite/micro/tools/make/downloads/cmsis/CMSIS/NN/Source/SoftmaxFunctions/arm_softmax_q15.c"
        "tensorflow/lite/micro/tools/make/downloads/cmsis/CMSIS/NN/Source/SoftmaxFunctions/arm_softmax_q7.c"
        "tensorflow/lite/micro/tools/make/downloads/cmsis/CMSIS/NN/Source/SoftmaxFunctions/arm_softmax_s8.c"
        "tensorflow/lite/micro/tools/make/downloads/cmsis/CMSIS/NN/Source/SoftmaxFunctions/arm_softmax_u8.c"
        "tensorflow/lite/micro/tools/make/downloads/cmsis/CMSIS/NN/Source/SoftmaxFunctions/arm_softmax_with_batch_q7.c"
        "tensorflow/lite/micro/kernels/activations.cc"
        "tensorflow/lite/micro/kernels/activations_common.cc"
        "tensorflow/lite/micro/kernels/cmsis_nn/add.cc"
        "tensorflow/lite/micro/kernels/add_common.cc"
        "tensorflow/lite/micro/kernels/add_n.cc"
        "tensorflow/lite/micro/kernels/arg_min_max.cc"
        "tensorflow/lite/micro/kernels/assign_variable.cc"
        "tensorflow/lite/micro/kernels/batch_to_space_nd.cc"
        "tensorflow/lite/micro/kernels/call_once.cc"
        "tensorflow/lite/micro/kernels/cast.cc"
        "tensorflow/lite/micro/kernels/ceil.cc"
        "tensorflow/lite/micro/kernels/circular_buffer.cc"
        "tensorflow/lite/micro/kernels/circular_buffer_common.cc"
        "tensorflow/lite/micro/kernels/comparisons.cc"
        "tensorflow/lite/micro/kernels/concatenation.cc"
        "tensorflow/lite/micro/kernels/cmsis_nn/conv.cc"
        "tensorflow/lite/micro/kernels/conv_common.cc"
        "tensorflow/lite/micro/kernels/cumsum.cc"
        "tensorflow/lite/micro/kernels/depth_to_space.cc"
        "tensorflow/lite/micro/kernels/cmsis_nn/depthwise_conv.cc"
        "tensorflow/lite/micro/kernels/depthwise_conv_common.cc"
        "tensorflow/lite/micro/kernels/dequantize.cc"
        "tensorflow/lite/micro/kernels/dequantize_common.cc"
        "tensorflow/lite/micro/kernels/detection_postprocess.cc"
        "tensorflow/lite/micro/kernels/elementwise.cc"
        "tensorflow/lite/micro/kernels/elu.cc"
        "tensorflow/lite/micro/kernels/ethosu.cc"
        "tensorflow/lite/micro/kernels/exp.cc"
        "tensorflow/lite/micro/kernels/expand_dims.cc"
        "tensorflow/lite/micro/kernels/fill.cc"
        "tensorflow/lite/micro/kernels/floor.cc"
        "tensorflow/lite/micro/kernels/floor_div.cc"
        "tensorflow/lite/micro/kernels/floor_mod.cc"
        "tensorflow/lite/micro/kernels/cmsis_nn/fully_connected.cc"
        "tensorflow/lite/micro/kernels/fully_connected_common.cc"
        "tensorflow/lite/micro/kernels/gather.cc"
        "tensorflow/lite/micro/kernels/gather_nd.cc"
        "tensorflow/lite/micro/kernels/hard_swish.cc"
        "tensorflow/lite/micro/kernels/hard_swish_common.cc"
        "tensorflow/lite/micro/kernels/if.cc"
        "tensorflow/lite/micro/kernels/kernel_runner.cc"
        "tensorflow/lite/micro/kernels/kernel_util_micro.cc"
        "tensorflow/lite/micro/kernels/l2norm.cc"
        "tensorflow/lite/micro/kernels/l2_pool_2d.cc"
        "tensorflow/lite/micro/kernels/leaky_relu.cc"
        "tensorflow/lite/micro/kernels/leaky_relu_common.cc"
        "tensorflow/lite/micro/kernels/logical.cc"
        "tensorflow/lite/micro/kernels/logical_common.cc"
        "tensorflow/lite/micro/kernels/logistic.cc"
        "tensorflow/lite/micro/kernels/logistic_common.cc"
        "tensorflow/lite/micro/kernels/log_softmax.cc"
        "tensorflow/lite/micro/kernels/maximum_minimum.cc"
        "tensorflow/lite/micro/kernels/cmsis_nn/mul.cc"
        "tensorflow/lite/micro/kernels/mul_common.cc"
        "tensorflow/lite/micro/kernels/neg.cc"
        "tensorflow/lite/micro/kernels/pack.cc"
        "tensorflow/lite/micro/kernels/pad.cc"
        "tensorflow/lite/micro/kernels/cmsis_nn/pooling.cc"
        "tensorflow/lite/micro/kernels/pooling_common.cc"
        "tensorflow/lite/micro/kernels/prelu.cc"
        "tensorflow/lite/micro/kernels/prelu_common.cc"
        "tensorflow/lite/micro/kernels/quantize.cc"
        "tensorflow/lite/micro/kernels/quantize_common.cc"
        "tensorflow/lite/micro/kernels/read_variable.cc"
        "tensorflow/lite/micro/kernels/reduce.cc"
        "tensorflow/lite/micro/kernels/reshape.cc"
        "tensorflow/lite/micro/kernels/resize_bilinear.cc"
        "tensorflow/lite/micro/kernels/resize_nearest_neighbor.cc"
        "tensorflow/lite/micro/kernels/round.cc"
        "tensorflow/lite/micro/kernels/shape.cc"
        "tensorflow/lite/micro/kernels/slice.cc"
        "tensorflow/lite/micro/kernels/cmsis_nn/softmax.cc"
        "tensorflow/lite/micro/kernels/softmax_common.cc"
        "tensorflow/lite/micro/kernels/space_to_batch_nd.cc"
        "tensorflow/lite/micro/kernels/space_to_depth.cc"
        "tensorflow/lite/micro/kernels/split.cc"
        "tensorflow/lite/micro/kernels/split_v.cc"
        "tensorflow/lite/micro/kernels/squeeze.cc"
        "tensorflow/lite/micro/kernels/strided_slice.cc"
        "tensorflow/lite/micro/kernels/sub.cc"
        "tensorflow/lite/micro/kernels/sub_common.cc"
        "tensorflow/lite/micro/kernels/cmsis_nn/svdf.cc"
        "tensorflow/lite/micro/kernels/svdf_common.cc"
        "tensorflow/lite/micro/kernels/tanh.cc"
        "tensorflow/lite/micro/kernels/transpose.cc"
        "tensorflow/lite/micro/kernels/transpose_conv.cc"
        "tensorflow/lite/micro/kernels/unpack.cc"
        "tensorflow/lite/micro/kernels/var_handle.cc"
        "tensorflow/lite/micro/kernels/zeros_like.cc"

        "tensorflow/lite/micro/examples/hello_world/hello_world_model_data.cc"
)

add_executable(${PROJECT_NAME}.elf ${SOURCES} ${LINKER_SCRIPT} ${TENSORFLOW})

set(HEX_FILE ${PROJECT_BINARY_DIR}/${PROJECT_NAME}.hex)
set(BIN_FILE ${PROJECT_BINARY_DIR}/${PROJECT_NAME}.bin)

add_custom_command(TARGET ${PROJECT_NAME}.elf POST_BUILD
        COMMAND ${CMAKE_OBJCOPY} -Oihex $<TARGET_FILE:${PROJECT_NAME}.elf> ${HEX_FILE}
        COMMAND ${CMAKE_OBJCOPY} -Obinary $<TARGET_FILE:${PROJECT_NAME}.elf> ${BIN_FILE}
        COMMENT "Building ${HEX_FILE}
Building ${BIN_FILE}")
