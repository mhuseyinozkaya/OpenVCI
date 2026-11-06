# Chevrolet Cruze 2010

**English** | [Türkçe](README.tr.md)

## Vehicle

| | |
|---|---|
| Make | Chevrolet |
| Model | Cruze |
| Year | 2010 |
| Platform | ? |
| Engine / Trim | 1.6 LS |

## OBD-II Pinout

| Pin | Description |
|-----|-------------|
| 1 | SW-CAN (LS-GMLAN): body control module, radio, A/C, windows, cluster |
| 4 | Chassis ground |
| 5 | Signal ground |
| 6 | CAN High |
| 14 | CAN Low |
| 16 | Constant +12V supply |

![](cruze_2010_obd_pins.jpg)

## Networks

| Network | Bitrate | OBD-II Pins (H / L) | Notes |
|---------|---------|---------------------|-------|
| HS-CAN | ? | 6 / 14 | |
| SW-CAN (LS-GMLAN) | ? | 1 (single wire) | Body / comfort |

## Protocol Support

`✓` supported · `✗` not supported · `?` not tested

| Protocol | Status | Notes |
|----------|--------|-------|
| OBD-II (ISO 15765-4) | ? | |
| UDS (ISO 14229) | ? | |
| KWP2000 (ISO 14230) | ? | |
| DoIP (ISO 13400) | ? | |

## Test Setup

- Adapter: ?
- Firmware: ?
- Date: ?

## Notes

- **Use signal ground (pin 5) as the reference when reading data.** The
  chassis ground (pin 4) is connected directly to the battery's negative
  terminal and is exposed to electromagnetic interference. Using it as the
  reference can give wrong readings and may damage the vehicle. Signal
  ground is a clean, noise-free line provided by the vehicle's computer.

## References

- https://en.wikipedia.org/wiki/On-board_diagnostics
