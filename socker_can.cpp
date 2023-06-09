#include "socket_can.hpp"
#include <stdlib.h>



    
u_int32_t SOCKET_CAN_PROTO::CAN_INIT_(const char *ifname)
{
    //socket Function 
    u_int32_t SOCK_CAN = socket(PF_CAN, SOCK_RAW, CAN_RAW);
        
    if (SOCK_CAN < 0){
        cout << "Somethiong Error in Create Socket[" << ifname <<"]" << endl;
        return -1;  
        }
    cout << "Success to creat Socket[" << ifname << "]" << endl;

    // ioctl Function
    struct ifreq ifr;
    strcpy(ifr.ifr_name,ifname);

    if(ioctl(SOCK_CAN,SIOCGIFINDEX, &ifr) < 0){
        perror("Fail to get can interface index -");
        return -1;
    } 
    cout << "Success to get can index [" << ifr.ifr_ifindex << "]" << endl;

    // bind Function
    struct sockaddr_can canaddr;
    canaddr.can_family = AF_CAN;
    canaddr.can_ifindex = ifr.ifr_ifindex;
    if(bind(SOCK_CAN, (struct sockaddr*)&canaddr, sizeof(canaddr))){
        perror("Fail to bind can socket -");
        return -1;
    }
    cout << "Success to bind can socket" << endl; 

    return SOCK_CAN;       

}//end CAN_INIT


/* Write 8 bytes data */
void SOCKET_CAN_PROTO::CAN_WRITE_8BYTES(const u_int32_t sock, const u_int32_t id, const u_int8_t *data, u_int8_t opt)
{
    struct can_frame tx_frame; 
    tx_frame.can_id = id | 0x10000000;
    memcpy(tx_frame.data, data, CAN_8_BYTES_LEN);
    tx_frame.can_dlc = CAN_8_BYTES_LEN; 
    
    int tx_bytes = write(sock, &tx_frame, sizeof(tx_frame));
    if(tx_bytes == -1){
        perror("Fail to transmit can tx_frame -");

    }
    else if(tx_bytes != CAN_8_BYTES_LEN+8){
        cout << "Fail to transmit can tx_frame   " << tx_bytes << " is transmited     " << endl;
    }
    else{
        if(opt == _PRINT){
            cout << "Success to transmit can tx_frame  " << tx_bytes << " is transmited" << endl;
        }
    }


}//end of write FNC


/* Read 8 bytes data and return data with array[8](address)  */
__u8* SOCKET_CAN_PROTO::CAN_READ_RETURN_8BYTES(const u_int32_t sock, const u_int32_t id, u_int8_t cmd_id, u_int8_t opt)
{       
    int rx_bytes = 0;
    command_data[0] = cmd_id;
    CAN_WRITE_8BYTES(sock, id, command_data, opt); 

        //set ID filter
    rfilter.can_id = id;
    setsockopt(sock, SOL_CAN_RAW, CAN_RAW_FILTER, &rfilter, sizeof(rfilter));
    
    // self data filter
    do{
        rx_bytes = read(sock, &rx_frame, sizeof(rx_frame) );
        }while((rx_frame.can_id&0x10000000) || (rx_frame.data[0]!=cmd_id));

    if(opt == _PRINT){
        cout << "Success to recived! " << dec << rx_bytes << "bytes can rx_frame and " << sizeof(rx_frame.data) << "bytes data are recived" << endl;
        cout<<"ID = "<< hex<<rx_frame.can_id<<endl;

        for(int i =0 ; i<sizeof(rx_frame.data); i++){
            cout << hex << (0xff&rx_frame.data[i]) << ", ";        
        }
        cout << endl << "--------------------------------------------"<<endl;
    }
    return rx_frame.data;
}// end of read 


/* Read only 8 bytes data and return data with array(address) & non print */
__u8* SOCKET_CAN_PROTO::CAN_READ_8BYTES(const u_int32_t sock, const u_int32_t id,u_int8_t cmd_id)
{       
    int rx_bytes = 0;
        //set Filter
    rfilter.can_id = id;
    setsockopt(sock, SOL_CAN_RAW, CAN_RAW_FILTER, &rfilter, sizeof(rfilter));
    
    do{
        read(sock, &rx_frame, sizeof(rx_frame) );
        }while((rx_frame.can_id&0x10000000) || (rx_frame.data[0]!=cmd_id));

    return rx_frame.data;
}// end of read non print 




int8_t SOCKET_CAN_PROTO::CAN_RECV_8BYTES(const u_int32_t sock, const u_int32_t id, struct can_frame* r_frame){

    struct can_filter r_filter[1];
    r_filter[0].can_id = (canid_t)id;
    r_filter[0].can_mask = CAN_EFF_MASK;
    setsockopt(sock, SOL_CAN_RAW, CAN_RAW_FILTER, &r_filter, sizeof(rfilter));


    struct timeval timeout;
    timeout.tv_sec = 0;
    timeout.tv_usec = 500;
    setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (const char*) &timeout, sizeof(timeout));

    ssize_t recv_byte = recv((int)sock, r_frame, sizeof(struct can_frame),0);
    if(recv_byte < 0){
        std::cout<< "Failed to receive CAN data!"<< "  id: "<< hex << id << endl;
        //std::cout<< "fillter ID: "<< hex << r_filter[0].can_id << endl;
        return -1;

    }
    return recv_byte;

}// end of recv


void SOCKET_CAN_PROTO::SOCKET_LIB_TEST(){
std::cout<<"Socket LIB TEST: Fine"<< std::endl ;

}