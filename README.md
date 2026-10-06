# Nightmare Genesis

Nightmare Genesis is an original 2D survival horror game prototype for Sega Genesis / Mega Drive.

This second iteration expands the project from a minimal proof of concept into a more complete prototype with:

- title screen and loop flow
- clearer objective and story framing
- bigger map layout with corridors and rooms
- more enemies with simple pursuit behavior
- health, keys, escape condition, and restart flow
- more readable HUD and gameplay state management

## Story pitch

You wake up in an abandoned clinic beneath a flooded town. The power is gone, the corridors are silent, and the only way out is to recover the three ritual keys hidden in the hospital wing before the creatures stalking the halls reach you.

## Controls

- D-pad: move
- Start: confirm/restart
- A/B/C: reserved for future mechanics and special actions

## Build

1. Install SGDK.
2. Update the SGDK path in `Makefile` if needed.
3. Run:

```bash
make
```

4. Launch the generated ROM in your Mega Drive emulator.

## Current gameplay loop

1. Start in the title screen.
2. Collect the three keys.
3. Reach the exit while avoiding monsters.
4. If your health reaches zero, the game ends and you can restart.

## Project structure

- `src/main.c`: init and main loop
- `src/game.h`: shared declarations
- `src/game.c`: gameplay implementation
- `Makefile`: build settings
