/*
MIT License

Copyright (c) 2021 Piter

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
   
#include <fstream>
#include <boost/filesystem.hpp>
#include <iostream>
#include <string.h>

using namespace std;
namespace fs = boost::filesystem;

// PROJECT.INFO STRUCTURE (sorry for bad english)
/* HEX
   name   | offset | size | value? | type    | desc...
 header   | 0x00   | 2    |   PB   | char[2] | -
 version  | 0x02   | 2    | now 01 | uint16  | version
 codename | 0x04   | 16   |   -    | char[16]| codename (folder name by default)
 name     | 0x14   | 32   |   -    | char[32]| cart name
 owner id | 0x34   | 4    |   -    | uint16  | real owner (0) if project not downloaded (to do)
*/

// SPRITES.PIT STRUCTURE
/* HEX
   name   | offset | size | value? | type    | desc...
 header   | 0x00   | 4    |  PBSP  | char[4] | -
 version  | 0x04   | 2    | now 01 | uint16  | version
 sprites  | 0x06   | 32768|   -    | char[16]| sprites, from left up to right, saved in 8bpp (0-3, ik waste of space but its 2021) and later converted in player to 2bpp
*/
// MAP.PIT STRUCTURE
/* HEX
   name   | offset | size | value? | type    | desc...
 header   | 0x00   | 4    |  PBMP  | char[4] | -
 version  | 0x04   | 2    | now 01 | uint16  | version
 map data | 0x06   | 65536|   -    | char[16]| map, from left up to right
*/
class PityProject {

    public:

        /**
        Folder of project. Should end with /
        */
        string folder=""; // should end with '/'

        int project_info_pass=2;

        char sprites[8][8][512];

        unsigned char maps[256][256][4];

        /**
        Check is folder good
        @return 1 when its good
        @return 2 when saved in newer version
        @return 0 on fail
        */
        int is_good() {
        if(folder=="") return false;

        // check header and size
        ifstream ifile;
        ifile.open(folder+"project.info",ios::binary|ios::in);
        if(ifile.bad()) return 0; // project.info unreadable or not exist
        // size test //
        ifile.seekg (0,ifile.end);
        long fsize = ifile.tellg();
        ifile.seekg (0);

        if(fsize!=54) return false; // bad size

        // header test //
        char buffer[3];
        ifile.read(&buffer[0], 2);
        buffer[2]='\0';

        if(strcmp(buffer,"PB") != 0) return false; // wrong header
        ifile.seekg (2);
        ifile.read(&buffer[0], 2);
        buffer[2]='\0';

        if(buffer[1]>PityBoy_Version) return 2;

        // should be fine, close and return ture

        ifile.close();
        return true;
        }


        /**
        Creates new project
        @param Project name
        @param Project folder name
        @return 1 when project created with no errors
        @return 2 when project exists
        */
        int create_project(string name,string codename) {
        if(codename.length()>16) return false;
        if(name.length()>32) return false;


        if(!fs::create_directory("projects/"+codename)) {
        return 2;
        }


        ofstream ofile("projects/"+codename+"/project.info", std::ios::binary);
        if(ofile.bad()) return false;
        char result[55];
        memset(result,0,54);

        result[0]='P';
        result[1]='B';
        result[2]=0;
        result[3]=PityBoy_Version;
        result[53]=0;
        result[54]=0;

        int i=0;
        while(1) {
            if(i>(codename.length()-1)) {
            result[4+i]=0;
            } else {
            result[4+i]=codename[i];
            }
            i++;
            if(i>=16) break;
            }
        i=0;
        while(1) {
            if(i>(name.length()-1)) {
            result[20+i]=0;
            } else {
            result[20+i]=name[i];
            }
            i++;
            if(i>=31) break;
            }

         ofile.write(result,54);
         ofile.close();
         return true;
        }
        /**
        @return Project name
        */
        string get_name() {
        if(!is_good()) return "-";
        ifstream ifile;
        ifile.open(folder+"project.info",ios::binary|ios::in);
        if(ifile.bad()) return "-";
        ifile.seekg(20);
        char buffer[32];
        ifile.read(&buffer[0], 30);
        buffer[31]='\0';

        return string(buffer);

        }

