/*
 *
 * This example shows the differences between ranging modes of VL53L5CX
 * (mode continuous and autonomous). For both modes, it initializes the VL53L5CX
 * ULD, set the mode, and starts a ranging to capture 10 frames.
 *
 * In this example, we also suppose that the number of target per zone is
 * set to 1 , and all output are enabled (see file platform.h).
 *
 *
 *  Copyright (c) 2021 Simon D. Levy
 *
 *  MIT License
*/ 


#include "Wire.h"

#include "Debugger.hpp"
#include "vl53l5cx_api.h"

static const uint8_t LPN_PIN = 3;

static VL53L5CX_Configuration Dev = {};  // Sensor configuration

void setup(void)
{
    // Start I^2C
    Wire.begin();

    // Start serial debugging
    Serial.begin(115200);

    // Fill the platform structure with customer's implementation. For this
    // example, only the I2C address is used.
    Dev.platform.address = 0x29;

    // Reset the sensor by toggling the LPN pin
    Reset_Sensor(LPN_PIN);

    // Make sure there is a VL53L5CX sensor connected
    uint8_t isAlive = 0;
    uint8_t error = vl53l5cx_is_alive(&Dev, &isAlive);
    if(!isAlive || error) {
        Debugger::reportForever("VL53L5CX not detected at requested address");
    }

    // Init VL53L5CX sensor
    error = vl53l5cx_init(&Dev);
    if(error) {
        Debugger::reportForever("VL53L5CX ULD Loading failed");
    }

    Debugger::printf("VL53L5CX ULD ready ! (Version : %s)\n",
            VL53L5CX_API_REVISION);

    // Set ranging mode autonomous  
    uint8_t status = vl53l5cx_set_ranging_mode(&Dev, VL53L5CX_RANGING_MODE_AUTONOMOUS);
    if(status) {
        Debugger::reportForever("vl53l5cx_set_ranging_mode failed, status %u\n", status);
    }

    // Using autonomous mode, the integration time can be updated (not possible
    // using continuous)
    vl53l5cx_set_integration_time_ms(&Dev, 20);

    // Start a ranging session 
    vl53l5cx_start_ranging(&Dev);
    Debugger::printf("Start ranging autonomous\n");
}

void loop(void)
{
    VL53L5CX_ResultsData Results = {};  // Results data from VL53L5CX
}

/*

    loop = 0;
    while(loop < 10)
    {
        status = vl53l5cx_check_data_ready(&Dev, &isReady);
        if(isReady)
        {
            vl53l5cx_get_ranging_data(&Dev, &Results);

            // As the sensor is set in 4x4 mode by default, we have a total
            // of 16 zones to print. For this example, only the data of first zone are
            // printed
            Debug::printf("Print data no : %3u\n", Dev.streamcount);
            for(i = 0; i < 16; i++)
            {
                Debug::printf("Zone : %3d, Status : %3u, Distance : %4d mm\n",
                        i,
                        Results.target_status[VL53L5CX_NB_TARGET_PER_ZONE*i],
                        Results.distance_mm[VL53L5CX_NB_TARGET_PER_ZONE*i]);
            }
            Debug::printf("\n");
            loop++;
        }

        // Wait a few ms to avoid too high polling (function in platform
        // file, not in API)
        WaitMs(&(Dev.platform), 5);
    }

    status = vl53l5cx_stop_ranging(&Dev);
    Debug::printf("Stop ranging autonomous\n");


    // ******************************
    // Set ranging mode continuous   
    // ******************************

    // In continuous mode, the integration time cannot be programmed
    // (automatically set to maximum value)

    status = vl53l5cx_set_ranging_mode(&Dev, VL53L5CX_RANGING_MODE_CONTINUOUS);
    if(status)
    {
        Debug::printf("vl53l5cx_set_ranging_mode failed, status %u\n", status);
        return status;
    }

    // Trying to update value below will have no impact on integration time 
    //status = vl53l5cx_set_integration_time_ms(&Dev, 20);

    // Start a ranging session 
    status = vl53l5cx_start_ranging(&Dev);
    Debug::printf("Start ranging continuous\n");

    loop = 0;
    while(loop < 10)
    {
        status = vl53l5cx_check_data_ready(&Dev, &isReady);
        if(isReady)
        {
            vl53l5cx_get_ranging_data(&Dev, &Results);

            // As the sensor is set in 4x4 mode by default, we have a total
            // of 16 zones to print 
            Debug::printf("Print data no : %3u\n", Dev.streamcount);
            for(i = 0; i < 16; i++)
            {
                Debug::printf("Zone : %3d, Status : %3u, Distance : %4d mm\n",
                        i,
                        Results.target_status[VL53L5CX_NB_TARGET_PER_ZONE*i],
                        Results.distance_mm[VL53L5CX_NB_TARGET_PER_ZONE*i]);
            }
            Debug::printf("\n");
            loop++;
        }

        // Wait a few ms to avoid too high polling (function in platform
        // file, not in API)
        WaitMs(&(Dev.platform), 5);
    }

    status = vl53l5cx_stop_ranging(&Dev);
    Debug::printf("Stop ranging continuous\n");

    Debug::printf("End of ULD demo\n");
    return status;
}
*/
