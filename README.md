# PityBoy
**Originally made by maniek207. <br>**
**This project may not receive updates - Development is halted due to inexperience.**

PityBoy is a fantasy console written in C++, inspired by the famous GameBoy.

It has a resolution of 160x128 pixels, uses 4 colors and doesn't have bitmap drawing (excluding the editor). Currently there's no support for SFX and music.
Each game can contain up to 256 tiles, 256 sprites and four 256x256 maps.

![PityBoy Logo](https://user-images.githubusercontent.com/46056766/130839283-62d9a684-3b3f-4638-975c-cdc8c4d9900d.png)

## Usage:
`PityBoy directory [-h] [-s] [-m]`

### Parameters:
`directory` - path to the project folder. Requires `code.lua` inside the directory. PityBoy Editor is not included with the text editor.  
`-h` - Shows the help menu for PityBoy and exits  
`-s` - Skips the startup animation  
`-m` - Enables pixel smoothing (can also be toggled by pressing Ctrl + S)

## Controls:
### Key mapping:

| Keyboard | PityBoy |
| -------- | ------- |
| X        | A       |
| Z        | B       |
| Q        | SELECT  |
| E        | START   |
| Arrows   | D-PAD   |

### Additional bindings:
`Ctrl + S` - Toggle pixel smoothing  
`Ctrl + PLUS` - Larger scale (1:1)  
`Ctrl + MINUS` - Smaller scale (1:1)  
Only the numpad keys work for the scale changing binds, but support for the normal variants will be added in the future.

# PityBoy Editor
PityBoy Editor is a tool for creating projects & editing sprites and maps.  
It has a higher resolution for easier editing.

![PityBoy Sprite Editor](https://user-images.githubusercontent.com/46056766/130841440-15a29074-307c-41ed-88a9-5a240b42260c.png)

# Discord
There is an official PityBoy Discord server created by me!  
If you have any questions, issues, want to share your creation or just chat - join [here](https://discord.gg/5Kxb9qzHKW)

# Documentation
See the [wiki](https://github.com/maniek207/PityBoy/wiki)

# Building
~~I am not sure if these instructions works, please let me know.~~ <br>
These instruction work 100% for Linux, i'm sure they work for Windows too however i didn't test it yet.

### Windows:
1. Install `MSYS2`
2. In `MSYS2` install the following libs: `SFML`, `Boost` and `Lua (5.4)`
3. Type `make` in Editor or Player folder

### Ubuntu/Debian:
1. Install `g++` and `binutils`
2. Install dev libs: `SFML (libsfml-dev)`, `boost-filesystem & boost-system (both libboost-all-dev)` and `Lua 5.4 (liblua5.4-dev)`
3. Type `make` in the Editor or Player folder

### Fedora/CentOS:
1. Install `g++` and `binutils`
2. Install dev libs: `SFML-devel`, `boost-devel`, `lua`, `libstdc++-static`
3. Type `make` in the Editor or Player folder.