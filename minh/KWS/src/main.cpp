<<<<<<< HEAD
/* Edge Impulse Arduino examples
 * Copyright (c) 2022 EdgeImpulse Inc.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

// If your target is limited in memory remove this macro to save 10K RAM
#define EIDSP_QUANTIZE_FILTERBANK   0

/*
 ** NOTE: If you run into TFLite arena allocation issue.
 **
 ** This may be due to may dynamic memory fragmentation.
 ** Try defining "-DEI_CLASSIFIER_ALLOCATION_STATIC" in boards.local.txt (create
 ** if it doesn't exist) and copy this file to
 ** `<ARDUINO_CORE_INSTALL_PATH>/arduino/hardware/<mbed_core>/<core_version>/`.
 **
 ** See
 ** (https://support.arduino.cc/hc/en-us/articles/360012076960-Where-are-the-installed-cores-located-)
 ** to find where Arduino installs cores on your machine.
 **
 ** If the problem persists then there's not enough memory for this model and application.
 */

/* Includes ---------------------------------------------------------------- */
#include <KWS_inferencing.h>
#include "HardwareSerial.h"
#include <I2S.h>
#define SAMPLE_RATE 16000U
#define SAMPLE_BITS 16

typedef struct {
    int16_t *buffer;
    uint8_t buf_ready;
    uint32_t buf_count;
    uint32_t n_samples;
} inference_t;

static bool microphone_inference_record(void);
static int microphone_audio_signal_get_data(size_t offset, size_t length, float *out_ptr);
static bool microphone_inference_start(uint32_t n_samples);
void Sys_init(void);
void Sys_run(void);
HardwareSerial uart1(1);

static inference_t inference;
static const uint32_t sample_buffer_size = 2048;
static signed short sampleBuffer[sample_buffer_size];
static signed short ringbuffer[sample_buffer_size] = {0};
static bool debug_nn = false; // Set this to true to see e.g. features generated from the raw signal
static bool record_status = true; // Set this to true to see e.g. features generated from the raw signal
static int print_results = -(EI_CLASSIFIER_SLICES_PER_MODEL_WINDOW);
static bool flag_status = 0;

/**
 * @brief      Arduino setup function
 */

void setup()
{
    Sys_init();
}

/**
 * @brief      Arduino main function. Runs the inferencing loop.
 */
void loop()
{
    Sys_run();
}

void Sys_init(void)
{
    Serial.begin(115200);
    uart1.begin(115200, SERIAL_8N1, 44, 43);
    // comment out the below line to cancel the wait for USB connection (needed for native USB)
    while (!Serial);
    Serial.println("Edge Impulse Inferencing Demo");
    I2S.setAllPins(-1, 42, 41, -1, -1);
    if (!I2S.begin(PDM_MONO_MODE, SAMPLE_RATE, SAMPLE_BITS)) 
    {
        Serial.println("Failed to initialize I2S!");
        while (1) ;
    }
    // summary of inferencing settings (from model_metadata.h)
    // ei_printf("Inferencing settings:\n");
    // ei_printf("\tInterval: ");
    // ei_printf_float((float)EI_CLASSIFIER_INTERVAL_MS);
    // ei_printf(" ms.\n");
    // ei_printf("\tFrame size: %d\n", EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE);
    // ei_printf("\tSample length: %d ms.\n", EI_CLASSIFIER_RAW_SAMPLE_COUNT / 16);
    // ei_printf("\tNo. of classes: %d\n", sizeof(ei_classifier_inferencing_categories) / sizeof(ei_classifier_inferencing_categories[0]));

    run_classifier_init();
    // ei_printf("\nStarting continious inference in 2 seconds...\n");
    // ei_sleep(2000);

    if (microphone_inference_start(EI_CLASSIFIER_SLICE_SIZE) == false) 
    {
        ei_printf("ERR: Could not allocate audio buffer (size %d), this could be due to the window length of your model\r\n", EI_CLASSIFIER_RAW_SAMPLE_COUNT);
        return;
    }

    ei_printf("Recording...\n");
}

