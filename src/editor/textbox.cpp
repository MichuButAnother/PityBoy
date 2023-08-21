#include "../headers/controls.hpp" 

#include <iostream>

/*
    Text box and built in PityBoy code editor

    Splited into another file because it's getting big

*/

namespace PityBoy::controls {

    textBox::textBox(int x, int y, int w, int h) { // width and height are here *8 !!
        this->x = x;
        this->y = y;
        this->w = w;
        this->h = h;

        this->realW = w*8+3;
        this->realH = h*9+2;
        text = "WWWWWWWWWWWWWWWWWFFFFFFFSSSSSSSSSSAAAAAAAAAAWWWWWWWWAAAAAWWWW\nThis is line 2\ntstsetst\nbadgdshaeysa\nabc\nabc\nabc\nabc\ncrap\nmore crap\nidk to write";
        convertTextIntoVector();

        cursorPosX = 0;
        cursorPosY = 0;

        // inSel = true;
        // selXstart=40;
        // selYstart=0;
        // selXend=2;
        // selYend=3;
        

        defaultCursor.loadFromSystem(sf::Cursor::Arrow);
        textCursor.loadFromSystem(sf::Cursor::Text);
        lastCursor = 0; // 0 arrow 1 text

        if((signed)textLines.size()==0) {
            textLines.push_back("");
        }
    }

    void textBox::draw(PityBoy::controls::Parent* myWindow) {
        const int bkColor = 3;
        const int textColor = 1;
        const int selColor = 2;
        const int cursorColor = 0;

        frameCounter++;
        myWindow->drawBox(x, y, realW, realH, bkColor, true);
        //myWindow->drawBox(x, y, realW, realH, 2);

        for(unsigned int i=0;i<(unsigned)h;i++) { // unsigned to prevent compiler warnings
            if(scrollY+i>=textLines.size()) break;
            std::string splitStr = textLines.at(scrollY+i);
            if(scrollX<(signed)splitStr.length()) { // when to long to substr then just dont draw it

                splitStr = splitStr.substr(scrollX, w);

                // draw selection (i should rewrite this probably)
                if(inSel) {

                    int dx = scrollX;
                    int dy = scrollY+i; // where we are

                    if(dy>=selYstart&&dy<=selYend) { // this line is in selection

                        if(dy==selYstart) { // this line starts selection

                            int sx=selXstart-scrollX; // starting position of selection
                            if(sx<0) sx = 0; 
                            
                            int wx=splitStr.length(); // length of selection on current line

                            if(selYstart==selYend) { // also if selection ends on same line, draw in other way with proper width
                               
                                wx = selXend - selXstart; // this code is somehow working mess
                                int dsx = 0; // amount of width to subtract

                                if(scrollX>selXstart) dsx = scrollX - selXstart;
                                //std::cout<<"vars:\nwx: "<<wx<<"\nsx: "<<sx<<std::endl;
                                if(sx+wx>w) {
                                    dsx = dsx + (sx+wx)-w;
                                } 
                                if(sx==0&&wx>w) {
                                    dsx = wx - (selXend - scrollX);   
                                }
                                if(wx-dsx>w) dsx = wx - w;

                                int dwidth = wx*8 - dsx*8 + 1;
                                if(dwidth <= 1) dwidth = 0;
                                //std::cout<<"dsx: "<<dsx<<std::endl;
                                if(scrollX+w>selXstart) myWindow->drawBox(x+1+sx*8, y+1+i*9, dwidth , 9, selColor, true); // draw if end on same line
                            } else {
                                if(scrollX+w>selXstart) myWindow->drawBox(x+1+sx*8, y+1+i*9, wx*8 - sx*8+1, 9, selColor, true); // draw if end on not same line
                            }
                            
                        } else if(dy==selYend) { // this line ends selection
                        
                            int sx=selXend-scrollX;
                            if(scrollX<selXend) myWindow->drawBox(x+1, y+1+i*9, sx*8+1, 9, selColor, true);

                        } else { // this line is entire selected

                            myWindow->drawBox(x+1, y+1+i*9, splitStr.length()*8+1, 9, selColor, true);

                        }

                    }
                }

                myWindow->drawText(x+2, y+2+i*9, splitStr, textColor);
            }
        }

        sf::Vector2i mousePos = myWindow->getMousePos();

        // check if mouse is over text box to change pointer to text cursor
        if(isMouseOver(x, y, realW, realH, mousePos.x, mousePos.y)&&myWindow->isFocused()) { // mouse cursor
            if(!lastCursor) {
                myWindow->setMouseCursor(&textCursor);
            }
            lastCursor = true;
        } else {
            if(lastCursor) {
                myWindow->setMouseCursor(&defaultCursor);
            }
            lastCursor = false;
        }

        if(held&&frameCounter%5==0) {
            // smooth scroll while hed
            if(cursorPosY>=scrollY+h) scrollY++;
            if(cursorPosY<scrollY) scrollY--;            

            if(cursorPosX>=scrollX+w) scrollX++;
            if(cursorPosX<scrollX) {
                scrollX--;
                scrollToCursor();
            }
            
        }

        if(focused) { // in textbox blinking cursor
            if(frameCounter%60<30) {
                int dx=cursorPosX - scrollX;
                int dy=cursorPosY - scrollY;

                if(isTextCursorOnScreen()) { // check is the cursor in bounds on screen
                    for(int i=0; i<8; i++) {
                        myWindow->drawPixel(x+2+dx*8-1, y+2+dy*9+i, cursorColor);
                    }
                    //myWindow->drawChar(x+2+dx*8, y+2+dy*9, '|', 0);
                }
            }
        }
    }

