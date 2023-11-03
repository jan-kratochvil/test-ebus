 #include "esphome/core/component.h"
 #include "esphome/components/uart/uart.h"
 #include <vector>

 namespace esphome {
 namespace ebus {
 
 class Ebus : public uart::UARTDevice, public Component {
  public:
   void setup() override;
   void loop() override;
   void dump_config() override;
   float get_setup_priority() const override;
   void send_raw(const std::vector<uint8_t> &payload);

   uint8_t waiting_for_response{0};

  protected:
   bool parse_byte_(uint8_t byte);
   uint16_t send_wait_time_{250};
   std::vector<uint8_t> rx_buffer_;
   uint32_t last_byte_{0};
   uint32_t last_send_{0};
 };
 
 
 }  // namespace ebus
 }  // namespace esphome