void Sys_run(void)
{
        bool m = microphone_inference_record();
    if (!m) 
    {
        ei_printf("ERR: Failed to record audio...\n");
        return;
    }

    signal_t signal;
    signal.total_length = EI_CLASSIFIER_SLICE_SIZE;
    signal.get_data = &microphone_audio_signal_get_data;
    ei_impulse_result_t result = {0};

    EI_IMPULSE_ERROR r = run_classifier_continuous(&signal, &result, debug_nn);
    if (r != EI_IMPULSE_OK) 
    {
        ei_printf("ERR: Failed to run classifier (%d)\n", r);
        return;
    }
    // *ringbuffer = *sampleBuffer;
    // Serial.print(*ringbuffer);
    // Serial.print("\n");
    // for(int i = 0; i < sample_buffer_size; i++) 
    //  {
    //     ringbuffer[i] = sampleBuffer[i];
    //     Serial.print(ringbuffer[i]);
    //     Serial.print(" ");
    // }
    if (++print_results >= (EI_CLASSIFIER_SLICES_PER_MODEL_WINDOW)) 
    {
        // print the predictions
        // ei_printf("Predictions ");
        // ei_printf("(DSP: %d ms., Classification: %d ms., Anomaly: %d ms.)",
        //     result.timing.dsp, result.timing.classification, result.timing.anomaly);
        // ei_printf("\n");
        for (size_t ix = 0; ix < EI_CLASSIFIER_LABEL_COUNT; ix++) {
        ei_printf("    %s: ", result.classification[ix].label);
        ei_printf_float(result.classification[ix].value);
        ei_printf("\n");
        if (result.classification[2].value > 0.8)
        {
            flag_status = 1;
            Serial.println("start");
        }

        if (result.classification[0].value > 0.8 && flag_status == 1)
        {
            uart1.println("1");
            Serial.println("1");
            flag_status = 0;
        }
        else if (result.classification[3].value > 0.8 && flag_status == 1)
        {
            uart1.println("0");
            Serial.println("0");
            flag_status = 0;
        }
    }
        


        ei_printf("\n");
#if EI_CLASSIFIER_HAS_ANOMALY == 1
        ei_printf("    anomaly score: ");
        ei_printf_float(result.anomaly);
        ei_printf("\n");
#endif

        print_results = 0;
    }
}

static void audio_inference_callback(uint32_t n_bytes)
{
     for(int i = 0; i < n_bytes>>1; i++) 
     {
        inference.buffer[inference.buf_count++] = sampleBuffer[i];

        if(inference.buf_count >= inference.n_samples) 
        {
          inference.buf_count = 0;
          inference.buf_ready = 1;
        }
    }
}

static void capture_samples(void* arg) 
{

  const int32_t i2s_bytes_to_read = (uint32_t)arg;
  size_t bytes_read = i2s_bytes_to_read;

  while (record_status) 
  {

    /* read data at once from i2s */
    esp_i2s::i2s_read(esp_i2s::I2S_NUM_0, (void*)sampleBuffer, i2s_bytes_to_read, &bytes_read, 100);

   if (bytes_read <= 0) 
   {
      ei_printf("Error in I2S read : %d", bytes_read);
    }
    else 
    {
        if (bytes_read < i2s_bytes_to_read) 
        {
        ei_printf("Partial I2S read");
        }

        // scale the data (otherwise the sound is too quiet)
        for (int x = 0; x < i2s_bytes_to_read/2; x++) 
        {
            sampleBuffer[x] = (int16_t)(sampleBuffer[x]) * 8;
        }

        if (record_status) 
        {
            audio_inference_callback(i2s_bytes_to_read);
        }
        else 
        {
            break;
        }
    }
  }
  vTaskDelete(NULL);
}

static bool microphone_inference_start(uint32_t n_samples)
{
    inference.buffer = (int16_t *)malloc(n_samples * sizeof(int16_t));

    if(inference.buffer == NULL) 
    {
        return false;
    }

    inference.buf_count  = 0;
    inference.n_samples  = n_samples;
    inference.buf_ready  = 0;

//    if (i2s_init(EI_CLASSIFIER_FREQUENCY)) {
//        ei_printf("Failed to start I2S!");
//    }

    ei_sleep(100);

    record_status = true;

    xTaskCreate(capture_samples, "CaptureSamples", 1024 * 32, (void*)sample_buffer_size, 10, NULL);

    return true;
}

static bool microphone_inference_record(void)
{
    bool ret = true;

    if (inference.buf_ready == 1) 
    {
        ei_printf(
            "Error sample buffer overrun. Decrease the number of slices per model window "
            "(EI_CLASSIFIER_SLICES_PER_MODEL_WINDOW)\n");
        ret = false;
    }

    while (inference.buf_ready == 0) 
    {
        delay(1);
    }

    inference.buf_ready = 0;
    return true;
}
static int microphone_audio_signal_get_data(size_t offset, size_t length, float *out_ptr)
{
    numpy::int16_to_float(&inference.buffer[offset], out_ptr, length);

    return 0;
}

/**
 * @brief      Stop PDM and release buffers
 */
static void microphone_inference_end(void)
{
    free(sampleBuffer);
    ei_free(inference.buffer);
}

