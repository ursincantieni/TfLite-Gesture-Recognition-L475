/*
 * Created by Ursin Cantieni on 06.12.22.
 */

#include "tflite_interface.hpp"

#include "Core/Inc/model_data.h"  // tensorflow lite model as C array
#include "tensorflow/lite/micro/all_ops_resolver.h"
// #include "tensorflow/lite/micro/examples/hello_world/constants.h"
// #include
// "tensorflow/lite/micro/examples/hello_world/hello_world_model_data.h"
#include "tensorflow/lite/micro/micro_error_reporter.h"
#include "tensorflow/lite/micro/micro_interpreter.h"
#include "tensorflow/lite/micro/system_setup.h"
#include "tensorflow/lite/micro/testing/micro_test.h"
#include "tensorflow/lite/schema/schema_generated.h"

#include <deque>

// declarations for error framework
int micro_test::tests_passed;
int micro_test::tests_failed;
bool micro_test::is_test_complete;
bool micro_test::did_test_fail;

TfLiteTensor* input;
TfLiteTensor* output;
TfLiteStatus tflite_status;
const int tensor_arena_size = 12 * 1024;
uint8_t tensor_arena[tensor_arena_size];

tflite::ErrorReporter* error_reporter = nullptr;
const tflite::Model* TfModel = nullptr;
tflite::AllOpsResolver resolver;
tflite::MicroInterpreter* interpreter = nullptr;

std::deque<float> queueX (48, 0);
std::deque<float> queueY (48, 0);
std::deque<float> queueZ (48, 0);

void addValues(float x, float y, float z) {
    queueX.pop_front();
    queueX.push_back(x);
    queueY.pop_front();
    queueY.push_back(y);
    queueZ.pop_front();
    queueZ.push_back(z);

    for (size_t i = 0; i < 48; ++i) {
        input->data.f[i] = queueX.at(i);
        input->data.f[i + 48] = queueY.at(i);
        input->data.f[i + 96] = queueZ.at(i);
    }
}

void runInference(float* prediction) {
    TfLiteStatus invoke_status = interpreter->Invoke();
    TF_LITE_MICRO_EXPECT_EQ(kTfLiteOk, invoke_status);

    float x = output->data.f[0];
    float y = output->data.f[1];

    if (x < -3. || x > 3.) {
        prediction[0] = x;
    } else {prediction[0] = 0;}
    if (y < -1. || y > 1.) {
        prediction[1] = y;
    } else {prediction[1] = 0;}
}

void initTflite() {
    printf("Set up tflite model\r\n");
    /*** BEGIN set up model ***/
    tflite::MicroErrorReporter micro_error_reporter;
    error_reporter = &micro_error_reporter;

    TfModel = ::tflite::GetModel(model_tflite);
    if (TfModel->version() != TFLITE_SCHEMA_VERSION) {
        TF_LITE_REPORT_ERROR(error_reporter,
                             "Model provided is schema version %d not equal "
                             "to supported version %d.\n",
                             TfModel->version(), TFLITE_SCHEMA_VERSION);
    }
    static tflite::MicroInterpreter static_interpreter(
        TfModel, resolver, tensor_arena, tensor_arena_size, error_reporter);
    interpreter = &static_interpreter;

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
    TF_LITE_MICRO_EXPECT_EQ(kTfLiteFloat32, input->type);

    TF_LITE_MICRO_EXPECT_EQ(2, output->dims->size);
    TF_LITE_MICRO_EXPECT_EQ(1, output->dims->data[0]);
    TF_LITE_MICRO_EXPECT_EQ(2, output->dims->data[1]);
    TF_LITE_MICRO_EXPECT_EQ(kTfLiteFloat32, output->type);

    // print_model_info();
    /*** END set up model ***/

    /*for (int8_t value; value < 50; ++value) {
        for (size_t i = 0; i < 144; ++i) {
            int8_t d;
            if (i < 96) {
                d = 2;
            } else {
                d = 10;
            }
            input->data.f[i] = value;
        }

        TfLiteStatus invoke_status = interpreter->Invoke();
        TF_LITE_MICRO_EXPECT_EQ(kTfLiteOk, invoke_status);

        // TfLiteTensor* output = interpreter.output(0);
        float y0 = output->data.f[0];
        float y1 = output->data.f[1];

        printf("%d: %f %f\n\r", value, y0, y1);
    }*/
}

static void print_model_info() {
    printf("Input Dims->size: %d\r\n", input->dims->size);
    printf("Input Dims->data0: %d\r\n", input->dims->data[0]);
    printf("Input Dims->data1: %d\r\n", input->dims->data[1]);
    printf("Input Dims->data2: %d\r\n", input->dims->data[2]);
    printf("Input Type: %d\r\n\n", input->type);
    printf("Input quantization: %d\r\n", input->quantization.type);
    printf("Output quantization: %d\r\n", output->quantization.type);
}