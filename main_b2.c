#include "stm32f10x.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "FreeRTOS.h"
#include "task.h"

// Bien trang thai UART
volatile uint8_t uart_busy = 0;

// Khoi tao UART1 
void UART1_Init(void) {
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;

    // Bat clock GPIOA và USART1
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_USART1, ENABLE);

    // PA9: TX
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // PA10: RX
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // Cau hinh USART1
    USART_InitStructure.USART_BaudRate = 115200;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
    USART_Init(USART1, &USART_InitStructure);

    USART_Cmd(USART1, ENABLE);
}

// Gui 1 ki tu
void UART1_SendChar(char c) {
    while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
    USART_SendData(USART1, c);
}

// Gui chuoi
void UART1_SendString(const char *str) {
    while (*str) {
        UART1_SendChar(*str++);
    }
}

// Task 1
void Task1(void *pvParameters) {
    const char *msg = "Task 1: Hello from Task 1\r\n";
    const char *conflict = "Task 1: XUNG DOT UART!\r\n";
    while (1) {
        if (uart_busy) {
            UART1_SendString(conflict);
        } else {
            uart_busy = 1;
            UART1_SendString(msg);
						vTaskDelay(pdMS_TO_TICKS(500));
            uart_busy = 0;
        }
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

// Task 2
void Task2(void *pvParameters) {
    const char *msg = "Task 2: Hello from Task 2\r\n";
    const char *conflict = "Task 2: XUNG DOT UART!\r\n";
    while (1) {
        if (uart_busy) {
            UART1_SendString(conflict);
        } else {
            uart_busy = 1;
            UART1_SendString(msg);
						vTaskDelay(pdMS_TO_TICKS(500));
            uart_busy = 0;
        }
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

// Hàm main
int main(void) {
    SystemInit();
    UART1_Init();

    xTaskCreate(Task1, "Task1", 128, NULL, 1, NULL);
    xTaskCreate(Task2, "Task2", 128, NULL, 1, NULL);

    vTaskStartScheduler();

    while (1);
}
