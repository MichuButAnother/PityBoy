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

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include <math.h>

#include <fstream>

#include <boost/filesystem.hpp>

#include <vector>

#include "font.h"

#include "icons.h"

#include "projectparser.hpp"

using namespace std;
namespace fs = boost::filesystem;

const unsigned int W = 256;
const unsigned int H = 160;
int scaling = 3;

uint8_t *pixels;

bool projectselected = false;
string projectname = "";
string projectfolder = "";

const uint8_t pallete[4][3] = {
    {
      230,
      230,
      0
    },
    {
      204,
      170,
      0
    },
    {
      128,
      128,
      0
    },
    { 
      64,
      64,
      0
    }
};

sf::RenderWindow window(sf::VideoMode(W *scaling, H *scaling), "PityBoy Editor");
sf::Texture stexture;
sf::Sprite sscreen;
sf::Cursor cursor;

sf::Clock timeElapsed;

unsigned char ToByte(bool b[8])
{
  unsigned char c = 0;
  for (int i = 0; i < 8; ++i)
    if (b[i])
      c |= 1 << i;
  return c;
}

void FromByte(unsigned char c, bool b[8])
{
  for (int i = 0; i < 8; ++i)
    b[i] = (c & (1 << i)) != 0;
}

void pix(int x, int y, int color)
{
  if ((x + (y * 256)) > (W * H))
    return;
  pixels[((x + (y * 256)) * 4) + 0] = pallete[color][0];
  pixels[((x + (y * 256)) * 4) + 1] = pallete[color][1];
  pixels[((x + (y * 256)) * 4) + 2] = pallete[color][2];
}

