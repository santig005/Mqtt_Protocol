#include "basics.h"
#include "packer.h"

int process_connect(uint8_t * buff){
    struct connect * connect_messg=(struct connect *)malloc(sizeof(struct connect));
    connect_messg->header.remaining_length=remaining_length(&buff);
    connect_messg->variable_header.protocol_name.length=next_16b(&buff);
    //char * name[]=(char *)malloc(connect_messg->variable_header.protocol_name.length);
    // array for the name
    for(int i=0;i<connect_messg->variable_header.protocol_name.length;i++){
        connect_messg->variable_header.protocol_name.name[i]=next_byte(&buff);
    }

    if(strcmp(connect_messg->variable_header.protocol_name.name,"MQTT")!=0){
        return 0x01;
    }
    connect_messg->variable_header.protocol_level=next_byte(&buff);
    connect_messg->variable_header.connect_flags.byte=next_byte(&buff);
    connect_messg->variable_header.keep_alive=next_16b(&buff);
    // payload
    uint16_t client_id_length=next_16b(&buff);
    if(client_id_length>0){
        connect_messg->payload.client_id=(uint8_t *)malloc(client_id_length+1);
        memcpy(connect_messg->payload.client_id,buff,client_id_length);
        connect_messg->payload.client_id[client_id_length]='\0';
        buff+=client_id_length;
    }
    if(connect_messg->variable_header.connect_flags.bits.will_flag){
        read_string16(&buff,&connect_messg->payload.will_topic);
        read_string16(&buff,&connect_messg->payload.will_message);
    }
    

    
    if(connect_messg->variable_header.connect_flags.bits.username){
        read_string16(&buff,&connect_messg->payload.username);
    }

    if(connect_messg->variable_header.connect_flags.bits.password){
        read_string16(&buff,&connect_messg->payload.password);
    }
    return 0x00;
}
void process_packet(int connfd,uint8_t * buff){
    uint8_t first_byte=next_byte(&buff);
    uint8_t packet_type=(first_byte && 0xF0)>>4;
    switch(packet_type){
        case CONNECT:
            uint8_t response=process_connect(buff);
            if(response==0x00){
                uint8_t connack[4]={0x20,0x02,0x00,0x00};
                write(connfd,connack,sizeof(connack));
            }else{
                uint8_t connack[4]={0x20,0x02,0x00,response};
                write(connfd,connack,sizeof(connack));
            }

    }

}
