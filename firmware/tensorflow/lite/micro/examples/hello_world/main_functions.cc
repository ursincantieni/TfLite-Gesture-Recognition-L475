/* Copyright 2020 The TensorFlow Authors. All Rights Reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
==============================================================================*/

#include "tensorflow/lite/micro/examples/hello_world/main_functions.h"

#include "tensorflow/lite/micro/all_ops_resolver.h"
#include "tensorflow/lite/micro/examples/hello_world/constants.h"
#include "tensorflow/lite/micro/examples/hello_world/hello_world_model_data.h"
#include "tensorflow/lite/micro/examples/hello_world/model.h"
#include "tensorflow/lite/micro/examples/hello_world/model_float.h"
#include "tensorflow/lite/micro/examples/hello_world/model_q80704_data.h"
#include "tensorflow/lite/micro/examples/hello_world/tiny_model_q8_fancy_tflite_data.h"
#include "tensorflow/lite/micro/examples/hello_world/new_model.h"
#include "tensorflow/lite/micro/examples/hello_world/tiny_model.h"
#include "tensorflow/lite/micro/examples/hello_world/output_handler.h"
#include "tensorflow/lite/micro/examples/hello_world/face_id_float_model.h"
#include "tensorflow/lite/micro/examples/hello_world/face_id_quantized_model.h"
#include "tensorflow/lite/micro/micro_error_reporter.h"
#include "tensorflow/lite/micro/micro_interpreter.h"
#include "tensorflow/lite/micro/system_setup.h"
#include "tensorflow/lite/schema/schema_generated.h"

//////////////
// Sample input
//////////////
#include "tensorflow/lite/micro/examples/hello_world/face_id_sample_jim_parson.h"
#include "tensorflow/lite/micro/examples/hello_world/data_0_0.h"
#include "tensorflow/lite/micro/examples/hello_world/sample_data_in.h"
// #include "tensorflow/lite/micro/examples/hello_world/data_0_1.h"
// #include "tensorflow/lite/micro/examples/hello_world/data_0_2.h"

// #include "tensorflow/lite/micro/examples/hello_world/data_1_0.h"

// #include "tensorflow/lite/micro/examples/hello_world/data_2_0.h"
// #include "tensorflow/lite/micro/examples/hello_world/data_2_1.h"
#include "SEGGER_RTT.h"

/////////////
// Profiling
/////////////

  /* DWT (Data Watchpoint and Trace) registers, only exists on ARM Cortex with a DWT unit */
  #define KIN1_DWT_CONTROL             (*((volatile uint32_t*)0xE0001000))
    /*!< DWT Control register */
  #define KIN1_DWT_CYCCNTENA_BIT       (1UL<<0)
    /*!< CYCCNTENA bit in DWT_CONTROL register */
  #define KIN1_DWT_CYCCNT              (*((volatile uint32_t*)0xE0001004))
    /*!< DWT Cycle Counter register */
  #define KIN1_DEMCR                   (*((volatile uint32_t*)0xE000EDFC))
    /*!< DEMCR: Debug Exception and Monitor Control Register */
  #define KIN1_TRCENA_BIT              (1UL<<24)
    /*!< Trace enable bit in DEMCR register */

  
  #define KIN1_InitCycleCounter() \
  KIN1_DEMCR |= KIN1_TRCENA_BIT
  /*!< TRCENA: Enable trace and debug block DEMCR (Debug Exception and Monitor Control Register */
 
#define KIN1_ResetCycleCounter() \
  KIN1_DWT_CYCCNT = 0
  /*!< Reset cycle counter */
 
#define KIN1_EnableCycleCounter() \
  KIN1_DWT_CONTROL |= KIN1_DWT_CYCCNTENA_BIT
  /*!< Enable cycle counter */
 
#define KIN1_DisableCycleCounter() \
  KIN1_DWT_CONTROL &= ~KIN1_DWT_CYCCNTENA_BIT
  /*!< Disable cycle counter */
 
#define KIN1_GetCycleCounter() \
  KIN1_DWT_CYCCNT
  /*!< Read cycle counter register */



// Globals, used for compatibility with Arduino-style sketches.
namespace {
tflite::ErrorReporter* error_reporter = nullptr;
const tflite::Model* model = nullptr;
tflite::MicroInterpreter* interpreter = nullptr;
TfLiteTensor* input = nullptr;
TfLiteTensor* output = nullptr;
int inference_count = 0;

constexpr int kTensorArenaSize = 350000;
// constexpr int kTensorArenaSize = 150000;
uint8_t tensor_arena[kTensorArenaSize];
}  // namespace

