#include <reg51.h>
#include <intrins.h>
#include <stdio.h>

sbit DHT = P2 ^ 0; // setting DHT pin
int rh_h, rh_l, temp_h, temp_l, check;

void uart_init()
{
  SCON = 0x50;
  TMOD = 0x20;
  TH1 = 0xfd;
  TR1 = 1;
  TI = 1;
}

void delay1ms(unsigned int count)
{
  unsigned char j;
  while (count--)
    for (j = 0; j < 122; ++j)
      ;
}

int getData()
{
  unsigned char i, j;
  int value = 0;

  for (i = 0; i < 8; i++)
  {
    while (DHT == 0)
      ;

    for (j = 0; j < 6; j++) // 46.66us
    {
      _nop_();
    }

    value = value << 1;
    if (DHT == 1)
    {
      value |= 0x01;
    }
    while (DHT == 1)
      ;
  }

  return value;
}

void main()
{

  unsigned int temp;

  uart_init();

  delay1ms(3000);

  while (1)
  {

    // request();

    DHT = 0;
    delay1ms(30);

    DHT = 1;

    // response();

    while (DHT == 1)
      ;
    while (DHT == 0)
      ;
    while (DHT == 1)
      ;

    rh_h = getData();

    rh_l = getData();

    temp_h = getData();

    temp_l = getData();

    check = getData();

    if ((rh_h + rh_l + temp_h + temp_l) == check)
    {

      printf("RH = %d.%d%%\n", rh_h, rh_l);

      if ((temp_h >> 7) & 0x01)
      {
        temp = -1;
        temp_h &= 0x7F;
      }
      else
      {
        temp = 1;
      }

      printf("TEMP = %d.%d%cC\n", temp * temp_h, temp_l);
    }
    else
    {
      printf("error");
    }

    delay1ms(3000);
  }

  return 0;
}
