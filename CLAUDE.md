# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Development Commands

**Build the project:**
```bash
cmake -B build -S .
cmake --build build
```

**Run the application:**
```bash
build/deckproto
```

**Run with custom scripts:**
```bash
build/deckproto --pdscript /path/to/pd/dir --luascript /path/to/lua/dir
```

Default directories: `./puredata` for Pure Data patches, `./scripts` for Lua scripts.

## High-Level Architecture

PulseRasterNG is a real-time multimedia creative coding framework that integrates audio synthesis, visual rendering, and precise timing through a Lua scripting interface. The system follows a modular architecture with four primary components:

### Core Components

- **ScriptRunner**: Central coordination hub using sol2 for C++/Lua integration. Manages multiple script environments with callback-based event handling.
- **pdEngine**: Audio synthesis engine embedding Pure Data (libpd) running at 44.1kHz with 64-sample stereo buffers. Communicates with Lua through thread-safe event queues.
- **VisualEngine**: Graphics rendering system using raylib, exposing comprehensive drawing operations to Lua scripts at 60 FPS.
- **Scheduler**: High-precision timing system (microsecond accuracy) using std::chrono, providing timer, metro, and variable interval functions to Lua.

### Data Flow

The main loop coordinates all systems: processes scheduled tasks → drains Pure Data event queue → renders graphics frames → performs Lua garbage collection. Communication between Pure Data and Lua occurs through lock-free queues (readerwriterqueue) handling bang, float, and symbol messages.

### Key Dependencies

- **libpd**: Pure Data integration for audio synthesis
- **raylib**: Graphics rendering and audio stream management  
- **sol2**: C++/Lua binding layer
- **readerwriterqueue**: Lock-free inter-thread communication

## File Structure

- `audio_engine/`: Pure Data integration (pdEngine, pdApi, event queuing)
- `visual_engine/`: Graphics rendering (VisualEngine, GraphicsApi)  
- `script_runner/`: Lua script execution and management
- `timing_api/`: High-precision scheduling system
- `controller_engine/`: External input handling
- `puredata/`: Pure Data patches and audio processing modules
- `examples/`: Sample Lua scripts and demonstrations
- `pieces/`: Complete audio-visual compositions

## Development Notes

- Audio engine runs in separate thread with lock-free communication
- Graphics rendering is synchronized to 60 FPS in main thread  
- Lua scripts can access all engine APIs through exposed interfaces
- Pure Data patches communicate via send/receive objects