#include <msp430g2553.h>
#include "ascii_char.h"
#include "lib_lcd.h"
#include "delay.h"
#include "lib_math.h"
							
#define max_f_size		8		// max. font cache size for resize function
#define meta_w_size		64		// metaballs

unsigned char byte,bit_num;		// global buffer and bit index
unsigned int h_index,w_index;	// global width_index and height_index

void bouncy_bmp(signed int x, unsigned int f_height, unsigned int time_ms, 
				unsigned int b_width, unsigned int b_height,const char *bmap)
{
	signed int y=0;
	bit_num=0; // up or down check
	
	y=lcd_height-f_height-b_height;
	while(f_height)
	{
		draw_bitmap(x,y,b_width,b_height,bmap);
		if(y+(signed int)b_height==lcd_height&&!bit_num)
		{
			f_height/=2;
			bit_num=1;
		}
		if(lcd_height-b_height-y==f_height&&bit_num)
			bit_num=0;
		if(!bit_num) y++;
		else y--;
		wait_ms(time_ms);
	}
}

void draw_metaballs(struct meta_b *balls, unsigned char s_size, unsigned int ball_size)
{
	unsigned char y_real=0,index=s_size;
	unsigned int potential=0;
	byte=0;bit_num=0;h_index=lcd_height_b,w_index=0;
	
	while(h_index--)
	{
		set_cursor(0,h_index);
		while(w_index<meta_w_size)
		{
			y_real=h_index*8;
			while(bit_num<8)
			{
				while(index--)
					potential+=get_meta_pot(w_index,y_real,balls[index],ball_size);
					
				if(potential>100&&potential<160)
					byte|=(3<<bit_num); // resize factor = 2 (1px = 2*2px)
										// this will result in a much faster rendering
				bit_num+=2;
				y_real+=2;
				index=s_size;
				potential=0;
			}
			set_instruction(1,byte);
			set_instruction(1,byte);
			byte=0;
			bit_num=0;
			w_index+=2;
		}
		w_index=0;
	}
}

unsigned int get_meta_pot(signed int x, signed int y, struct meta_b ball, unsigned int factor)
{	
	signed int x_n,y_n;
	x_n=ball.x-x;
	y_n=ball.y-y;
	return factor/(((x_n*x_n)+(y_n*y_n))+1); // "+1" prevents infintity
	// for different ball sizes, add "charge" to struct
	//return (factor*charge)/(((ball.x-x)*(ball.x-x)+(ball.y-y)*(ball.y-y))+1);
}

void meta_ball_physics(struct meta_b *balls, unsigned char s_size, unsigned char pos_change, unsigned int mid_off)
{
	while(s_size--)
	{
		if(balls[s_size].direction==1) 	// move to upper left
        {
			balls[s_size].y-=pos_change;
			balls[s_size].x-=pos_change;
       	}
		if(balls[s_size].direction==2) 	// move to upper right
        {
			balls[s_size].y-=pos_change;
			balls[s_size].x+=pos_change;
       	}
		if(balls[s_size].direction==3) 	// move to bottom right
        {
			balls[s_size].y+=pos_change;
			balls[s_size].x-=pos_change;
       	}
		if(balls[s_size].direction==4) 	// move to bottom left
        {
			balls[s_size].y+=pos_change;
			balls[s_size].x+=pos_change;
       	}
        // for every direction, 2 possible over-/underflows
        if(balls[s_size].direction==2&&balls[s_size].x>=meta_w_size-mid_off)
        	balls[s_size].direction=1;
        if(balls[s_size].direction==4&&balls[s_size].x>=meta_w_size-mid_off)
        	balls[s_size].direction=3;
        if(balls[s_size].direction==3&&balls[s_size].y>=lcd_height-mid_off)
        	balls[s_size].direction=1;
        if(balls[s_size].direction==4&&balls[s_size].y>=lcd_height-mid_off)
        	balls[s_size].direction=2;

        if(balls[s_size].direction==1&&balls[s_size].x<=0+mid_off)
        	balls[s_size].direction=2;
        if(balls[s_size].direction==3&&balls[s_size].x<=0+mid_off)
        	balls[s_size].direction=4;
        if(balls[s_size].direction==2&&balls[s_size].y<=0+mid_off)
        	balls[s_size].direction=4;
        if(balls[s_size].direction==1&&balls[s_size].y<=0+mid_off)
        	balls[s_size].direction=3;	
	}
}

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

void draw_string_sin_fixed(signed int x, signed int y, const char *string, unsigned int x_s, unsigned char div)
{
	while(*string!=0&&x<lcd_width)
	{
		if(x>=-(signed int)f_width-1)
			draw_bitmap(x,y+get_sin(x_s)/div,f_width,8,ascii_table[*string]);
		*string++;
		x+=f_width+space_char;
		x_s+=f_width+space_char;
	}
}

void draw_string_sin(signed int x, signed int y, const char *string, unsigned char div)
{
	while(*string!=0&&x<lcd_width)
	{	
		if(x>=-(signed int)f_width-1)	// part of char inside display
			draw_bitmap(x,y+get_sin(x)/div,f_width,8,ascii_table[*string]);
		*string++;
		x+=f_width+space_char;
	}
}

