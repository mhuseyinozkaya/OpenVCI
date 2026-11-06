# Ford

**English** | [Türkçe](README.tr.md)

Notes that apply to Ford vehicles in general. See each vehicle's own
document for model and year specific details.

## CAN Networks

Ford vehicles have two separate CAN networks:

| Network | Bitrate | OBD-II Pins (H / L) | Carries |
|---------|---------|---------------------|---------|
| HS-CAN (High Speed) | 500 kbps | 6 / 14 | Critical functions: engine, RPM, transmission control |
| MS-CAN (Medium Speed) | 125 kbps | 3 / 11 | Comfort / body: air conditioning, doors, radio |

Since the networks are physically separate, packets with the same CAN ID
can appear on both HS-CAN and MS-CAN.

## Vehicles

- [Fiesta 2005](fiesta/2005/README.md)
- [Galaxy 2008](galaxy/2008/README.md)
