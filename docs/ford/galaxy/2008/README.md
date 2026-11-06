# Ford Galaxy 2008

**English** | [Türkçe](README.tr.md)

## Vehicle

| | |
|---|---|
| Make | Ford |
| Model | Galaxy |
| Year | 2008 |
| Platform | S-Max / Galaxy (2006–2015) |
| Engine / Trim | 2.0L Duratorq-TDCi (143 PS), 6-speed automatic |

## OBD-II Pinout

![](galaxy_2008_obd_pins.jpg)

## Networks

| Network | Bitrate | OBD-II Pins (H / L) | Notes |
|---------|---------|---------------------|-------|
| HS-CAN | 500 kbps | 6 / 14 | Engine, transmission |
| MS-CAN | 125 kbps | 3 / 11 | Body / comfort (lights, hazard lights, etc.) |

See [Ford notes](../../README.md) for general information about the networks.

## Protocol Support

`✓` supported · `✗` not supported · `?` not tested

| Protocol | Status | Notes |
|----------|--------|-------|
| OBD-II (ISO 15765-4) | ✓ | HS-CAN |
| UDS (ISO 14229) | ✓ | HS-CAN |
| KWP2000 (ISO 14230) | ? | |
| DoIP (ISO 13400) | ? | |

## Test Setup

- Adapter: Arduino Uno + MCP2515
- Firmware: [slcan_mcp2515_firmware_uno](../../../../firmware/arduino/uno/can/slcan_mcp2515_firmware_uno/README.md) (SLCAN, USB)
- Date: August – November 2026

## Notes

- HS-CAN traffic was recorded while driving; MS-CAN traffic was recorded
  while operating the lights and hazard lights.
