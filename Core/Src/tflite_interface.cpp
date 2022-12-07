/*
 * Created by Ursin Cantieni on 06.12.22.
 */

#include "tflite_interface.hpp"

#include "Core/Inc/model_data.h"  // tensorflow lite model as C array
#include "tensorflow/lite/micro/all_ops_resolver.h"
#include "tensorflow/lite/micro/micro_error_reporter.h"
#include "tensorflow/lite/micro/micro_interpreter.h"
#include "tensorflow/lite/micro/testing/micro_test.h"
#include "tensorflow/lite/schema/schema_generated.h"
// #include
// "tensorflow/lite/micro/examples/hello_world/hello_world_model_data.h"

// declarations for error framework
int micro_test::tests_passed;
int micro_test::tests_failed;
bool micro_test::is_test_complete;
bool micro_test::did_test_fail;

void initTflite() {
    tflite::MicroErrorReporter micro_error_reporter;
    tflite::ErrorReporter* error_reporter = &micro_error_reporter;
    TfLiteStatus tflite_status;

    const tflite::Model* TFmodel = ::tflite::GetModel(mouse_model);
    if (TFmodel->version() != TFLITE_SCHEMA_VERSION) {
        TF_LITE_REPORT_ERROR(error_reporter,
                             "Model provided is schema version %d not equal "
                             "to supported version %d.\n",
                             TFmodel->version(), TFLITE_SCHEMA_VERSION);
        printf(
            "Model provided is schema version %d not equal "
            "to supported version %d.\n");
    }

    tflite::AllOpsResolver resolver;

    const int tensor_arena_size = 32 * 1024;
    uint8_t tensor_arena[tensor_arena_size];

    tflite::MicroInterpreter interpreter(TFmodel, resolver, tensor_arena,
                                         tensor_arena_size, error_reporter);
    tflite_status = interpreter.AllocateTensors();
    if (tflite_status != kTfLiteOk) {
        printf("Error in %s:%d", __FILE__, __LINE__);
    }

    // Obtain a pointer to the model's input tensor
    TfLiteTensor* input = interpreter.input(0);

    for (size_t i = 0; i < 144; ++i) {
        float d;
        if (i < 96) {
            d = 30;
        } else {
            d = 200;
        }
        interpreter.input(i)->data.f[0] = d;
        // input->data.int8[i] = d;
    }

    /*******************/
    printf("TfLiteTensor type: %d\r\n", input->type);
    printf("TfLiteTensor data: %d\r\n", input->data);
    printf("TfLiteTensor dims: %d\r\n", input->dims);
    printf("TfLiteTensor params: %d\r\n", input->params);
    printf("TfLiteTensor quantization: %d\r\n\n\n", input->quantization);
    /*******************/

    TfLiteStatus invoke_status = interpreter.Invoke();
    if (invoke_status != kTfLiteOk) {
        TF_LITE_REPORT_ERROR(error_reporter, "Invoke failed\n");
    }
    TF_LITE_MICRO_EXPECT_EQ(kTfLiteOk, invoke_status);

    // TfLiteTensor* output = interpreter.output(0);
    printf("%f; %f; %f; %f \n\r", interpreter.output(0)->data.f[0],
           interpreter.output(0)->data.f[1],
           interpreter.output(1)->data.f[0],
           interpreter.output(1)->data.f[1]);
}