=======
/* Edge Impulse Arduino examples
 * Copyright (c) 2022 EdgeImpulse Inc.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

// If your target is limited in memory remove this macro to save 10K RAM
#define EIDSP_QUANTIZE_FILTERBANK   0

/*
 ** NOTE: If you run into TFLite arena allocation issue.
 **
 ** This may be due to may dynamic memory fragmentation.
 ** Try defining "-DEI_CLASSIFIER_ALLOCATION_STATIC" in boards.local.txt (create
 ** if it doesn't exist) and copy this file to
 ** `<ARDUINO_CORE_INSTALL_PATH>/arduino/hardware/<mbed_core>/<core_version>/`.
 **
 ** See
 ** (https://support.arduino.cc/hc/en-us/articles/360012076960-Where-are-the-installed-cores-located-)
 ** to find where Arduino installs cores on your machine.
 **
 ** If the problem persists then there's not enough memory for this model and application.
 */

/* Includes ---------------------------------------------------------------- */
#include <KWS_inferencing.h>
#include "HardwareSerial.h"
#include <I2S.h>
#define SAMPLE_RATE 16000U
#define SAMPLE_BITS 16

typedef struct {
    int16_t *buffer;
    uint8_t buf_ready;
    uint32_t buf_count;
    uint32_t n_samples;
} inference_t;

static bool microphone_inference_record(void);
static int microphone_audio_signal_get_data(size_t offset, size_t length, float *out_ptr);
static bool microphone_inference_start(uint32_t n_samples);
void Sys_init(void);
void Sys_run(void);
HardwareSerial uart1(1);

static inference_t inference;
static const uint32_t sample_buffer_size = 2048;
static signed short sampleBuffer[sample_buffer_size];
static signed short ringbuffer[sample_buffer_size] = {0};
static bool debug_nn = false; // Set this to true to see e.g. features generated from the raw signal
static bool record_status = true; // Set this to true to see e.g. features generated from the raw signal
static int print_results = -(EI_CLASSIFIER_SLICES_PER_MODEL_WINDOW);
static bool flag_status = 0;

/**
 * @brief      Arduino setup function
 */

void setup()
{
    Sys_init();
}

/**
 * @brief      Arduino main function. Runs the inferencing loop.
 */
void loop()
{
    Sys_run();
}

void Sys_init(void)
{
    Serial.begin(115200);
    uart1.begin(115200, SERIAL_8N1, 44, 43);
    // comment out the below line to cancel the wait for USB connection (needed for native USB)
    while (!Serial);
    Serial.println("Edge Impulse Inferencing Demo");
    I2S.setAllPins(-1, 42, 41, -1, -1);
    if (!I2S.begin(PDM_MONO_MODE, SAMPLE_RATE, SAMPLE_BITS)) 
    {
        Serial.println("Failed to initialize I2S!");
        while (1) ;
    }
    // summary of inferencing settings (from model_metadata.h)
    // ei_printf("Inferencing settings:\n");
    // ei_printf("\tInterval: ");
    // ei_printf_float((float)EI_CLASSIFIER_INTERVAL_MS);
    // ei_printf(" ms.\n");
    // ei_printf("\tFrame size: %d\n", EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE);
    // ei_printf("\tSample length: %d ms.\n", EI_CLASSIFIER_RAW_SAMPLE_COUNT / 16);
    // ei_printf("\tNo. of classes: %d\n", sizeof(ei_classifier_inferencing_categories) / sizeof(ei_classifier_inferencing_categories[0]));

    run_classifier_init();
    // ei_printf("\nStarting continious inference in 2 seconds...\n");
    // ei_sleep(2000);

    if (microphone_inference_start(EI_CLASSIFIER_SLICE_SIZE) == false) 
    {
        ei_printf("ERR: Could not allocate audio buffer (size %d), this could be due to the window length of your model\r\n", EI_CLASSIFIER_RAW_SAMPLE_COUNT);
        return;
    }

    ei_printf("Recording...\n");
}

