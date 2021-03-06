//
//  Type.cpp
//  BassLive 2.0
//
//  Created by Bass Jansson on 09/02/16.
//
//

#include "Syntax.h"


//========================================================================
Type::Type (char c) : Character(c)
{
    charType = TYPE;
    
    typeType = NO_TYPE;
    typeColor = COLOR_DEFAULT;
    typeString = "";
    
    flashValue = 0.0f;
}

//========================================================================
void Type::draw (float& x, float& y, bool vertical, bool selection, bool floating)
{
    // Draw type animation
    drawTypeAnimation();
    
    
    // Draw type character
    selection = selection || charSelected == begin;
    
    if (charType == TYPE ||
        charType == MAIN || right->charType == MAIN)
        ofSetColor(typeColor);
    else
        ofSetColor(COLOR_DEFAULT);
    
    Character::draw(x, y, vertical, selection, floating);
    
    
    // Draw type string
    string str = "";
    
    for (Character* c = right; c->charType == CHAR; c = c->right)
    {
        ofSetColor(typeColor);
        c->draw(x, y, HORIZONTAL, selection, false);
        
        str += c->charString;
    }
    
    
    // Draw type flash
    if (flashValue > 0.001f)
    {
        ofSetColor(typeColor.r, typeColor.g, typeColor.b, flashValue * 255);
        ofDrawRectangle(xAnim, yAnim, x - xAnim + charWidth, charHeight);
        
        flashValue *= 0.925f;
    }
    
    
    // Default color if string is changed
    if (str != typeString && charType != MAIN)
        typeColor = COLOR_DEFAULT;
    
    typeString = str;
}

void Type::flash (const ofColor& color)
{
    typeColor  = color;
    flashValue = 1.0f;
}

Character* Type::end()
{
    return getType(RIGHT)->left;
}

//========================================================================
void Type::keyPressed (int key)
{
    if (typeType != NO_TYPE)
    {
        if (key > 47 && key < 58)
        {
            if (charSelected != begin)
                charSelected->add(new Character(key));
        }
        else if ((key > 64 && key < 91) ||
                 (key > 96 && key < 123) ||
                 key == '+' || key == '-' ||
                 key == '*' || key == '/')
        {
            charSelected->add(new Character(key));
        }
    }
}

sig* Type::compile (Memory* memory, bool record)
{
    flash(COLOR_DEFAULT);
    
    return NULL;
}

//========================================================================
void Type::copyTo (Character* c)
{
    switch (typeType)
    {
        case NUMBER:  c->add(new  NumberType()); break;
        case PERCENT: c->add(new PercentType()); break;
        case INPUT:   c->add(new   InputType()); break;
        case MODULE:  c->add(new  ModuleType()); break;
        case BUFFER:  c->add(new  BufferType()); break;
            
        default: return;
    }
    
    for (Character* cc = right; cc->charType == CHAR; cc = cc->right)
        charSelected->add(new Character(cc->charString[0]));
}
