/**
 * File              : dfplayer_lib.c
 * Author            : Souleymane Dembele <sdembele@uw.edu>
 * Date              : 12.09.2023
 * Last Modified Date: 12.09.2023
 * Last Modified By  : Souleymane Dembele <sdembele@uw.edu>
 */

#include "dfplayer_lib.h"
#include "uart.h"
#include <stdint.h>
#include <stdio.h>

#define CMD_REPEAT_PLAY 0x11
#define START_REPEAT_PLAY 0x01

#define NORMAL_VOLUME 20
#define CMD_FRAME_LEN 10
void Delay(void)
{
  unsigned long volatile time;
  time = 727240 * 200 / 91; // 0.1sec
  while (time)
  {
    time--;
  }
}
void send_byte(const uint8_t *data, int length)
{
  for (int i = 0; i < length; i++)
  {
    UART_OutChar(data[i]);
  }
}
void dfplayer_send_cmd(uint8_t cmd, uint8_t param1, uint8_t param2)
{
  uint16_t checksum;
  UART_Init();
  // Delay();
  checksum = VERSION_INFO + CMD_LEN + cmd + FEEDBACK_EN + param1 + param2;

  // calculate the 2 complement
  checksum = ~checksum + 1;
  uint8_t cmd_frame[CMD_FRAME_LEN] = {
      START_BYTE, VERSION_INFO, CMD_LEN, cmd, FEEDBACK_EN,
      param1, param2, (checksum >> 8), checksum, END_BYTE};
  send_byte(cmd_frame, sizeof(cmd_frame));
}

void dfplayer_init(void)
{
  dfplayer_send_cmd(CMD_RESET, NULL_PARAM, NULL_PARAM);
  for (int j = 0; j < 4; j++)
  {
    Delay();
  }
  dfplayer_send_cmd(CMD_SET_VOL, NULL_PARAM, NORMAL_VOLUME);
  for (int i = 0; i < 10; i++)
  {
    Delay();
  }
}

void dfplayer_play_first_track(void)
{
  dfplayer_send_cmd(CMD_GET_TRACK, NULL_PARAM, START_REPEAT_PLAY);
  // for (int i = 0; i < 10; i++) {
  //   Delay();
  // }
}