int getpix(int x, int y)
{
  if ((x + (y * 256)) > (W * H))
    return 0;

  if (pallete[0][0] == pixels[((x + (y * 256)) * 4) + 0] && pallete[0][1] == pixels[((x + (y * 256)) * 4) + 1] && pallete[0][2] == pixels[((x + (y * 256)) * 4) + 2])
    return 0;
  if (pallete[1][0] == pixels[((x + (y * 256)) * 4) + 0] && pallete[1][1] == pixels[((x + (y * 256)) * 4) + 1] && pallete[1][2] == pixels[((x + (y * 256)) * 4) + 2])
    return 1;
  if (pallete[2][0] == pixels[((x + (y * 256)) * 4) + 0] && pallete[2][1] == pixels[((x + (y * 256)) * 4) + 1] && pallete[2][2] == pixels[((x + (y * 256)) * 4) + 2])
    return 2;
  if (pallete[3][0] == pixels[((x + (y * 256)) * 4) + 0] && pallete[3][1] == pixels[((x + (y * 256)) * 4) + 1] && pallete[3][2] == pixels[((x + (y * 256)) * 4) + 2])
    return 3;

  return 0;
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
  if (g == 0)
    return;
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

void drawicon(char icon[64], int dx, int dy, int transparency = 0)
{
  int x = 0;
  int y = 0;
  int i = 0;

  while (1)
  {
    while (1)
    {
      if (icon[i] != transparency)
        pix(dx + x, dy + y, icon[i]);
      x++;
      i++;
      if (x >= 8)
        break;
    }
    x = 0;
    y++;
    if (y >= 8)
      break;
  }
}

void errorexit(string message = "")
{
  clearscr();
  drawtext("PityBoy Editor runtime error!", 1, 1);
  drawtext(message, 1, 10);
  bool lbut = false;

  while (window.isOpen())
  {
    sf::Event event;
    while (window.pollEvent(event))
    {
      if (event.type == sf::Event::Closed)
        window.close();
    }

    sf::Vector2i position = sf::Mouse::getPosition(window);
    int mousex = floor(position.x / float(window.getSize().x) * 256);
    int mousey = floor(position.y / float(window.getSize().y) * 160);
    if (mousex > 255)
      mousex = 256;
    if (mousey > 159)
      mousey = 160;
    if (mousex < 0)
      mousex = 0;
    if (mousey < 0)
      mousey = 0;

    box(8, 130, 47, 144);
    box(9, 131, 46, 143, 0, true);
    if (mousex > 8 && mousey > 130 && mousex < 48 && mousey < 144)
      box(9, 131, 46, 143, 1, true);
    drawtext("Exit", 13, 133);

    if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
    {
      lbut = 1;
    }
    else
    {
      if (mousex > 8 && mousey > 130 && mousex < 48 && mousey < 143 && lbut)
        window.close();
      lbut = 0;
    }

    stexture.update(pixels);
    window.draw(sscreen);
    window.display();
  }
}

bool compareFunction(std::string a, std::string b)
{
  return a < b;
}

bool laststate = false;
int openedmenu = 1;
int framecounter = 0;

// options
bool skipstartanim = true;
bool smooth = false;

PityProject project;

int main()
{

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

  fs::path projectfolder("projects");
  if (!fs::is_directory(projectfolder))
  {
    if (!fs::create_directory("projects"))
    {
      errorexit("Cannot create projects directory!");
      return 0;
    }
  }

  if (!skipstartanim)
  {
    bool animfinished = false;
    int8_t ylogopos = -24;
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
      if (framecounter < (64 + 24))
        ylogopos++;
      if (framecounter > 160)
      {
        animfinished = 1;
        break;
      }
      clearscr();
      drawtext("PityBoy Editor", 57, ylogopos, 3, true);
      // }
      stexture.update(pixels);
      window.draw(sscreen);

      window.display();
    }
    if (!animfinished)
      return 0;
  }

  int mousex = 0;
  int mousey = 0;
  timeElapsed.restart();
  bool singlerun = false;
  int originbutton = 0;
  int dircount = 0;
  bool listselected = false;
  vector<string> dirlist;
  int listelement = 3;

  int barsize = 119;
  int barpos = 0;
  bool barhold = false;
  int baroffset = 0;
  float barover = 0;
  bool scrollactive = false;
  int scrollwhell = 0;

  string tprojectname;
  string tprojectcodename;

  string textboxstring = "";
  int textboxmaxlen = 0;
  bool intextbox = false;
  int stextbox = 0;
  int textboxspaces = 0; // 0 allow, 1 not allow, 2 replace with _
  bool allowupper = true;

  int pcreationerr = 0;
  bool pcreationtext = false;

  memset(project.sprites, 0, sizeof(char) * 8 * 8 * 512);
  memset(project.maps, 0, sizeof(char) * 256 * 256 * 4);

  bool spriteh = false;
  bool spritehh = false;
  int spritec = 3;
  int spritee = 0;
  bool spritepage = 0;
  int tooltip = 0;

  int mapindex = 0;
  int mapx = 0;
  int mapy = 0;
  bool mapdrag = false;
  int mapdragx = 0;
  int mapdragy = 0;
  int mapox = 0;
  int mapoy = 0;
  bool mapgrid = true;
  int maptool = 0;
  bool mapdraw = false;
  int mapspr = 1;
  bool mapsmenu = false;
  bool mapar = false;

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
        if (event.key.code == sf::Keyboard::Left)
          spritee--;
        if (event.key.code == sf::Keyboard::Right)
          spritee++;

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
          // event.key.code+97 - ascii character (a-z)
          // event.key.code (>26) +22
          // space 57
          // enter 58
          // backspace 59
          int key = event.key.code;
          if (intextbox)
          {
            if (textboxmaxlen > textboxstring.length())
            {
              if (key >= 0 && key <= 25)
              {
                if (!event.key.shift)
                {
                  textboxstring = textboxstring + char(key + 97);
                }
                else
                {
                  if (allowupper)
                    textboxstring = textboxstring + char(key + 65);
                }
              }
              if (key >= 26 && key <= 35)
              {
                textboxstring = textboxstring + char(key + 22);
              }
              if (textboxspaces == 0)
              {
                if (key == 57)
                  textboxstring = textboxstring + ' ';
              }
              else if (textboxspaces == 2)
              {
                if (key == 57)
                  textboxstring = textboxstring + '_';
              }
            }
            if (key == 59 && textboxstring.length() != 0)
              textboxstring = textboxstring.erase(textboxstring.size() - 1);
            if (key == 58)
            {
              intextbox = false;
              stextbox = 0;
            }
          }
        }
      }

      if (event.type == sf::Event::MouseWheelMoved) // Scroll
      {
        if (scrollactive && window.hasFocus())
        {
          scrollwhell = event.mouseWheel.delta;
        }
      }
    }
    if (window.hasFocus())
    {
      sf::Vector2i position = sf::Mouse::getPosition(window);
      mousex = floor(position.x / float(window.getSize().x) * 256);
      mousey = floor(position.y / float(window.getSize().y) * 160);
    }
    if (mousex > 255)
      mousex = 256;
    if (mousey > 159)
      mousey = 160;
    if (mousex < 0)
      mousex = 0;
    if (mousey < 0)
      mousey = 0;

    if (openedmenu == 1)
    {
      if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && window.hasFocus())
      {

        if (!laststate)
        {
          if (mousex > 70 && mousex < 186 && mousey > 20 && mousey < 31)
            originbutton = 1;
          if (mousex > 70 && mousex < 186 && mousey > 40 && mousey < 51)
            originbutton = 1;
          if (mousex > 70 && mousex < 186 && mousey > 60 && mousey < 71)
            originbutton = 1;
        }
        laststate = true;
      }
      else
      {
        if (laststate)
        {
          if (mousex > 70 && mousex < 186 && mousey > 20 && mousey < 31 && originbutton == 1)
          {
            openedmenu = 2;
            singlerun = true;
            listselected = false;
          }
          if (mousex > 70 && mousex < 186 && mousey > 40 && mousey < 51 && originbutton == 1 && projectselected)
          {
            openedmenu = 3;
            project.load_sprites();
            singlerun = true;
          }
          if (mousex > 70 && mousex < 186 && mousey > 60 && mousey < 71 && originbutton == 1 && projectselected)
          {
            openedmenu = 4;
            singlerun = true;
            mapsmenu = false;
            project.load_maps();
          }

          laststate = false;
        }
      }

      clearscr();
      box(0, 150, 255, 159, 3, true);
      drawtext("PityBoy Editor ", 1, 151, 1);

      box(0, 0, 255, 9, 3, true);
      if (projectselected)
      {
        drawtext("Project: " + projectname, 1, 1, 1);
      }
      else
      {
        drawtext("No project selected.", 1, 1, 1);
      }

      if (mousex > 70 && mousex < 186 && mousey > 20 && mousey < 31)
        box(70, 20, 186, 31, 1, true);
      box(70, 20, 186, 31);
      drawtext("Select project", 74, 22);
      int color = 0;
      if (projectselected)
      {
        color = 3;
      }
      else
      {
        color = 1;
      }
      if (mousex > 70 && mousex < 186 && mousey > 40 && mousey < 51 && projectselected)
        box(70, 40, 186, 51, 1, true);
      box(70, 40, 186, 51, color);
      drawtext("Sprite editor", 77, 42, color);
      if (mousex > 70 && mousex < 186 && mousey > 60 && mousey < 71 && projectselected)
        box(70, 60, 186, 71, 1, true);
      box(70, 60, 186, 71, color);
      drawtext("  Map editor", 74, 62, color);
    }

    if (openedmenu == 2)
    {
      //--------------------------------------//
      //---------- project select menu -------//
      //--------------------------------------//
      if (singlerun)
      {

        fs::directory_iterator end_itr;
        dirlist.clear();
        // cycle through the directory
        for (fs::directory_iterator itr(projectfolder); itr != end_itr; ++itr)
        {
          if (fs::is_directory(itr->path()))
          {
            // assign current file name to current_file and echo it out to the console.
            string current_file = itr->path().string();
            dircount = dircount + 1;
            dirlist.push_back(current_file);
          }
        }
        sort(dirlist.begin(), dirlist.end(), compareFunction);
        singlerun = false;
        originbutton = 0;
        laststate = false;
        barpos = 0;
      }

      if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && window.hasFocus())
      {

        if (!laststate)
        {
          if (mousex >= 246 && mousex <= 255 && mousey >= 150 && mousey <= 159)
            originbutton = 1;
          if (mousex >= 120 && mousex <= 230 && mousey >= 130 && mousey <= 141)
            originbutton = 1;
          if (mousex >= 14 && mousex <= 232 && mousey >= 11 && mousey <= 126)
            originbutton = 1;
          if (mousex >= 2 && mousex <= 112 && mousey >= 130 && mousey <= 141 && (listselected || projectselected))
            originbutton = 1;
        }
        laststate = true;
      }
      else
      {
        if (laststate)
        {
          if (mousex >= 246 && mousex <= 255 && mousey >= 150 && mousey <= 159 && originbutton == 1)
          {
            openedmenu = 1; // to main menu
          }
          if (mousex >= 120 && mousex <= 230 && mousey >= 130 && mousey <= 141 && originbutton == 1 && !projectselected)
          {
            openedmenu = 200; // to create new project menu
            tprojectcodename = "";
            tprojectname = "";
            pcreationtext = false;
            singlerun = true;
          }

          if (mousex >= 2 && mousex <= 112 && mousey >= 130 && mousey <= 141 && (listselected || projectselected) && originbutton == 1)
          {

            if (!projectselected)
            {
              project.folder = dirlist.at(listelement) + "/";
              int ret = project.is_good();
              if (ret == 0)
              {
                openedmenu = 201;
                pcreationerr = 3;
                singlerun = true;
                listselected = false;
                originbutton = 0;
              }
              else if (ret == 2)
              {
                openedmenu = 201;
                pcreationerr = 4;
                singlerun = true;
                listselected = false;
                originbutton = 0;
              }
              else
              {
                projectselected = true;
                projectname = project.get_codename();
                project.load_sprites();
              }
            }
            else
            {
              projectselected = false;
              projectname = "";
            }
          }

          if (mousex >= 14 && mousex <= 232 && mousey >= 11 && mousey <= 126 && originbutton == 1)
          {

            int yelem = -(11 - (barpos * (2 + barover)) - mousey);

            int i = 0;
            while (1)
            {
              if (yelem > (i * 10) - 3 && yelem < (i * 10) + 9)
              {
                listselected = true;
                break;
              }
              if (i > dirlist.size())
              {
                break;
              }
              i++;
            }

            if (i > dirlist.size())
            {
              listselected = false;
            }

            listelement = i;
          }

          laststate = false;
        }
      }

      clearscr();

      box(0, 150, 255, 159, 3, true);
      box(246, 150, 255, 159, 1, true);

      if (dircount == 0)
      { // 12 per page
        drawtext("No projects.", 16, 16);
      }
      else
      {
        barsize = 116; // barsize is 116 max
        barover = 0;
        int b = 0;
        barover = dirlist.size() / 15;
        for (int i = 0; i < dirlist.size(); i++)
        {
          if (dirlist.at(i).length() < 30)
          {
            b = b + 1;
            if (b > 12)
            {
              if (barsize > 12)
              {
                barsize = barsize - 5;
              }
              if (b == 13)
                barsize = barsize - 1;
            }
            if (i == listelement && listselected)
            {
              box(14, 11 + (b * 10) - (barpos * (2 + barover)) - 11, 232, 11 + (b * 10) - (barpos * (2 + barover)) - 2, 1, true);
            }
            drawtext(dirlist.at(i), 16, 11 + (b * 10) - (barpos * (2 + barover)) - 10);
          }
        }
      }
      line(0, 10, 255, 10, 0);
      line(0, 127, 255, 127, 0);
      if (mousex >= 246 && mousex <= 255 && mousey >= 150 && mousey <= 159)
        box(246, 150, 255, 159, 2, true);
      drawtext("<", 248, 151);
      drawtext("Select project ", 1, 151, 1);
      box(0, 0, 255, 9, 3, true);

      // projectselected=true;

      box(0, 128, 255, 149, 1, true);
      int color = 3;
      box(3, 131, 111, 140, 1, true);
      if (!projectselected)
      {
        if (mousex > 2 && mousex < 112 && mousey > 130 && mousey < 141 && listselected)
          box(2, 130, 112, 141, 0, true);
        if (listselected)
        {
          box(2, 130, 112, 141, color);
          drawtext("Open selected", 6, 132, color);
        }
        else
        {
          box(2, 130, 112, 141, 2);
          drawtext("Open selected", 6, 132, 2);
        }
      }
      else
      {
        if (mousex > 2 && mousex < 112 && mousey > 130 && mousey < 141)
          box(2, 130, 112, 141, 0, true);
        box(2, 130, 112, 141, color);
        drawtext("Close project", 6, 132, color);
      }

      color = 3;
      if (mousex > 120 && mousex < 230 && mousey > 130 && mousey < 141 && !projectselected)
        box(120, 130, 230, 141, 0, true);
      if (projectselected)
        color = 2;
      box(120, 130, 230, 141, color);
      drawtext("Create new", 135, 132, color);

      if (projectselected)
      {
        drawtext("Project: " + projectname, 1, 1, 1);
      }
      else
      {
        drawtext("No project selected.", 1, 1, 1);
      }

      // draw bar
      box(247, 10, 255, 127, 3);
      box(248, 11, 254, 126, 0, true);

      box(248, 11 + barpos, 254, 10 + barpos + barsize, 1, true);

      // BAR DRAG
      if (mousex > 248 && mousex < 254 && mousey > 11 + barpos && mousey < 10 + barpos + barsize && sf::Mouse::isButtonPressed(sf::Mouse::Left) && !barhold)
      {
        barhold = true;
        baroffset = barpos - mousey;
      }
      if (barhold && !sf::Mouse::isButtonPressed(sf::Mouse::Left))
        barhold = false;
      if (barhold)
      {
        barpos = baroffset + mousey;

        if (barpos < 0 || mousey <= 11)
        {
          barpos = 0;
          baroffset = barpos - mousey;
        }
        if (barpos + barsize > 116 || mousey >= 126)
        {
          barpos = 116 - barsize;
          baroffset = barpos - mousey;
        }
      }
      // mouse wheel parse
      scrollactive = false;
      if (mousex > 9 && mousey < 128 && !barhold)
        scrollactive = true;
      if (scrollwhell > 0 || scrollwhell < 0)
      {
        barpos = barpos - scrollwhell * 3;
        if (barpos < 0)
          barpos = 0;
        if (barpos + barsize > 116)
          barpos = 116 - barsize;
        scrollwhell = 0;
      }
    }
    if (openedmenu == 200)
    {
      //--------------------------------------//
      //------- new project menu -------------//
      //--------------------------------------//

      if (singlerun)
      {
        tprojectcodename = "";
        tprojectname = "";
        originbutton = 0;
        laststate = false;
        singlerun = false;
      }

      if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && window.hasFocus())
      {

        if (!laststate)
        {
          if (mousex >= 2 && mousex <= 254 && mousey >= 46 && mousey <= 58)
            originbutton = 1;
          if (mousex >= 2 && mousex <= 254 && mousey >= 72 && mousey <= 84)
            originbutton = 1;
          if (mousex >= 246 && mousex <= 255 && mousey >= 150 && mousey <= 159)
            originbutton = 1;
          if (mousex >= 100 && mousex <= 154 && mousey >= 100 && mousey <= 114)
            originbutton = 1;
          if (mousex >= 2 && mousex <= 12 && mousey >= 86 && mousey <= 96)
            originbutton = 1;

          if (!(mousex >= 2 && mousex <= 254 && mousey >= 46 && mousey <= 58) && stextbox == 1)
            intextbox = false;
          if (!(mousex >= 2 && mousex <= 254 && mousey >= 72 && mousey <= 84) && stextbox == 2)
            intextbox = false;
        }
        laststate = true;
      }
      else
      {
        if (laststate)
        {
          if (mousex >= 2 && mousex <= 254 && mousey >= 46 && mousey <= 58 && originbutton == 1)
          {
            intextbox = true;
            stextbox = 1;
            textboxmaxlen = 30;
            textboxstring = tprojectname;
          }
          if (mousex >= 2 && mousex <= 254 && mousey >= 72 && mousey <= 84 && originbutton == 1)
          {
            intextbox = true;
            stextbox = 2;
            textboxmaxlen = 16;
            textboxstring = tprojectcodename;
          }
          if (mousex >= 246 && mousex <= 255 && mousey >= 150 && mousey <= 159 && originbutton == 1)
          {
            openedmenu = 2;
            singlerun = true;
            listselected = false;
          }
          if (mousex >= 2 && mousex <= 12 && mousey >= 86 && mousey <= 96 && originbutton == 1)
            pcreationtext = !pcreationtext;

          if (mousex >= 100 && mousex <= 154 && mousey >= 100 && mousey <= 114 && originbutton == 1)
          {
            openedmenu = 2;
            int ret = project.create_project(tprojectname, tprojectcodename);
            if (tprojectcodename == "" || tprojectname == "")
            {
              openedmenu = 201;
              pcreationerr = 2;
            }
            else if (ret != 1)
            {
              openedmenu = 201;
              if (ret == 2)
              {
                pcreationerr = 8;
              }
              else
              {
                pcreationerr = 1;
              }
            }
            else
            {
              project.folder = "projects/" + tprojectcodename + "/";
              memset(project.sprites, 0, sizeof(char) * 8 * 8 * 512);
              memset(project.maps, 0, sizeof(char) * 256 * 256 * 4);
              if (pcreationtext)
              {
                bool u[8];
                int b = 0;
                int c = 0;
                while (1)
                {
                  b = 0;
                  while (1)
                  {

                    for (int i = 0; i < 8; ++i)
                      u[i] = (font8[c][b] & (1 << i)) != 0;

                    project.sprites[0][b][127 + c] = u[0] * 3;
                    project.sprites[1][b][127 + c] = u[1] * 3;
                    project.sprites[2][b][127 + c] = u[2] * 3;
                    project.sprites[3][b][127 + c] = u[3] * 3;
                    project.sprites[4][b][127 + c] = u[4] * 3;
                    project.sprites[5][b][127 + c] = u[5] * 3;
                    project.sprites[6][b][127 + c] = u[6] * 3;
                    project.sprites[7][b][127 + c] = u[7] * 3;

                    b++;
                    if (b > 7)
                      break;
                  }
                  c++;
                  if (c > 127)
                    break;
                }
              }
              project.save_maps();
              project.save_sprites();
            }

            singlerun = true;
            listselected = false;
            originbutton = 0;
          }

          laststate = false;
        }
      }

      clearscr();
      box(0, 0, 255, 9, 3, true);
      box(0, 150, 255, 159, 3, true);
      box(246, 150, 255, 159, 1, true);
      if (mousex >= 246 && mousex <= 255 && mousey >= 150 && mousey <= 159)
        box(246, 150, 255, 159, 2, true);
      drawtext("<", 248, 151);

      if (mousex >= 2 && mousex <= 12 && mousey >= 86 && mousey <= 96)
        box(2, 86, 12, 96, 2, true);
      box(2, 86, 12, 96);
      if (pcreationtext)
        drawchar('X', 4, 88);
      drawtext("Put default font into tiles", 16, 88);

      drawtext("Create new project ", 1, 151, 1);

      drawtext("Name:", 2, 37, 3);
      drawtext("Folder name:", 2, 64, 3);

      box(2, 46, 254, 58);
      if (stextbox == 1 && framecounter > 30)
      {
        drawtext(tprojectname + "_", 4, 49);
      }
      else
      {
        drawtext(tprojectname, 4, 49);
      }

      box(2, 72, 254, 84);
      if (stextbox == 2 && framecounter > 30)
      {
        drawtext(tprojectcodename + "_", 4, 75);
      }
      else
      {
        drawtext(tprojectcodename, 4, 75);
      }

      if (!intextbox)
        stextbox = 0;
      if (stextbox == 1)
      {
        textboxspaces = 0;
        allowupper = true;
        tprojectname = textboxstring;
      }
      if (stextbox == 2)
      {
        textboxspaces = 2;
        allowupper = false;
        tprojectcodename = textboxstring;
      }

      if (mousex >= 100 && mousex <= 154 && mousey >= 100 && mousey <= 114)
        box(100, 100, 154, 114, 1, true);
      box(100, 100, 154, 114);
      drawtext("Create", 104, 104);

      framecounter++;
      if (framecounter > 60)
        framecounter = 0;
    }

    if (openedmenu == 201)
    {
      // submenu for create new project //

      if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && window.hasFocus())
      {

        if (!laststate)
        {
          if (mousex >= 246 && mousex <= 255 && mousey >= 150 && mousey <= 159)
            originbutton = 1;
        }
        laststate = true;
      }
      else
      {
        if (laststate)
        {

          if (mousex >= 246 && mousex <= 255 && mousey >= 150 && mousey <= 159 && originbutton == 1)
          {
            if (pcreationerr == 1 || pcreationerr == 2 || pcreationerr == 8)
            {
              openedmenu = 200;
            }
            if (pcreationerr == 3 || pcreationerr == 4)
            {
              openedmenu = 2;
            }
            singlerun = true;
            listselected = false;
          }

          laststate = false;
        }
      }

      clearscr();
      box(0, 0, 255, 9, 3, true);
      box(0, 150, 255, 159, 3, true);
      box(246, 150, 255, 159, 1, true);
      if (mousex >= 246 && mousex <= 255 && mousey >= 150 && mousey <= 159)
        box(246, 150, 255, 159, 2, true);
      drawtext("<", 248, 151);

      if (pcreationerr == 1)
      {
        drawtext("Could not create project!", 28, 64);
      }
      else if (pcreationerr == 2)
      {
        drawtext("Name or Folder name\n  cannot be empty!", 52, 64);
      }
      else if (pcreationerr == 3)
      {
        drawtext("Invalid project folder!", 36, 64);
      }
      else if (pcreationerr == 4)
      {
        drawtext("Project is saved in\n   newer version!", 52, 64);
      }
      else if (pcreationerr == 8)
      {
        drawtext("Project already exist!", 40, 64);
      }
    }

    if (openedmenu == 3)
    {
      //--------------------------------------//
      //---------- sprite editor -------------//
      //--------------------------------------//

      if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && window.hasFocus())
      {

        if (!laststate)
        {
          if (mousex >= 246 && mousex <= 255 && mousey >= 150 && mousey <= 159)
            originbutton = 1;

          if (mousex >= 27 && mousex <= 35 && mousey >= 96 && mousey <= 104)
            originbutton = 1;
          if (mousex >= 38 && mousex <= 46 && mousey >= 96 && mousey <= 104)
            originbutton = 1;
          if (mousex >= 49 && mousex <= 57 && mousey >= 96 && mousey <= 104)
            originbutton = 1;
          if (mousex >= 60 && mousex <= 68 && mousey >= 96 && mousey <= 104)
            originbutton = 1;
          if (mousex >= 235 && mousex <= 244 && mousey >= 150 && mousey <= 159)
            originbutton = 1;

          if (mousex > 15 && mousex < 80 && mousey > 16 && mousey < 81)
            spriteh = true;
          if (mousex > 110 && mousex < 239 && mousey > 10 && mousey < 239)
            spritehh = true;
        }
        laststate = true;

        if (mousex > 15 && mousex < 80 && mousey > 16 && mousey < 81 && spriteh)
        { // drawing
          int fx = (mousex - 17) / 8;
          int fy = (mousey - 18) / 8;
          project.sprites[fx][fy][spritee] = spritec;
        }

        if (mousex > 110 && mousex < 239 && mousey > 10 && mousey < 239 && spritehh)
        {
          int fx = (mousex - 111) / 8;
          int fy = (mousey - 11) / 8;
          int fd = (fx + (fy * 16));
          if (fd <= 255 && fd >= 0)
          {
            if (spritepage)
            {
              spritee = fd + 256;
            }
            else
            {
              spritee = fd;
            }
          }
        }
      }
      else
      {
        spriteh = false;
        spritehh = false;
        if (laststate)
        {

          if (mousex >= 246 && mousex <= 255 && mousey >= 150 && mousey <= 159 && originbutton == 1)
          {
            openedmenu = 1;
            singlerun = true;
            listselected = false;
            project.save_sprites();
          }

          if (mousex >= 27 && mousex <= 35 && mousey >= 96 && mousey <= 104 && originbutton == 1)
            spritec = 0;
          if (mousex >= 38 && mousex <= 46 && mousey >= 96 && mousey <= 104 && originbutton == 1)
            spritec = 1;
          if (mousex >= 49 && mousex <= 57 && mousey >= 96 && mousey <= 104 && originbutton == 1)
            spritec = 2;
          if (mousex >= 60 && mousex <= 68 && mousey >= 96 && mousey <= 104 && originbutton == 1)
            spritec = 3;

          if (mousex >= 235 && mousex <= 244 && mousey >= 150 && mousey <= 159 && originbutton == 1)
          {
            spritepage = !spritepage;
            if (spritepage)
            {
              spritee = spritee + 256;
            }
            else
            {
              spritee = spritee - 256;
            }
          }

          laststate = false;
        }
      }
      tooltip = 0;
      clearscr();

      box(0, 150, 255, 159, 3, true);
      box(246, 150, 255, 159, 1, true);
      box(235, 150, 244, 159, 1, true);
      if (mousex >= 246 && mousex <= 255 && mousey >= 150 && mousey <= 159)
      {
        tooltip = 1;
        box(246, 150, 255, 159, 2, true);
      }
      if (mousex >= 235 && mousex <= 244 && mousey >= 150 && mousey <= 159)
      {
        tooltip = 2;
        box(235, 150, 244, 159, 2, true);
      }
      drawtext("<", 248, 151);

      if (spritepage)
      {
        drawtext("2", 237, 151);
      }
      else
      {
        drawtext("1", 237, 151);
      }

      if (tooltip == 1)
      {
        drawtext("Exit and save", 1, 151, 1);
      }
      else if (tooltip == 2)
      {
        drawtext("Switch page (0-255/256-511)", 1, 151, 1);
      }
      else
      {
        drawtext("Sprite editor", 1, 151, 1);
      }
      line(96, 0, 96, 245, 3);
      line(95, 0, 95, 245, 3);
      box(15, 16, 80, 81);

      box(26, 83, 69, 93, spritec, true);
      box(26, 83, 69, 93);

      box(26, 95, 36, 105);
      box(27, 96, 35, 104, 0, true);
      box(37, 95, 47, 105);
      box(38, 96, 46, 104, 1, true);
      box(48, 95, 58, 105);
      box(49, 96, 57, 104, 2, true);
      box(59, 95, 69, 105);
      box(60, 96, 68, 104, 3, true);

      if (spritee > 9)
      {
        if (spritee > 99)
        {
          drawtext(to_string(spritee) + "#", 33, 4);
        }
        else
        {
          drawtext("0" + to_string(spritee) + "#", 33, 4);
        }
      }
      else
      {
        drawtext("00" + to_string(spritee) + "#", 33, 4);
      }

      int i = 0;
      int b = 0;
      while (1)
      {
        while (1)
        {
          box(16 + (i * 8), 17 + (b * 8), 23 + (i * 8), 24 + (b * 8), project.sprites[i][b][spritee], true);
          i++;
          if (i >= 8)
            break;
        }
        i = 0;
        b++;
        if (b >= 8)
          break;
      }

      box(96, 0, 255, 149, 3, true);
      box(110, 10, 239, 139, 0);

      i = 0;
      b = 0;
      int d = 0;
      if (spritepage)
        d = 256;
      int vx = 0;
      int vy = 0;
      int bxx = 0;
      int bxy = 0;
      while (1)
      {

        while (1)
        {
          i = 0;
          b = 0;
          if (spritepage)
          {
            if (spritee == d)
            {
              bxx = vx;
              bxy = vy;
            }
          }
          else
          {
            if (spritee == d)
            {
              bxx = vx;
              bxy = vy;
            }
          }
          while (1)
          {
            while (1)
            {
              pix((vx * 8) + 111 + i, 11 + (vy * 8) + b, project.sprites[i][b][d]);
              i++;
              if (i >= 8)
                break;
            }
            i = 0;
            b++;
            if (b >= 8)
              break;
          }

          vx++;
          d++;
          if (vx >= 16)
            break;
        }
        vx = 0;
        vy++;

        if (vy >= 16)
          break;
      }

      box((bxx * 8) + 110, (bxy * 8) + 10, (bxx * 8) + 119, (bxy * 8) + 19);
    }

    if (openedmenu == 4)
    {

      if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && window.hasFocus())
      {

        if (!laststate)
        {

          if (!mapsmenu)
          {
            if (mousex >= 246 && mousex <= 255 && mousey >= 150 && mousey <= 159)
              originbutton = 1;
            if (mousex >= 234 && mousex <= 244 && mousey >= 150 && mousey <= 159)
              originbutton = 1;
            if (mousex >= 222 && mousex <= 232 && mousey >= 150 && mousey <= 159)
              originbutton = 1;
            if (mousex >= 210 && mousex <= 220 && mousey >= 150 && mousey <= 159)
              originbutton = 1;
            if (mousex >= 198 && mousex <= 208 && mousey >= 150 && mousey <= 159)
              originbutton = 1;

            if (mousex >= 148 && mousex <= 158 && mousey >= 150 && mousey <= 159)
              originbutton = 1;
            if (mousex >= 160 && mousex <= 170 && mousey >= 150 && mousey <= 159)
              originbutton = 1;
            if (mousex >= 172 && mousex <= 182 && mousey >= 150 && mousey <= 159)
              originbutton = 1;
            if (mousex >= 184 && mousex <= 194 && mousey >= 150 && mousey <= 159)
              originbutton = 1;
          }

          if (mousex >= 1 && mousex <= 254 && mousey >= 1 && mousey <= 148 && maptool == 0 && !mapsmenu)
          {
            mapdrag = true;
            mapdragx = mousex;
            mapdragy = mousey;
            mapox = mapx;
            mapoy = mapy;
          }
          if (mousex >= 1 && mousex <= 254 && mousey >= 1 && mousey <= 148 && maptool == 1 && !mapsmenu)
          {
            mapdraw = true;
          }
          if (!(mousex >= 109 && mousex <= 241 && mousey >= 9 && mousey <= 140) && mapsmenu)
          {
            mapsmenu = false;
            originbutton = 0;
          }
        }
        laststate = true;

        if (mousex > 110 && mousex < 239 && mousey > 10 && mousey < 140 && mapsmenu)
        {
          int fx = (mousex - 111) / 8;
          int fy = (mousey - 11) / 8;
          int fd = (fx + (fy * 16));
          if (fd <= 255 && fd >= 0)
          {
            mapspr = fd;
          }
        }

        if (mapdrag)
        {
          mapx = mapox - (mapdragx - mousex);
          mapy = mapoy - (mapdragy - mousey);
          if (-mapx < -256)
            mapx = 256;
          if (-mapx > 2048)
            mapx = -2048;
          if (-mapy < -256)
            mapy = 256;
          if (-mapy > 2048)
            mapy = -2048;
        }
        if (mapdraw)
        {
          int cx = (-mapx - 2 + mousex) / 8;
          int cy = (-mapy - 1 + mousey) / 8;
          if (cx >= 0 && cx <= 255 && cy >= 0 && cy <= 255)
            project.maps[cx][cy][mapindex] = unsigned(mapspr);
        }
      }
      else
      {
        mapdrag = false;
        mapdraw = false;
        if (laststate)
        {
          if (mousex >= 234 && mousex <= 244 && mousey >= 150 && mousey <= 159 && originbutton == 1)
            maptool = 0;
          if (mousex >= 222 && mousex <= 232 && mousey >= 150 && mousey <= 159 && originbutton == 1)
            maptool = 1;
          if (mousex >= 210 && mousex <= 220 && mousey >= 150 && mousey <= 159 && originbutton == 1)
            mapgrid = !mapgrid;
          if (mousex >= 198 && mousex <= 208 && mousey >= 150 && mousey <= 159 && originbutton == 1)
            mapsmenu = !mapsmenu;

          if (mousex >= 148 && mousex <= 158 && mousey >= 150 && mousey <= 159 && originbutton == 1)
            mapindex = 0;
          if (mousex >= 160 && mousex <= 170 && mousey >= 150 && mousey <= 159 && originbutton == 1)
            mapindex = 1;
          if (mousex >= 172 && mousex <= 182 && mousey >= 150 && mousey <= 159 && originbutton == 1)
            mapindex = 2;
          if (mousex >= 184 && mousex <= 194 && mousey >= 150 && mousey <= 159 && originbutton == 1)
            mapindex = 3;

          if (mousex >= 246 && mousex <= 255 && mousey >= 150 && mousey <= 159 && originbutton == 1)
          {
            openedmenu = 1;
            singlerun = true;
            listselected = false;
            project.save_maps();
          }

          laststate = false;
        }
      }

      tooltip = 0;
      clearscr();

      int vx = 0;
      int vy = -mapy / 8;
      if (vy < 0)
        vy = 0;
      if (vy > 255)
        vy = 255;
      int b = 0;
      int i = 0;

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
              int bx = mapx + 1 + (vx * 8) + i;
              int by = mapy + 1 + (vy * 8) + b;
              if (bx < 255 && bx > 0 && by > 0 && by < 149)
              {
                pix(bx, by, project.sprites[i][b][project.maps[vx][vy][mapindex]]);
                if (mapgrid && i == 0 && (vx % 20) == 0)
                {
                  if (getpix(bx, by) == 3)
                  {
                    pix(bx, by, 2);
                  }
                  else
                  {
                    pix(bx, by, 3);
                  }
                }
                if (mapgrid && b == 0 && vy % 16 == 0)
                {
                  if (getpix(bx, by) == 3)
                  {
                    pix(bx, by, 2);
                  }
                  else
                  {
                    pix(bx, by, 3);
                  }
                }
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
          if (vx >= (-mapx / 8) + (-mapx / 8) - vx + 70)
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
        if (vy >= (-mapy / 8) + (-mapy / 8) - vy + 50)
          break;
        if (vy >= 255)
          break;
      }

      box(0, 150, 255, 159, 3, true);
      box(246, 150, 255, 159, 1, true);
      if (maptool == 0)
      {
        box(234, 150, 244, 159, 2, true);
      }
      else
      {
        box(234, 150, 244, 159, 1, true);
      }
      if (maptool == 1)
      {
        box(222, 150, 232, 159, 2, true);
      }
      else
      {
        box(222, 150, 232, 159, 1, true);
      }
      if (mapgrid)
      {
        box(210, 150, 220, 159, 2, true);
      }
      else
      {
        box(210, 150, 220, 159, 1, true);
      }
      box(198, 150, 208, 159, 1, true);

      if (mapindex == 3)
      {
        box(184, 150, 194, 159, 2, true);
      }
      else
      {
        box(184, 150, 194, 159, 1, true);
      }
      if (mapindex == 2)
      {
        box(172, 150, 182, 159, 2, true);
      }
      else
      {
        box(172, 150, 182, 159, 1, true);
      }
      if (mapindex == 1)
      {
        box(160, 150, 170, 159, 2, true);
      }
      else
      {
        box(160, 150, 170, 159, 1, true);
      }
      if (mapindex == 0)
      {
        box(148, 150, 158, 159, 2, true);
      }
      else
      {
        box(148, 150, 158, 159, 1, true);
      }

      if (mousex >= 148 && mousex <= 158 && mousey >= 150 && mousey <= 159)
      {
        tooltip = 7;
        box(148, 150, 158, 159, 2, true);
      }
      if (mousex >= 160 && mousex <= 170 && mousey >= 150 && mousey <= 159)
      {
        tooltip = 7;
        box(160, 150, 170, 159, 2, true);
      }
      if (mousex >= 172 && mousex <= 182 && mousey >= 150 && mousey <= 159)
      {
        tooltip = 7;
        box(172, 150, 182, 159, 2, true);
      }
      if (mousex >= 184 && mousex <= 194 && mousey >= 150 && mousey <= 159)
      {
        tooltip = 7;
        box(184, 150, 194, 159, 2, true);
      }

      if (mousex >= 246 && mousex <= 255 && mousey >= 150 && mousey <= 159)
      {
        tooltip = 1;
        box(246, 150, 255, 159, 2, true);
      }
      if (mousex >= 234 && mousex <= 244 && mousey >= 150 && mousey <= 159)
      {
        tooltip = 5;
        box(234, 150, 244, 159, 2, true);
      }
      if (mousex >= 222 && mousex <= 232 && mousey >= 150 && mousey <= 159)
      {
        tooltip = 2;
        box(222, 150, 232, 159, 2, true);
      }
      if (mousex >= 210 && mousex <= 220 && mousey >= 150 && mousey <= 159)
      {
        tooltip = 3;
        box(210, 150, 220, 159, 2, true);
      }
      // if(mousex>=198&&mousex<=208&&mousey>=150&&mousey<=159) {
      // tooltip=4;
      // box(198,150,208,159,2,true);
      // }
      if (mousex >= 198 && mousex <= 208 && mousey >= 150 && mousey <= 159)
      {
        tooltip = 6;
        box(198, 150, 208, 159, 2, true);
      }

      drawtext("<", 248, 151);
      drawicon(drag_icon, 235, 151, 1);
      drawicon(pen_icon, 223, 151, 1);
      drawicon(grid_icon, 211, 151, 1);
      // drawicon(select_icon,187,151,1);//in future
      drawicon(sprites_icon, 199, 151, 1);
      drawchar('3', 186, 151);
      drawchar('2', 174, 151);
      drawchar('1', 162, 151);
      drawchar('0', 150, 151);

      int cx = (-mapx - 2 + mousex) / 8;
      int cy = (-mapy - 1 + mousey) / 8;
      if (cx < 0)
        cx = 0;
      if (cx > 255)
        cx = 255;
      if (cy < 0)
        cy = 0;
      if (cy > 255)
        cy = 255;
      string cp = to_string(project.maps[cx][cy][mapindex]);

      if (tooltip == 1)
      {
        drawtext("Exit and save", 1, 151, 1);
      }
      else if (tooltip == 2)
      {
        drawtext(cp + " Draw", 1, 151, 1);
      }
      else if (tooltip == 3)
      {
        drawtext("Grid", 1, 151, 1);
      }
      else if (tooltip == 4)
      {
        drawtext("Select/move", 1, 151, 1);
      }
      else if (tooltip == 5)
      {
        drawtext("Drag", 1, 151, 1);
      }
      else if (tooltip == 6)
      {
        drawtext("Sprites", 1, 151, 1);
      }
      else if (tooltip == 7)
      {
        drawtext("Edited map", 1, 151, 1);
      }
      else if (mapdraw || mapdrag)
      {
        drawtext(cp + " X:" + to_string(cx) + " Y:" + to_string(cy), 1, 151, 1);
      }
      else
      {

        drawtext(cp + " Map editor", 1, 151, 1);
      }

      if (mapsmenu)
      {
        box(109, 9, 240, 140, 3);
        box(110, 10, 239, 139, 0);
        i = 0;
        b = 0;
        int d = 0;
        vx = 0;
        vy = 0;
        int bxx = 0;
        int bxy = 0;
        while (1)
        {

          while (1)
          {
            i = 0;
            b = 0;
            if (mapspr == d)
            {
              bxx = vx;
              bxy = vy;
            }
            while (1)
            {
              while (1)
              {
                pix((vx * 8) + 111 + i, 11 + (vy * 8) + b, project.sprites[i][b][d]);
                i++;
                if (i >= 8)
                  break;
              }
              i = 0;
              b++;
              if (b >= 8)
                break;
            }

            vx++;
            d++;
            if (vx >= 16)
              break;
          }
          vx = 0;
          vy++;

          if (vy >= 16)
            break;
        }
        box((bxx * 8) + 110, (bxy * 8) + 10, (bxx * 8) + 119, (bxy * 8) + 19);
      }
    }

    stexture.update(pixels);
    window.draw(sscreen);

    window.display();
  }

  return 0;
}