"""Generate the 100 mm wheel, preserving the original Gray tracks at 1:1 scale."""

import argparse
import math
from pathlib import Path
import xml.etree.ElementTree as ET

SVG = "http://www.w3.org/2000/svg"
INKSCAPE = "http://www.inkscape.org/namespaces/inkscape"
SOURCE = Path(__file__).with_name("wheel.svg")
CENTER = 50.0
RIM_CYCLES = 128
TRACK_HALF_WIDTH = 1.7
# Radii at the next two positions of the existing straight sensor row.
TRACKS = (("A", 35.531, 0.0), ("B", 39.324, 0.25))

ET.register_namespace("", SVG)
ET.register_namespace("inkscape", INKSCAPE)


def element(tag, **attrs):
    return ET.Element(f"{{{SVG}}}{tag}", attrs)


def layer(root, name, layer_id):
    group = element("g", id=layer_id)
    group.set(f"{{{INKSCAPE}}}groupmode", "layer")
    group.set(f"{{{INKSCAPE}}}label", name)
    root.append(group)
    return group


def point(radius, angle):
    return f"{CENTER + radius * math.cos(angle):.6f},{CENTER + radius * math.sin(angle):.6f}"


def add_quadrature(side):
    for name, radius, phase in TRACKS:
        track = element("g", id=f"quadrature-{name}", fill="black")
        side.append(track)
        inner, outer = radius - TRACK_HALF_WIDTH, radius + TRACK_HALF_WIDTH
        for i in range(RIM_CYCLES):
            a = math.tau * (i + phase) / RIM_CYCLES
            b = a + math.pi / RIM_CYCLES
            d = (f"M {point(outer, a)} A {outer:.3f} {outer:.3f} 0 0 1 {point(outer, b)} "
                 f"L {point(inner, b)} A {inner:.3f} {inner:.3f} 0 0 0 {point(inner, a)} Z")
            track.append(element("path", d=d))


def build_wheel():
    original = ET.parse(SOURCE).getroot()
    hole = original.find(".//*[@id='path37050-8']")
    dx = CENTER - float(hole.get("cx"))
    dy = CENTER - float(hole.get("cy"))
    wheel = element("svg", width="100mm", height="100mm", viewBox="0 0 100 100")
    outline = layer(wheel, "Edge.Cuts", "edge-cuts")
    outline.append(element("circle", cx="50", cy="50", r="50", fill="none",
                           stroke="black", **{"stroke-width": "0.1"}))

    for name, source_id in (("F.Silk", "g8663-3-1"), ("B.Silk", "g8663-3-1-3")):
        side = layer(wheel, name, name.lower().replace(".", "-"))
        gray = original.find(f".//*[@id='{source_id}']")
        encoder = element("g", id=source_id,
                          transform=f"translate({dx:.9f},{dy:.9f}) {gray.get('transform')}")
        side.append(encoder)
        for item in gray:
            if item.tag == f"{{{SVG}}}path" and not item.get("id").startswith("circle"):
                encoder.append(element("path", id=item.get("id"), d=item.get("d"), fill="black"))

        if name == "B.Silk":
            add_quadrature(side)

    drill = layer(wheel, "Drill", "drill")
    drill.append(element("circle", cx="50", cy="50", r=hole.get("r"), fill="black"))
    return ET.ElementTree(wheel)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("output", type=Path, help="SVG output path")
    args = parser.parse_args()
    if args.output.resolve() == SOURCE.resolve():
        parser.error("use a different output path to preserve the original wheel.svg")
    tree = build_wheel()
    ET.indent(tree)
    tree.write(args.output, encoding="utf-8", xml_declaration=True)


if __name__ == "__main__":
    main()
