#include <msp430g2553.h>
#include "lib_lcd.h"
#include "delay.h"

const char init_cmd_array[]={0xAE,			// DISPLAY OFF
							 0xD5,			// SET OSC FREQUENY
							 0x80,			// divide ratio = 1 (bit 3-0), OSC (bit 7-4)
							 0xA8,			// SET MUX RATIO
							 0x3F,			// 64MUX
							 0xD3,			// SET DISPLAY OFFSET
							 0x00,			// offset = 0
							 0x40,			// set display start line, start line = 0
							 0x8D,			// ENABLE CHARGE PUMP REGULATOR
							 0x14,			// 
							 0x20,			// SET MEMORY ADDRESSING MODE
							 0x02,			// horizontal addressing mode
							 0xA1,			// set segment re-map, column address 127 is mapped to SEG0
							 0xC8,			// set COM/Output scan direction, remapped mode (COM[N-1] to COM0)
							 0xDA,			// SET COM PINS HARDWARE CONFIGURATION
							 0x12,			// alternative COM pin configuration 
							 0x81,			// SET CONTRAST CONTROL
							 0xCF,			// 
							 0xD9,			// SET PRE CHARGE PERIOD
							 0xF1,			// 
							 0xDB,			// SET V_COMH DESELECT LEVEL
							 0x40,			// 
							 0xA4,			// DISABLE ENTIRE DISPLAY ON
							 0xA6,			// NORMAL MODE (A7 for inverse display)
							 0xAF};			// DISPLAY ON
							 
void init_LCD(void)
{
	unsigned char byte=sizeof(init_cmd_array);
	wait_ms(100);
	P2OUT|=RES;
	
	while(byte)
	{
		// really dirty stuff
		set_instruction(0,init_cmd_array[sizeof(init_cmd_array)-byte]);		
		byte--;
	}
	// display RAM is undefined after reset, clean dat shit
	fill_display(lcd_width,lcd_height,0x00);	
}

void init_ports(void)
{
	//P1DIR|=0xFF;				// parallel port for OLED-Display
	//P1OUT&=~0xFF;
	P2DIR|=CS+DC+RES;
	P2OUT&=~RES;			// reset, write, read LOW
	P2OUT|=CS;					// chip select HIGH
}
