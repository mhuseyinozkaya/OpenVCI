# Ford Fiesta 2005

[English](README.md) | **Türkçe**

## Araç

| | |
|---|---|
| Marka | Ford |
| Model | Fiesta |
| Yıl | 2005 |
| Platform | Mk5 |
| Motor / Donanım | ? |

## OBD-II Pin Dizilimi

?

## Ağlar

| Ağ | Hız | OBD-II Pinleri (H / L) | Not |
|----|-----|------------------------|-----|
| HS-CAN | 500 kbps | 6 / 14 | OBD-II teşhisi |

Ağlarla ilgili genel bilgi için [Ford notlarına](../../README.tr.md) bakın.

## Protokol Desteği

`✓` destekliyor · `✗` desteklemiyor · `?` test edilmedi

| Protokol | Durum | Not |
|----------|-------|-----|
| OBD-II (ISO 15765-4) | ✓ | HS-CAN |
| UDS (ISO 14229) | ✗ | İstekler `serviceNotSupported` ile reddediliyor |
| KWP2000 (ISO 14230) | ? | |
| DoIP (ISO 13400) | ? | |

## Test Düzeneği

- Adaptör: Arduino Uno + MCP2515
- Firmware: [slcan_mcp2515_firmware_uno](../../../../firmware/arduino/uno/can/slcan_mcp2515_firmware_uno/README.tr.md) (SLCAN, USB)
- Tarih: Ekim 2026

## Notlar

- Yıl, VIN'in 11. karakterinden okundu (Ford Avrupa VIN'lerinde üretim
  yılı). Model yılı üretim yılından farklı olabilir.
- [Galaxy 2008](../../galaxy/2008/README.tr.md) aracından farklı olarak bu
  araç UDS desteklemiyor.