    void textBox::handleEvent(sf::Event event, PityBoy::controls::Parent* myWindow) {
        try { // safeguard for typed code in pityboy editor

        if(event.type == sf::Event::MouseButtonPressed) {

            if(event.mouseButton.button == sf::Mouse::Left) { // pressed on text box
            
                sf::Vector2i mousePos = myWindow->getMousePos();
                
                if(isMouseOver(x+1, y+1, realW-3, realH-3, mousePos.x, mousePos.y)) { // textbox: when mouse pressed on it
                    focused = true;
                    held = true;
                    frameCounter = 0;

                    if(shiftHeld&&!inSel) {
                        cursorSelUpdate(true);
                    }

                    moveCursorToMouse(mousePos.x, mousePos.y);

                    if(shiftHeld) {
                        cursorSelUpdate();
                    } else {
                        cursorSelUpdate(true);
                    }

                } else {
                    focused = false;
                }
            }

        }

        if(event.type == sf::Event::MouseMoved) {
            if(held) {
                sf::Vector2i mousePos = myWindow->getMousePos();
                moveCursorToMouse(mousePos.x, mousePos.y);
                cursorSelUpdate();

            }
        }

        if(event.type == sf::Event::MouseButtonReleased) {

            if(event.mouseButton.button == sf::Mouse::Left) {

                sf::Vector2i mousePos = myWindow->getMousePos();
                if(isMouseOver(x, y, realW, realH, mousePos.x, mousePos.y) && held) {
                    // ...
                } 
                held = false;
            }

        }

        if (event.type == sf::Event::MouseWheelScrolled) { // textbox: wheel support
            
            sf::Vector2i mousePos = myWindow->getMousePos();

            if(isMouseOver(x, y, realW, realH, mousePos.x, mousePos.y)) {

                if(event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel && allowNewLines) { // Y
                    scrollY = scrollY - event.mouseWheelScroll.delta;
                    if(scrollY < 0) scrollY = 0;
                    int maxScrollY = (signed)textLines.size();
                    if(scrollY >= maxScrollY) scrollY = maxScrollY-1;
                }

                if(event.mouseWheelScroll.wheel == sf::Mouse::HorizontalWheel) { // X (untested with mouse)
                    scrollX = scrollX - event.mouseWheelScroll.delta;
                    if(scrollX < 0) scrollX = 0;
                    int maxScrollX = getTextLongestX();
                    if(scrollX >= maxScrollX) scrollX = maxScrollX-1;
               }
                    
            }

        }   

        if(event.type == sf::Event::TextEntered) { // textbox: input
            typeChar((int)event.text.unicode);
        }

        if(event.type == sf::Event::KeyReleased) {
            if(event.key.code==sf::Keyboard::LShift||event.key.code==sf::Keyboard::RShift) {
                shiftHeld = false;
            }
        }


        if(event.type == sf::Event::KeyPressed) { // textbox: special keys support
            if(event.key.code==sf::Keyboard::LShift||event.key.code==sf::Keyboard::RShift) {
                event.key.shift = true;
            }
            if(event.key.control) { // control and shift
                controlHeld = true; 
            } else {
                controlHeld = false;
            }
            if(event.key.shift) {
                shiftHeld = true;
            } else {
                shiftHeld = false;
            }
            

            // textbox: arrows
            if(event.key.code == sf::Keyboard::Right) { // right arrow
                if(event.key.control) {
                    moveCursorToWord(right);
                } else {
                    moveCursor(right);    
                }
            }
            if(event.key.code == sf::Keyboard::Left) { // Left arrow
                if(event.key.control) {
                    moveCursorToWord(left);
                } else {
                    moveCursor(left); 
                }
            }

            if(event.key.code == sf::Keyboard::Down) { // Down arrow
                moveCursor(down);       
            }
            if(event.key.code == sf::Keyboard::Up) { // Up arrow
                moveCursor(up); 
            }

            // textbox: home and end
            if(event.key.code == sf::Keyboard::Home) { // Home
                while(1) {
                    if(event.key.control) {
                        if(cursorPosX==0&&cursorPosY==0) break;
                    } else {
                        if(cursorPosX==0) break;
                    }
                    moveCursor(left);
                }

                frameCounter = 0; // this code doesn't use moveCursor()
                scrollToCursor();
                rememberXPos = cursorPosX;
            }
            if(event.key.code == sf::Keyboard::End) { // End
                if(event.key.control) {
                    while(1) {
                        if(cursorPosY == textLines.size() - 1 && cursorPosX == textLines.at(cursorPosY).length()) break;
                        moveCursor(right);
                    }
                    
                    scrollToCursor();
                } else {
                    for(int i=cursorPosX; i < (signed)textLines.at(cursorPosY).length(); i++) {
                        moveCursor(right);
                    }
                }
                rememberXPos = cursorPosX;
            }

            // textbox: Page Down and Page Up
            if(event.key.code == sf::Keyboard::PageDown) {
                cursorPosY = cursorPosY + h + 1; // some extended code to fix viewport scroll
                if(cursorPosY > (signed)textLines.size()) cursorPosY = (signed)textLines.size();
                scrollToCursor();
                moveCursor(up);
            }

            if(event.key.code == sf::Keyboard::PageUp) {
                cursorPosY = cursorPosY - h + 1; // some extended code to fix viewport scroll
                if(cursorPosY < 1) cursorPosY = 1;
                moveCursor(up);
            }

            // textbox: Ctrl+V
            if(event.key.code == sf::Keyboard::V && event.key.control) {
                std::string clipboardText = sf::Clipboard::getString();
                std::stringstream stream(clipboardText);
                std::string line;
                int b=0;
                while (std::getline(stream, line)) {
                    if(b>0) typeChar(13);
                    b++;
                    std::replace( line.begin(), line.end(), '\t', ' '); 
                    for(int i=0;i<(signed)line.length();i++) {
                        typeChar(line.at(i));
                    }
                }
            }

            // textbox: Ctrl+A
            if(event.key.code == sf::Keyboard::A && event.key.control) {
                bool AshiftHeld = shiftHeld;
                cursorPosX = 0;
                cursorPosY = 0;
                shiftHeld = true;
                cursorSelUpdate(true);
                while(1) {
                    if(cursorPosY == textLines.size() - 1 && cursorPosX == textLines.at(cursorPosY).length()) break;
                    moveCursor(right);
                }
                shiftHeld = AshiftHeld;
            }


        }

        } catch(const std::exception& e) {
            std::cout << "Fatal error!" << std::endl;
            std::cout << e.what() << std::endl;
            // in case of fatal error in string manipulation in textbox we can save user code
            exit(0);
        }

    } 

