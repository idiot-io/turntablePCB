# Turntable PCB

## Work on the right revision

- The current base board is `hardware/kicad/direct_midi/base_direct_midi.kicad_pro`. Open the project (not a detached PCB) to pick up `fp-lib-table` and the local `Turntable.pretty` footprints. `hardware/kicad/base.*`, `hardware/kicad/comparator/`, and `hardware/plot-*.zip` are historical, not current fabrication inputs.
- Use local KiCad 10 tools, not Konnect. On this Windows setup, `kicad-cli.exe` and the `python.exe` that imports `pcbnew` are in `C:\Program Files\KiCad\10.0\bin`. Close the PCB editor before scripted writes so it cannot overwrite them.
- Keep the base within 100 × 100 mm and the wheel at 100 mm diameter. Preserve the eight Gray-code tracks and sensor centers; quadrature extends the same straight sensor row on the same board. Route with native KiCad arcs at bends, using `hardware/base.svg` as the style reference, not as copper to import.
- `F.Cu` faces the wheel and must stay free of components and exposed pads. The QRE1113GRs are **reverse-mounted on `B.Cu`**, lenses facing through the slit. Their pad pattern is mirrored for hand soldering and intentionally has no paste apertures. See `hardware/kicad/direct_midi/README.md` for the mechanical checks a clean DRC cannot replace.

## Check layout changes

From the repo root in PowerShell, refill, save, and check schematic parity in one pass:

```powershell
& 'C:/Program Files/KiCad/10.0/bin/kicad-cli.exe' pcb drc --refill-zones --save-board --schematic-parity --exit-code-violations -o "$env:TEMP/turntable-drc.txt" hardware/kicad/direct_midi/base_direct_midi.kicad_pcb
```

- Schematic ERC has a known pin-type error for each stock QRE1113GR emitter tied to ground; investigate new findings separately. `hardware/kicad_gerber_gen.py` is a legacy Python 2 script; use KiCad 10 exports for the current board.

## Firmware boundary

- `firmware/firmware.ino` is the legacy eight-channel Arduino/QTRSensors sketch with CV/gate output, not ESP32-S3 MIDI firmware. On the current board GPIO11/12 are direct RC quadrature inputs: charge and time the decay before decoding. A comparator-output PCNT test sketch cannot read these pins directly.
