
#include <driver/twai.h>
#include <WiFi.h>             // Arduino WiFi Wrapper library
#include <BluetoothSerial.h>
#include <string.h>

#define LOOPBACK_MODE 1
#define SERIAL_PORT_BAUDRATE 500000
#define PORT_READ_TIMEOUT_MS 50
#define CAN_TX_TIMEOUT_MS    30

#define PIN_5V_EN      16  // Enables the on-board 5V boost regulator (ME2107)
#define CAN_SE_PIN     23  // CAN transceiver slope/standby mode pin (LOW = High-Speed / Active)
#define CAN_TX_PIN     GPIO_NUM_27
#define CAN_RX_PIN     GPIO_NUM_26
#define RGB_LED_PIN    4   // On-board WS2812 addressable RGB LED pin
#define LED_BRIGHTNESS 32  // RGB LED brightness (0-255)

struct wifi_config {
  const char *ssid;
  const char *password;
  uint8_t channel;
  uint8_t max_connection;
  uint16_t port;
};

struct bluetooth_config {
  const char *name;
  const char *pin;
};

/* UPDATE THESE VALUES ACCORDING TO YOUR NETWORK */
static const struct wifi_config wf_config = {
  .ssid           = "OpenVCI WiFi",
  .password       = "lilygo2026", // Min 8 characters (NULL for open AP)
  .channel        = 1,            // Wi-Fi channel (1-13)
  .max_connection = 1,            // Allow only 1 diagnostic host at a time
  .port           = 3333          // TCP port (tcp:192.168.4.1:3333)
};
/* UPDATE THESE VALUES ACCORDING TO YOUR NETWORK */

/* UPDATE THESE VALUES ACCORDING TO YOUR BLUETOOTH */
static const struct bluetooth_config bt_config = {
  .name           = "OpenVCI Bluetooth",
  .pin            = "1337"        // Pairing PIN code (NULL for SSP auto-pairing)
};
/* UPDATE THESE VALUES ACCORDING TO YOUR BLUETOOTH */

/* Default global program variables */
twai_timing_config_t t_config = TWAI_TIMING_CONFIG_500KBITS();  // Default CAN bus speed 500KBps
WiFiServer tcp_server(wf_config.port);
WiFiClient tcp_client;
BluetoothSerial bt_serial;

static bool canbus_open = false;
static bool wifi_enabled = false;
char sl_rx_buffer[30]; // Receive buffer for SLCAN commands
char sl_tx_buffer[30]; // Transmit buffer for SLCAN frames

bool can_open() {
#if LOOPBACK_MODE
    twai_general_config_t g_config = TWAI_GENERAL_CONFIG_DEFAULT(CAN_TX_PIN, CAN_RX_PIN, TWAI_MODE_NO_ACK);
#else
    twai_general_config_t g_config = TWAI_GENERAL_CONFIG_DEFAULT(CAN_TX_PIN, CAN_RX_PIN, TWAI_MODE_NORMAL);
#endif
    // No filtered, all packets will be accepted
    twai_filter_config_t f_config = TWAI_FILTER_CONFIG_ACCEPT_ALL();
    if (twai_driver_install(&g_config, &t_config, &f_config) != ESP_OK) return false;
    if (twai_start() != ESP_OK) {
        twai_driver_uninstall();
        return false;
    }
    return true;
}

bool can_close() {
    twai_stop();
    twai_driver_uninstall(); // Required to allow re-installing with a new bitrate (S0..S8)
    return true;
}

bool set_can_bus_speed(int speed){
  switch(speed){
    case 8:
      t_config = TWAI_TIMING_CONFIG_1MBITS(); break;
    case 7:
      t_config = TWAI_TIMING_CONFIG_800KBITS(); break;
    case 6:
      t_config = TWAI_TIMING_CONFIG_500KBITS(); break;
    case 5:
      t_config = TWAI_TIMING_CONFIG_250KBITS(); break;
    case 4:
      t_config = TWAI_TIMING_CONFIG_125KBITS(); break;
    case 3:
      t_config = TWAI_TIMING_CONFIG_100KBITS(); break;
    case 2:
      t_config = TWAI_TIMING_CONFIG_50KBITS(); break;
#ifdef TWAI_TIMING_CONFIG_20KBITS
    case 1:
      t_config = TWAI_TIMING_CONFIG_20KBITS(); break;
    case 0:
      t_config = TWAI_TIMING_CONFIG_10KBITS(); break;
#endif
    default:
      return false;
  }
  return true;
}

void update_mode_led(){
  if(wifi_enabled){
    // White LED when Wi-Fi is active
    neopixelWrite(RGB_LED_PIN, LED_BRIGHTNESS, LED_BRIGHTNESS, LED_BRIGHTNESS);
  }else{
    // Blue LED when Bluetooth is active
    neopixelWrite(RGB_LED_PIN, 0, 0, LED_BRIGHTNESS);
  }
}

void send_ack(Stream &port){
  update_mode_led();
  port.write('\r'); // ACK to slcand
}

