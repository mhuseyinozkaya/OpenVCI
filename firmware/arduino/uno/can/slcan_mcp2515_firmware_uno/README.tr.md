# Arduino Uno + MCP2515 SLCAN Firmware

[English](README.md) | **Türkçe**

Arduino Uno ve MCP2515 CAN kontrolcüsü ile çalışan SLCAN (LAWICEL)
firmware'i. Seri porttan gelen SLCAN komutlarını CAN bus'a aktarır, gelen
CAN mesajlarını aynı formatta seri porta geri yazar.

## Bağlantı Şeması

| MCP2515 | Arduino Uno |
|---------|-------------|
| VCC     | 5V          |
| GND     | GND         |
| CS      | D10         |
| SI (MOSI) | D11       |
| SO (MISO) | D12       |
| SCK     | D13         |
| INT     | D2          |

- MCP2515 modülü: 8 MHz kristal
- Seri port hızı: **115200 baud**

## Yükleme

`slcan_mcp2515_firmware_uno.ino` dosyasını Arduino IDE ile aç, Board olarak
**Arduino Uno** ve doğru Portu seçip karta yükle.
