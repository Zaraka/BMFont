/* 
 * File:   BMF_Char.cpp
 * Author: ZarakaAngel
 * 
 * Created on 23. prosinec 2012, 15:50
 */

#include "BMF_Char.h"


BMF_Char::BMF_Char(unsigned char character, unsigned char width, unsigned char height, unsigned char rel_x, unsigned char rel_y, unsigned char shift, SDL_Surface *char_surf) {
    this->char_surf = char_surf;
    this->character = character;
    this->width = width;
    this->height = height;
    this->rel_x = rel_x;
    this->rel_y = rel_y;
    this->shift = shift;
}

BMF_Char::BMF_Char(const BMF_Char& orig) {
}

BMF_Char::~BMF_Char() {
    SDL_FreeSurface(this->char_surf);
}

SDL_Surface* BMF_Char::getSurf() const{
    return this->char_surf;
}

unsigned char BMF_Char::getHeight() const{
    return this->height;
}

char BMF_Char::getRelX() const{
    return this->rel_x;
}

char BMF_Char::getRelY() const{
    return this->rel_y;
}

unsigned char BMF_Char::getShift() const{
    return this->shift;
}

unsigned char BMF_Char::getWidth() const{
    return this->width;
}