void send_nack(Stream &port){
  // Red LED on error (NACK)
  neopixelWrite(RGB_LED_PIN, LED_BRIGHTNESS, 0, 0);
  port.write('\a'); // NACK to slcand
}

void setup_bluetooth(){
  if(bt_config.pin != NULL){
#if defined(ESP_ARDUINO_VERSION_MAJOR) && ESP_ARDUINO_VERSION_MAJOR >= 3
    bt_serial.setPin(bt_config.pin, strlen(bt_config.pin));
#else
    bt_serial.setPin(bt_config.pin);
#endif
  }
  bt_serial.begin(bt_config.name);
  wifi_enabled = false;
  update_mode_led();
}

void stop_bluetooth(){
  bt_serial.end();
}

void setup_wifi(bool is_ap){
  if(is_ap){
    // Turn off Bluetooth before starting Wi-Fi
    stop_bluetooth();

    WiFi.mode(WIFI_AP);
    // Access Point Configuration
    WiFi.softAP(
      wf_config.ssid,
      wf_config.password,
      wf_config.channel,
      false,                            // ssid_hidden (0 = discoverable, 1 = hidden)
      wf_config.max_connection
    );
    WiFi.setSleep(false);
  }else{
    // Station mode not implemented yet
    return;
  }
  // Start TCP server and disable Nagle's algorithm for low-latency SLCAN frames
  tcp_server.begin();
  tcp_server.setNoDelay(true);
  wifi_enabled = true;
  update_mode_led();
}

void stop_wifi(){
  if (tcp_client) {
    tcp_client.stop();
  }
  tcp_server.end();
  WiFi.softAPdisconnect(true);
  WiFi.mode(WIFI_OFF);
  wifi_enabled = false;

  // Re-enable Bluetooth after turning off Wi-Fi
  setup_bluetooth();
}

void setup(){
  Serial.begin(SERIAL_PORT_BAUDRATE);

  pinMode(PIN_5V_EN, OUTPUT);
  digitalWrite(PIN_5V_EN, HIGH); // Open 5V bus

  pinMode(CAN_SE_PIN, OUTPUT);
  digitalWrite(CAN_SE_PIN, LOW); // Wake CAN transceiver (High-Speed mode)
  // Setup bluetooth on default
  setup_bluetooth();
}

void loop(){
  // 1. Listen for incoming SLCAN commands over USB
  if (Serial.available()) {
    lawicel_parser(Serial);
  }

  // 2. Listen on the active wireless interface (Wi-Fi OR Bluetooth)
  if (wifi_enabled) {
    if (tcp_server.hasClient()) {
      if (!tcp_client || !tcp_client.connected()) {
        if (tcp_client) tcp_client.stop();
        tcp_client = tcp_server.available();
        tcp_client.setNoDelay(true);
      } else {
        tcp_server.available().stop(); // Reject new client if one is already connected
      }
    }

    if (tcp_client && tcp_client.connected() && tcp_client.available()) {
      lawicel_parser(tcp_client);
    }
  } else {
    if (bt_serial.available()) {
      lawicel_parser(bt_serial);
    }
  }

  // 3. Read CAN frames from the bus and forward them to connected hosts
  if (canbus_open) {
    twai_message_t rx_msg;
    while (twai_receive(&rx_msg, 0) == ESP_OK) {
      send_frame_to_hosts(&rx_msg);
    }
  }
}


/** SLCAN Protocol & Frame Conversion Functions **/

void send_frame_to_hosts(const twai_message_t *rx_msg){
  frame_to_serialcan(rx_msg);
  size_t tx_len = strlen(sl_tx_buffer);

  Serial.write((const uint8_t *)sl_tx_buffer, tx_len);

  if (wifi_enabled) {
    if (tcp_client && tcp_client.connected()) {
      tcp_client.write((const uint8_t *)sl_tx_buffer, tx_len);
    }
  } else {
    if (bt_serial.hasClient()) {
      bt_serial.write((const uint8_t *)sl_tx_buffer, tx_len);
    }
  }
}

