 #include "ebus.h"
 #include "esphome/core/log.h"
 #include "esphome/core/helpers.h"
 
 namespace esphome {
 namespace ebus {
 
 static const char *const TAG = "ebus";
 
 void Ebus::setup() {
    ESP_LOGCONFIG(TAG, "Setting up Ebus...");
 }
 void Ebus::loop() {
   const uint32_t now = millis();
 
   if (now - this->last_modbus_byte_ > 50) {
     this->rx_buffer_.clear();
     this->last_modbus_byte_ = now;
   }
   // stop blocking new send commands after send_wait_time_ ms regardless if a response has been received since then
   if (now - this->last_send_ > send_wait_time_) {
     waiting_for_response = 0;
   }
 
   while (this->available()) {
     uint8_t byte;
     this->read_byte(&byte);
     if (this->parse_modbus_byte_(byte)) {
       this->last_modbus_byte_ = now;
     } else {
       this->rx_buffer_.clear();
     }
   }
 }
 
 bool Modbus::parse_modbus_byte_(uint8_t byte) {
   size_t at = this->rx_buffer_.size();
   this->rx_buffer_.push_back(byte);
   const uint8_t *raw = &this->rx_buffer_[0];
   ESP_LOGV(TAG, "Received Byte  %d (0X%x)", byte, byte);

   // return false to reset buffer
   return false;
 }
 
 void Modbus::dump_config() {
   ESP_LOGCONFIG(TAG, "Ebus:");
   ESP_LOGCONFIG(TAG, "  Send Wait Time: %d ms", this->send_wait_time_);
   ESP_LOGCONFIG(TAG, "  CRC Disabled: %s", YESNO(this->disable_crc_));
 }
 float Modbus::get_setup_priority() const {
   // After UART bus
   return setup_priority::BUS - 1.0f;
 }
 
 }  // namespace modbus
 }  // namespace esphome