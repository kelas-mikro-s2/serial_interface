#include <chrono>
#include <functional>
#include <memory>
#include <string>

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"
#include <libserialport.h>

// Serial Port
struct sp_port *port;
struct sp_port_config *config;
struct sp_event_set *event_set;
sp_return ret;

uint8_t rx_buffer[4];
uint16_t rx_data;
uint8_t status_rx = 0, idx_rx;

void init_serial(void)
{

  sp_set_baudrate(port, 115200);
  sp_set_bits(port, 8);
  sp_set_parity(port, SP_PARITY_NONE);
  sp_set_stopbits(port, 1);
  sp_get_port_by_name("/dev/serial/by-path/platform-vhci_hcd.0-usb-0:1:1.0-port0", &port);
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
  // wait for data

  ret = sp_blocking_read(port, rx_buffer, 4, 500);

  if (ret >= 4)
  {
    if (rx_buffer[0] == 'a' && rx_buffer[1] == 'b')
    {

      rx_data = (rx_buffer[3] << 8) | rx_buffer[2];
      printf("Received: %d\n", rx_data);

    }
  }
}

// Timer Transmitter
void Transmit_Routine()
{

  uint8_t tx_buffer = 112;
  printf("Transmitting: %d\n", tx_buffer);
  sp_nonblocking_write(port, &tx_buffer, 1);
}

int main(int argc, char *argv[])
{
  rclcpp::init(argc, argv);

  auto node = rclcpp::Node::make_shared("comserial");
  init_serial();

  // auto timer = node->create_wall_timer(std::chrono::milliseconds(1000), Transmit_Routine);
  auto timer2 = node->create_wall_timer(std::chrono::milliseconds(1), Receive_Routine);

  rclcpp::spin(node);

  return 0;
}