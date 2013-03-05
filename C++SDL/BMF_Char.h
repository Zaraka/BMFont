/* 
 * File:   BMF_Char.h
 * Author: ZarakaAngel
 *
 * Created on 23. prosinec 2012, 15:50
 */

#ifndef BMF_CHAR_H
#define	BMF_CHAR_H

#include "BMF.h"

class BMF_Char {
private:
    unsigned char character;
    unsigned char width;
    unsigned char height;
    char rel_x;
    char rel_y;
    unsigned char shift;
    SDL_Surface *char_surf;
public:
    BMF_Char(unsigned char character, unsigned char width, unsigned char height, unsigned char rel_x, unsigned char rel_y, unsigned char shift, SDL_Surface *char_surf);
    BMF_Char(const BMF_Char& orig);
    virtual ~BMF_Char();
    
    SDL_Surface* getSurf() const;
    unsigned char getCharacter() const;
    unsigned char getWidth() const;
    unsigned char getHeight() const;
    char getRelX() const;
    char getRelY() const;
    unsigned char getShift() const;
};

#endif	/* BMF_CHAR_H */