    /*
        Big TO DO LIST:
        break characters for the working with words section (all tested in vscode and vscode like word cursor moving will be used)
           [space],   ! @ # $ % ^ & * ( ) - =   + { } [ ] | \ ` ~ / < > ? . ; ' : "
        test string: a!b@c#d$e%f^g&h*i(j)k-l=m_n+o{p}q[r]s|t\u`v~w/x<y>z?0.1;2'3:4"5 test string2: dsd  ds  fsfs  dd[[dd
        // there must be non breakable character before breakable character: aa[a' <- will skip to [ and ', [[!\';;[ <- will skip to end. 
        // Also [space] is special because it ignores this rule and has some other properties. 


        ##  Working with Words:
        Ctrl + Left Arrow: Move cursor to the beginning of the previous word.   DONE
        Ctrl + Right Arrow: Move cursor to the beginning of the next word.      DONE
        Ctrl + Backspace: Delete the previous word.                             DONE
        Ctrl + Delete: Delete the next word.                                    DONE
        ## Moving the Cursor:
        Home: Move cursor to the beginning of the current line.                 DONE
        End: Move cursor to the end of the current line.                        DONE
        Ctrl + Home: Move cursor to the top of the text entry field.            DONE
        Ctrl + End: Move cursor to the bottom of the text entry field.          DONE
        Page Up: Move cursor up a frame.                                        DONE
        Page Down: Move cursor down a frame.                                    DONE
        ## Selecting Text:
        Shift + Left/Right Arrow Keys: Select characters one at a time.                                                 DONE
        Shift + Up/Down Arrow Keys: Select lines one at a time.                                                         DONE
        Shift + Ctrl + Left/Right Arrow Keys: Select words. Keep pressing the arrow keys to select additional words.    DONE
        Shift + Home: Select the text between the cursor and the beginning of the current line.                         DONE
        Shift + End: Select the text between the cursor and the end of the current line.                                DONE
        Shift + Ctrl + Home: Select the text between the cursor and the beginning of the text entry field.              DONE
        Shift + Ctrl + End: Select the text between the cursor and the end of the text entry field.                     DONE
        Shift + Page Down: Select a frame of text below the cursor.                                                     DONE
        Shift + Page Up: Select a frame of text above the cursor.                                                       DONE
        Ctrl + A: Select all text.                                                                                      DONE
        ## Editing:
        Ctrl + C: Copy selected text.
        Ctrl + X: Cut selected text.
        Ctrl + V: Paste text at cursor.         DONE
        Ctrl + Z: Undo.
        Ctrl + Y: Redo.
        ## While text is selected:
        Backspace / Delete: Remove selection
        ## While text is not selected:
        Shift + Del: Remove entire line
    */

