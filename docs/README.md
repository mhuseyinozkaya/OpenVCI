# Vehicle Knowledge Base

**English** | [Türkçe](README.tr.md)

Even within the same make and model, the platform, OBD-II pinout, CAN
networks and supported diagnostic protocols can change from one year to
another. This folder documents every vehicle we test, one by one.

## Layout

```
docs/
├── TEMPLATE.md                 # Template for a new vehicle document
└── <make>/
    ├── README.md               # Make-wide notes (e.g. Ford HS-CAN / MS-CAN)
    └── <model>/<year>/
        ├── README.md           # Vehicle document
        └── *.jpg               # OBD socket photos, etc.
```

## Vehicles

| Make | Model | Year |
|------|-------|------|
| Chevrolet | [Cruze](chevrolet/cruze/2010/README.md) | 2010 |
| Ford | [Fiesta](ford/fiesta/2005/README.md) | 2005 |
| Ford | [Galaxy](ford/galaxy/2008/README.md) | 2008 |

## Adding a Vehicle

1. Create a `docs/<make>/<model>/<year>/` folder.
2. Copy [TEMPLATE.md](TEMPLATE.md) as `README.md` (and
   [TEMPLATE.tr.md](TEMPLATE.tr.md) as `README.tr.md`) and fill it in.
3. Write only what you have actually tested. Mark anything not tested as
   `?`.
4. Add the vehicle to the list above.
