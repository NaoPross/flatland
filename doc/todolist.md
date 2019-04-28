Things To Do
============

Syntax
------
- [ ] task not done
- [x] completed task
- [!] completed task that should be reviewed / improved / optimized

- [n] nao is working on this task
- [r] raffa is working on this task
- [?] task needs to be discussed (conceptual or practical problem)


Project Metastuff
-----------------
- [x] build system (premake)
    - [x] build library
    - [x] build tests
    - [x] build dependencies

- [ ] uniform coding style (will it ever happen?)
    - [x] lowercase code
    - [x] capitalized metacode (template stuff)
    - [x] tab = 4 spaces (hard)
    - [?] comments style
    - [?] where to place brackets

- [ ] uniform git commit messages (will it ever happen? (too))
    - [x] max 72 characters
    - [ ] use imperative form


Mini Math
---------
- [x] vectors
    - [x] works with any numeric type
    - [x] +, -, +=, -= operators
    - [x] dot, cross product
- [ ] matrices
    - [ ] matrix product
    - [ ] +, -, +=, -= operators
    - [ ] dot, cross product
    - [ ] determinant

Wrap SDL2
---------
- [n] video
    - [x] window
    - [x] renderer
    - [x] texture (in VRAM)
    - [x] surface (in RAM)
- [x] events

Flatland
--------
- [!] core
    - [x] task and job
    - [x] signal, channel and listener
    - [!] initialization of the library (flatland.hpp)
    - [x] collector
        - [x] disallow two parents
    - [x] labeled type
        - [x] add a unique identifeir (hash or uuid)

- [ ] graphics
    - [x] window
    - [x] renderable objects
        - [x] layers
        - [!] sprite
    - [!] mouse / keyboard events
    - [ ] font loading
        - [ ] default font (unscii)
    - [?] graphical lua console

- [ ] sound
    - [?] audio resources loader

- [ ] game
    - [n] bounded (for collisions)
        - [n] collision checker
    - [n] entity (is bounded)
        - [n] actor (has a sprite)
    - [?] scenes (contains entities)
    - [?] maps (maybe load tmx?)
    - [?] lua interface for game scripting


Tests
-----
- [!] core
    - [ ] prioritized
    - [x] task and job
    - [x] signal, channel and listener
    - [x] collectors and children
    - [r] label and hashes

- [ ] graphics
    - [x] open a window
    - [ ] sprites
    - [?] animations

- [ ] sound
    - [ ] load and play audio files

- [ ] game
    - [n] bounds, collisions and actors
    - [?] simple game
