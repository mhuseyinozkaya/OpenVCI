# Arduino Uno + MCP2515 SLCAN Firmware

**English** | [Türkçe](README.tr.md)

SLCAN (LAWICEL) firmware for the Arduino Uno with an MCP2515 CAN
controller. It forwards SLCAN commands received on the serial port to the
CAN bus, and writes incoming CAN messages back to the serial port in the
same format.

## Wiring

| MCP2515 | Arduino Uno |
|---------|-------------|
| VCC     | 5V          |
| GND     | GND         |
| CS      | D10         |
| SI (MOSI) | D11       |
| SO (MISO) | D12       |
| SCK     | D13         |
| INT     | D2          |

- MCP2515 module: 8 MHz crystal
- Serial baud rate: **115200 baud**

## Upload

Open `slcan_mcp2515_firmware_uno.ino` in the Arduino IDE, select
**Arduino Uno** as the board and the correct port, then upload it.
