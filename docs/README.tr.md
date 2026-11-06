# Araç Bilgi Tabanı

[English](README.md) | **Türkçe**

Aynı marka ve modelde bile platform, OBD-II pin dizilimi, CAN ağları ve
desteklenen teşhis protokolleri yıldan yıla değişebilir. Bu klasör test
ettiğimiz her aracı tek tek belgeler.

## Düzen

```
docs/
├── TEMPLATE.tr.md              # Yeni araç belgesi için şablon
└── <marka>/
    ├── README.tr.md            # Markanın geneline ait notlar (örn. Ford HS-CAN / MS-CAN)
    └── <model>/<yıl>/
        ├── README.tr.md        # Araç belgesi
        └── *.jpg               # OBD soketi fotoğrafları vb.
```

## Araçlar

| Marka | Model | Yıl |
|-------|-------|-----|
| Chevrolet | [Cruze](chevrolet/cruze/2010/README.tr.md) | 2010 |
| Ford | [Fiesta](ford/fiesta/2005/README.tr.md) | 2005 |
| Ford | [Galaxy](ford/galaxy/2008/README.tr.md) | 2008 |

## Araç Ekleme

1. `docs/<marka>/<model>/<yıl>/` klasörünü oluşturun.
2. [TEMPLATE.tr.md](TEMPLATE.tr.md) dosyasını `README.tr.md` olarak
   ([TEMPLATE.md](TEMPLATE.md) dosyasını da `README.md` olarak) kopyalayıp
   doldurun.
3. Yalnızca gerçekten test ettiğiniz bilgileri yazın. Test edilmeyenleri
   `?` ile işaretleyin.
4. Aracı yukarıdaki listeye ekleyin.
