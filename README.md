# Nightmare Genesis

Nightmare Genesis is an original 2D survival horror game prototype for Sega Genesis / Mega Drive.

This third iteration fixes the project structure and makes the prototype more coherent for a real SGDK workflow:

- game logic moved into `src/game.c`
- entry point kept clean in `src/main.c`
- title screen and restart flow improved
- more readable HUD and escape conditions
- adjusted enemy AI and damage loop

## Story pitch

You awaken in a flooded hospital beneath a dead town. The power is out, the halls are still, and the only way out is to recover the three keys hidden in the darkness before the creatures in the corridors find you.

## Controls

- D-pad: move
- Start: start / restart

## Build

1. Install SGDK.
2. Edit `Makefile` if your SGDK path differs.
3. Run:

```bash
make
```

4. Launch the generated ROM in a Mega Drive emulator.

## Current status

This version is a solid survival-horror prototype: title screen, playable loop, key collection, enemy chase, health, win/lose states, and restart flow.
