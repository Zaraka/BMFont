/* 
 * File:   BMF.h
 * Author: ZarakaAngel
 *
 * Created on 23. prosinec 2012, 13:05
 */

#ifndef BMF_H
#define	BMF_H

#include <vector>
#include <fstream>
#include <sstream>
#include <map>

#include "Logger.h"
#include "Exception.h"
#include "SDL.h"

#include "BMF_Char.h"

class BMF_Char;

class BMF {
private:
    unsigned char line_height;
    char size_over_BL;
    char size_under_BL;
    char add_space;
    char size_inner;
    unsigned char count_of_colors;
    unsigned char highest_color;
    unsigned int number_of_chars;
    unsigned char rgb_entries;
    std::vector<SDL_Color> colors;
    std::map<unsigned char, BMF_Char*>chars;

    unsigned char strlen_info;
    std::string info;

public:
    BMF();
    BMF(const BMF& orig);
    virtual ~BMF();

    bool load_font(std::string filename);
    SDL_Texture* getTextLine(std::string text) const;
    SDL_Surface* getSurfLine(std::string text) const;
    int getTextLineHeight(std::string text) const;
    int getTextLineWidth(std::string text) const;
    SDL_Texture* getCharTexture(unsigned char character);
    
    void printAllChars();

};

#endif	/* BMF_H */

