#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h> //bzero
#include "ebur128.h"

#define EXAMPLE_PORT 5004 // livewire audio is always on this port
#define EXAMPLE_GROUP "239.192.0.1" // livewire channel number 1

int main(void)
{
  struct sockaddr_in addr;
  int addrlen, sock, packetLength;
  struct ip_mreq mreq;
  uint8_t packet[1452]; // A Livewire packet will never be larger than 1452 bytes. 
  uint16_t sequenceNumber;
  uint32_t timestamp;
  double audioPayload[480]; // 480 audio samples per packet (max.) 
  ebur128_state* state = NULL;
  double shortTermLoudness;
  uint16_t frameCounter = 0;
 
  /* set up socket */
  sock = socket(AF_INET, SOCK_DGRAM, 0);
  if (sock < 0) {
    perror("socket");
    exit(1);
  }

  bzero((char *)&addr, sizeof(addr));
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = htonl(INADDR_ANY);
  addr.sin_port = htons(EXAMPLE_PORT);
  addrlen = sizeof(addr);

  if (bind(sock, (struct sockaddr *) &addr, sizeof(addr)) < 0) {        
    perror("bind");
    exit(1);
  }    

  mreq.imr_multiaddr.s_addr = inet_addr(EXAMPLE_GROUP);         
  mreq.imr_interface.s_addr = htonl(INADDR_ANY);   

  if (setsockopt(sock, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(mreq)) < 0) {
    perror("setsockopt mreq");
    exit(1);
  }

  state = malloc((size_t) sizeof(ebur128_state*));
  state = ebur128_init((unsigned) 2, (unsigned) 48000, EBUR128_MODE_S);

  /* main loop */
  while(1){
    packetLength = recvfrom(sock, packet, sizeof(packet), 0, (struct sockaddr *) &addr, (socklen_t *) &addrlen);
    //sequenceNumber = (packet[2] << 8 | packet[3]);
    //timestamp = (packet[4] << 24 | packet[5] << 16 | packet[6] << 8 | packet[7]);

    /* Sample: 3 bytes. Header: 12 bytes. Payload: Variable. */
    for (int i = 12; i < packetLength; i += 3) {
      int32_t audioPCM = ((packet[i] << 16) | (packet[i + 1] << 8) | (packet[i + 2]));
      if (audioPCM & 0x800000) audioPCM |= ~0xffffff; // Convert to signed PCM.
      double audioFloat = (audioPCM * (1.0 / 0x7fffff));
      audioPayload[((i - 12) / 3)] = audioFloat;
    }

    ebur128_add_frames_double(state, audioPayload, (size_t) ((packetLength - 12) / 6));
    frameCounter += ((packetLength - 12) / 6);
    if (frameCounter >= 4800) {
      frameCounter = 0;
      ebur128_loudness_shortterm(state, &shortTermLoudness);
      fprintf(stderr, "%-2.1f\n", shortTermLoudness);
    } 
  }
  return 0;
}
