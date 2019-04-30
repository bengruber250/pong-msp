#ifndef LIB_LCD_H_
#define LIB_LCD_H_

struct meta_b
{
	signed int x,y;
	unsigned char direction;
};

/*
 * New CS is 2.2
 * New RES RST is P2.1
 * New DC is P2.0
 * Pretend
 * NEW BELOW
 */
#define CS        BIT2      // P2.2
#define RES       BIT1      // P2.1
#define DC        BIT0      // P2.0

// P1 is Data-Port (P1.7=D7...P1.0=D0)
// OLD BELOW
//#define CS		0x0010		// P2.4
//#define RES		0x0008		// P2.3
//#define DC		0x0004		// P2.2
//#define WR		0x0002		// P2.1
//#define RD		0x0001		// P2.0

// for 4-wire SPI
#define SDIN	0x0080		// data in at P1.7
#define SCLK	0x0020		// clk at P1.5

#define lcd_width		128
#define lcd_height		64
#define lcd_height_b	8

#define space_char	1		// space between chars

void meta_ball_physics(struct meta_b*,unsigned char,unsigned char,unsigned int);
void clean_area(unsigned char,unsigned char,unsigned char,unsigned char);
void draw_string(signed int,signed int,const char*);
//unsigned char draw_string_check(signed int*,unsigned int*,unsigned int);
void convert_font_size(unsigned char,unsigned char,unsigned char,unsigned char);
void write_string(unsigned char,unsigned char,const char*,unsigned char);
void write_char(unsigned char,unsigned char,unsigned char,unsigned char);
void send_data_array(const char*,unsigned char);
void set_cursor(unsigned char,unsigned char);
void fill_display(unsigned char,unsigned char,unsigned char);
void set_instruction(unsigned char,unsigned char);
void init_USCI(void);

void draw_rect(unsigned int x, unsigned int y, unsigned int width, unsigned int height);
void write_small_string(unsigned char x, unsigned char y, const char *string, unsigned char f_size);
/*
 *
 */
#endif /*LIB_LCD_H_*/
