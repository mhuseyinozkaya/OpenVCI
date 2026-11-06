#!/usr/bin/env bash
#
# can-up.sh - Bring up a SocketCAN interface from an SLCAN (LAWICEL) adapter
#             using socat (PTY bridge) + slcand.
#
# Flow:  adapter (serial / tcp) --socat--> PTY (/tmp/ttyCAN) --slcand--> can0
#
# Usage: sudo ./can-up.sh [-d device] [-b bitrate] [-i iface] [-s serial_baud]
#   -d  socat source: serial device (/dev/ttyUSB0) or full socat address (tcp:IP:PORT)
#   -b  CAN bitrate: 10k 20k 50k 100k 125k 250k 500k 800k 1M   (default: 500k)
#   -i  interface name                                         (default: can0)
#   -s  serial baud rate of the adapter                        (default: 115200)
#
# Press Ctrl+C to stop; everything is cleaned up automatically.

set -euo pipefail

DEVICE="/dev/ttyUSB0"
BITRATE="500k"
IFACE="can0"
SERIAL_BAUD="115200"
PTY_LINK="/tmp/ttyCAN"

SOCAT_PID=""
SLCAND_PID=""

log()  { echo "[*] $*"; }
err()  { echo "[!] $*" >&2; }
die()  { err "$*"; exit 1; }

usage() {
    sed -n '8,13p' "$0" | sed 's/^# \{0,1\}//'
    exit 1
}

# ---------- arguments ----------
while getopts "d:b:i:s:h" opt; do
    case "$opt" in
        d) DEVICE="$OPTARG" ;;
        b) BITRATE="$OPTARG" ;;
        i) IFACE="$OPTARG" ;;
        s) SERIAL_BAUD="$OPTARG" ;;
        *) usage ;;
    esac
done

# ---------- root check (re-run with sudo) ----------
if [[ $EUID -ne 0 ]]; then
    exec sudo "$0" "$@"
fi

# ---------- bitrate -> slcand -sN ----------
case "$BITRATE" in
    10k)  SPEED=0 ;;
    20k)  SPEED=1 ;;
    50k)  SPEED=2 ;;
    100k) SPEED=3 ;;
    125k) SPEED=4 ;;
    250k) SPEED=5 ;;
    500k) SPEED=6 ;;
    800k) SPEED=7 ;;
    1M)   SPEED=8 ;;
    *)    die "Unsupported bitrate: $BITRATE" ;;
esac

# ---------- dependencies ----------
for cmd in socat slcand ip; do
    command -v "$cmd" >/dev/null 2>&1 || die "'$cmd' not found (install socat / can-utils / iproute2)"
done

# ---------- cleanup on exit ----------
cleanup() {
    trap - EXIT INT TERM
    log "Shutting down..."
    ip link set "$IFACE" down 2>/dev/null || true
    [[ -n "$SLCAND_PID" ]] && kill "$SLCAND_PID" 2>/dev/null || true
    [[ -n "$SOCAT_PID"  ]] && kill "$SOCAT_PID"  2>/dev/null || true
    wait 2>/dev/null || true
    rm -f "$PTY_LINK"
    log "Done."
}
trap cleanup EXIT INT TERM

# ---------- kernel modules ----------
log "Loading kernel modules (can, can_raw, slcan)"
modprobe can
modprobe can_raw
modprobe slcan

ip link show "$IFACE" >/dev/null 2>&1 && die "Interface $IFACE already exists"

# ---------- socat source address ----------
if [[ "$DEVICE" == /dev/* ]]; then
    [[ -e "$DEVICE" ]] || die "Device not found: $DEVICE"
    SOURCE="${DEVICE},b${SERIAL_BAUD},raw,echo=0"
    IS_SERIAL=1
else
    SOURCE="$DEVICE"          # e.g. tcp:192.168.4.1:3333
    IS_SERIAL=0
fi

# ---------- step 1: socat (adapter <-> PTY) ----------
log "Starting socat: $SOURCE <-> $PTY_LINK"
rm -f "$PTY_LINK"
socat "$SOURCE" "pty,link=${PTY_LINK},raw,echo=0" &
SOCAT_PID=$!

for _ in {1..20}; do
    [[ -e "$PTY_LINK" ]] && break
    sleep 0.1
done
[[ -e "$PTY_LINK" ]] || die "socat did not create $PTY_LINK"

# Arduino resets when the serial port is opened; wait for the bootloader.
if [[ $IS_SERIAL -eq 1 ]]; then
    log "Waiting for adapter to boot"
    sleep 2
fi

# ---------- step 2: slcand (PTY -> SocketCAN) ----------
log "Starting slcand: $PTY_LINK -> $IFACE @ $BITRATE"
slcand -F -o -c -s"$SPEED" -S "$SERIAL_BAUD" "$PTY_LINK" "$IFACE" &
SLCAND_PID=$!

for _ in {1..30}; do
    ip link show "$IFACE" >/dev/null 2>&1 && break
    sleep 0.1
done
ip link show "$IFACE" >/dev/null 2>&1 || die "slcand did not create $IFACE"

# ---------- step 3: interface up ----------
ip link set "$IFACE" txqueuelen 1000
ip link set "$IFACE" up
log "$IFACE is UP ($BITRATE). Try: candump $IFACE"
log "Press Ctrl+C to stop."

# ---------- keep running until something dies ----------
wait -n "$SOCAT_PID" "$SLCAND_PID" || true
err "socat or slcand exited unexpectedly"
