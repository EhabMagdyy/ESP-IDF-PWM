/*************************************************************************************************************************
********************************    Author  : Ehab Magdy Abdullah                      ***********************************
********************************    Linkedin: https://www.linkedin.com/in/ehabmagdyy/  ***********************************
********************************    Youtube : https://www.youtube.com/@EhabMagdyy      ***********************************
**************************************************************************************************************************/

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/ledc.h"

void PWM_Task(void *pvParameter);

#define PWM_RESOLUTION  LEDC_TIMER_12_BIT     // 12-bit PWM resolution
#define MAX_DUTY_CYCLE  4095                  // 2^PWM_RESOLUTION(12) - 1
#define PWM_GPIO        GPIO_NUM_25           // GPIO 25 for PWM output

void app_main(void)
{
    // Create PWM task
    xTaskCreate(PWM_Task, "PWM_Task", 2048, NULL, 5, NULL);
}

void PWM_Task(void *pvParameter)
{
    // Configure PWM timer
    ledc_timer_config_t ledc_timer = {
        .speed_mode = LEDC_HIGH_SPEED_MODE, // Use High-Speed mode
        .timer_num = LEDC_TIMER_0,          // Use Timer 0
        .duty_resolution = PWM_RESOLUTION,  // 12-bit duty cycle  (2^12 = 4096)
        .freq_hz = 5000,                    // 5 kHz PWM frequency
        .clk_cfg = LEDC_USE_APB_CLK         // Use APB clock for faster response
    };
    ledc_timer_config(&ledc_timer);

    // Configure PWM channel
    ledc_channel_config_t ledc_channel = {
        .channel = LEDC_CHANNEL_0,          // Using Channel 
        .duty = 0,                          // Start with 0 duty cycle
        .gpio_num = PWM_GPIO,               // GPIO for output pwm
        .speed_mode = LEDC_HIGH_SPEED_MODE, // Use High-Speed mode
        .hpoint = 0,                        // Default high point
        .timer_sel = LEDC_TIMER_0           // Use Timer 0
    };
    ledc_channel_config(&ledc_channel);

    for(;;)
    {
        // Gradually increase duty cycle
        for(int dutyCycle = 0 ; dutyCycle <= MAX_DUTY_CYCLE ; dutyCycle += 100)
        {
            ledc_set_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_0, dutyCycle);
            ledc_update_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_0);
            vTaskDelay(pdMS_TO_TICKS(10));
        }

        // Gradually decrease duty cycle
        for(int dutyCycle = MAX_DUTY_CYCLE ; dutyCycle > 0 ; dutyCycle -= 100)
        {
            ledc_set_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_0, dutyCycle);
            ledc_update_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_0);
            vTaskDelay(pdMS_TO_TICKS(10));
        }

        // for mapping 0-100 to 0-4095
        // dutyCycleValue = (dutyCycleFrom0to100 / 100.0) * MAX_DUTY_CYCLE
    }
}