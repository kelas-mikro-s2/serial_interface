#include <chrono>
#include <functional>
#include <memory>
#include <string>


#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"
#include <libserialport.h>

//Serial Port
struct sp_port *port;
struct sp_port_config *config;
struct sp_event_set *event_set;
sp_return ret;

uint32_t cnt_rx;

int rx_state;
char rx_buffer[24]={'i','t','s'};
char rx_temp;
int idx_rx;

float data[3];


void init_serial(void)
{   
    
    sp_set_baudrate(port, 115200);
    sp_set_bits(port, 8);
    sp_set_parity(port, SP_PARITY_NONE);
    sp_set_stopbits(port, 1);
    sp_get_port_by_name("/dev/ttyACM0", &port);
    sp_close(port);
    ret = sp_open(port, SP_MODE_READ_WRITE);

    if (ret != SP_OK)
    {
        printf("Error opening serial port\n");
        exit(1);
    }
    
    printf("Serial Open\n");
    



}

//Timer Transmitter
void Transmit_Routine()
{
  // printf("Transmitting...\n");
}

void Receive_Routine()
{
  // check if there is data to read
  // while((sp_nonblocking_read(port, &rx_temp, 1))>0)
  // {

  //     if(rx_temp == 'i' && rx_state == 0)
  //     {
  //       rx_state = 1;
  //     }
  //     else if(rx_temp == 't' && rx_state == 1)
  //     {
  //       rx_state = 2;
  //     }
  //     else if(rx_temp == 's' && rx_state == 2)
  //     {
  //         rx_state = 3;
  //         idx_rx = 3;
  //     }
  //     else if(rx_state == 3)
  //     {
  //         rx_buffer[idx_rx] = rx_temp;
  //         if(++idx_rx>23)
  //         {
  //           rx_state = 0;
  //           memcpy(data,rx_buffer+3,12);            
  //           printf("Data:%.3f,%.3f,%.3f\n",data[0],data[1],data[2]);
  //         }
  //     }
  //     else
  //     {
  //       rx_state = 0;
  //     }
      
       
  // }


  char read_buffer[24];
  sp_blocking_read(port, read_buffer, 24, 0);
  printf("%s\n", read_buffer);
  
}

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);

  auto node = rclcpp::Node::make_shared("comserial");
  // auto timer = node->create_wall_timer(std::chrono::milliseconds(10), Transmit_Routine);
  // auto timer2 = node->create_wall_timer(std::chrono::milliseconds(1), Receive_Routine);

  init_serial();

  while (rclcpp::ok())
  {
      Receive_Routine();
  }
  

  // rclcpp::spin(node);
  rclcpp::shutdown();

  return 0;
}