# OpenVCI - Open Vehicle Communication Interface

[English](README.md) | **Türkçe**

Araç teşhisi için kullanılan VCI (Vehicle Communication Interface)
cihazları genellikle pahalı, kapalı kaynaklı ve belirli yazılımlara
bağımlıdır. **OpenVCI**, herkesin elindeki imkanlarla, yani ucuz ve kolay
bulunabilir donanımlar ve açık kaynaklı yazılımlarla aracına bağlanıp
teşhis yapabilmesi için bir temel oluşturmayı hedefler.

OpenVCI belirli bir karta, kontrolcüye ya da protokole bağlı değildir.
Amaç, hazır bir teşhis programı sunmaktan çok, herkesin kendi donanımıyla
kullanabileceği ve üzerine kendi teşhis yazılımını geliştirebileceği açık
bir alt yapı sağlamaktır.

OpenVCI; masaüstü ve mobil teşhis uygulamaları ile LLM destekli
özetleme içeren daha geniş bir projenin donanım katmanıdır.

## OpenVCI Ne Sağlar

- **Hazır donanım ve yazılım desteği:** Test edilmiş adaptör firmware'leri
  ve host araçları.
- **Araç bilgi tabanı:** Gerçek araçlara nasıl bağlanılacağı. Aynı modelde
  bile platform, pin dizilimi ve desteklenen protokoller yıldan yıla
  değişebildiği için test ettiğimiz her araç marka, model ve yıl bazında
  belgelenir.

## Mimari

OpenVCI birbirinden bağımsız katmanlardan oluşur. Her katman, diğerlerini
değiştirmeden farklı bir uygulamayla değiştirilebilir.

| Katman | Görevi | Örnekler |
|--------|--------|----------|
| **Araç ağı** | Araçtaki haberleşme hattı | CAN, CAN FD, Automotive Ethernet (DoIP) |
| **Adaptör donanımı** | Araç ağına fiziksel erişim | Arduino, ESP32, STM32 + MCP2515, SN65HVD230, dahili CAN çevre birimi |
| **Adaptör firmware'i** | Araç ağı ile host arasında köprü protokolü | SLCAN (LAWICEL), ELM327 AT komut seti |
| **Bağlantı** | Adaptörün host'a bağlanma yolu | USB seri, Bluetooth, Wi-Fi (TCP) |
| **Host** | Veriyi işleyen taraf | Linux SocketCAN, `can-utils`, SavvyCAN, kendi teşhis yazılımınız |

Yeni bir kart, kontrolcü, protokol ya da bağlantı yöntemi eklemek için
yalnızca ilgili katmanı yazmak yeterlidir.

## Proje Yapısı

```
.
├── firmware/      # Adaptör firmware'leri: firmware/<platform>/<kart>/...
├── docs/          # Araç bilgi tabanı: docs/<marka>/<model>/<yıl>/
└── can-up.sh      # SLCAN adaptörlerini Linux'ta SocketCAN arayüzü olarak açar
```

Her firmware kendi klasöründe, desteklediği donanımı, bağlantı şemasını ve
kurulum adımlarını anlatan bir `README` ile birlikte bulunur.

Belgeler İngilizce (`*.md`) ve Türkçe (`*.tr.md`) olarak hazırlanır.

## Mevcut Uygulamalar

| Firmware | Donanım | Protokol | Bağlantı |
|----------|---------|----------|----------|
| [slcan_mcp2515_firmware_uno](firmware/arduino/uno/can/slcan_mcp2515_firmware_uno/README.tr.md) | Arduino Uno + MCP2515 | SLCAN | USB |
| [slcan_mcp2515_bt_hm10_firmware_uno](firmware/arduino/uno/can/slcan_mcp2515_bt_hm10_firmware_uno/) | Arduino Uno + MCP2515 + HM-10 | SLCAN | Bluetooth |

## Host Tarafı (Linux)

SLCAN konuşan herhangi bir adaptör, `can-up.sh` ile bir SocketCAN arayüzüne
dönüştürülebilir:

```bash
sudo apt install can-utils socat
./can-up.sh -d /dev/ttyUSB0 -b 500k -i can0
candump can0
```

| Parametre | Açıklama | Varsayılan |
|-----------|----------|------------|
| `-d` | Seri cihaz veya socat adresi (`tcp:IP:PORT` gibi) | `/dev/ttyUSB0` |
| `-b` | CAN hızı (`10k` … `1M`) | `500k` |
| `-i` | Arayüz adı | `can0` |
| `-s` | Adaptörün seri port hızı | `115200` |

## Araç Bilgi Tabanı

[`docs/`](docs/README.tr.md) test edilen her araca nasıl bağlanılacağını
belgeler: OBD-II pin dizilimi, ağlar ve hızları, desteklenen teşhis
protokolleri (OBD-II, UDS, …). Böylece kullanıcı aracına uygun donanımı
seçebilir. Tüm araç belgeleri aynı [şablonu](docs/TEMPLATE.tr.md) takip
eder.

## Uyarı

Aracın haberleşme hattına bağlanmak yanlış kullanımda araca zarar
verebilir. Hattı önce yalnızca dinleyin ve araca mesaj göndermeden önce ne
yaptığınızdan emin olun. Yazılım ve belgeler kendi sorumluluğunuzda
kullanılmak üzere sunulur.

## Lisans

[GNU GPL v3](LICENSE)
