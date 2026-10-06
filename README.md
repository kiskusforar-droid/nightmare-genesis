# Nightmare Genesis

Nightmare Genesis is an original 2D survival horror game prototype designed for the Sega Genesis / Mega Drive. It is not a remake of any existing game; it is a new project with a horror atmosphere, exploration, enemy pressure, and a short but complete loop: explore the map, collect the three keys, reach the exit, and survive.

## Features

- Original IP and story concept
- 2D top-down exploration with a survival-horror mood
- Three keys to collect before escaping
- Several roaming enemies
- Health system and simple risk/reward loop
- Designed for SGDK (Sega Genesis C development kit)

## Controls

- D-pad: move
- A / B / C: reserved for future actions (currently the game keeps the loop simple and acts on movement only)
- Reset: start the game again after a defeat or victory

## Build instructions

1. Install SGDK on your machine.
2. Clone this repository and open it in a shell.
3. Make sure SGDK is available at `/opt/sgdk` or update `Makefile` to match your local SGDK path.
4. Run:

```bash
make
```

5. If your environment is configured with a Mega Drive emulator, run the generated ROM.

## Project structure

- `src/main.c`: entry point
- `src/game.h`: public game API
- `src/game.c`: gameplay loop, logic, rendering
- `Makefile`: SGDK build configuration

## Notes

This project is intentionally designed as a compact, playable prototype, not a full commercial-scale game engine. It is a solid foundation for ongoing iteration, content expansion, enemy variation, new rooms, and art/audio integration.
