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
    sp_get_port_by_name("/dev/serial/by-id/usb-Silicon_Labs_CP2102_USB_to_UART_Bridge_Controller_0001-if00-port0", &port);
    sp_close(port);
    ret = sp_open(port, SP_MODE_READ_WRITE);

    if (ret != SP_OK)
    {
        printf("Error opening serial port\n");
        exit(1);
    }
    
    printf("Serial Open\n");
    



}



void Receive_Routine()
{
  
  uint8_t read_buffer;
  sp_return ret;
  
  ret = sp_nonblocking_read(port, &read_buffer, 1);

  if( ret > 0)
    printf("Receiving: %d\n", read_buffer);
  
}


//Timer Transmitter
void Transmit_Routine()
{
  
  uint8_t tx_buffer = 112;
  printf("Transmitting: %d\n", tx_buffer);
  sp_nonblocking_write(port, &tx_buffer, 1);

}

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);

  auto node = rclcpp::Node::make_shared("comserial");
  init_serial();

  auto timer = node->create_wall_timer(std::chrono::milliseconds(1000), Transmit_Routine);
  auto timer2 = node->create_wall_timer(std::chrono::milliseconds(1), Receive_Routine);

  rclcpp::spin(node);

  return 0;
}