// The name of this function is important for Arduino compatibility.
void setup() {
  tflite::InitializeTarget();

  // Set up logging. Google style is to avoid globals or statics because of
  // lifetime uncertainty, but since this has a trivial destructor it's okay.
  // NOLINTNEXTLINE(runtime-global-variables)
  static tflite::MicroErrorReporter micro_error_reporter;
  error_reporter = &micro_error_reporter;

  // Map the model into a usable data structure. This doesn't involve any
  // copying or parsing, it's a very lightweight operation.
  // model = tflite::GetModel(quant_1dcnn_tflite);
  // model = tflite::GetModel(g_hello_world_model_data);
  // model = tflite::GetModel(model_float);
  // model = tflite::GetModel(new_model);
  model = tflite::GetModel(model_q80704_tflite);
  // model = tflite::GetModel(tiny_model_q8_fancy_tflite);
    // model = tflite::GetModel(face_id_quantized_model);
  // model = tflite::GetModel(tiny_model);


  if (model->version() != TFLITE_SCHEMA_VERSION) {
    TF_LITE_REPORT_ERROR(error_reporter,
                         "Model provided is schema version %d not equal "
                         "to supported version %d.",
                         model->version(), TFLITE_SCHEMA_VERSION);
    return;
  }

  // This pulls in all the operation implementations we need.
  // NOLINTNEXTLINE(runtime-global-variables)
  // static tflite::AllOpsResolver resolver; // Let's use only needed ops
  static tflite::MicroMutableOpResolver<9> resolver;  // NOLINT
  resolver.AddConv2D();
  resolver.AddFullyConnected();
  resolver.AddRelu();
  resolver.AddReshape();
  resolver.AddSoftmax();
  resolver.AddQuantize();
  resolver.AddDequantize();
  resolver.AddAdd();
  resolver.AddMaxPool2D();

  // Build an interpreter to run the model with.
  static tflite::MicroInterpreter static_interpreter(
      model, resolver, tensor_arena, kTensorArenaSize, error_reporter);
  interpreter = &static_interpreter;

  // Allocate memory from the tensor_arena for the model's tensors.
  TfLiteStatus allocate_status = interpreter->AllocateTensors();
  if (allocate_status != kTfLiteOk) {
    TF_LITE_REPORT_ERROR(error_reporter, "AllocateTensors() failed");
    return;
  }

  // Obtain pointers to the model's input and output tensors.
  input = interpreter->input(0);
  output = interpreter->output(0);

  // Keep track of how many inferences we have performed.
  inference_count = 0;
}

// The name of this function is important for Arduino compatibility.
void loop() {
  // Calculate an x value to feed into the model. We compare the current
  // inference_count to the number of inferences per cycle to determine
  // our position within the range of possible x values the model was
  // trained on, and use this to calculate a value.
  // float position = static_cast<float>(inference_count) /
  //                  static_cast<float>(kInferencesPerCycle);
  // float x = position * kXrange;

  float x = 2;

  // Quantize the input from floating-point to integer
  // int8_t x_quantized = x / input->params.scale + input->params.zero_point;
  // Place the quantized input in the model's input tensor
  // input->data.int8[0] = x_quantized;
  SEGGER_RTT_printf(0, "input->data.f address! (%08x)\r\n", (input->data.int8));
  // memcpy(input->data.f, sample_data_in, 23232*sizeof(float));
  memcpy(input->data.int8, sample_data_in, 23232*sizeof(int8_t));
    // memcpy(input->data.int8, face_id_data, 1200*sizeof(int8_t));
  
  // memcpy(input->data.f, data_0_0, 1344*sizeof(float));

  // Run inference, and report any error

  volatile uint32_t cycles; /* number of cycles */
 
  KIN1_InitCycleCounter(); /* enable DWT hardware */
  KIN1_ResetCycleCounter(); /* reset cycle counter */
  KIN1_EnableCycleCounter(); /* start counting */

  TfLiteStatus invoke_status = interpreter->Invoke();

  cycles = KIN1_GetCycleCounter(); /* get cycle counter */
  KIN1_DisableCycleCounter(); /* disable counting if not used any more */


  if (invoke_status != kTfLiteOk) {
    TF_LITE_REPORT_ERROR(error_reporter, "Invoke failed on x: %f\n",
                         static_cast<double>(cycles));
    return;
  }

  volatile float y_0 = output->data.f[0];
  volatile float y_1 = output->data.f[1];
  volatile float y_2 = output->data.f[2];
  
  // Obtain the quantized output from model's output tensor
  // int8_t y_quantized = output->data.int8[0];
  // Dequantize the output from integer to floating-point
  // volatile float y = (y_quantized - output->params.zero_point) * output->params.scale;

  // Output the results. A custom HandleOutput function can be implemented
  // for each supported hardware target.
  // HandleOutput(error_reporter, x, y);
  HandleOutput(error_reporter, x, y_0);
  HandleOutput(error_reporter, x, y_1);
  HandleOutput(error_reporter, x, y_2);

  // Increment the inference_counter, and reset it if we have reached
  // the total number per cycle
  inference_count += 1;
  if (inference_count >= kInferencesPerCycle) inference_count = 0;
}