        string get_codename() {
        if(!is_good()) return "-";
        ifstream ifile;
        ifile.open(folder+"project.info",ios::binary|ios::in);
        if(ifile.bad()) return "-";
        ifile.seekg(4);
        char buffer[17];
        ifile.read(&buffer[0], 16);
        buffer[16]='\0';

        return string(buffer);

        }


        /**
        @return True on save success
        */
        bool save_sprites() {
        ofstream ofile;
        ofile.open(folder+"sprites.pit",std::ios::binary|std::ofstream::trunc);
        if(ofile.bad()) return false;
        int ix=0;
        int iy=0;
        int i=0;
        char buf[8];
        buf[0]='P';
        buf[1]='B';
        buf[2]='S';
        buf[3]='P';
        buf[4]=0;
        buf[5]=PityBoy_Version;
        ofile.write(buf,6);

        while(1) {
            ix=0;
            iy=0;
            while(1) {
            while(1) {
            buf[0]=sprites[ix][iy][i];
            ofile.write(buf,1);
            ix++;
            if(ix>=8) break;
            }
            iy++;
            ix=0;
            if(iy>=8) break;
            }
            i++;


            if(i>511) break;
        }

        return true;
        }

        /**
        @return true on load success
        */
        bool load_sprites() {
        if(!is_good()) return false;
        if(!boost::filesystem::exists(folder+"sprites.pit")) {
        save_sprites();
        }
        ifstream ifile;
        ifile.open(folder+"sprites.pit",ios::binary|ios::in);
        if(ifile.bad()) return false;

        char buffer[5];
        ifile.read(&buffer[0], 4);
        buffer[4]='\0';

        if(strcmp(buffer,"PBSP") != 0) return false;

        ifile.seekg(6);
        int ix,iy;
        int i=0;
        char buf[3];

        while(1) {
            ix=0;
            iy=0;
            while(1) {
            while(1) {
            ifile.read(&buf[0], 1);
            sprites[ix][iy][i]=buf[0];
            ix++;
            if(ix>=8) break;
            }
            iy++;

            ix=0;
            if(iy>=8) break;
            }
            i++;


            if(i>511) break;
        }

        return true;

        }


        bool save_maps() {
        int g=0;

        while(1) {
        ofstream ofile;
        ofile.open(folder+"map"+to_string(g)+".pit",std::ios::binary|std::ofstream::trunc);
        if(ofile.bad()) return false;
        int ix=0;
        int iy=0;
        int i=0;
        char buf[8];
        buf[0]='P';
        buf[1]='B';
        buf[2]='M';
        buf[3]='P';
        buf[4]=0;
        buf[5]=PityBoy_Version;
        ofile.write(buf,6);

       // while(1) {
            ix=0;
            iy=0;
            while(1) {
            while(1) {
            buf[0]=maps[ix][iy][g];
            ofile.write(buf,1);
            ix++;
            if(ix>=256) break;
            }
            iy++;
            ix=0;
            if(iy>=256) break;
            }
           // i++;

           // if(i>65536) break;
           ofile.close();
           g++;
           if(g>3) break;
            }
        }

        bool load_maps() {
        int g=0;

        while(1) {
        if(!is_good()) return false;
        if(!boost::filesystem::exists(folder+"map"+to_string(g)+".pit")) {
        save_maps();
        }
        ifstream ifile;
        ifile.open(folder+"map"+to_string(g)+".pit",ios::binary|ios::in);
        if(ifile.bad()) return false;

        char buffer[5];
        ifile.read(&buffer[0], 4);
        buffer[4]='\0';

        if(strcmp(buffer,"PBMP") != 0) return false;

        ifile.seekg(6);
        int ix,iy;
        int i=0;
        char buf[3];

        //while(1) {
            ix=0;
            iy=0;
            while(1) {
            while(1) {
            ifile.read(&buf[0], 1);
            maps[ix][iy][g]=buf[0];
            ix++;
            if(ix>=256) break;
            }
            iy++;

            ix=0;
            if(iy>=256) break;
            }
           // i++;


           // if(i>511) break;
      //  }
        ifile.close();
        g++;
        if(g>3) break;
        }

        return true;
        }
};
