# Ford

[English](README.md) | **Türkçe**

Ford araçların geneline ait notlar. Model ve yıla özgü ayrıntılar için her
aracın kendi belgesine bakın.

## CAN Ağları

Ford araçlarda iki ayrı CAN ağı vardır:

| Ağ | Hız | OBD-II Pinleri (H / L) | Taşıdığı işlemler |
|----|-----|------------------------|-------------------|
| HS-CAN (High Speed) | 500 kbps | 6 / 14 | Kritik işlemler: motor, devir, şanzıman kontrolü |
| MS-CAN (Medium Speed) | 125 kbps | 3 / 11 | Konfor / gövde: klima, kapılar, radyo |

Ağlar fiziksel olarak ayrı olduğu için hem HS-CAN hem de MS-CAN üzerinde
aynı CAN ID'ye sahip paketler olabilir.

## Araçlar

- [Fiesta 2005](fiesta/2005/README.tr.md)
- [Galaxy 2008](galaxy/2008/README.tr.md)
