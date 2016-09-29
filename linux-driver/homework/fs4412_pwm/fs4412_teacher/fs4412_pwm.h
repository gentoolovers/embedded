#ifndef _FS4412_PWM_H_
#define _FS4412_PWM_H_

#define PWM_ON _IO('K',0)
#define PWM_OFF _IO('K',1)

#define SET_PRE _IOW('K',2,int)
#define SET_CNT _IOW('K',3,int)

#endif
