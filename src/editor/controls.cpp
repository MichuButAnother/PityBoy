#include "../headers/controls.hpp" 

#include <iostream>

namespace PityBoy::controls {

    // helper functions // 

    bool isMouseOver(int x, int y, int w, int h, int mx, int my) {
        if(mx>=x && my>y && mx < x+w && my < y+h) return true;
        return false;
    }

    // label //

    label::label(int x, int y,std::string text, int color) {
        this->text = text;
        this->x = x;
        this->y = y;
        this->color = color;
    }


    void label::draw(PityBoy::controls::Parent* myWindow) {
        myWindow->drawText(x, y, text, color);
    }

    void label::handleEvent(__attribute__((unused)) sf::Event event, __attribute__((unused)) PityBoy::controls::Parent* myWindow) { // to hide "unused variable" error 

    }

    void label::setPosition(int x,int y) {
        this->x = x;
        this->y = y;
    }

    void label::setText(std::string text) {
        this->text = text;
    }
    
    void label::setColor(int color) {
        this->color = color;
    }

    // button //

    button::button(int x, int y, int w,int h, std::string text) {
        this->text = text;
        
        this->x = x;
        this->y = y;
        this->w = w;
        this->h = h;

        this->textColor = 3;
        this->buttonColor = 3;
        this->buttonFillColor = 1;
        this->buttonPressedFillColor = 2;
        this->enabled = 1;
    }


    void button::draw(PityBoy::controls::Parent* myWindow) {
        sf::Vector2i mousePos = myWindow->getMousePos();

        if(held && isMouseOver(x, y, w, h, mousePos.x, mousePos.y)) { // fill draw
            myWindow->drawBox(x, y, w, h, buttonPressedFillColor, true); 
        } else {
            if(enabled) {
                myWindow->drawBox(x, y, w, h, buttonFillColor, true); 
            } else {
                myWindow->drawBox(x, y, w, h, 2, true); 
            }
        }
        if(enabled) { // outline
            myWindow->drawBox(x, y, w, h, buttonColor); 
        } else {
            myWindow->drawBox(x, y, w, h, 1); 
        }

        // draw text at middle
        int textPixelLength = text.length() * 8;
        int dx = x + (w / 2) - (textPixelLength / 2);
        int dy = y + (h / 2) - 4;
        if(enabled) {
            myWindow->drawText(dx, dy, text, textColor);
        } else {
            myWindow->drawText(dx, dy, text, 1);
        }
    }
   

    void button::handleEvent(sf::Event event, PityBoy::controls::Parent* myWindow) {

        if(event.type == sf::Event::MouseButtonPressed) {
            if(event.mouseButton.button == sf::Mouse::Left) {
                if(!enabled) {
                    held = false;
                    return;
                }
                sf::Vector2i mousePos = myWindow->getMousePos();
                
                if(isMouseOver(x, y, w, h, mousePos.x, mousePos.y)) {
                    held = true;
                }
            }
        }
        if(event.type == sf::Event::MouseButtonReleased) {
            if(event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2i mousePos = myWindow->getMousePos();
                if(isMouseOver(x, y, w, h, mousePos.x, mousePos.y) && held) {
                    if(callbackClicked!=nullptr) callbackClicked();
                }
                held = false;
            }
        }
    }

    void button::registerCallback_clicked(void (*callback)()) {
        callbackClicked = callback;
    }

    void button::setColors(int textColor, int buttonColor, int buttonFillColor, int buttonPressedFillColor) {
        this->textColor = textColor;
        this->buttonColor = buttonColor;
        this->buttonFillColor = buttonFillColor;
        this->buttonPressedFillColor = buttonPressedFillColor;
    }

    void button::setDimensions(int x,int y, int w,int h) {
        this->x = x;
        this->y = y;
        this->w = w;
        this->h = h;
    }
    void button::setText(std::string text) {
        this->text = text;
    }
    void button::setEnabled(bool enabled) {
        this->enabled = enabled;
    }


    //// textbox ////

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

                if(event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel) { // Y
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

            if(event.text.unicode > 31 && event.text.unicode < 128) { // any printable character
                
                std::string currentLine = textLines.at(cursorPosY);
                
                currentLine = currentLine.substr(0,cursorPosX) + char(event.text.unicode) + currentLine.substr(cursorPosX);
                textLines.at(cursorPosY) = currentLine;

                moveCursor(right);
            }

            if(event.text.unicode == 8) { // backspace
                std::string currentLine = textLines.at(cursorPosY);
                
                currentLine = currentLine.substr(0,cursorPosX-1) + currentLine.substr(cursorPosX);
                textLines.at(cursorPosY) = currentLine;
                
                moveCursor(left);
            }

        }

        if(event.type == sf::Event::KeyPressed) { // textbox: special keys support
            // if(event.key.control) {} ...
            frameCounter = 0;

            // textbox: arrows
            if(event.key.code == sf::Keyboard::Right) { // right arrow
                moveCursor(right);    
            }
            if(event.key.code == sf::Keyboard::Left) { // Left arrow
                moveCursor(left);    
            }

            if(event.key.code == sf::Keyboard::Down) { // Down arrow
                moveCursor(down);       
            }
            if(event.key.code == sf::Keyboard::Up) { // Up arrow
                moveCursor(up); 
            }

        }

    } 

    // to do: textbox newlines, selection, pg up, pg down, delete, 

    void textBox::convertTextIntoVector() { // as function says: convert "string text" into line split vector
        textLines.clear();
        std::stringstream stream(text);
        std::string line;
        while (std::getline(stream, line)) {
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

        scrollToCursor();
    }


}