    void textBox::convertTextIntoVector() { // as function says: convert "string text" into line split vector
        textLines.clear();
        std::stringstream stream(text);
        std::string line;
        while (std::getline(stream, line)) {
            std::replace( line.begin(), line.end(), '\t', ' '); 
            textLines.push_back(line);
        }
    }

    int textBox::getTextLongestX() {
        auto textLinesLongest = std::ranges::max_element(textLines, std::ranges::less{}, std::ranges::size);
        return textLinesLongest->length();
    }

    bool textBox::isTextCursorOnScreen() {
        int dx=cursorPosX - scrollX;
        int dy=cursorPosY - scrollY;

        if(dx>=0 && dy>=0 && dx<w && dy<h) return true;
        return false;
    }

    void textBox::scrollToCursor() { // this function force scrolls when cursor is outside
        if(!isTextCursorOnScreen()) {
            scrollX = cursorPosX-w+1;
            if(scrollX<0) scrollX = 0;
        }
        // check if cursor is really invisible on Y axis before forcing scrollY
        if(!((cursorPosY-scrollY) >= 0 && (cursorPosY-scrollY) < h)) {
            scrollY = cursorPosY -1 ;
            if(scrollY<0) scrollY=0;
        }
    }

    void textBox::moveCursor(moveDirection direction) { // the black magic happens here...

        bool canMoveLR=true;

        if(inSel&&!shiftHeld) {
            canMoveLR = false;
            inSel = false;
        }
        if(shiftHeld&&!inSel) {
            cursorSelUpdate(true);
        }

        if(direction == moveDirection::right && canMoveLR) {
            cursorPosX++;
            if(cursorPosX>(signed)textLines.at(cursorPosY).length()) { // if at the end
                if(cursorPosY+1 < (signed)textLines.size()) { // next line if we can
                    cursorPosX = 0;
                    cursorPosY++;
                } else {
                    cursorPosX--;
                }  
            }
            // went out of bounds Y?
            if((cursorPosY - scrollY)>=h) {
                scrollY++;
            } 

            // if went out of bounds X, move the scrollX
            if((cursorPosX - scrollX) >= w) scrollX++;
            rememberXPos = cursorPosX;
        }
        
        if(direction == moveDirection::left && canMoveLR) {
            cursorPosX--;
            if(cursorPosX<0) { // if at the beg
                if(cursorPosY-1>=0) { // go to end of line before
                    cursorPosY--;
                    cursorPosX = textLines.at(cursorPosY).length();
                } else {
                    cursorPosX=0;
                }
            }

            // went out of bounds?
            if((cursorPosX - scrollX) < 0) scrollX--;
            rememberXPos = cursorPosX;
        }

        if(direction == moveDirection::down) {
            cursorPosY++;

            if(cursorPosY>=(signed)textLines.size()) cursorPosY--;

            // rememberXPos logic
            cursorPosX = rememberXPos;

            if((signed)textLines.at(cursorPosY).length()<cursorPosX) { // if we are beyond line on X, go back to line
                cursorPosX = textLines.at(cursorPosY).length();
            }

            // went out of bounds Y?
            if((cursorPosY - scrollY)>=h) {
                scrollY++;
            } 
        }

        if(direction == moveDirection::up) {
            cursorPosY--;

            if(cursorPosY<0) cursorPosY=0;

            // rememberXPos logic
            cursorPosX = rememberXPos;

            if((signed)textLines.at(cursorPosY).length()<cursorPosX) { // if we are beyond line on X, go back to line
                cursorPosX = textLines.at(cursorPosY).length();
            }

            // went out of bounds Y?
            if((cursorPosY - scrollY)<0) {
                scrollY--;
            } 
        }
        frameCounter = 0;
        scrollToCursor();

        if(shiftHeld) {
            if(inSel) {
                cursorSelUpdate();
            }
        } 
    }

