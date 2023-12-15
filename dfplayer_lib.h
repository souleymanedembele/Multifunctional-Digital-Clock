#ifndef __DFPLAYER_LIB_H__
#define __DFPLAYER_LIB_H__

#define START_BYTE 0x7E
#define END_BYTE 0xEF
#define CMD_LEN 0x06
#define FEEDBACK_EN 0x01
#define VERSION_INFO 0xFF

#define CMD_NEXT 0x01
#define CMD_PREV 0x02
#define CMD_INC_VOL 0x04
#define CMD_DEC_VOL 0x05
#define CMD_SET_EQ 0x07
#define CMD_SET_PLAYBACK 0x08
#define CMD_SET_PLAYBACKSRC 0x09
#define CMD_INIT_PARAMS 0x3F

#define CMD_SET_VOL 0x06

#define CMD_GET_TRACK 0x03
#define CMD_RESET 0x0C
#define CMD_PLAY 0x0D
#define CMD_PAUSE 0x0E

#define NULL_PARAM 0x00
#define PLAYBACK_SRC_TF 0x02
typedef enum {
  Normal = 0x00,
  Pop = 0x01,
  Rock = 0x02,
  Jazz = 0x03,
  Classic = 0x04,
  Base = 0x05,
} eq_type;

typedef enum {
  Repeat = 0x00,
  FolderRepeat = 0x01,
  SingleRepeat = 0x02,
  Shuffle = 0x03,
} playbackmode_type;

typedef enum {
  U = 0x00,
  TF = 0x01,
  AUX = 0x02,
  SLEEP = 0x03,
  FLASH_SR = 0x04,
} playbacksrc_type;

void dfplayer_init(void);
void dfplayer_play_first_track(void);
void Delay(void);
#endif
