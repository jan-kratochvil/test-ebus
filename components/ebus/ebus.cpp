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

   //send
   if (now - this->last_send_ > 1000) {
     std::vector<uint8_t> data;
     data.push_back(0x10);
     data.push_back(0x08);
     data.push_back(0xb5);
     data.push_back(0x11);
     data.push_back(0x01);
     data.push_back(0x01);
     data.push_back(0x89);
     this->send_raw(data);
   }
 
   if (now - this->last_byte_ > 50) {
     this->rx_buffer_.clear();
     this->last_byte_ = now;
   }
   // stop blocking new send commands after send_wait_time_ ms regardless if a response has been received since then
   if (now - this->last_send_ > send_wait_time_) {
     waiting_for_response = 0;
   }
 
   while (this->available()) {
     uint8_t byte;
     this->read_byte(&byte);
     if (this->parse_byte_(byte)) {
       this->last_byte_ = now;
     } else {
       this->rx_buffer_.clear();
     }
   }
 }
 
 bool Ebus::parse_byte_(uint8_t byte) {
   size_t at = this->rx_buffer_.size();
   this->rx_buffer_.push_back(byte);
   const uint8_t *raw = &this->rx_buffer_[0];
   ESP_LOGD(TAG, "Received Byte  %d (0X%x)", byte, byte);

   // return false to reset buffer
   return false;
 }
 
 void Ebus::dump_config() {
   ESP_LOGCONFIG(TAG, "Ebus:");
   ESP_LOGCONFIG(TAG, "  Send Wait Time: %d ms", this->send_wait_time_);
 }
 float Ebus::get_setup_priority() const {
   // After UART bus
   return setup_priority::BUS - 1.0f;
 }

 void Ebus::send_raw(const std::vector<uint8_t> &payload) {
   if (payload.empty()) {
     return;
   }
 
   //auto crc = crc16(payload.data(), payload.size());
   this->write_array(payload);
   //this->write_byte(crc & 0xFF);
   //this->write_byte((crc >> 8) & 0xFF);
   this->flush();
   ESP_LOGD(TAG, "Modbus write raw: %s", format_hex_pretty(payload).c_str());
   last_send_ = millis();
 }
 
 }  // namespace ebus
 }  // namespace esphome