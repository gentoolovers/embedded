#ifndef FS4412_LED_HH
#define FS4412_LED_HH

#define LED_ON	_IOW('L', 0, int)
#define LED_OFF	_IOW('L', 1, int)


#define LED1	0x01
#define LED2	0x02
#define LED3	0x03
#define LED4	0x04

struct led_action 
{
	int act;
	int nr;
};

#endif