    // textbox: typeCharacter
    void textBox::typeChar(char c) {
        if(c<8) return;
        
        bool canInsertChar=true;
        bool wasInSel=false;

        if(inSel) {
            if(!shiftHeld) {
                inSel = false;
                wasInSel = true;
            }
            if(selXstart==selXend&&selYstart==selYend) {
                inSel = false;
                wasInSel = false;
            }
        }

        if(maxLength!=0) { // check length of our string
            int i=0;
            int end=textLines.size();
            int totalLength=0;
            for(const std::string& b : textLines ) {
                i++;
                totalLength += b.length();
                if(i!=end) totalLength++; // include new lines 
            }
            if(totalLength>=maxLength) canInsertChar=false;
        }

        std::cout<<(int)c<<std::endl;
        
        if(c== '\t') c = ' '; // convert tab to space

        if(c > 31 && c < 127&& canInsertChar) { // any printable character
            if(wasInSel) deleteSel();
            
            std::string currentLine = textLines.at(cursorPosY);
            
            currentLine = currentLine.substr(0,cursorPosX) + c + currentLine.substr(cursorPosX);
            textLines.at(cursorPosY) = currentLine;

            moveCursor(right);
        }

        if((c == 13 || c == '\n') && canInsertChar) { // textbox: newline
            if(wasInSel) deleteSel();

            if(allowNewLines) { // if textbox is multiline

                textLines.insert(textLines.begin() + cursorPosY + 1, "") ;

                if(cursorPosX==(signed)textLines.at(cursorPosY).length()) { // if at the end of line, just do nothing
                    moveCursor(right);
                } else { // if not, substring the line
                    std::string tempStr = textLines.at(cursorPosY).substr(cursorPosX);
                    textLines.at(cursorPosY) = textLines.at(cursorPosY).substr(0,cursorPosX);
                    moveCursor(right);
                    textLines.at(cursorPosY) = tempStr;
                }
            }
        }


        if(c == 8) { // textbox: backspace 
            if(wasInSel) {
                deleteSel();
                return;
            }
            if(controlHeld) {
                controlHeld = false; // to prevent recursion, because moveCursorToWord calls typeChar to remove character
                moveCursorToWord(left, true);
                controlHeld = true;
                return;
            }
            std::string currentLine = textLines.at(cursorPosY);

            // if line length is 0, just remove the line
            if(currentLine.length()==0) {

                if(cursorPosY!=0) { // we cannot remove at begining

                    int tempY = cursorPosY; 
                    moveCursor(left);
                    textLines.erase(textLines.begin()+tempY); // remove the line
                }

            } else {
                if(cursorPosX==0) { // if we are at beginning (and not at the Y begin), remove the line but move the contents
                    if(cursorPosY!=0) {
                        int tempY = cursorPosY; // before moving shift the cursor
                        moveCursor(left);
                        std::string tempStr = textLines.at(tempY); // get the line
                        textLines.erase(textLines.begin()+tempY);  // remove the line
                        // connect the line to next line
                        textLines.at(cursorPosY) = textLines.at(cursorPosY) + tempStr;
                    }
                } else {
                    textLines.at(cursorPosY) = currentLine.substr(0,cursorPosX-1) + currentLine.substr(cursorPosX);
                    moveCursor(left);
                }
            }
        }

        if(c == 127) { // textbox: delete (backspace but removes character at front)
            if(wasInSel) {
                deleteSel();
                return;
            }
            if(controlHeld) {
                controlHeld = false; // to prevent recursion, because moveCursorToWord calls typeChar to remove character
                moveCursorToWord(right, true);
                controlHeld = true;
                return;
            }
            std::string currentLine = textLines.at(cursorPosY);

            // if at end of line, remove next line (if not end) and merge it
            if(cursorPosX == (signed)currentLine.length()) {
                if(cursorPosY < (signed)textLines.size()-1) { // if we are on end then dont remove
                    textLines.at(cursorPosY) = currentLine + textLines.at(cursorPosY+1);
                    textLines.erase(textLines.begin()+cursorPosY+1);
                }
            } else { // not at end of line, remove character at front
                textLines.at(cursorPosY) = currentLine.substr(0,cursorPosX) + currentLine.substr(cursorPosX+1);
            }
        }
        cursorSelUpdate();
    }
    //////////

