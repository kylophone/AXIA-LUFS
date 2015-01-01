#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <ebur128.h>


int config_livewire_socket(const char *multicastAddr)
{
  int sock = socket(AF_INET, SOCK_DGRAM, 0);
  int reuse = 1;
  if (setsockopt(sock, SOL_SOCKET, SO_REUSEPORT, &reuse, sizeof(reuse)) == -1) {
      fprintf(stderr, "setsockopt: %d\n", errno);
      return -1;
  }
  
  struct sockaddr_in addr; 
  memset(&addr, 0, sizeof(addr));
  addr.sin_family = AF_INET;
  addr.sin_port = htons(5004);
  addr.sin_addr.s_addr = inet_addr(multicastAddr);

  struct ip_mreq mreq;
  mreq.imr_multiaddr.s_addr = inet_addr(multicastAddr);         
  mreq.imr_interface.s_addr = htonl(INADDR_ANY);

  if(setsockopt(sock, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char *)&mreq, sizeof(mreq)) == -1) {
      fprintf(stderr, "setsockopt: %d\n", errno);
      return -1;
  }

  if (bind(sock, (struct sockaddr*) &addr, sizeof(addr)) == -1) {
      fprintf(stderr, "bind: %d\n", errno);
      return -1;
  }

  return sock;
}


int main(int argc, const char *argv[])
{
  
  int packetLength;
  uint8_t packet[1452]; // A Livewire packet will never be larger than 1452 bytes. 
  //uint16_t sequenceNumber;
  //uint32_t timestamp;
  double audioPayload[480]; // 480 audio samples per packet (max.) 
  ebur128_state* state = NULL;
  double shortTermLoudness;
  uint16_t frameCounter = 0;
  const char *multicastAddr = argv[1];
 
  int sock = config_livewire_socket(multicastAddr);

  state = ebur128_init((unsigned) 2, (unsigned) 48000, EBUR128_MODE_S);

  /* main loop */
  while(1){
    packetLength = recv(sock, packet, sizeof(packet), 0);
    
    //sequenceNumber = (packet[2] << 8 | packet[3]);
    //timestamp = (packet[4] << 24 | packet[5] << 16 | packet[6] << 8 | packet[7]);

    /* Sample: 3 bytes. Header: 12 bytes. Payload: Variable. */
    for (int i = 12; i < packetLength; i += 3) {
      int32_t audioPCM = ((packet[i] << 16) | (packet[i + 1] << 8) | (packet[i + 2]));
      if (audioPCM & 0x800000) audioPCM |= ~0xffffff; // Convert to signed PCM.
      double audioFloat = (audioPCM * (1.0 / 0x7fffff)); // Convert to float.
      audioPayload[((i - 12) / 3)] = audioFloat;
    }

    ebur128_add_frames_double(state, audioPayload, (size_t) ((packetLength - 12) / 6));
    frameCounter += ((packetLength - 12) / 6);
    if (frameCounter >= 4799) {
      frameCounter = 0;
      ebur128_loudness_shortterm(state, &shortTermLoudness);
      //ebur128_loudness_momentary(state, &momentaryLoudness);
      fprintf(stderr, "%-2.1f\n", shortTermLoudness);
    } 
  }
  return 0;
}
