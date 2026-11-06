# Ford Galaxy 2008

[English](README.md) | **Türkçe**

## Araç

| | |
|---|---|
| Marka | Ford |
| Model | Galaxy |
| Yıl | 2008 |
| Platform | S-Max / Galaxy (2006–2015) |
| Motor / Donanım | 2.0L Duratorq-TDCi (143 PS), 6 ileri otomatik |

## OBD-II Pin Dizilimi

![](galaxy_2008_obd_pins.jpg)

## Ağlar

| Ağ | Hız | OBD-II Pinleri (H / L) | Not |
|----|-----|------------------------|-----|
| HS-CAN | 500 kbps | 6 / 14 | Motor, şanzıman |
| MS-CAN | 125 kbps | 3 / 11 | Gövde / konfor (farlar, dörtlü ikaz vb.) |

Ağlarla ilgili genel bilgi için [Ford notlarına](../../README.tr.md) bakın.

## Protokol Desteği

`✓` destekliyor · `✗` desteklemiyor · `?` test edilmedi

| Protokol | Durum | Not |
|----------|-------|-----|
| OBD-II (ISO 15765-4) | ✓ | HS-CAN |
| UDS (ISO 14229) | ✓ | HS-CAN |
| KWP2000 (ISO 14230) | ? | |
| DoIP (ISO 13400) | ? | |

## Test Düzeneği

- Adaptör: Arduino Uno + MCP2515
- Firmware: [slcan_mcp2515_firmware_uno](../../../../firmware/arduino/uno/can/slcan_mcp2515_firmware_uno/README.tr.md) (SLCAN, USB)
- Tarih: Ağustos – Kasım 2026

## Notlar

- HS-CAN trafiği sürüş sırasında, MS-CAN trafiği farlar ve dörtlü ikaz
  kullanılırken kaydedildi.
