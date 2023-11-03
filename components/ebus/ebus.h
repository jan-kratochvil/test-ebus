 #include "esphome/core/component.h"
 #include "esphome/components/uart/uart.h"
 #include <vector>

 namespace esphome {
 namespace ebus {
 
 class Ebus : public uart::UARTDevice, public Component {
  public:
   Ebus() = default;
 
   void setup() override;
 
   void loop() override;
 
   void dump_config() override;
 
   float get_setup_priority() const override;
 
 
  protected:
   bool parse_modbus_byte_(uint8_t byte);
   uint16_t send_wait_time_{250};
   bool disable_crc_;
   std::vector<uint8_t> rx_buffer_;
   uint32_t last_modbus_byte_{0};
   uint32_t last_send_{0};
 };
 
 
 }  // namespace ebus
 }  // namespace esphome