void lawicel_parser(Stream &port){
  read_port(port);

  if(!strcmp(sl_rx_buffer, "O")){
    if(canbus_open) can_close();
    if(can_open()){
      canbus_open = true;
      send_ack(port);
    }else{
      send_nack(port);
    }
  }
  else if(!strcmp(sl_rx_buffer, "C")){
    if(canbus_open){
      can_close();
      canbus_open = false;
    }
    send_ack(port);
  }
  // Sets speed (S0..S8)
  else if(sl_rx_buffer[0] == 'S'){
    int speed_rate = ascii_to_hex(sl_rx_buffer[1]);
    if(set_can_bus_speed(speed_rate)){
      send_ack(port);
    }else{
      send_nack(port);
    }
  }
  // Switch from Bluetooth to Wi-Fi ('WA' for AP mode)
  else if(sl_rx_buffer[0] == 'W'){
    if(!strncmp(&sl_rx_buffer[1], "A", 1)){
      send_ack(port);
      port.flush();
      delay(20); // Allow ACK to reach host before shutting down Bluetooth
      if(!wifi_enabled){
        setup_wifi(true);
      }
    }
    // Station mode not implemented
    else if(!strncmp(&sl_rx_buffer[1], "S", 1)){
      send_nack(port);
    }
    else{
      send_nack(port);
    }
  }
  // Turn off Wi-Fi diagnosis and switch back to Bluetooth ('w')
  else if(!strcmp(sl_rx_buffer, "w")){
    send_ack(port);
    port.flush();
    delay(20); // Allow ACK to reach host before shutting down Wi-Fi
    if(wifi_enabled){
      stop_wifi();
    }
  }
  // Standard ('t') or Extended ('T') CAN frame transmission
  else if(sl_rx_buffer[0] == 'T' || sl_rx_buffer[0] == 't'){
    if(!canbus_open){
      send_nack(port);
      return;
    }
    bool is_ext = (sl_rx_buffer[0] == 'T');
    twai_message_t tx_msg;
    if(serialcan_to_frame(is_ext, &tx_msg) &&
       twai_transmit(&tx_msg, pdMS_TO_TICKS(CAN_TX_TIMEOUT_MS)) == ESP_OK){
      send_ack(port);
    }else{
      send_nack(port);
    }
  }
}

void read_port(Stream &port){
  char recv_chr;
  size_t i = 0;
  uint32_t start_ms = millis();

  while(i < sizeof(sl_rx_buffer) - 1 && (millis() - start_ms) < PORT_READ_TIMEOUT_MS){
    if(port.available()){
      recv_chr = port.read();
      if(recv_chr == '\r') break;
      if(recv_chr == '\n') continue;
      sl_rx_buffer[i++] = recv_chr;
    }
  }
  sl_rx_buffer[i] = '\0';
}

void frame_to_serialcan(const twai_message_t *rx_msg){
  uint32_t clean_id;
  uint8_t offset, high_nibble, low_nibble;
  uint8_t len = rx_msg->data_length_code;

  if(rx_msg->extd){
    sl_tx_buffer[0] = 'T';
    offset = 0x5;
    clean_id = rx_msg->identifier & 0x1FFFFFFF;
  }else{
    sl_tx_buffer[0] = 't';
    offset = 0x0;
    clean_id = rx_msg->identifier & 0x7FF;
  }

  // Convert ID and payload length to serialcan
  sl_tx_buffer[4+offset] = hex_to_ascii(len);
  sl_tx_buffer[5+offset+(2*len)] = '\r';
  sl_tx_buffer[6+offset+(2*len)] = '\0';
  for(int i=3+offset; 0 < i; --i){
    sl_tx_buffer[i] = hex_to_ascii(clean_id & 0x0F);
    clean_id >>= 4;
  }

  // Convert payload to serialcan format
  for(int i=0; i<len; ++i){
    high_nibble = (rx_msg->data[i] & 0xF0) >> 4;
    low_nibble  = rx_msg->data[i] & 0x0F;
    sl_tx_buffer[(2*i)+5+offset] = hex_to_ascii(high_nibble);
    sl_tx_buffer[(2*i)+6+offset] = hex_to_ascii(low_nibble);
  }
}

bool serialcan_to_frame(bool is_ext, twai_message_t *tx_msg){
  uint32_t tmp_id = 0;
  uint8_t offset = (is_ext) ? 0x5 : 0x0;

  uint8_t len = ascii_to_hex(sl_rx_buffer[4+offset]);
  if(len > 8){
    return false; // Length overflow
  }

  // Convert ID and payload length to frames
  for(int i=1; i <= 3+offset; ++i){
    tmp_id <<= 4;
    tmp_id += ascii_to_hex(sl_rx_buffer[i]);
  }

  memset(tx_msg, 0, sizeof(twai_message_t));
  tx_msg->extd = is_ext ? 1 : 0;
  tx_msg->rtr = 0;
#if LOOPBACK_MODE
  tx_msg->self = 1; // Self Reception Request: echo transmitted frame into RX queue
#endif
  tx_msg->identifier = tmp_id;
  tx_msg->data_length_code = len;

  /* Write actual payload */
  for(int i=0; i<len; ++i){
    uint8_t tmp_data = 0;
    tmp_data += ascii_to_hex(sl_rx_buffer[(2*i)+5+offset]);
    tmp_data <<= 4;
    tmp_data += ascii_to_hex(sl_rx_buffer[(2*i)+6+offset]);
    tx_msg->data[i] = tmp_data;
  }
  return true;
}

/** Useful helper functions **/

char hex_to_ascii(uint8_t c) {
  return (c > 0x09) ? (c - 0x0A + 'A') : (c + 0x30);
}

char ascii_to_hex(char c){
  if (c >= '0' && c <= '9') return c - '0';
  if (c >= 'A' && c <= 'F') return c - 'A' + 10;
  if (c >= 'a' && c <= 'f') return c - 'a' + 10;
  return 0;
}