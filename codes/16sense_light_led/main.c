 /* Centre for Electronic Design and Technology, NSIT, New Delhi
 * 
 * Dhananjay V Gadre, dhananjay.gadre@gmail.com
 * Shanjit Singh Jajmann, shanjitsingh@gmail.com
 * Rohit Dureja, rohit.dureja@gmail.com
 * 
 */

/* Defines the base address of the memories and peripherals */
#include "inc/hw_memmap.h"

/* Defines the common types and macros */
#include "inc/hw_types.h"

/* Defines and Macros for GPIO API */
#include "driverlib/gpio.h"

/* Prototypes for the system control driver */
#include "driverlib/sysctl.h"

/* Defines and Macros for the UART. */
#include "driverlib/uart.h"

/* Utility driver to provide simple UART console functions. */
#include "utils/uartstdio.h"

static volatile unsigned long g_ulcomp = 10000;
static volatile unsigned long val;

void Pin_Int(void)
	{

		SysCtlDelay(SysCtlClockGet()/2);
		g_ulcomp = val;
		GPIOPinIntClear(GPIO_PORTE_BASE, GPIO_PIN_0);

	}

void InitConsole(void)
{
    /* Enable GPIO port A which is used for UART0 pins. */
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);

    /* Make the UART pins be peripheral controlled. */
	GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

    /* Initialize the UART for console I/O. */
    UARTStdioInit(0);
}

int main(void)
{
	/* Set the clocking to directly run from the crystal at 8MHz */
	SysCtlClockSet(SYSCTL_SYSDIV_10 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_8MHZ);

	/* Set the clock for the GPIO Port C */
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);

	/* UART configuration */
	InitConsole();

	/* Set the type of the GPIO Pin */
	GPIOPinTypeGPIOOutput(GPIO_PORTD_BASE, GPIO_PIN_1);
	GPIOPinTypeGPIOOutput(GPIO_PORTC_BASE, GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7);
	GPIOPinTypeGPIOInput(GPIO_PORTE_BASE,GPIO_PIN_0);
	GPIOPinTypeGPIOInput(GPIO_PORTE_BASE,GPIO_PIN_1);

	/* GPIO Pins 5, 6, 7 on PORT C initialized to 0
	 * All LEDs on for a very short duration.
	 * */
	GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7, 0);

	/* GPIO Pins 0, 1 on PORT D initialized to 0 */
	GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_1, 0);

	/* GPIO Pins 0, 1 on PORT D initialized to pull up resistors */
	GPIOPadConfigSet(GPIO_PORTE_BASE, GPIO_PIN_0, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);
    GPIOPadConfigSet(GPIO_PORTE_BASE, GPIO_PIN_1, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);

    /*Register Interrupt to call Interrupt Handler*/
	GPIOPortIntRegister(GPIO_PORTE_BASE, Pin_Int);

	/*Clear interrupt register*/
	GPIOPinIntClear(GPIO_PORTE_BASE, GPIO_PIN_0);

	/*Set interrupt triggering sequence*/
	GPIOIntTypeSet(GPIO_PORTE_BASE, GPIO_PIN_0, GPIO_RISING_EDGE);

	/*Enable interrupts on selected pin*/
	GPIOPinIntEnable(GPIO_PORTE_BASE, GPIO_PIN_0);

	g_ulcomp = 0;

	while(1)
    {



		GPIOPinTypeGPIOOutput(GPIO_PORTD_BASE, GPIO_PIN_1);
		GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_1, GPIO_PIN_1);
		SysCtlDelay(SysCtlClockGet() / 12);
		GPIOPinTypeGPIOInput(GPIO_PORTD_BASE,GPIO_PIN_1);
		val = 0 ;
		while(GPIOPinRead(GPIO_PORTD_BASE, GPIO_PIN_1)==GPIO_PIN_1)
		{
			val++;
		}

		UARTprintf("Value = %4u\r", val);
		UARTprintf("g_ulcomp = %4u\r", g_ulcomp);

		if(val > (g_ulcomp)/3)
		{
			GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7, GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7);
			GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_5 , 0);
		}

		else if((val>(g_ulcomp)/2)&& (val<(g_ulcomp)/3))
		{
			GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7, GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7);
			GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_6 ,0 );
		}

		else {
			GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7, GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7);
			GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_7, 0);
		}


		/*
		 for(on_time=val, off_time =0 ;on_time>0 ;on_time=on_time-20,off_time=off_time+20)
		{
        GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_5, GPIO_PIN_5);
        for(count=0;count<on_time;count++)
        {
		}
		GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_5, 0);
		for(count=0;count<off_time;count++)
        {
		}
		}

		for(on_time=val, off_time =0 ;on_time>0 ;on_time=on_time-20,off_time=off_time+20)
		{
        GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_5, 0);
        for(count=0;count<on_time;count++)
        {
		}
		GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_5, GPIO_PIN_5);
		for(count=0;count<off_time;count++)
        {
		}
		}
		*/


    }

}
