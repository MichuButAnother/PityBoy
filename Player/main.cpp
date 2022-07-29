/*
MIT License

Copyright (c) 2022 Michu

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#define PityBoy_Version 1         // PityBoy code version
#define PityBoy_Str_Version pre_1 // PityBoy name version

#include <iostream>
#ifdef _WIN32
#include <lua.hpp>
#else
#include <lua5.4/lua.hpp>
#endif
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <math.h>

#include "projectparser.hpp"
#include "font.h"

using namespace std;

const unsigned int W = 160;
const unsigned int H = 128;
int scaling = 3;

uint8_t *pixels;

const uint8_t pallete[4][3] = {{230, 230, 0}, {204, 170, 0}, {128, 128, 0}, {64, 64, 0}};

sf::RenderWindow window(sf::VideoMode(W *scaling, H *scaling), "PityBoy");
sf::Texture stexture;
sf::Sprite sscreen;

bool controller[8]; // A B, Start Select, Arrows

sf::Clock timeElapsed;

int mapx = 0;
int mapy = 0;
int mapindex = 0;
int mapdraw = false;
bool halted = false;
PityProject project;

bool fexists(const std::string &name)
{
    ifstream f(name.c_str());
    return f.good();
}

void pix(int x, int y, int color)
{
    if (color > 3 || color < 0)
        return;
    if (x > 159)
        return;
    if (x < 0)
        return;
    if (y > 127)
        return;
    if (y < 0)
        return;
    if ((x + (y * 160)) > (W * H))
        return;
    pixels[((x + (y * 160)) * 4) + 0] = pallete[color][0];
    pixels[((x + (y * 160)) * 4) + 1] = pallete[color][1];
    pixels[((x + (y * 160)) * 4) + 2] = pallete[color][2];
}

void line(int x1, int y1, int x2, int y2, int c = 3)
{
    int x, y, dx, dy, dx1, dy1, px, py, xe, ye, i;
    dx = x2 - x1;
    dy = y2 - y1;
    dx1 = fabs(dx);
    dy1 = fabs(dy);
    px = 2 * dy1 - dx1;
    py = 2 * dx1 - dy1;
    if (dy1 <= dx1)
    {
        if (dx >= 0)
        {
            x = x1;
            y = y1;
            xe = x2;
        }
        else
        {
            x = x2;
            y = y2;
            xe = x1;
        }
        pix(x, y, c);
        for (i = 0; x < xe; i++)
        {
            x = x + 1;
            if (px < 0)
            {
                px = px + 2 * dy1;
            }
            else
            {
                if ((dx < 0 && dy < 0) || (dx > 0 && dy > 0))
                {
                    y = y + 1;
                }
                else
                {
                    y = y - 1;
                }
                px = px + 2 * (dy1 - dx1);
            }
            //   delay(0);
            pix(x, y, c);
        }
    }
    else
    {
        if (dy >= 0)
        {
            x = x1;
            y = y1;
            ye = y2;
        }
        else
        {
            x = x2;
            y = y2;
            ye = y1;
        }
        pix(x, y, c);
        for (i = 0; y < ye; i++)
        {
            y = y + 1;
            if (py <= 0)
            {
                py = py + 2 * dx1;
            }
            else
            {
                if ((dx < 0 && dy < 0) || (dx > 0 && dy > 0))
                {
                    x = x + 1;
                }
                else
                {
                    x = x - 1;
                }
                py = py + 2 * (dx1 - dy1);
            }
            //  delay(0);
            pix(x, y, c);
        }
    }
}

void box(int x1, int y1, int x2, int y2, int color = 3, bool bfill = false)
{
    if (color > 3 || color < 0)
        return;
    if (!bfill)
    {
        line(x1, y1, x2, y1, color);
        line(x1, y2, x2, y2, color);
        line(x1, y1, x1, y2, color);
        line(x2, y1, x2, y2, color);
    }
    else
    {
        int x = 0;
        int y = 0;
        while (1)
        {
            while (1)
            {
                pix(x1 + x, y1 + y, color);
                x = x + 1;
                if (x > x2 - x1 && y > y2 - y1)
                    return;
                if (x > x2 - x1)
                    break;
            }
            x = 0;
            y = y + 1;
            if (y > y2 - y1)
                break;
        }
    }
}

void drawchar(char chr, int x, int y, int color = 3, bool altfont = false)
{
    if (color < 0 && color > 3)
        return;
    int8_t c;
    if (altfont)
    {
        c = chr - 32;
    }
    else
    {
        c = chr;
    }
    int b = 0;
    int g = 0;
    bool u[8];
    while (1)
    {
        if (altfont)
        {
            for (int i = 0; i < 8; ++i)
                u[i] = (font16[c][b] & (1 << i)) != 0;
        }
        else
        {
            for (int i = 0; i < 8; ++i)
                u[i] = (font8[c][b] & (1 << i)) != 0;
        }
        g = 0;
        while (1)
        {
            if (altfont)
            {
                if (u[g])
                    pix(x + 8 - g, y + 8 - b, color);
            }
            else
            {
                if (u[g])
                    pix(x + g, y + b, color);
            }
            g = g + 1;
            if (g > 7)
                break;
        }

        b = b + 1;
        if (altfont)
        {
            if (b > 12)
                break;
        }
        else
        {
            if (b > 7)
                break;
        }
    }
}

void clearscr(int color = 0)
{
    if (color < 0 && color > 3)
        return;
    for (register int i = 0; i < W * H * 4; i += 4)
    {

        pixels[i] = pallete[color][0];
        pixels[i + 1] = pallete[color][1];
        pixels[i + 2] = pallete[color][2];
        // pixels[i+3] = 255;
    }
}

void drawtext(string text, int x, int y, int color = 3, bool altfont = false)
{
    if (color < 0 && color > 3)
        return;
    int g = text.length();
    int i = 0;
    int sx = 0;
    int sy = 0;
    while (1)
    {
        if (text.at(i) == '\0')
            return;
        if (text.at(i) == '\n')
        {
            sx = -1;
            sy++;
        }
        else
        {
            if (160 < x + (10 * sx))
            {
                sx = 0;
                sy++;
            }
            if (altfont)
            {
                drawchar(text.at(i), x + (10 * sx), y + (14 * sy), color, true);
            }
            else
            {
                drawchar(text.at(i), x + (8 * sx), y + (8 * sy), color, false);
            }
        }
        i = i + 1;
        sx = sx + 1;
        if (i >= g)
            break;
    }
}

void errorexit(bool showmessage = false, string message = "")
{
    clearscr();
    if (showmessage)
    {
        drawtext("Error:", 0, 0);
        drawtext(message, 0, 8);
    }
    else
    {
        drawtext("Lua error!\nCheck console.", 0, 0);
    }
    stexture.update(pixels);
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.draw(sscreen);
        window.display();
    }
}

void print_error(lua_State *state)
{
    // The error message is on top of the stack.
    // Fetch it, print it and then pop it off the stack.
    const char *message = lua_tostring(state, -1);
    cout << message << endl;
    errorexit(true, message);
    lua_pop(state, 1);
}

void force_error(lua_State *state, string message)
{
    lua_Debug ar;
    lua_getstack(state, 1, &ar);
    lua_getinfo(state, "nSl", &ar);
    int line = ar.currentline;
    cout << "code.lua:" << line << ": " << message << endl;
    errorexit();
    lua_pop(state, 1);
    exit(1);
}

//  ---------------------------------------------------
// --------------- LUA FUNCTIONS ---------------------
//---------------------------------------------------

int l_log(lua_State *state)
{
    int args = lua_gettop(state);
    if (args != 1)
        force_error(state, " log - Excepted 1 parameter");
    cout << lua_tostring(state, 1) << endl;
    return 0;
}

int l_time(lua_State *state)
{
    int args = lua_gettop(state);
    if (args != 0)
        force_error(state, " time - Excepted no parameters");
    sf::Time elapsed = timeElapsed.getElapsedTime();
    uint32_t msec = elapsed.asMilliseconds();
    lua_pushnumber(state, msec);
    return 1;
}

int l_key(lua_State *state)
{
    int args = lua_gettop(state);
    if (args != 1)
        force_error(state, " key - Excepted 1 parameter");
    int i = lua_tointeger(state, 1);
    if (i < 0 || i > 7)
    {
        lua_pushinteger(state, 0);
        return 1;
    }
    lua_pushboolean(state, controller[i]);
    return 1;
}

int l_setmapx(lua_State *state)
{
    int args = lua_gettop(state);
    if (args != 1)
        force_error(state, " setmapx - Excepted 1 parameter");
    int i = -lua_tointeger(state, 1);
    mapx = i;
    return 0;
}

int l_setmapy(lua_State *state)
{
    int args = lua_gettop(state);
    if (args != 1)
        force_error(state, " setmapy - Excepted 1 parameter");
    int i = -lua_tointeger(state, 1);
    mapy = i;
    return 0;
}

int l_getmapx(lua_State *state)
{
    int args = lua_gettop(state);
    if (args != 0)
        force_error(state, " getmapx - Excepted no parameters");
    lua_pushinteger(state, -mapx);
    return 1;
}

int l_getmapy(lua_State *state)
{
    int args = lua_gettop(state);
    if (args != 0)
        force_error(state, " getmapy - Excepted no parameters");
    lua_pushinteger(state, -mapy);
    return 1;
}

int l_setmapd(lua_State *state)
{
    int args = lua_gettop(state);
    if (args != 1)
        force_error(state, " setmapd - Excepted 1 parameter");
    int i = lua_tointeger(state, 1);
    if (i < 0 || i > 1)
        return 0;
    mapdraw = i;
    return 0;
}

int l_getmapd(lua_State *state)
{
    int args = lua_gettop(state);
    if (args != 0)
        force_error(state, " getmapd - Excepted no parameters");
    lua_pushboolean(state, int(mapdraw));
    return 1;
}

int l_setmapi(lua_State *state)
{
    int args = lua_gettop(state);
    if (args != 1)
        force_error(state, " setmapi - Excepted 1 parameter");
    int i = lua_tointeger(state, 1);
    if (i < 0 || i > 3)
        return 0;
    mapindex = i;
    return 0;
}

int l_getmapi(lua_State *state)
{
    int args = lua_gettop(state);
    if (args != 0)
        force_error(state, " getmapi - Excepted no parameters");
    lua_pushinteger(state, mapindex);
    return 1;
}

int l_setmap(lua_State *state)
{
    int args = lua_gettop(state);
    if (args != 3)
        force_error(state, " setmap - Excepted 3 parameters");
    int x = lua_tointeger(state, 1);
    int y = lua_tointeger(state, 2);
    if (x < 0)
        x = 0;
    if (y < 0)
        y = 0;
    if (y > 255)
        y = 255;
    if (x > 255)
        x = 255;
    int i = lua_tointeger(state, 3);
    if (i < 0)
        i = 0;
    if (i > 255)
        i = 255;
    project.maps[x][y][mapindex] = i;
    return 0;
}

int l_getmap(lua_State *state)
{
    int args = lua_gettop(state);
    if (args != 2)
        force_error(state, " getmap - Excepted 2 parameters");
    int x = lua_tointeger(state, 1);
    int y = lua_tointeger(state, 2);
    if (x < 0)
        x = 0;
    if (y < 0)
        y = 0;
    if (y > 255)
        y = 255;
    if (x > 255)
        x = 255;
    lua_pushinteger(state, project.maps[x][y][mapindex]);
    return 1;
}

int l_sprite(lua_State *state)
{
    int args = lua_gettop(state);
    if (args < 3)
        force_error(state, " sprite - Excepted minimum 3 parameters");
    if (args > 6)
        force_error(state, " sprite - Excepted maximum 6 parameters");

    int x = lua_tointeger(state, 2);
    int y = lua_tointeger(state, 3);
    int sp = lua_tointeger(state, 1);
    if (sp < 0)
        sp = 0;
    if (sp > 511)
        sp = 511;

    int tcolor = 5;
    int scale = 1;
    bool flip = false;

    if (args >= 4)
        tcolor = lua_tointeger(state, 4);
    if (args >= 5)
        scale = lua_tointeger(state, 5);
    if (args >= 6)
        flip = lua_toboolean(state, 6);
    if (scale == 0)
        return 0;
    if (scale > 160)
        scale = 160;

    int b = 0;
    int i = 0;
    while (1)
    {
        while (1)
        {
            int bx, by, cx, cy;
            bx = 7;
            if (flip)
            {
                if (scale > 1)
                {
                    bx = (8 * scale) + x - (i * scale) - scale;
                    by = y + (b * scale) - scale;
                }
                else
                {
                    bx = 8 + x - i;
                    by = y + b;
                }
                cx = bx + scale - 1;
                cy = by + scale - 1;
            }
            else
            {
                if (scale > 1)
                {
                    bx = x + (i * scale) - scale;
                    by = y + (b * scale) - scale;
                }
                else
                {
                    bx = x + i;
                    by = y + b;
                }
                cx = bx + scale - 1;
                cy = by + scale - 1;
            }
            // if(bx<160&&bx>=0&&by>=0&&by<128) {
            if (project.sprites[i][b][sp] != tcolor)
                box(bx, by, cx, cy, project.sprites[i][b][sp], true);
            //}
            i++;
            if (i >= 8)
                break;
        }
        i = 0;
        b++;
        if (b >= 8)
            break;
    }

    return 0;
}

int l_halt(lua_State *state)
{
    cout << "HALT" << endl;
    halted = true;
}

int l_exit(lua_State *state)
{
    exit(0);
}

int framecounter = 0;

// options
bool skipstartanim = false;
bool smooth = false;

// int l_print(lua_State* state) {l_log(state); return 0;} // replaced by log

// static const struct luaL_Reg printlib [] = {
//  {"print", l_print},
//   {NULL, NULL} /* end of array */
// };

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        cerr << "Usage: " << argv[0] << " DIRECTORY [-h] [-s] [-m]\n\n";

        return -1;
    }
    int ai = 0;
    while (1)
    {
        if (argc == 2)
            break;
        if (strcmp(argv[ai], "-h") == 0)
        {
            cout << "Usage: " << argv[0] << " DIRECTORY [-h] [-s] [-m]\n\nArguments:\n  -h   Show this help and exit\n  -s   Skip startup animation\n  -m   Enable pixel smoothing (Toggleable with CTRL+S)\n"
                 << endl;
            return 0;
        }
        if (strcmp(argv[ai], "-s") == 0)
            skipstartanim = true;
        if (strcmp(argv[ai], "-m") == 0)
            smooth = true;
        ai++;
        if (ai > (argc - 1))
            break;
    }
    string projectpath = argv[1];

    string endchar = projectpath.substr(projectpath.length() - 1);
    if (endchar != "/" && endchar != "\\")
    {
        projectpath = projectpath + "/";
    }
    cout << projectpath << endl;

    project.folder = projectpath;
    project.load_sprites();
    project.load_maps();

    controller[0] = 0;
    controller[1] = 0;
    controller[2] = 0;
    controller[3] = 0;
    controller[4] = 0;
    controller[5] = 0;
    controller[6] = 0;
    controller[7] = 0;

    pixels = new uint8_t[W * H * 4];

    for (register int i = 0; i < W * H * 4; i += 4)
    {

        pixels[i] = pallete[0][0];
        pixels[i + 1] = pallete[0][1];
        pixels[i + 2] = pallete[0][2];
        pixels[i + 3] = 255;
    }
    // window = new sf::RenderWindow //x3 scalling , 160x128 , 5128 bytes req

    stexture.create(W, H);
    // sf::Sprite sscreen(stexture);
    sscreen.setTexture(stexture);
    sscreen.setScale(scaling, scaling);
    stexture.update(pixels);

    stexture.setSmooth(smooth);

    window.setFramerateLimit(60);
    lua_State *state = luaL_newstate();
    luaL_openlibs(state);
    lua_getglobal(state, "_G");
    // luaL_setfuncs(state, printlib, 0);

    // sandbox lua //
    lua_pushnil(state);
    lua_setglobal(state, "io");
    lua_pushnil(state);
    lua_setglobal(state, "os");
    lua_pushnil(state);
    lua_setglobal(state, "dofile");
    lua_pushnil(state);
    lua_setglobal(state, "getfenv");
    lua_pushnil(state);
    lua_setglobal(state, "load");
    lua_pushnil(state);
    lua_setglobal(state, "loadfile");
    lua_pushnil(state);
    lua_setglobal(state, "loadstring");
    lua_pushnil(state);
    lua_setglobal(state, "rawget");
    lua_pushnil(state);
    lua_setglobal(state, "rawset");
    lua_pushnil(state);
    lua_setglobal(state, "coroutine");
    lua_pushnil(state);
    lua_setglobal(state, "module");
    lua_pushnil(state);
    lua_setglobal(state, "package");
    lua_pushnil(state);
    lua_setglobal(state, "require");
    lua_pushnil(state);
    lua_setglobal(state, "debug");
    lua_pushnil(state);
    lua_setglobal(state, "getmetatable");
    lua_pushnil(state);
    lua_setglobal(state, "setmetatable");

    lua_pushnil(state);
    lua_setglobal(state, "print"); // replaced by log
    // ----------- //

    lua_pop(state, 1);

    lua_register(state, "log", l_log);
    lua_register(state, "time", l_time);

    lua_register(state, "setmapx", l_setmapx);
    lua_register(state, "setmapy", l_setmapy);
    lua_register(state, "getmapx", l_getmapx);
    lua_register(state, "getmapy", l_getmapy);
    lua_register(state, "setmapd", l_setmapd);
    lua_register(state, "getmapd", l_getmapd);
    lua_register(state, "setmapi", l_setmapi);
    lua_register(state, "getmapi", l_getmapi);
    lua_register(state, "setmap", l_setmap);
    lua_register(state, "getmap", l_getmap);

    lua_register(state, "sprite", l_sprite);

    lua_register(state, "key", l_key);

    lua_register(state, "halt", l_halt);
    lua_register(state, "exit", l_exit);

    int result;

    if (!skipstartanim)
    {
        bool animfinished = false;
        int8_t ylogopos = -32;
        while (window.isOpen())
        {
            sf::Event event;
            while (window.pollEvent(event))
            {
                if (event.type == sf::Event::Closed)
                    window.close();
            }
            framecounter++;
            // if(framecounter%2==1) {
            if (framecounter < (56 + 32))
                ylogopos++;
            if (framecounter > 160)
            {
                animfinished = 1;
                break;
            }
            clearscr();
            drawtext("PityBoy", 45, ylogopos, 3, true);
            // }
            stexture.update(pixels);
            window.draw(sscreen);
            window.display();
        }
        if (!animfinished)
            return 0;
    }

    result = luaL_loadfile(state, string(projectpath + "code.lua").c_str());

    if (result != LUA_OK)
    {
        print_error(state);
        return -1;
    }

    result = lua_pcall(state, 0, LUA_MULTRET, 0);

    if (result != LUA_OK)
    {
        print_error(state);
        return -1;
    }

    timeElapsed.restart();

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::KeyPressed && window.hasFocus())
            { // Key handler (ctrl+key)
                // Special keys
                if (event.key.control)
                {
                    if (event.key.code == sf::Keyboard::Add)
                    { // Resize +
                        scaling = scaling + 1;
                        window.setSize(sf::Vector2u(W * scaling, H * scaling));
                    }
                    if (event.key.code == sf::Keyboard::Subtract)
                    { // Resize -
                        if (scaling != 1)
                        {
                            scaling = scaling - 1;
                            window.setSize(sf::Vector2u(W * scaling, H * scaling));
                        }
                    }

                    if (event.key.code == sf::Keyboard::S)
                    {
                        smooth = !smooth;
                        stexture.setSmooth(smooth);
                    }
                }
                else
                {
                    // Key handler normal

                    if (event.key.code == sf::Keyboard::X)
                    { // A press
                        controller[0] = 1;
                    }
                    if (event.key.code == sf::Keyboard::Z)
                    { // B press
                        controller[1] = 1;
                    }
                    if (event.key.code == sf::Keyboard::Up)
                    { // UP press
                        controller[2] = 1;
                    }
                    if (event.key.code == sf::Keyboard::Down)
                    { // DOWN press
                        controller[3] = 1;
                    }
                    if (event.key.code == sf::Keyboard::Left)
                    { // LEFT press
                        controller[4] = 1;
                    }
                    if (event.key.code == sf::Keyboard::Right)
                    { // RIGHT press
                        controller[5] = 1;
                    }
                    if (event.key.code == sf::Keyboard::Q)
                    { // SELECT press
                        controller[6] = 1;
                    }
                    if (event.key.code == sf::Keyboard::E)
                    { // START press
                        controller[7] = 1;
                    }
                }
            }

            if (event.type == sf::Event::KeyReleased && window.hasFocus())
            {
                if (event.key.control)
                {
                }
                else
                {
                    if (event.key.code == sf::Keyboard::X)
                    { // A press
                        controller[0] = 0;
                    }
                    if (event.key.code == sf::Keyboard::Z)
                    { // B press
                        controller[1] = 0;
                    }
                    if (event.key.code == sf::Keyboard::Up)
                    { // UP press
                        controller[2] = 0;
                    }
                    if (event.key.code == sf::Keyboard::Down)
                    { // DOWN press
                        controller[3] = 0;
                    }
                    if (event.key.code == sf::Keyboard::Left)
                    { // LEFT press
                        controller[4] = 0;
                    }
                    if (event.key.code == sf::Keyboard::Right)
                    { // RIGHT press
                        controller[5] = 0;
                    }
                    if (event.key.code == sf::Keyboard::Q)
                    { // SELECT press
                        controller[6] = 0;
                    }
                    if (event.key.code == sf::Keyboard::E)
                    { // START press
                        controller[7] = 0;
                    }
                }
            }
        }

        if (!halted)
        {

            lua_getglobal(state, "code");

            if (!lua_isfunction(state, -1))
            {
                errorexit(true, "code() not found");
            }

            clearscr();

            int vx = 0;
            int vy = -mapy / 8;
            if (vy < 0)
                vy = 0;
            if (vy > 255)
                vy = 255;
            int b = 0;
            int i = 0;

            if (mapdraw)
            {
                while (1)
                {
                    while (1)
                    {
                        b = 0;
                        i = 0;
                        while (1)
                        {
                            while (1)
                            {
                                int bx = mapx + (vx * 8) + i;
                                int by = mapy + (vy * 8) + b;
                                if (bx < 160 && bx >= 0 && by >= 0 && by < 128)
                                {
                                    pix(bx, by, project.sprites[i][b][project.maps[vx][vy][mapindex]]);
                                }
                                i++;
                                if (i >= 8)
                                    break;
                            }
                            i = 0;
                            b++;
                            if (b >= 8)
                                break;
                        }
                        vx = vx + 1;
                        if (vx >= (-mapx / 8) + (-mapx / 8) - vx + 50)
                            break;
                        if (vx >= 255)
                            break;
                    }
                    vx = -mapx / 8;
                    if (vx < 0)
                        vx = 0;
                    if (vx > 255)
                        vx = 255;

                    vy = vy + 1;
                    if (vy >= (-mapy / 8) + (-mapy / 8) - vy + 40)
                        break;
                    if (vy >= 255)
                        break;
                }
            }

            result = lua_pcall(state, 0, 0, 0);
            if (result != LUA_OK)
            {
                print_error(state);
                return -1;
            }
        }

        stexture.update(pixels);
        window.draw(sscreen);
        window.display();
    }
    lua_close(state);

    return 0;
}
