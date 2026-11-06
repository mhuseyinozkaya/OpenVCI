# Ford Fiesta 2005

**English** | [Türkçe](README.tr.md)

## Vehicle

| | |
|---|---|
| Make | Ford |
| Model | Fiesta |
| Year | 2005 |
| Platform | Mk5 |
| Engine / Trim | ? |

## OBD-II Pinout

?

## Networks

| Network | Bitrate | OBD-II Pins (H / L) | Notes |
|---------|---------|---------------------|-------|
| HS-CAN | 500 kbps | 6 / 14 | OBD-II diagnostics |

See [Ford notes](../../README.md) for general information about the networks.

## Protocol Support

`✓` supported · `✗` not supported · `?` not tested

| Protocol | Status | Notes |
|----------|--------|-------|
| OBD-II (ISO 15765-4) | ✓ | HS-CAN |
| UDS (ISO 14229) | ✗ | Requests are rejected with `serviceNotSupported` |
| KWP2000 (ISO 14230) | ? | |
| DoIP (ISO 13400) | ? | |

## Test Setup

- Adapter: Arduino Uno + MCP2515
- Firmware: [slcan_mcp2515_firmware_uno](../../../../firmware/arduino/uno/can/slcan_mcp2515_firmware_uno/README.md) (SLCAN, USB)
- Date: October 2026

## Notes

- The year was read from the 11th character of the VIN (production year in
  Ford Europe VINs). The model year may differ from the production year.
- Unlike the [Galaxy 2008](../../galaxy/2008/README.md), this vehicle does
  not support UDS.
