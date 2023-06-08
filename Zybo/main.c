/* FreeRTOS includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
/* Xilinx includes. */
#include "xgpio.h"
#include "xstatus.h"
#include "xparameters.h"
/* GPIO device that LEDs are connected to */
#define GPIO_DEVICE_ID  XPAR_AXI_GPIO_0_DEVICE_ID
/* GPIO port for LEDs */
#define LED_CHANNEL 1
#define DELAY_500_MilliSec 500
#define DELAY_700_MilliSec 700
static void StartBlink01( void *arguments );
static void StartBlink02( void *arguments );
static TaskHandle_t blink01Handle;
static TaskHandle_t blink02Handle;

XGpio Gpio;

int main( void ) {
   /* GPIO driver initialization */
   int status = XGpio_Initialize(&Gpio, GPIO_DEVICE_ID);
   if (status != XST_SUCCESS) {
       return XST_FAILURE;
   }
   /*Set the direction for the LEDs to output. */
   XGpio_SetDataDirection(&Gpio, LED_CHANNEL, 0x0);

   xTaskCreate(
               /* The function that implements the task. */
               StartBlink01,
               /* Text name for the task, provided to assist debugging only. */
               ( const char * ) "Blink01",
               /* The stack allocated to the task. */
               configMINIMAL_STACK_SIZE,
               /* The task parameter is not used, so set to NULL. */
               NULL,
               /* The task runs at the idle priority. */
               tskIDLE_PRIORITY + 1,
               &blink01Handle);

   xTaskCreate(StartBlink02,
               ( const char * ) "Blink02",
               configMINIMAL_STACK_SIZE,
               NULL,
               tskIDLE_PRIORITY,
               &blink02Handle );

   /* Start the tasks and timer running. */
   vTaskStartScheduler();

   /* If all is well, the scheduler will now be running, and the following line
   will never be reached.  If the following line does execute, then there was
   insufficient FreeRTOS heap memory available for the idle and/or timer tasks
   to be created.  See the memory management section on the FreeRTOS web site
   for more details. */
   for( ;; );
}

static void StartBlink01( void *arguments )
{
   int led = 0;
   const TickType_t x500MilliSec = pdMS_TO_TICKS( DELAY_500_MilliSec );

   for( ;; )
   {
   led = (~led) & 0b1;
       XGpio_DiscreteWrite(&Gpio, LED_CHANNEL, led);
       xil_printf("task1: %d\n", led);
       /* Delay for 500 millisecond. */
       vTaskDelay( x500MilliSec );

   }
}


static void StartBlink02( void *arguments )
{
   int led = 1;
   const TickType_t x700MilliSec = pdMS_TO_TICKS( DELAY_700_MilliSec );

   for( ;; )
   {
   led = (~led) & 0b1;
       XGpio_DiscreteWrite(&Gpio, LED_CHANNEL, led);
       xil_printf("task2: %d\n", led);
       /* Delay for 700 millisecond. */
       vTaskDelay( x700MilliSec );

   }
}
