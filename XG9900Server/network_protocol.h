#ifndef NETWORK_PROTOCOL_H
#define NETWORK_PROTOCOL_H

#define NETWORK_RPOTOCOL_HEADER_SIZE    (12)

typedef unsigned short (*network_protocol_callback)(unsigned char *buffer, unsigned short length);

extern unsigned short network_protocol_callback_ack(unsigned char *buffer, unsigned short length);
extern unsigned short network_protocol_callback_ping(unsigned char *buffer, unsigned short length);

extern network_protocol_callback network_protocol_search(char *fourcc);

#endif
