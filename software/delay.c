void wait_ms(unsigned int m_sec)
{
	while(m_sec--)
		__delay_cycles(16000);	//1000 f�r 1MHz
}

void wait_us(unsigned int u_sec)
{
	while(u_sec--)
		__delay_cycles(16);		//1 f�r 1MHz
}
