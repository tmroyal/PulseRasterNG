## Compiling and Running

cmake -B build -S .
cmake --build build
build/deckproto

# Flatpak for Steamdeck

# Install flatpack sdk

flatpak install -y flathub org.freedesktop.Sdk//23.08

# Run bash

flatpak run --command=bash   --socket=wayland --socket=fallback-x11 --device=all   --socket=pulseaudio --share=network   --filesystem=home --filesystem=/
run/udev:ro   org.freedesktop.Sdk//23.08

From then install cmake and g++ etc
