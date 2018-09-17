#include "keeloq.h"
#include "main.h"


volatile uint8_t level=255;
volatile unsigned long last, len;
uint8_t p_level;
unsigned long p_len, p_len_prev;

extern volatile uint8_t RF_bufer[66];






void setbit(uint8_t *data, uint8_t n)
{
  data[n/8]|=1<<(n%8);
}

void process_keeloq()
{
  switch(keeloq.state)
  {
    case 0:
      if(p_level) break;
      keeloq.state=1;
      keeloq.pre_count=0;
      break;

    case 1: //pre+hdr
      if(p_len>=KL_MIN_TE && p_len<=KL_MAX_TE) keeloq.pre_count++;
      else if(!p_level && p_len>=KL_MIN_TE*10 && p_len<=KL_MAX_TE*10 && keeloq.pre_count>=KL_MIN_PRE_COUNT)
      {
        keeloq.TE=p_len/10;
        keeloq.state=2;
        keeloq.dat_bit=0;
        keeloq.data[0]=0x00;
        keeloq.data[1]=0x00;
        keeloq.data[2]=0x00;
        keeloq.data[3]=0x00;
        keeloq.data[4]=0x00;
        keeloq.data[5]=0x00;
        keeloq.data[6]=0x00;
        keeloq.data[7]=0x00;
		keeloq.data[8]=0x00;
		  
		for(uint8_t i=0; i<66; i++)
		{
			RF_bufer[i]=0;
		}
      }
        else{
        keeloq.state=0;
        break;
      }
      break;

    case 2: //dat
      if(!p_level) break;

      if(p_len<keeloq.TE/2 || p_len>keeloq.TE*3)
      {
        keeloq.state=0;
        break;
      }

      if(p_len<=keeloq.TE+keeloq.TE/2) 
			{
				setbit(keeloq.data, keeloq.dat_bit);
				RF_bufer[keeloq.dat_bit]=1;
			}
      if(++keeloq.dat_bit==KL_MAX_BITS) keeloq.state=100;
      break;
  }
}

void dump_hex(uint8_t *buf, uint8_t bits)
{
  uint8_t a;
  
  for(a=0; a<(bits+7)/8; a++)
  {
    if(buf[a]<=0x0f) {}
  }
}

void level_check(void)
{
    if(level!=255)
  {
    LL_EXTI_DisableIT_0_31(LL_EXTI_LINE_8);
    p_level=level;
    p_len=len;
    len=0;
    level=255;
	  p_len_prev=p_len;
    LL_EXTI_EnableIT_0_31(LL_EXTI_LINE_8);
    
    process_keeloq();    
  }
  
}
