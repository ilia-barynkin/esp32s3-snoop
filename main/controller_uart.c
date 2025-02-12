#include "controller_uart.h"
#include "memory.h"

QueueHandle_t uart_tx_queue = NULL;
QueueHandle_t uart_rx_queue = NULL;

void uart_controller_init(void) {
    uart_config_t uart_config = {
        .baud_rate = CONFIG_UART_BAUD_RATE,  // Set baud rate 
        .data_bits = UART_DATA_8_BITS,     // 8 data bits 
        .parity    = UART_PARITY_DISABLE,  // No parity bit 
        .stop_bits = UART_STOP_BITS_1,     // 1 stop bit 
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE, // Disable hardware flow control 
        .source_clk = UART_SCLK_DEFAULT,   // Default clock source 
    };

    // TODO: wtf is this?
    int intr_alloc_flags = 0;  // Interrupt allocation flags 

#if CONFIG_UART_ISR_IN_IRAM
    intr_alloc_flags = ESP_INTR_FLAG_IRAM;  // Allocate interrupts to IRAM 
#endif

    // Install UART driver 
    ESP_ERROR_CHECK(uart_driver_install(UART_NUM_1, CONFIG_UART_BUF_SZ * 2, 0, 0, NULL, intr_alloc_flags));
    ESP_ERROR_CHECK(uart_param_config(UART_NUM_1, &uart_config));
    ESP_ERROR_CHECK(uart_set_pin(UART_NUM_1, CONFIG_UART_TX_PIN, CONFIG_UART_RX_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE));  // Set UART pins 

    uart_tx_queue = xQueueCreate(10, sizeof(uart_message_t));
    uart_rx_queue = xQueueCreate(10, sizeof(uart_message_t));
}

void uart_controller_task(void *pvParameters) {
    uart_message_t message;
    uint8_t data[128];

    uart_controller_init();

    while (1) {
        if (xQueueReceive(uart_tx_queue, &message, portMAX_DELAY) == pdPASS) {
            uart_write_bytes(UART_NUM_1, (const char *)message.data, message.length);
        }
        int length = uart_read_bytes(UART_NUM_1, data, sizeof(data), pdMS_TO_TICKS(100));
        if (length > 0) {
            uart_message_t response;
            memcpy(response.data, data, length);
            response.length = length;
            xQueueSend(uart_rx_queue, &response, portMAX_DELAY);
        }
    }
}