#ifndef __SOCKET_SERVER_H__
#define __SOCKET_SERVER_H__

typedef struct socket_server_t *socket_server_p;

//socket模式 tcp udp
typedef enum
{
    TCP,
    UDP
} SOCKET_MODE;

//socket角色 server或者client
// typedef enum
// {
//     SERVER,
//     CLIENT
// }SOCKET_ROLE;

socket_server_p socket_server_create(int port,SOCKET_MODE mode);
void            socket_server_start(socket_server_p server);

// void socket_set_message_callback(char *msg);
// int socket_send_message(int socketfd,char *msg);


#endif
