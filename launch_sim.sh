#!/bin/bash
# Remove any existing lock files just in case
rm -f /tmp/.X0-lock

# Start Xvfb (Virtual Framebuffer)
echo "Starting Xvfb..."
Xvfb :0 -screen 0 1280x1024x24 &
export DISPLAY=:0

# Wait for Xvfb to be ready
sleep 1

# Start x11vnc
echo "Starting VNC server..."
x11vnc -display :0 -forever -nopw -bg -quiet

# Build the watchface
echo "Building project..."
pebble build

# Install and run emulator
echo "Starting Emulator (Diorite)..."
pebble install --emulator diorite --logs
