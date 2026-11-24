# Diagonal sensor placement variant

Open `base_direct_midi_diagonal.kicad_pro`. This variant shares the existing
footprint library and schematic connectivity. The matching wheel is
[`wheel_quadrature_100mm_diagonal.svg`](../../wheel_quadrature_100mm_diagonal.svg).

- The 100 × 100 mm base is centered on H3. The ten 3.5 × 3.1 mm sensor
  cutouts have 0.5 mm internal corner radii. The two rows are 90° apart,
  each on a board diagonal, with 6.19 mm of FR-4 between cutouts in each row.
- In KiCad's top view, the northwest row carries U9, U7, U5, U3, U11
  (`SENSE_8`, `SENSE_6`, `SENSE_4`, `SENSE_2`, `SENSE_9`). The northeast row
  carries U8, U6, U4, U2, U12 (`SENSE_7`, `SENSE_5`, `SENSE_3`, `SENSE_1`,
  `SENSE_10`). Both lists run from the spindle outward.
- The wheel's `B.Silk` artwork is registered in the same top-view coordinates
  as the PCB. Its odd and even tracks are rotated −45° and +45° respectively
  from the straight-row artwork. Check mirroring if printing from the
  opposite side. Hide the non-wheel reference layers when exporting.
- The ESP32 module and TRS jack are on `B.Cu`, with both ports facing the
  bottom edge in KiCad's top view. The old USB power and MIDI TX traces
  still need reconnecting after the module was moved.
- The passives follow the northeast placement pattern, mirrored for the
  northwest row. The ten series resistor outputs are routed on `B.Cu`
  without vias; local sensor circuits and power still need routing.
- U1 pads 9, 8, 7, 5, and 4 carry `SENSE_7`, `SENSE_5`, `SENSE_3`,
  `SENSE_1`, and `SENSE_10`, respectively. Pads 10, 11, 12, 14, and 15 carry
  `SENSE_8`, `SENSE_6`, `SENSE_4`, `SENSE_2`, and `SENSE_9`. Update any
  firmware pin definitions to match. MIDI TX stays on pad 6.
- These are PCB channel names only. The linked wheel still has quadrature
  artwork on its two outer rings.

Before fabrication, route and refill, then rerun DRC with schematic parity.
Dry-fit a reverse-mounted QRE1113GR in a routed coupon: DRC does not check
package fit, solder access, or clearance to the wheel and spindle hardware.