void Sys_run(void)
{
        bool m = microphone_inference_record();
    if (!m) 
    {
        ei_printf("ERR: Failed to record audio...\n");
        return;
    }

    signal_t signal;
    signal.total_length = EI_CLASSIFIER_SLICE_SIZE;
    signal.get_data = &microphone_audio_signal_get_data;
    ei_impulse_result_t result = {0};

    EI_IMPULSE_ERROR r = run_classifier_continuous(&signal, &result, debug_nn);
    if (r != EI_IMPULSE_OK) 
    {
        ei_printf("ERR: Failed to run classifier (%d)\n", r);
        return;
    }
    // *ringbuffer = *sampleBuffer;
    // Serial.print(*ringbuffer);
    // Serial.print("\n");
    // for(int i = 0; i < sample_buffer_size; i++) 
    //  {
    //     ringbuffer[i] = sampleBuffer[i];
    //     Serial.print(ringbuffer[i]);
    //     Serial.print(" ");
    // }
    if (++print_results >= (EI_CLASSIFIER_SLICES_PER_MODEL_WINDOW)) 
    {
        // print the predictions
        // ei_printf("Predictions ");
        // ei_printf("(DSP: %d ms., Classification: %d ms., Anomaly: %d ms.)",
        //     result.timing.dsp, result.timing.classification, result.timing.anomaly);
        // ei_printf("\n");
        for (size_t ix = 0; ix < EI_CLASSIFIER_LABEL_COUNT; ix++) {
        ei_printf("    %s: ", result.classification[ix].label);
        ei_printf_float(result.classification[ix].value);
        ei_printf("\n");
        if (result.classification[2].value > 0.8)
        {
            flag_status = 1;
            Serial.println("start");
        }

        if (result.classification[0].value > 0.8 && flag_status == 1)
        {
            uart1.println("1");
            Serial.println("1");
            flag_status = 0;
        }
        else if (result.classification[3].value > 0.8 && flag_status == 1)
        {
            uart1.println("0");
            Serial.println("0");
            flag_status = 0;
        }
    }
        


        ei_printf("\n");
#if EI_CLASSIFIER_HAS_ANOMALY == 1
        ei_printf("    anomaly score: ");
        ei_printf_float(result.anomaly);
        ei_printf("\n");
#endif

        print_results = 0;
    }
}

static void audio_inference_callback(uint32_t n_bytes)
{
     for(int i = 0; i < n_bytes>>1; i++) 
     {
        inference.buffer[inference.buf_count++] = sampleBuffer[i];

        if(inference.buf_count >= inference.n_samples) 
        {
          inference.buf_count = 0;
          inference.buf_ready = 1;
        }
    }
}

static void capture_samples(void* arg) 
{

  const int32_t i2s_bytes_to_read = (uint32_t)arg;
  size_t bytes_read = i2s_bytes_to_read;

  while (record_status) 
  {

    /* read data at once from i2s */
    esp_i2s::i2s_read(esp_i2s::I2S_NUM_0, (void*)sampleBuffer, i2s_bytes_to_read, &bytes_read, 100);

   if (bytes_read <= 0) 
   {
      ei_printf("Error in I2S read : %d", bytes_read);
    }
    else 
    {
        if (bytes_read < i2s_bytes_to_read) 
        {
        ei_printf("Partial I2S read");
        }

        // scale the data (otherwise the sound is too quiet)
        for (int x = 0; x < i2s_bytes_to_read/2; x++) 
        {
            sampleBuffer[x] = (int16_t)(sampleBuffer[x]) * 8;
        }

        if (record_status) 
        {
            audio_inference_callback(i2s_bytes_to_read);
        }
        else 
        {
            break;
        }
    }
  }
  vTaskDelete(NULL);
}

static bool microphone_inference_start(uint32_t n_samples)
{
    inference.buffer = (int16_t *)malloc(n_samples * sizeof(int16_t));

    if(inference.buffer == NULL) 
    {
        return false;
    }

    inference.buf_count  = 0;
    inference.n_samples  = n_samples;
    inference.buf_ready  = 0;

//    if (i2s_init(EI_CLASSIFIER_FREQUENCY)) {
//        ei_printf("Failed to start I2S!");
//    }

    ei_sleep(100);

    record_status = true;

    xTaskCreate(capture_samples, "CaptureSamples", 1024 * 32, (void*)sample_buffer_size, 10, NULL);

    return true;
}

static bool microphone_inference_record(void)
{
    bool ret = true;

    if (inference.buf_ready == 1) 
    {
        ei_printf(
            "Error sample buffer overrun. Decrease the number of slices per model window "
            "(EI_CLASSIFIER_SLICES_PER_MODEL_WINDOW)\n");
        ret = false;
    }

    while (inference.buf_ready == 0) 
    {
        delay(1);
    }

    inference.buf_ready = 0;
    return true;
}
static int microphone_audio_signal_get_data(size_t offset, size_t length, float *out_ptr)
{
    numpy::int16_to_float(&inference.buffer[offset], out_ptr, length);

    return 0;
}

/**
 * @brief      Stop PDM and release buffers
 */
static void microphone_inference_end(void)
{
    free(sampleBuffer);
    ei_free(inference.buffer);
}

>>>>>>> 1fec75929e8613230dfa903ad245c1eb954b857d
