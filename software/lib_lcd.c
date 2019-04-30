/*
 * This code is adapted from source found here: http://xdec.de/msp430-oled-display-ssd1306-128x64/
 */

// START "PROVIDED" CODE
#include <msp430g2553.h>
#include "ascii_char.h"
#include "lib_lcd.h"
#include "delay.h"
#include "lib_math.h"
							
#define max_f_size		8		// max. font cache size for resize function
#define meta_w_size		64		// metaballs


#define MIN(x,y)  (x < y ? x : y)
unsigned char byte,bit_num;		// global buffer and bit index
unsigned int h_index,w_index;	// global width_index and height_index


void clean_area(unsigned char x_start, unsigned char x_end, unsigned char y_start, unsigned char y_end)
{
	byte=x_start;
	while(y_start<=y_end)
	{
		set_cursor(x_start,y_start++);
		while(x_start++<=x_end)
			set_instruction(1,0);
		x_start=byte;
	}
} 


// horizontal resize with cache (every bit in one byte will be resized to f_size, for example: 
// f_size=2, every pixel will have a new size of 2*2 pixel, f_size=3, pixel-size: 3*3 pixel etc.
// f_size=1, double height font, only horizontal duplication of pixels (bits), pixel-size: 1*2 pixel
// the vertical duplication is running over a cache (every horizontal row (resized byte to f_size), 
// will be write "f_size"-times to the display)
void convert_font_size(unsigned char x, unsigned char y, unsigned char character, unsigned char f_size)
{
	unsigned char x_char=0,bit_num_b=0,size=0,px_size=0,y_pos_new=0,x_pos_new=0;
	unsigned char cache[max_f_size],i=0;
	byte=0;
	bit_num=0;
	
	if (f_size==1) size=2;
	else size=f_size;
	
	while(x_char<f_width)													// test byte, starting at 0 to f_width (font width)
	{
		while(bit_num<8)													// test bit 0..7 of current byte
		{
			if(ascii_table[character][x_char]&(1<<bit_num))					// if bit=1 in byte...
			{
				while(px_size<size)											// duplicate bits (f_size*f_size)
				{
					if(bit_num_b>7&&px_size>0)								// byte overflow, new byte
					{
						set_cursor(x+x_pos_new,y+y_pos_new++);				// set cursor (increment y-new position)
						set_instruction(1,byte);							// send byte
						bit_num_b=0;										// reset bit counter (buffer)
						cache[i++]=byte;									// save byte in cache
						byte=0;												// reset byte
					}
					byte|=(1<<bit_num_b);									// set bit in byte
					px_size++;												// increment pixel duplicate counter
					bit_num_b++;											// increment bit position for buffer
				}
				px_size=0;													// reset pixel duplicate counter
			}
			else bit_num_b+=size;											// bit=0, calculate new bit position in byte
																			// if bit=0, remaining bits are 0, too
			
			if(bit_num_b>7)													// byte overflow, new byte
			{
				set_cursor(x+x_pos_new,y+y_pos_new++);
				set_instruction(1,byte);
				bit_num_b-=8;													
				cache[i++]=byte;
				byte=0;
			}
			bit_num++;														// test next byte in array
		}
		y_pos_new=0;														// reset y-offset
		x_pos_new++;														// increment x-position						
		i=0;																// reset cache counter
		if(f_size==1) size=0;												// double height font (only for f_size=1)
		else size--;														// first row is ready, only size-1
		while(size--)														
		{
			while(i<f_size) 
			{	
				set_cursor(x+x_pos_new,y+y_pos_new++);
				set_instruction(1,cache[i++]);								// horizontal cache write
			}
			i=0;
			y_pos_new=0;
			x_pos_new++;
		}	
		x_char++;					 // increment byte position
		if(f_size==1) size=2;		 // size correction
		else size=f_size;		
		i=0;						// reset cache counter
		bit_num=0;					
	}
}


void write_string(unsigned char x, unsigned char y, const char *string, unsigned char f_size)
{
	x*=(f_width+space_char);
	set_cursor(x,y);
	while(*string!=0)
	{
		if(f_size)	// font converter enabled
		{
			convert_font_size(x,y,*string++,f_size);
			x+=(f_size*f_width+space_char);		// new x-position
		}
		else
		{
			send_data_array(ascii_table[*string++],f_width);
			y=space_char;	// space between chars
			while(y--)
				set_instruction(1,0);
		}
	}
}

