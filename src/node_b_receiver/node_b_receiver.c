#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include <net/if.h>
#include <sys/epoll.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/uio.h>

#include <linux/can.h>
#include <linux/can/raw.h>
#include <linux/net_tstamp.h>
#include <linux/if.h>
#include "../common/protocol.h"

int main(){
    int s;
    struct sockaddr_can addr;
    struct ifreq ifr;
    struct can_frame frame;

    if((s=socket(PF_CAN, SOCK_RAW, CAN_RAW))<0){
        perror("socket error");
		return 1;
    }

    strcpy(ifr.ifr_name, "vcan0");
    ifr.ifr_name[IFNAMSIZ -1]='\0'; // 이건 뭐 굳이?

    if(ioctl(s, SIOCGIFINDEX, &ifr) < 0){
        perror("SIOCGIFINDEX");
		exit(1);
    }

    addr.can_family = AF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;

    
    if (bind(s, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("bind");
        return 1;
    }

    printf("[Node B] vcan0로 부터 수신 대기 중 ...\n ");

    while(1){
        int nbytes = read(s, &frame, sizeof(struct can_frame));
        if (nbytes<0){
            perror("Read Error");
            return 1;
        }

        if(frame.can_id == 0x100){
            APSRedundantPacket_t received_packet;

            memcpy(&received_packet, frame.data, sizeof(APSRedundantPacket_t));
            printf("[수신] ID: 0x%X | DLC: %d | SEQ: %d | Main: %d%% | Sub: %d%%\n",
                   frame.can_id, frame.can_dlc, 
                   received_packet.seq_num, 
                   received_packet.main_aps, 
                   received_packet.sub_aps);
        }
    }
    close(s);
    return 0;
}