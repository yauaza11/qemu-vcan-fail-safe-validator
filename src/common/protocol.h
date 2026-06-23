#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <stdint.h>

//can통신은 ID 값이 작을 수록 버스 점유 우선순위가 높다.
// 그러므로 비상 알림의 값을 더 올린 것c

#define CAN_ID_FAILSAFE 0x080   // 비상상태 및 림홈(Limp Home) 모드 알림 신호
#define CAN_ID_APS_DATA 0x100   // 이중화 가속 페달 센서 데이터 신호

typedef struct{
    uint8_t seq_num;
    uint8_t main_aps;
    uint8_t sub_aps;
    uint8_t checksum;
} __attribute__((packed)) APSRedundantPacket_t;
// APS : Accelerator Pedal Sensor
#endif