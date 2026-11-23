# Direct RC + MIDI revision

Open `base_direct_midi.kicad_pro` to load the schematic, PCB, and local
footprint library for this revision. The historical artwork and its imported
KiCad board remain separate.

- All populated parts mount on `B.Cu`. The wheel's patterned face faces the
  clear `F.Cu` side of the base. The lenses point from the underside through
  the slot toward the wheel. All ten sensor centers and five mounting holes
  are unchanged. The slot has extra clearance at both ends for the inverted
  packages and still leaves the outer board outline unchanged.
- The Waveshare ESP32-S3-Zero and the 3.5 mm Type A MIDI OUT jack mount on the
  underside, with both connectors still facing the same board edge.
  U1 uses the v2 STEP model from [Waveshare's resources][waveshare] for 3D
  visualization; the solder-down footprint is unchanged.
- The board uses USB for power and native USB MIDI. TRS MIDI uses GPIO10
  through a 5 V buffer. All ten optical inputs use direct RC readout.
- GPIO11 (`QUAD_A`, U11) and GPIO12 (`QUAD_B`, U12) read the two outer rings on
  the [100 mm wheel](../../wheel_quadrature_100mm.svg). Each ring has 128
  cycles; the quarter-cycle pattern offset gives 512 nominal counts per turn.
  These pins are not continuous logic-level outputs: firmware must charge,
  time and threshold the two RC channels before decoding quadrature.
- The jack footprint is CUI Devices `SJ-3523-SMT`. The local
  `QRE1113GR_OpticalSlot` footprint mirrors the lead pattern for lens-first
  assembly. Hand-solder these ten parts with the lenses inserted into the
  slot and the lead toes resting on the underside rails; do not place them
  lens-out for standard reflow. The pin-1 mark identifies the anode pad.
  These ten footprints intentionally have no stencil paste apertures.
- The curved routing is made of native KiCad tracks and circular arcs, not
  imported copper graphics. Ground uses the two GND pours and stitching vias
  instead of separate traces. The pours have thermal reliefs. This wired-only
  revision has no S3-Zero antenna keepout.

KiCad 10 DRC passes with no violations, unconnected items, or schematic parity
issues. The schematic ERC still flags the grounded emitter in each stock
QRE1113GR symbol.

Before fabrication, dry-fit an actual QRE1113GR and the jack on a routed
coupon. Check inverted lead overlap and solder access, the lens height and
slot corner radius, and that the jack's nominal 0.8 mm locating pegs stay
below the top of the 1.6 mm base. Set wheel spacing to clear the lenses and
mounting hardware through a full turn, then check stripe contrast and tune
the RC timing on the actual wheel. Verify that the TENSTAR clone matches
the Waveshare castellations and clears the carrier on its underside.

References: [Waveshare hardware resources][waveshare] and
[MIDI electrical specification][midi].

[waveshare]: https://docs.waveshare.com/ESP32-S3-Zero/Resources-And-Documents
[midi]: https://midi.org/5-pin-din-electrical-specs
