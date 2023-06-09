# include <sys/socket.h>
# include <unistd.h>
# include <sys/ioctl.h>  
# include <linux/can.h>  
# include <linux/can/raw.h> 
# include <errno.h> 
# include <net/if.h> 
# include <iostream> 
# include <string.h>  
# include <stdio.h>

using namespace std;

# define CAN_8_BYTES_LEN 8


enum COMMAND_LIST{
    _NON_PRINT = 0,
    _PRINT
};

class SOCKET_CAN_PROTO{

    private:
        /* for fillter optio / now not use */
        //struct can_filter rfilter[2] = {{.can_id = 0x141, .can_mask = CAN_SFF_MASK},{.can_id = 0x141, .can_mask = CAN_SFF_MASK}};
        struct can_filter rfilter = {0x141, .can_mask = CAN_SFF_MASK };
        
        /* read data buffer */
        struct can_frame rx_frame;
        /* data frame*/
        u_int8_t command_data[CAN_8_BYTES_LEN]={0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00};;
 
    public:
        /* Can Initialize fuction */
        u_int32_t CAN_INIT_(const  char *);
        /* wite 8 bytes data with can port  */       
        void CAN_WRITE_8BYTES(const u_int32_t, const u_int32_t, const u_int8_t *, u_int8_t);
        
        /* Write read command and read reply Data */
        __u8* CAN_READ_RETURN_8BYTES(const u_int32_t, const u_int32_t, u_int8_t, u_int8_t);
        /* Only Read Function */
        __u8* CAN_READ_8BYTES(const u_int32_t, const u_int32_t, u_int8_t);
        /* Only Recv */
        int8_t CAN_RECV_8BYTES(const u_int32_t, const u_int32_t, struct can_frame*);

        void SOCKET_LIB_TEST();
       

    }; //END OF CLASS   