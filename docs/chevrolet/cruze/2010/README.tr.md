# Chevrolet Cruze 2010

[English](README.md) | **Türkçe**

## Araç

| | |
|---|---|
| Marka | Chevrolet |
| Model | Cruze |
| Yıl | 2010 |
| Platform | ? |
| Motor / Donanım | 1.6 LS |

## OBD-II Pin Dizilimi

| Pin | Açıklama |
|-----|----------|
| 1 | SW-CAN (LS-GMLAN): gövde kontrol modülü, radyo, klima, cam, panel |
| 4 | Şase topraklaması |
| 5 | Sinyal topraklaması |
| 6 | CAN High |
| 14 | CAN Low |
| 16 | Kararlı +12V besleme |

![](cruze_2010_obd_pins.jpg)

## Ağlar

| Ağ | Hız | OBD-II Pinleri (H / L) | Not |
|----|-----|------------------------|-----|
| HS-CAN | ? | 6 / 14 | |
| SW-CAN (LS-GMLAN) | ? | 1 (tek hat) | Gövde / konfor |

## Protokol Desteği

`✓` destekliyor · `✗` desteklemiyor · `?` test edilmedi

| Protokol | Durum | Not |
|----------|-------|-----|
| OBD-II (ISO 15765-4) | ? | |
| UDS (ISO 14229) | ? | |
| KWP2000 (ISO 14230) | ? | |
| DoIP (ISO 13400) | ? | |

## Test Düzeneği

- Adaptör: ?
- Firmware: ?
- Tarih: ?

## Notlar

- **Veri okurken referans olarak sinyal topraklamasını (pin 5) kullanın.**
  Şase topraklaması (pin 4) doğrudan akünün negatif kutbuna bağlıdır ve
  elektromanyetik parazitten etkilenir. Referans olarak kullanılırsa okunan
  değerler hatalı olabilir ve araca zarar verilebilir. Sinyal topraklaması
  ise araç bilgisayarının sağladığı temiz, gürültüsüz hattır.

## Kaynaklar

- https://en.wikipedia.org/wiki/On-board_diagnostics
