/*
 * Created by Ursin Cantieni on 06.12.22.
 */

#include "tflite_interface.hpp"

#include "Core/Inc/model_data.h"  // tensorflow lite model as C array
#include "tensorflow/lite/micro/all_ops_resolver.h"
// #include "tensorflow/lite/micro/examples/hello_world/constants.h"
// #include "tensorflow/lite/micro/examples/hello_world/hello_world_model_data.h"
#include "tensorflow/lite/micro/micro_error_reporter.h"
#include "tensorflow/lite/micro/micro_interpreter.h"
#include "tensorflow/lite/micro/system_setup.h"
#include "tensorflow/lite/micro/testing/micro_test.h"
#include "tensorflow/lite/schema/schema_generated.h"

// declarations for error framework
int micro_test::tests_passed;
int micro_test::tests_failed;
bool micro_test::is_test_complete;
bool micro_test::did_test_fail;

TfLiteTensor* input;
TfLiteTensor* output;

void initTflite() {
    printf("Set up tflite model\r\n");
    /*** BEGIN set up model ***/
    // tflite::InitializeTarget();
    tflite::MicroErrorReporter micro_error_reporter;
    tflite::ErrorReporter* error_reporter = &micro_error_reporter;
    TfLiteStatus tflite_status;

    const tflite::Model* TFmodel = ::tflite::GetModel(model_tflite);
    if (TFmodel->version() != TFLITE_SCHEMA_VERSION) {
        TF_LITE_REPORT_ERROR(error_reporter,
                             "Model provided is schema version %d not equal "
                             "to supported version %d.\n",
                             TFmodel->version(), TFLITE_SCHEMA_VERSION);
    }
    const int tensor_arena_size = 4 * 1024;
    uint8_t tensor_arena[tensor_arena_size];
    tflite::AllOpsResolver resolver;
    static tflite::MicroInterpreter static_interpreter(
        TFmodel, resolver, tensor_arena, tensor_arena_size, error_reporter);
    tflite::MicroInterpreter* interpreter = &static_interpreter;

    tflite_status = interpreter->AllocateTensors();
    if (tflite_status != kTfLiteOk) {
        printf("Error in %s:%d", __FILE__, __LINE__);
    }
    input = interpreter->input(0);
    output = interpreter->output(0);

    TF_LITE_MICRO_EXPECT(input != nullptr);
    TF_LITE_MICRO_EXPECT_EQ(2, input->dims->size);
    TF_LITE_MICRO_EXPECT_EQ(1, input->dims->data[0]);
    TF_LITE_MICRO_EXPECT_EQ(144, input->dims->data[1]);
    TF_LITE_MICRO_EXPECT_EQ(kTfLiteInt8, input->type);

    TF_LITE_MICRO_EXPECT_EQ(2, output->dims->size);
    TF_LITE_MICRO_EXPECT_EQ(1, output->dims->data[0]);
    TF_LITE_MICRO_EXPECT_EQ(2, output->dims->data[1]);
    TF_LITE_MICRO_EXPECT_EQ(kTfLiteInt8, output->type);
    /*** END set up model ***/

    // print_model_info();

    for (int8_t value; value < 50; ++value) {
        for (size_t i = 0; i < 144; ++i) {
            int8_t d;
            if (i < 96) {
                d = 2;
            } else {
                d = 1;
            }
            input->data.int8[i] = 1;
        }

        TfLiteStatus invoke_status = interpreter->Invoke();
        TF_LITE_MICRO_EXPECT_EQ(kTfLiteOk, invoke_status);

        // TfLiteTensor* output = interpreter.output(0);
        int8_t y0 = output->data.int8[0];
        int8_t y1 = output->data.int8[1];

        printf("%d: %d %d\n\n\r", value, y0, y1);
    }
}

static void print_model_info() {
    printf("Input:\r\n");
    printf("Dims->size: %d\r\n", input->dims->size);
    printf("Dims->data0: %d\r\n", input->dims->data[0]);
    printf("Dims->data1: %d\r\n", input->dims->data[1]);
    printf("Dims->data2: %d\r\n", input->dims->data[2]);
    printf("Type: %d\r\n", input->type);
    printf("Bytes: %d\r\n", input->bytes);
    printf("Bytes/int8: %d\r\n", input->bytes / sizeof(int8_t));
}