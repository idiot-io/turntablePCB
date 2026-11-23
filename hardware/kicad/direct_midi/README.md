# Direct RC + MIDI revision

Open `base_direct_midi.kicad_pro` to load the schematic, PCB, and local
footprint library for this revision. The historical artwork and its imported
KiCad board remain separate.

- The optical array stays on the disc-facing side (`B.Cu`). Its centres, the
  optical slot, and the five mechanical holes follow the original board.
- The Waveshare ESP32-S3-Zero mounts flat on `F.Cu` with its USB-C port facing
  the right edge. The 3.5 mm Type A MIDI OUT jack faces the same edge.
  In this layout the eight sense routes are 23–40 mm long.
- The board uses USB for power and native USB MIDI. TRS MIDI uses GPIO10
  through a 5 V buffer; the eight optical inputs are direct RC channels.
- The jack footprint is CUI Devices `SJ-3523-SMT`. The project footprint
  library includes the Waveshare carrier land pattern and the slot-adapted
  sensor footprint, derived from KiCad's `OptoDevice:OnSemi_CASE100CY`.
- The curved routing is made of native KiCad tracks and circular arcs, not
  imported copper graphics. Ground uses the two GND pours and stitching vias
  instead of separate traces. The pours have thermal reliefs and respect
  the S3-Zero antenna keepout on both layers.

KiCad 10 DRC passes with no violations, unconnected items, or schematic parity
issues. The schematic ERC still flags the grounded emitter in each stock
QRE1113GR symbol.

Before fabrication, check the solder-down module and the jack against physical
samples. In particular, verify that the TENSTAR clone matches the Waveshare
castellations and clears the carrier on its underside. Check the optical lead
overlap at the slot and tune the prototype timing capacitors on the real disc.

References: [Waveshare hardware resources][waveshare] and
[MIDI electrical specification][midi].

[waveshare]: https://docs.waveshare.com/ESP32-S3-Zero/Resources-And-Documents
[midi]: https://midi.org/5-pin-din-electrical-specs
