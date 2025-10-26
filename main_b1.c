#include "stm32f10x.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h" // Thu vien cho Semaphore

/*  Dinh nghia Cac Chan  */
#define LED1_PIN        GPIO_Pin_1    // Noi vào PA1
#define LED1_PORT       GPIOA
#define LED1_CLK        RCC_APB2Periph_GPIOA

#define LED2_PIN        GPIO_Pin_2    // Noi vào PA2
#define LED2_PORT       GPIOA
#define LED2_CLK        RCC_APB2Periph_GPIOA

#define BUTTON_PIN      GPIO_Pin_0    // Noi vào PA0
#define BUTTON_PORT     GPIOA
#define BUTTON_CLK      RCC_APB2Periph_GPIOA
#define BUTTON_AFIO_CLK RCC_APB2Periph_AFIO
#define BUTTON_EXTI_LINE EXTI_Line0
#define BUTTON_IRQn      EXTI0_IRQn
#define BUTTON_PORT_SOURCE GPIO_PortSourceGPIOA
#define BUTTON_PIN_SOURCE  GPIO_PinSource0

/* Khai báo prototype cho các hàm */
void Hardware_Config(void);
void vBlinkLEDTask(void *pvParameters);
void vHandleAlertTask(void *pvParameters);
void EXTI0_IRQHandler(void); 

/* Khai bao bien handle cho Semaphore */
SemaphoreHandle_t xButtonSemaphore = NULL;

/**
  * @brief  Hàm Main
  */
int main(void)
{
    Hardware_Config();
    xButtonSemaphore = xSemaphoreCreateBinary();

    if (xButtonSemaphore != NULL)
    {
        // Task 1: Nháy LED 1 (Uu tiên thap)
        xTaskCreate(vBlinkLEDTask,"BlinkLED",128,NULL,
                    tskIDLE_PRIORITY + 1, // Priority 1
                    NULL);

        // Task 2: Bat LED 2 (Uu tiên cao hon)
        xTaskCreate(vHandleAlertTask,
                    "AlertTask",
                    128,
                    NULL,
                    tskIDLE_PRIORITY + 2, // Priority 2 (CAO HON)
                    NULL);

        vTaskStartScheduler();
    }
    
    while (1);
}

/**
  * @brief C?u hình phan cung (GPIO, EXTI, NVIC)
  */
void Hardware_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    EXTI_InitTypeDef EXTI_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    // B?t clock cho Port A và AFIO
    RCC_APB2PeriphClockCmd(LED1_CLK | BUTTON_CLK | BUTTON_AFIO_CLK, ENABLE);
    
    /* --- C?u hình GPIO --- */
    // Cau hình LED 1 (PA1) và LED 2 (PA2)
    GPIO_InitStructure.GPIO_Pin = LED1_PIN | LED2_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(LED1_PORT, &GPIO_InitStructure);
    
    GPIO_WriteBit(LED1_PORT, LED1_PIN, Bit_RESET);
    GPIO_WriteBit(LED2_PORT, LED2_PIN, Bit_RESET);
    
    // Cau hình Nút nhan (PA0)
    GPIO_InitStructure.GPIO_Pin = BUTTON_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 
    GPIO_Init(BUTTON_PORT, &GPIO_InitStructure);
    
    /* --- Cau hình EXTI và NVIC --- */
    GPIO_EXTILineConfig(BUTTON_PORT_SOURCE, BUTTON_PIN_SOURCE);

    EXTI_InitStructure.EXTI_Line = BUTTON_EXTI_LINE;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    // Cau hình NVIC (Bo dieu khien ngat)
    
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4); 
    
    NVIC_InitStructure.NVIC_IRQChannel = BUTTON_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 5; 
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

/**
  * @brief TASK 1: Nháy LED 1
  */
void vBlinkLEDTask(void *pvParameters)
{
    const TickType_t xDelay = pdMS_TO_TICKS(500);
    while(1)
    {
        // Ðao tr?ng thái LED 1
        GPIO_WriteBit(LED1_PORT, LED1_PIN, (BitAction)(1 - GPIO_ReadOutputDataBit(LED1_PORT, LED1_PIN)));
        // Tam dung task
        vTaskDelay(xDelay);
    }
}

/**
  * @brief TASK 2: Cho ngat và bat LED 2 
  */
void vHandleAlertTask(void *pvParameters)
{
    TickType_t xStartTime;
    const TickType_t xBlockTime = pdMS_TO_TICKS(2000); // 2 giây

    while(1)
    {
        //    1.Task 1 dang chay bình thuong
        if (xSemaphoreTake(xButtonSemaphore, portMAX_DELAY) == pdTRUE)
        {
            // 2. Da nhan Semaphore Task 2 (Prio 2) chay.
            //    Task 1 (Prio 1) b? "dóng bang" (d?ng nháy).
            GPIO_WriteBit(LED2_PORT, LED2_PIN, Bit_SET); // B?t LED 2
            
            // 3. lay thoi gian bat dau
            xStartTime = xTaskGetTickCount();

            // 4. Vòng lap
            //    Task 2 (uu tien cao) chiem 100% CPU trong 2 s
            //    Task 1 (uu tien thap) task 1 dung task 2 chay
            while ( (xTaskGetTickCount() - xStartTime) < xBlockTime )
            {
               // khong lam gi ca chi cho 2s
            }
            
            // Khi het 2s
            GPIO_WriteBit(LED2_PORT, LED2_PIN, Bit_RESET); // Tat LED 2

        }
    }
}

/* HAM XU LY NGAT*/

void EXTI0_IRQHandler(void)
{
    // 1. Kiem tra ngat
    if(EXTI_GetITStatus(BUTTON_EXTI_LINE) != RESET)
    {
        BaseType_t xHigherPriorityTaskWoken = pdFALSE;

        //     "dánh thuc" Task 2
        if (xButtonSemaphore != NULL)
        {
            xSemaphoreGiveFromISR(xButtonSemaphore, &xHigherPriorityTaskWoken);
        }

        EXTI_ClearITPendingBit(BUTTON_EXTI_LINE);
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    }
}
