# Radial Time ⏱️

A unique geometric watchface for Pebble 2 (and other Pebble models) that displays time using radial segments and an inner octagonal indicator.

![Pebble 2](https://img.shields.io/badge/Platform-Pebble%202-black)
![SDK](https://img.shields.io/badge/SDK-4.9.77-blue)

## Design

The watchface features a distinctive time display:

- **Outer Ring (Hours)**: 12 radial segments that fill clockwise starting from 12 o'clock. The number of filled segments indicates the current hour (e.g., 3 filled segments = 3 o'clock).
- **Inner Octagon (Minutes)**: An octagonal center divided into 12 triangular segments. These fill clockwise to indicate minutes in 5-minute increments.
- **Radial Lines**: 12 black lines separate the segments. Only the active lines (up to the current time) are drawn, creating a clean black background for future time.

## Simulator

This project includes a web-based simulator (`simulator.html`) that allows you to preview the watchface logic and geometry directly in your browser without compiling.

To use it:
1. Open `simulator.html` in any web browser.
2. Use the sliders to test different times.
3. Observe how the segments fill and the octagon geometry behaves.

## Prerequisites

To build and install the watchface, you need the **Pebble SDK**.

### Official Pebble Tool via Pip

```bash
pip3 install pebble-tool
```

### Install the SDK

```bash
pebble sdk install latest
```

## Building the Watchface

Open a terminal in the project directory and run:

```bash
pebble build
```

The compiled watchface will be generated at `build/radial-time.pbw`.

## Installing on Your Pebble

### 1. Enable Developer Mode
1. Open the **Pebble app** on your phone.
2. Go to **Settings** → **Developer Connection**.
3. Enable it and note the IP address (e.g., `192.168.1.100`).

### 2. Install
Ensure your computer and phone are on the same Wi-Fi network, then run:

```bash
pebble install --phone 192.168.1.100
```
*(Replace the IP address with your phone's actual IP)*

### Manual Installation
Alternatively, you can copy the `build/radial-time.pbw` file to your phone and open it with the Pebble app.

## Customization

You can adjust the design by modifying the constants at the top of `src/c/radial-time.c`:

```c
#define LINE_WIDTH 4          // Thickness of the radial lines
#define INNER_V (H * 30 / 100) // Vertical size of inner octagon
#define INNER_H (W * 30 / 100) // Horizontal size of inner octagon
```

## Project Structure

```
radial-time/
├── README.md           # This file
├── package.json        # Project metadata
├── wscript             # Build script
├── simulator.html      # JS/Canvas simulator
└── src/
    └── c/
        └── radial-time.c   # Main watchface source code
```

## License

MIT License - Feel free to modify and share!
