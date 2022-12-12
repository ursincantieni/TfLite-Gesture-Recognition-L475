/*
* Created by Ursin Cantieni on 06.12.22.
*/

#ifndef TFLITE_INTERFACE_HPP
#define TFLITE_INTERFACE_HPP

#ifdef __cplusplus
extern "C" {
#endif

void initTflite();
void addValues(float x, float y, float z);
void runInference(float* output);
void runTest();

#ifdef __cplusplus
}
#endif

static void print_model_info();

#endif //TFLITE_INTERFACE_HPP
