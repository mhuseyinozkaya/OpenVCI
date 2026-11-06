# OpenVCI - Open Vehicle Communication Interface

**English** | [Türkçe](README.tr.md)

VCI (Vehicle Communication Interface) devices used for vehicle diagnostics
are usually expensive, closed source and tied to specific software.
**OpenVCI** aims to provide a foundation that lets anyone connect to and
diagnose their vehicle with whatever they have at hand: cheap, widely
available hardware and open source software.

OpenVCI is not tied to a specific board, controller or protocol. Rather
than shipping a finished diagnostic program, the goal is to provide an open
infrastructure that anyone can use with their own hardware and build their
own diagnostic software on top of.

OpenVCI is the hardware layer of a larger project that includes desktop
and mobile diagnostic applications with LLM-based summarization.

## What OpenVCI Provides

- **Ready-to-use hardware and software:** Tested adapter firmwares and host
  tools.
- **Vehicle knowledge base:** How to connect to real vehicles. Even within
  the same model, the platform, pinout and supported protocols can change
  from year to year, so every vehicle we test is documented by make, model
  and year.

## Architecture

OpenVCI is made of independent layers. Each layer can be swapped for a
different implementation without changing the others.

| Layer | Role | Examples |
|-------|------|----------|
| **Vehicle network** | The communication bus in the vehicle | CAN, CAN FD, Automotive Ethernet (DoIP) |
| **Adapter hardware** | Physical access to the vehicle network | Arduino, ESP32, STM32 + MCP2515, SN65HVD230, built-in CAN peripheral |
| **Adapter firmware** | Bridge protocol between the vehicle network and the host | SLCAN (LAWICEL), ELM327 AT command set |
| **Link** | How the adapter connects to the host | USB serial, Bluetooth, Wi-Fi (TCP) |
| **Host** | The side that processes the data | Linux SocketCAN, `can-utils`, SavvyCAN, your own diagnostic software |

Adding a new board, controller, protocol or link only requires writing the
relevant layer.

## Project Layout

```
.
├── firmware/      # Adapter firmwares: firmware/<platform>/<board>/...
├── docs/          # Vehicle knowledge base: docs/<make>/<model>/<year>/
└── can-up.sh      # Brings up SLCAN adapters as a SocketCAN interface on Linux
```

Each firmware lives in its own folder together with a `README` describing
the supported hardware, wiring and setup steps.

Documents are written in English (`*.md`) and Turkish (`*.tr.md`).

## Available Implementations

| Firmware | Hardware | Protocol | Link |
|----------|----------|----------|------|
| [slcan_mcp2515_firmware_uno](firmware/arduino/uno/can/slcan_mcp2515_firmware_uno/README.md) | Arduino Uno + MCP2515 | SLCAN | USB |
| [slcan_mcp2515_bt_hm10_firmware_uno](firmware/arduino/uno/can/slcan_mcp2515_bt_hm10_firmware_uno/) | Arduino Uno + MCP2515 + HM-10 | SLCAN | Bluetooth |

## Host Side (Linux)

Any adapter that speaks SLCAN can be turned into a SocketCAN interface with
`can-up.sh`:

```bash
sudo apt install can-utils socat
./can-up.sh -d /dev/ttyUSB0 -b 500k -i can0
candump can0
```

| Option | Description | Default |
|--------|-------------|---------|
| `-d` | Serial device or socat address (e.g. `tcp:IP:PORT`) | `/dev/ttyUSB0` |
| `-b` | CAN bitrate (`10k` … `1M`) | `500k` |
| `-i` | Interface name | `can0` |
| `-s` | Serial baud rate of the adapter | `115200` |

## Vehicle Knowledge Base

[`docs/`](docs/README.md) documents how to connect to each tested vehicle:
OBD-II pinout, networks and bitrates, and supported diagnostic protocols
(OBD-II, UDS, …), so users can choose the right hardware for their vehicle.
Every vehicle document follows the same [template](docs/TEMPLATE.md).

## Warning

Connecting to a vehicle's communication bus can damage the vehicle if done
incorrectly. Start by only listening on the bus, and make sure you know
what you are doing before sending any message to the vehicle. The software
and documents are provided for use at your own risk.

## License

[GNU GPL v3](LICENSE)
