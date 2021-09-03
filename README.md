# PityBoy
PityBoy is a fantasy console inspired by Gameboy

160x128, 4 colors and no bitmap drawing (Except editor).
Map : 256x256x4 switchable by game.
Tiles : 256
Sprites : 256

TO DO: SFX and music

![PityBoy](https://user-images.githubusercontent.com/46056766/130839283-62d9a684-3b3f-4638-975c-cdc8c4d9900d.png)

## Usage:

`PityBoy Project_folder [-h] [-s] [-m]`
### Parameters: 
`Project_folder` Path to the folder of a project, Requires `code.lua` inside project folder, PityBoy Editor is not coming with text editor!  
`-h` Shows help about PityBoy and exits  
`-s` Skips startup animation  
`-m` Enables pixel smoothing (Toggleable too by Ctrl+S)

## Controls: 
### Controller mapping:  
```
Keyboard | PityBoy
   X     |   A
   Z     |   B
   Q     | SELECT
   E     | START
 Arrows  | D-PAD
```
### PityBoy additional keys:
`Ctrl + S` - Toggle pixel smoothing  
`Ctrl + +` - Larger scale (1:1)  
`Ctrl + -` - Smaller scale (1:1)  
For changing scale only numpad keys work, but in the future it will be fixed or changed.


# PityBoy Editor/DevKit

PityBoy Editor is a tool for creating projects and editing sprites and maps. 
It has a higher resolution for easier editing.

![obraz](https://user-images.githubusercontent.com/46056766/130841440-15a29074-307c-41ed-88a9-5a240b42260c.png)

# Community
There is official PityBoy Discord server created by me!  
If you have any question, smaller issue, sharing your creation or just to chat join here:
https://discord.gg/5Kxb9qzHKW


# Documentation

See [wiki](https://github.com/maniek207/PityBoy/wiki)

# Building
I am not sure if these instructions works, please let me know.
### Windows:

1. Install MSYS2
2. In MSYS2 install libs: SFML Boost Lua (5.4) 
3. Type `make` in Editor or Player folder

### Linux:

1. Install g++, binutils
2. Install dev libs: SFML, boost-filesystem, boost-system, Lua (5.4)
3. Type `make` in Editor or Player folder
