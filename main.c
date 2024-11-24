#include <stdbool.h>
#include <stdint.h>

#include "nrf52840.h"
#include "nrf_delay.h"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"

#define SAMPLES_IN_BUFFER 1

static int16_t m_buffer_pool[SAMPLES_IN_BUFFER];  // Буфер для значений SAADC

void saadc_init(void) {
    // Включение SAADC
    NRF_SAADC->ENABLE = SAADC_ENABLE_ENABLE_Enabled;
    NRF_SAADC->RESOLUTION = SAADC_RESOLUTION_VAL_12bit;  // 12-битное разрешение
    NRF_SAADC->OVERSAMPLE = SAADC_OVERSAMPLE_OVERSAMPLE_Bypass;

    // Настройка канала 0
    NRF_SAADC->CH[0].CONFIG = (SAADC_CH_CONFIG_GAIN_Gain1_6 << SAADC_CH_CONFIG_GAIN_Pos) |
                              (SAADC_CH_CONFIG_MODE_SE << SAADC_CH_CONFIG_MODE_Pos) |
                              (SAADC_CH_CONFIG_REFSEL_Internal << SAADC_CH_CONFIG_REFSEL_Pos) |
                              (SAADC_CH_CONFIG_TACQ_10us << SAADC_CH_CONFIG_TACQ_Pos) |
                              (SAADC_CH_CONFIG_BURST_Disabled << SAADC_CH_CONFIG_BURST_Pos);
    NRF_SAADC->CH[0].PSELN = 0;                                  // Не используется в SE-режиме
    NRF_SAADC->CH[0].PSELP = SAADC_CH_PSELP_PSELP_AnalogInput2;  // AIN2

    // Настройка буфера
    NRF_SAADC->RESULT.PTR = (uint32_t)m_buffer_pool;
    NRF_SAADC->RESULT.MAXCNT = SAMPLES_IN_BUFFER;
}

int16_t saadc_sample(void) {
    // Запуск измерения
    NRF_SAADC->TASKS_START = 1;          // Запуск SAADC
    while (!NRF_SAADC->EVENTS_STARTED);  // Ожидание события STARTED
    NRF_SAADC->EVENTS_STARTED = 0;

    NRF_SAADC->TASKS_SAMPLE = 1;     // Сборка выборки
    while (!NRF_SAADC->EVENTS_END);  // Ожидание окончания
    NRF_SAADC->EVENTS_END = 0;

    // Считывание результата
    int16_t result = m_buffer_pool[0];

    // Подготовка к следующему измерению
    NRF_SAADC->TASKS_STOP = 1;           // Остановка SAADC
    while (!NRF_SAADC->EVENTS_STOPPED);  // Ожидание события STOPPED
    NRF_SAADC->EVENTS_STOPPED = 0;

    return result;
}

int main(void) {
    // Инициализация логирования
    APP_ERROR_CHECK(NRF_LOG_INIT(NULL));
    NRF_LOG_DEFAULT_BACKENDS_INIT();

    NRF_LOG_INFO("SAADC polling example started.");

    // Инициализация SAADC
    saadc_init();

    while (1) {
        // Получение значения
        int16_t adc_value = saadc_sample();
        NRF_LOG_INFO("ADC Value: %d", adc_value);

        // Задержка в 400 мс
        nrf_delay_ms(400);

        NRF_LOG_FLUSH();
    }
}