void write_char(unsigned char x, unsigned char y, unsigned char character, unsigned char f_size)
{
	x*=(f_width+space_char);
	set_cursor(x,y);
	if(f_size)
		convert_font_size(x,y,character,f_size);
	else
		send_data_array(ascii_table[character],f_width);
}

void send_data_array(const char *d_array, unsigned char size)
{	
	while(size--)
		set_instruction(1,*d_array++);
}

void set_cursor(unsigned char x, unsigned char y)
{
	set_instruction(0,0x0F&x);				// set lower nibble of the column start address
	set_instruction(0,0x10+(x>>4));			// set higher nibble of the column start address	
	set_instruction(0,0xB0+y);
}

void fill_display(unsigned char width, unsigned char height, unsigned char byte)
{	
	height/=8;							
	while(height--)
	{	
		set_cursor(0,height);
		while(width--)
			set_instruction(1,byte);
		width=lcd_width;
	}
}

// END "PROVIDED" CODE


// START OUR CODE
void draw_rect(unsigned int x, unsigned int y, unsigned int width, unsigned int height)
{
    int address, i;
    unsigned char top_byte = 0xFF, bot_byte = 0xFF;
    // Shift top_byte left by offset
    // Shift bot_byte right by offset w/o sign extension
    int top_address = y >> 3; // Y / 8
    top_address = MIN(top_address, 7);
    int offset_top = y - (top_address << 3);

    int bot_address = (y + height) >> 3;
    bot_address = MIN(bot_address, 7);
    int offset_bot = 8 - ((y + height) - (bot_address << 3));

    top_byte <<= offset_top;
    bot_byte >>= offset_bot;

    if (bot_address == top_address) {
        bot_byte = bot_byte & top_byte;
        /* Write bot. */
        set_cursor(x, bot_address);
        for(i = width; i > 0; i--)
            set_instruction(1, bot_byte);
        return;
    }

    /* Write top. */
    set_cursor(x, top_address);
    for(i = width; i > 0; i--)
        set_instruction(1, top_byte);

    /* Write mid. */
    for(address = top_address + 1; address <= bot_address - 1; address++) {
        set_cursor(x, address);
        for(i = width; i > 0; i--)
                set_instruction(1, 0xFF);
    }

    /* Write bot. */
    set_cursor(x, bot_address);
    for(i = width; i > 0; i--)
        set_instruction(1, bot_byte);
}


void write_small_string(unsigned char x, unsigned char y, const char *string, unsigned char f_size)
{
    while(*string!=0)
    {
        write_char(x,y,*string++,f_size);
        if(f_size==1) x+=f_size+1;
        else if(f_size>1) x+=f_size;
        else x++;
    }
}

// END OUR CODE


// START "PROVIDED" CODE
// for 4-wire SPI
void set_instruction(unsigned char register_sel, unsigned char number)
{
	if(register_sel)
		P2OUT|=DC;				// data
	else P2OUT&=~DC;			// command

	P2OUT&=~CS;					// start condition
	while(IFG2&UCB0TXIFG);		// TX buffer ready?
  	UCB0TXBUF=number;			// start transmission
}

void init_USCI(void)
{
	P1SEL|=SDIN+SCLK;
	P1SEL2|=SDIN+SCLK;

  	UCB0CTL1|=UCSWRST;					// USCI in reset state
  	// SPI Master, 8bit, MSB first, synchronous mode
  	UCB0CTL0|=UCMST+UCSYNC+/*UCCKPH*/+UCCKPL+UCMSB;
  	UCB0CTL1|=UCSSEL_2;					// USCI CLK-SRC=SMCLK=~16MHz
  	UCB0CTL1&=~UCSWRST;					// USCI released for operation
  	IE2|=UCB0TXIE;						// enable TX interrupt
  	IFG2&=~UCB0TXIFG;
  	_EINT();							// enable interrupts
}

#pragma INTERRUPT (USCI)
#pragma vector=USCIAB0TX_VECTOR
void USCI(void)
{
	P2OUT|=CS;				// transmission done
	IFG2&=~UCB0TXIFG;		// clear TXIFG
}
// END PROVIDED CODE
