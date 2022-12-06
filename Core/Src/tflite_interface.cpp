/*
* Created by Ursin Cantieni on 06.12.22.
*/

#include "tflite_interface.hpp"

#include "tensorflow/lite/micro/all_ops_resolver.h"
#include "tensorflow/lite/micro/micro_error_reporter.h"
#include "tensorflow/lite/micro/micro_interpreter.h"
#include "tensorflow/lite/schema/schema_generated.h"
//#include "tensorflow/lite/version.h"

#include "tensorflow/lite/micro/testing/micro_test.h"

//#include "model_data.h" // tensorflow lite model as C array
#include "tensorflow/lite/micro/examples/hello_world/model.h"

tflite::MicroErrorReporter micro_error_reporter;
tflite::ErrorReporter* error_reporter = &micro_error_reporter;

tflite::AllOpsResolver resolver;

const int tensor_arena_size = 32 * 1024;
uint8_t tensor_arena[tensor_arena_size];

void init_tflite() {

    const tflite::Model *model = ::tflite::GetModel(model);
    if (model->version() != TFLITE_SCHEMA_VERSION) {
        TF_LITE_REPORT_ERROR(error_reporter,
                             "Model provided is schema version %d not equal "
                             "to supported version %d.\n",
                             model->version(), TFLITE_SCHEMA_VERSION);
    }

    tflite::MicroInterpreter interpreter(model, resolver, tensor_arena,
                                         tensor_arena_size, error_reporter);

    interpreter.AllocateTensors();

    // Obtain a pointer to the model's input tensor
    TfLiteTensor* input = interpreter.input(0);

    // Make sure the input has the properties we expect
    TF_LITE_MICRO_EXPECT_NE(nullptr, input);
    // The property "dims" tells us the tensor's shape. It has one element for
    // each dimension. Our input is a 2D tensor containing 1 element, so "dims"
    // should have size 2.
    TF_LITE_MICRO_EXPECT_EQ(2, input->dims->size);
    // The value of each element gives the length of the corresponding tensor.
    // We should expect two single element tensors (one is contained within the
    // other).
    TF_LITE_MICRO_EXPECT_EQ(1, input->dims->data[0]);
    TF_LITE_MICRO_EXPECT_EQ(1, input->dims->data[1]);
    // The input is a 32 bit floating point value
    TF_LITE_MICRO_EXPECT_EQ(kTfLiteFloat32, input->type);

    input->data.f[0] = 0.;

    TfLiteStatus invoke_status = interpreter.Invoke();
    if (invoke_status != kTfLiteOk) {
        TF_LITE_REPORT_ERROR(error_reporter, "Invoke failed\n");
    }
    TF_LITE_MICRO_EXPECT_EQ(kTfLiteOk, invoke_status);

    TfLiteTensor* output = interpreter.output(0);
    TF_LITE_MICRO_EXPECT_EQ(2, output->dims->size);
    TF_LITE_MICRO_EXPECT_EQ(1, input->dims->data[0]);
    TF_LITE_MICRO_EXPECT_EQ(1, input->dims->data[1]);
    TF_LITE_MICRO_EXPECT_EQ(kTfLiteFloat32, output->type);

    // Obtain the output value from the tensor
    float value = output->data.f[0];
    // Check that the output value is within 0.05 of the expected value
    TF_LITE_MICRO_EXPECT_NEAR(0., value, 0.05);

    input->data.f[0] = 1.;
    interpreter.Invoke();
    value = output->data.f[0];
    TF_LITE_MICRO_EXPECT_NEAR(0.841, value, 0.05);

    input->data.f[0] = 3.;
    interpreter.Invoke();
    value = output->data.f[0];
    TF_LITE_MICRO_EXPECT_NEAR(0.141, value, 0.05);

    input->data.f[0] = 5.;
    interpreter.Invoke();
    value = output->data.f[0];
    TF_LITE_MICRO_EXPECT_NEAR(-0.959, value, 0.05);
}