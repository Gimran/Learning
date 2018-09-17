#ifndef KEELOQ_H
    #define KEELOQ_H

    #include "stm32f1xx.h"
    #include "stm32f1xx_ll_exti.h"

#define KL_MIN_PRE_COUNT 4
#define KL_MAX_TE 660		//660
#define KL_MIN_TE 260
#define KL_MAX_BITS 66

void process_keeloq(void);
void setbit(uint8_t *data, uint8_t n);
void dump_hex(uint8_t *buf, uint8_t bits);
void level_check(void);









#endif
