 /*
 * MAIN Generated Driver File
 * 
 * @file main.c
 * 
 * @defgroup main MAIN
 * 
 * @brief This is the generated driver implementation file for the MAIN driver.
 *
 * @version MAIN Driver Version 1.0.2
 *
 * @version Package Version: 3.1.2
*/

/*
© [2024] Microchip Technology Inc. and its subsidiaries.

    Subject to your compliance with these terms, you may use Microchip 
    software and any derivatives exclusively with Microchip products. 
    You are responsible for complying with 3rd party license terms  
    applicable to your use of 3rd party software (including open source  
    software) that may accompany Microchip software. SOFTWARE IS ?AS IS.? 
    NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS 
    SOFTWARE, INCLUDING ANY IMPLIED WARRANTIES OF NON-INFRINGEMENT,  
    MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT 
    WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY 
    KIND WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF 
    MICROCHIP HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE 
    FORESEEABLE. TO THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP?S 
    TOTAL LIABILITY ON ALL CLAIMS RELATED TO THE SOFTWARE WILL NOT 
    EXCEED AMOUNT OF FEES, IF ANY, YOU PAID DIRECTLY TO MICROCHIP FOR 
    THIS SOFTWARE.
*/
#include "mcc_generated_files/system/system.h"
#include "embedded-i2c-scd4x/scd4x_i2c.h"
#include "embedded-i2c-scd4x/sensirion_common.h"
#include "embedded-i2c-scd4x/sensirion_i2c_hal.h"

/*
    Main application
*/

int main(void)
{
    SYSTEM_Initialize();

    int16_t error = 0;

    sensirion_i2c_hal_init();

    // Clean up potential SCD40 states
    scd4x_wake_up();
    scd4x_stop_periodic_measurement();
    scd4x_reinit();

    uint16_t serial_0;
    uint16_t serial_1;
    uint16_t serial_2;
    error = scd4x_get_serial_number(&serial_0, &serial_1, &serial_2);
    if (error) {
	    printf("Error executing scd4x_get_serial_number(): %i\n", error);
	    } else {
	    printf("serial: 0x%04x%04x%04x\n", serial_0, serial_1, serial_2);
    }

    // Start Measurement

    error = scd4x_start_periodic_measurement();
    if (error) {
	    printf("Error executing scd4x_start_periodic_measurement(): %i\n",error);
    }

    printf("Waiting for first measurement... (5 sec)\n");

    for (;;) {
	    // Read Measurement
	    sensirion_i2c_hal_sleep_usec(100000);
	    bool data_ready_flag = false;
	    error = scd4x_get_data_ready_flag(&data_ready_flag);
	    if (error) {
		    printf("Error executing scd4x_get_data_ready_flag(): %i\n", error);
		    continue;
	    }
	    if (!data_ready_flag) {
		    continue;
	    }

	    uint16_t co2;
	    int32_t temperature;
	    int32_t humidity;
	    error = scd4x_read_measurement(&co2, &temperature, &humidity);
	    if (error) {
		    printf("Error executing scd4x_read_measurement(): %i\n", error);
		    } else if (co2 == 0) {
		    printf("Invalid sample detected, skipping.\n");
		    } else {
		    printf("CO2: %u\n", co2);
		    printf("Temperature: %2.1f °C\n", temperature/1000.0);
		    printf("Humidity: %2.1f RH\n", humidity/1000.0);
	    }
    }
}