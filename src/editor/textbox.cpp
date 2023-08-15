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
        text = "WWWWWWWWWWWWWWWWWWWWW\nThis is line 2\ntstsetst\nbadgdshaeysa\nabc\nabc\nabc\nabc\ncrap\nmore crap\nidk to write";
        convertTextIntoVector();

        cursorPosX = 0;
        cursorPosY = 0;
        

        defaultCursor.loadFromSystem(sf::Cursor::Arrow);
        textCursor.loadFromSystem(sf::Cursor::Text);
        lastCursor = 0; // 0 arrow 1 text

        if((signed)textLines.size()==0) {
            textLines.push_back("");
        }
    }

    void textBox::draw(PityBoy::controls::Parent* myWindow) {
        frameCounter++;
        myWindow->drawBox(x, y, realW, realH, 2);
        myWindow->drawBox(x, y, realW, realH, 3, true);

        for(unsigned int i=0;i<(unsigned)h;i++) { // unsigned to prevent compiler warnings
            if(scrollY+i>=textLines.size()) break;
            std::string splitStr = textLines.at(scrollY+i);
            if(scrollX<(signed)splitStr.length()) { // when to long to substr then just dont draw it

                splitStr = splitStr.substr(scrollX, w);
                myWindow->drawText(x+2, y+2+i*9, splitStr, 1);
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

        if(focused) { // in textbox blinking cursor
            if(frameCounter%60<30) {
                int dx=cursorPosX - scrollX;
                int dy=cursorPosY - scrollY;

                if(isTextCursorOnScreen()) { // check is the cursor in bounds on screen
                    for(int i=0; i<8; i++) {
                        myWindow->drawPixel(x+2+dx*8-1, y+2+dy*9+i, 0);
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

                    // calculate block cursor position based from mouse coordinates on mouse press on text box
                    // note: i think it is better to don't touch this code
                    int cx=mousePos.x - x - 5;
                    int cy=mousePos.y - y - 1;
                    int cxfix = ceil(cx/(float)8); // fix for X going outside view area due to calculation
                    if(cxfix==w) cxfix--;
                    cursorPosX = scrollX +  cxfix;
                    cursorPosY = scrollY + ceil(cy/(float)9) - 1;
                    

                    if(cursorPosY >= (signed)textLines.size()) { // outside Y
                        cursorPosY = textLines.size() - 1;
                        cursorPosX = textLines.at(cursorPosY).length();
                    }

                    if(cursorPosX > (signed)textLines.at(cursorPosY).length()) {
                        cursorPosX = textLines.at(cursorPosY).length();
                    }

                    rememberXPos = cursorPosX;

                } else {
                    focused = false;
                }
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


        if(event.type == sf::Event::KeyPressed) { // textbox: special keys support

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
                if(event.key.control) {
                    cursorPosX = 0;
                    cursorPosY = 0;
                } else {
                    cursorPosX = 0;    
                }
                frameCounter = 0; // this code doesn't use moveCursor()
                scrollToCursor();
                rememberXPos = cursorPosX;
            }
            if(event.key.code == sf::Keyboard::End) { // End
                if(event.key.control) {
                    // to move properly viewport, lets just move cursor using for loop
                    cursorPosY = textLines.size() - 1;
                    cursorPosX = textLines.at(cursorPosY).length();
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
        Ctrl + Backspace: Delete the previous word.
        Ctrl + Delete: Delete the next word.
        ## Moving the Cursor:
        Home: Move cursor to the beginning of the current line.                 DONE
        End: Move cursor to the end of the current line.                        DONE
        Ctrl + Home: Move cursor to the top of the text entry field.            DONE
        Ctrl + End: Move cursor to the bottom of the text entry field.          DONE
        Page Up: Move cursor up a frame.                                        DONE
        Page Down: Move cursor down a frame.                                    DONE
        ## Selecting Text:
        Shift + Left/Right Arrow Keys: Select characters one at a time.
        Shift + Up/Down Arrow Keys: Select lines one at a time.
        Shift + Ctrl + Left/Right Arrow Keys: Select words. Keep pressing the arrow keys to select additional words.
        Shift + Ctrl + Up/Down Arrow Keys: Select paragraphs.
        Shift + Home: Select the text between the cursor and the beginning of the current line.
        Shift + End: Select the text between the cursor and the end of the current line.
        Shift + Ctrl + Home: Select the text between the cursor and the beginning of the text entry field.
        Shift + Ctrl + End: Select the text between the cursor and the end of the text entry field.
        Shift + Page Down: Select a frame of text below the cursor.
        Shift + Page Up: Select a frame of text above the cursor.
        Ctrl + A: Select all text.
        ## Editing:
        Ctrl + C: Copy selected text.
        Ctrl + X: Cut selected text.
        Ctrl + V: Paste text at cursor.
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

            // check if cursor is really invisible on Y axis before forcing scrollY

            if(!((cursorPosY-scrollY) >= 0 && (cursorPosY-scrollY) < h)) {
                scrollY = cursorPosY;
            }
        }
    }

    void textBox::moveCursor(moveDirection direction) { // the black magic happens here...

        if(direction == moveDirection::right) {
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
        
        if(direction == moveDirection::left) {
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
    }

    void textBox::typeChar(char c) {
        bool canInsertChar=true;

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

        std::cout<<c<<std::endl;
        
        if(c== '\t') c = ' '; // convert tab to space

        if(c > 31 && c < 127&& canInsertChar) { // any printable character
            
            std::string currentLine = textLines.at(cursorPosY);
            
            currentLine = currentLine.substr(0,cursorPosX) + c + currentLine.substr(cursorPosX);
            textLines.at(cursorPosY) = currentLine;

            moveCursor(right);
        }

        if(c == 13 && canInsertChar) { // textbox: newline

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
            std::string currentLine = textLines.at(cursorPosY);

            // if line length is 0, just remove the line
            if(currentLine.length()==0) {

                if(cursorPosY!=0) { // we cannot remove at begining

                    int tempY = cursorPosY; 
                    moveCursor(left);
                    textLines.erase(textLines.begin()+tempY); // remove the line
                }

            } else {
                if(cursorPosX==0) { // if we are at beginning, remove the line but move the contents

                    int tempY = cursorPosY; // before moving shift the cursor
                    moveCursor(left);
                    std::string tempStr = textLines.at(tempY); // get the line
                    textLines.erase(textLines.begin()+tempY);  // remove the line
                    // connect the line to next line
                    textLines.at(cursorPosY) = textLines.at(cursorPosY) + tempStr;
                } else {
                    textLines.at(cursorPosY) = currentLine.substr(0,cursorPosX-1) + currentLine.substr(cursorPosX);
                    moveCursor(left);
                }
            }
        }

        if(c == 127) { // textbox: delete (backspace but removes character at front)
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
    }

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

    void textBox::moveCursorToWord(moveDirection dir) { // don't touch if its working

        auto isBreakable = [](char c) {
            char breakableCharacters[] = {'!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '-', '=', '+', '{', '}', '[', ']', '|', '\\', '`', '~', '/', '<', '>', '?','.', ';', '\'', ':', '"'};
            for(int i=0; i < (signed)sizeof(breakableCharacters); i++) {
                if(c==breakableCharacters[i]) return true;
            }
            return false;
        };
        
        // newline and space are special separators [[test[ test]

        std::string currentLine = textLines.at(cursorPosY);
        int currentLineLen = currentLine.length();

        if(dir==right) {
             // are we at the end
            if(isCursorAtTheEnd()) return;

            if(isCursorAtTheEndOfLine()) { // skip to next line
                moveCursor(right);
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
                
                i++;
                if(i>=currentLineLen) break;
            }

        }

        if(dir==left) {
             // are we at the beginning 
            if(isCursorAtTheBegin()) return;

            if(cursorPosX==0) { // skip to previous line
                moveCursor(left);
                return;
            }  

            // if we are at the end of line (prevent .at() exception)
            bool isFirstBreakable;
            bool isFirstSpace;
            bool nextSpaceBreaks;
            bool moveCursorAfterBreak=true;
            
            moveCursor(left); // moving cursor allows to reuse code 
            

            isFirstBreakable = isBreakable(currentLine.at(cursorPosX)); // check what type of characters we going through
            isFirstSpace = currentLine.at(cursorPosX)==' ';
            nextSpaceBreaks = !isFirstSpace; // dont break on spaces that we start
            

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
            }

            if(moveCursorAfterBreak) moveCursor(right);

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

}