    bool textBox::isCursorAtTheEnd() {
        if(cursorPosY==textLines.size()-1 && cursorPosX==textLines.at(cursorPosY).length()) return true;
        return false;
    }
    bool textBox::isCursorAtTheBegin() {
        if(cursorPosY==0&&cursorPosX==0) return true;
        return false;
    }
    bool textBox::isCursorAtTheEndOfLine() {
        if(cursorPosX==textLines.at(cursorPosY).length()) return true;
        return false;
    }

    void textBox::moveCursorToWord(moveDirection dir, bool deleteWord) { // This code is for ctrl+left/right arrow and deleting text by word. don't touch if its working. 

        auto isBreakable = [](char c) {
            char breakableCharacters[] = {'!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '-', '=', '+', '{', '}', '[', ']', '|', '\\', '`', '~', '/', '<', '>', '?','.', ';', '\'', ':', '"'};
            for(int i=0; i < (signed)sizeof(breakableCharacters); i++) {
                if(c==breakableCharacters[i]) return true;
            }
            return false;
        };
        
        // newline and space are special separators [[test[ test]

        if(dir==right) {
             // are we at the end
            if(isCursorAtTheEnd()) return;

            bool calledAtEnd=false;

            if(isCursorAtTheEndOfLine()) { // skip to next line
                moveCursor(right);
                calledAtEnd = true;
            }  

            std::string currentLine = textLines.at(cursorPosY);
            int currentLineLen = currentLine.length();

            if(currentLineLen==0) {
                if(deleteWord) {
                    typeChar(8);
                }
                return;
            }

            bool isFirstBreakable = isBreakable(currentLine.at(cursorPosX)); // check what type of characters we going through
            bool isFirstSpace = currentLine.at(cursorPosX)==' ';
            bool nextSpaceBreaks = !isFirstSpace; // dont break on spaces that we start

            int i=cursorPosX;
            while(1) {
                char c = currentLine.at(i);

                if(c==' '&&nextSpaceBreaks) break;

                if(isFirstSpace) { // ignore first spaces
                    if(c==' ') {
                        if(isCursorAtTheEndOfLine()) break;
                        moveCursor(right);
                        goto skipIfLogicRight;
                    } else {
                        isFirstBreakable = isBreakable(c);
                        nextSpaceBreaks = true;
                        isFirstSpace = false;
                    }
                }

                if(isFirstBreakable) { // if we are going through breakable characters, stop on non breakable
                    if(isBreakable(c)) {
                        if(isCursorAtTheEndOfLine()) break;
                        moveCursor(right);
                        nextSpaceBreaks = true;
                    } else {
                        break;
                    }
                } else { // if not, then stop on breakable
                    if(isBreakable(c)) {
                        break;
                    } else {
                        nextSpaceBreaks = true;
                        if(isCursorAtTheEndOfLine()) break;
                        moveCursor(right);
                    }
                }

                skipIfLogicRight:

                if(deleteWord) {
                    moveCursor(left);
                    typeChar(127);
                }
                
                i++;
                if(i>=currentLineLen) break;

            }

            if(calledAtEnd&&deleteWord) {
                typeChar(8);
            }

        }

        if(dir==left) {
             // are we at the beginning 
            if(isCursorAtTheBegin()) return;

            if(cursorPosX==0) { // skip to previous line
                if(deleteWord) {
                    typeChar(8);
                    return;
                } else {
                    moveCursor(left);
                }
            }  

            std::string currentLine = textLines.at(cursorPosY);
            int currentLineLen = currentLine.length();

            // if we are at the end of line (prevent .at() exception)
            bool isFirstBreakable; 
            bool isFirstSpace;
            bool nextSpaceBreaks;
            bool moveCursorAfterBreak=true;
            
            moveCursor(left); // moving cursor allows to reuse code (if not using deleteChar)
            

            isFirstBreakable = isBreakable(currentLine.at(cursorPosX)); // check what type of characters we going through
            isFirstSpace = currentLine.at(cursorPosX)==' ';
            nextSpaceBreaks = !isFirstSpace; // dont break on spaces that we start
            
            char fixDeletionChar; // this fixes ctrl + backspace removing one too much character

            if(deleteWord) {
                moveCursor(right);
                typeChar(8);
            }

            int i=cursorPosX;
            while(1) {
                char c = currentLine.at(i);

                if(c==' '&&nextSpaceBreaks) break;

                if(isFirstSpace) { // ignore first spaces
                    if(c==' ') {
                        if(cursorPosX==0) break;
                        moveCursor(left);                        
                        goto skipIfLogicLeft;
                    } else {
                        isFirstBreakable = isBreakable(c);
                        nextSpaceBreaks = true;
                        isFirstSpace = false;
                    }
                }

                if(isFirstBreakable) { // if we are going through breakable characters, stop on non breakable
                    if(isBreakable(c)) {
                        if(cursorPosX==0) break;
                        moveCursor(left);
                        nextSpaceBreaks = true;
                    } else {
                        break;
                    }
                } else { // if not, then stop on breakable
                    if(isBreakable(c)) {
                        break;
                    } else {
                        nextSpaceBreaks = true;
                        if(cursorPosX==0) break;
                        moveCursor(left);
                    }
                }

                skipIfLogicLeft:
                
                i--;
                if(i<=0) {
                    moveCursorAfterBreak=false;
                    break;
                }

                if(deleteWord) {
                    moveCursor(right);
                    fixDeletionChar = currentLine.at(i); // just to reuse code
                    typeChar(8);
                }
            }
            // weird fix, but works
            if(moveCursorAfterBreak) { 
                if(deleteWord) {
                    typeChar(fixDeletionChar);
                } else {
                    moveCursor(right);
                }
            } else {
                if(deleteWord) {
                    moveCursor(right);
                    typeChar(8);
                }
            }

        }
    }

    void textBox::moveCursorToMouse(int mx, int my) {
        // calculate block cursor position based from mouse coordinates on mouse press on text box
        // note: i think it is better to don't touch this code
        int cx=mx - x - 5;
        int cy=my - y - 1;
        int cxfix = ceil(cx/(float)8); // fix for X going outside view area due to calculation
        if(cxfix==w) cxfix--;
        cursorPosX = cxfix;
        cursorPosY = ceil(cy/(float)9) - 1;

        if(cursorPosX<-1) cursorPosX = -1;
        if(cursorPosY<-1) cursorPosY = -1;
        if(cursorPosX>w) cursorPosX = w;
        if(cursorPosY>h) cursorPosY = h;

        cursorPosX = scrollX + cursorPosX;
        cursorPosY = scrollY + cursorPosY;

        if(cursorPosX<0) cursorPosX=0;
        if(cursorPosY<0) cursorPosY=0;
        
        if(cursorPosY >= (signed)textLines.size()) { // outside Y
            cursorPosY = textLines.size() - 1;
            cursorPosX = textLines.at(cursorPosY).length();
        }

        if(cursorPosX > (signed)textLines.at(cursorPosY).length()) {
            cursorPosX = textLines.at(cursorPosY).length();
        }

        rememberXPos = cursorPosX;
    }

    void textBox::cursorSelUpdate(bool startSel) {

        if(startSel) {
            selXstart = cursorPosX;
            selYstart = cursorPosY;
            selXend = cursorPosX;
            selYend = cursorPosY;
            selXbegin = cursorPosX;
            selYbegin = cursorPosY;
            inSel = true;
            return;
        }

        // if moved right/down

        if((cursorPosX>selXbegin&&cursorPosY==selYbegin)||cursorPosY>selYbegin) {
            selXstart = selXbegin;
            selYstart = selYbegin;
            selXend = cursorPosX;
            selYend = cursorPosY;
        } else if(cursorPosX<selXbegin||cursorPosY<selYbegin) {
            selXend = selXbegin;
            selYend = selYbegin;
            selXstart = cursorPosX;
            selYstart = cursorPosY;
        }

        if(cursorPosX==selXbegin&&cursorPosY==selYbegin) {
            inSel = false;
        } else {
            if(held||shiftHeld) inSel = true;
        }

    }

    void textBox::setDimensions(int x,int y, int w,int h) {
        this->x = x;
        this->y = y;
        this->w = w;
        this->h = h;
    }

    void textBox::setConfig(bool allowNewLines, int maxLength) {
        this->allowNewLines = allowNewLines;
        this->maxLength = maxLength;
    } 

    void textBox::deleteSel() { 
        if(controlHeld&&shiftHeld) return;

        cursorPosX = selXend;
        cursorPosY = selYend;
        
        while(1) {
            if(selXstart==cursorPosX&&selYstart==cursorPosY) break;
            std::string currentLine = textLines.at(cursorPosY); // this code is same as in backspace
            
            if(currentLine.length()==0) {

                if(cursorPosY!=0) { // we cannot remove at begining

                    int tempY = cursorPosY; 
                    moveCursor(left);
                    textLines.erase(textLines.begin()+tempY); // remove the line
                }
            } else {
                if(cursorPosX==0) { // if we are at beginning, remove the line but move the contents
                    if(cursorPosY!=0) {
                        int tempY = cursorPosY; // before moving shift the cursor
                        moveCursor(left);
                        std::string tempStr = textLines.at(tempY); // get the line
                        textLines.erase(textLines.begin()+tempY);  // remove the line
                        // connect the line to next line
                        textLines.at(cursorPosY) = textLines.at(cursorPosY) + tempStr;
                    }
                } else {
                    textLines.at(cursorPosY) = currentLine.substr(0,cursorPosX-1) + currentLine.substr(cursorPosX);
                    moveCursor(left);
                }
            }
            
        }

    } 

}