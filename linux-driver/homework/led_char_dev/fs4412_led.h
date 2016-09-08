#ifndef _FS412_LED_H_
#define _FS412_LED_H_


#define BASEMINOR 0
#define DEVICE_NUMBER 1
#define CHRDEV_MEM_SIZE 8

#define LED_MAJOR 250
#define LED2_MINOR 0

#define LED_ON  _IOW('L',0,int)
#define LED_OFF _IOW('L',1,int)

#define GPX1_BASE  0x11000c20
#define GPX2_BASE  0x11000C40
#define GPF3_BASE  0x114001E0

#define LED_2 0x02
#define LED_3 0x03
#define LED_4 0x04
#define LED_5 0x05

#define CONFOFFSET 0x00
#define DATAOFFSET 0x04

struct led_action
{
    int act;
    int nr;
};
#endif
