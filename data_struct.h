#pragma once

#define READ_SIZE 1008
#define DATA_SIZE 1000

typedef struct _body{
    int flag;  //0 보내는중 1 마지막패킷
    unsigned int data_len;
    char data[1000];
}Body;