void draw_string(signed int x, signed int y, const char *string)
{
	//w_index=0;
	//if(!draw_string_check(&x,&w_index,str_size)) return;
	//string+=w_index;	// w_index is string offset
	while(*string!=0&&x<lcd_width)
	{
		if(x>=-(signed int)f_width-1)	// part of char inside display
		{
			draw_bitmap(x,y,f_width,8,ascii_table[*string]);		
			if(x+f_width<lcd_width)		// clear byte after, only if inside display!
				set_instruction(1,0);	// only need to prevent the clean_area() function
		}
		*string++;
		x+=f_width+space_char;
	}
}

/*
unsigned char draw_string_check(signed int *x, unsigned int *offset, unsigned int str_size)
{
	if(*x>=lcd_width) return 0; // overflow
	if(*x+(signed int)str_size*(f_width+space_char)<0) return 0; // underflow
	if(*x<0)
	{
		*x*=-1;
		*offset=(*x/(f_width+space_char));	// string offset
		*x%=(f_width+space_char);			// new x position								
		*x*=-1;
		// x=-26 -> string offset: 26/(5+1)=4 (5=font width, 1=space_char) -> string=string+4 -> 26%(5+1)=2 -> 
		// start at x=-2 (this will show the last 3 bytes of the char (horizontal bytes) on the display)
	}
	return 1;
}
*/

void string_typer(unsigned char x, unsigned char y, const char *string, unsigned char f_size, unsigned int ms)
{
	while(*string!=0)
	{
		write_char(x,y,*string++,f_size);
		if(f_size>1) x+=f_size;
		else x++;
		wait_ms(ms);
	}
}

void write_h_string(unsigned char x, unsigned char y, const char *string, unsigned char f_size)
{
	while(*string!=0)
	{
		write_char(x,y,*string++,f_size);
		if(f_size==1) y+=f_size+1;
		else if(f_size>1) y+=f_size;
		else y++;
	}
}

void draw_bitmap(signed int x, signed int y, unsigned int b_width, unsigned int b_height, const char *bmap)
{
	unsigned int width_max=0,x_off=0,y_off=0;
	byte=0;h_index=0;w_index=0;
	
	if(y+(signed int)b_height<0) return;	// outside display
	if(x+(signed int)b_width<0) return;
	if(y>=lcd_height) return;
	if(x>=lcd_width) return;

	if(x+(signed int)b_width>=lcd_width) 	// width overflow check
	{
		if(x<0) width_max=(x*-1)+lcd_width; // w_index will be set to x*-1
		else width_max=lcd_width-x;	
	}
	else width_max=b_width;

	if(x<0)
	{
		w_index=x*-1;		// set bmap x-offset
		x_off=w_index;
		x=0;				// start at display position x=0 with x-offset
	}

	if(b_height%8) b_height+=8;
	b_height/=8;
	
	if(y<0)
	{
		y*=-1;
		y_off=y/8;
		b_height-=y_off;
		while(h_index<b_height)
		{
			set_cursor(x,h_index);
			while(w_index<width_max)
			{
				byte=bmap[w_index+((h_index+y_off)*b_width)]>>(y%8);
				if(h_index+1!=b_height)
					byte|=bmap[w_index+((h_index+y_off+1)*b_width)]<<(8-(y%8));
				set_instruction(1,byte);
				w_index++;
				byte=0;
			}
			h_index++;
			w_index=x_off;
			if(h_index>=lcd_height_b) return;
		}
	}
	else
	{
		y_off=y/8;
		while(h_index<=b_height)
		{
			set_cursor(x,y_off+h_index);
			while(w_index<width_max)
			{
				if(h_index!=b_height)
					byte=bmap[w_index+(h_index*b_width)]<<(y%8);					
				if(h_index) 
					byte|=(bmap[w_index+((h_index-1)*b_width)]>>(8-(y%8)));	// byte before
				set_instruction(1,byte);
				w_index++;
				byte=0;
			}
			h_index++;
			w_index=x_off;		
			if(h_index+y_off>=lcd_height_b) return;
			if(h_index==b_height&&!(y%8)) return;
		}
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

void draw_rect(unsigned char x, unsigned char y, unsigned char width, unsigned char height)
{
    int address, i;
    unsigned char top_byte = 0xFF, bot_byte = 0xFF;
    // Shift top_byte left by offset
    // Shift bot_byte right by offset w/o sign extension
    int top_address = y >> 3; // Y / 8
    int offset_top = y - (top_address << 3);

    int bot_address = (y + height) >> 3;
    int offset_bot = 8 - (y + height) - (bot_address << 3);
//    int offset_bot = (1 + bot_address) << 3 - (y + height);

    top_byte <<= offset_top;
    bot_byte >>= offset_bot;


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

/*
// for parallel port (8080)
void set_instruction(unsigned char register_sel, unsigned char number)
{
	if(register_sel)
		P2OUT|=DC;
	else P2OUT&=~DC;

	P2OUT&=~CS;					// start condition
	P1OUT=number;
	P2OUT|=WR;
	P2OUT&=~WR;
	P2OUT|=CS;
}
*/

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
