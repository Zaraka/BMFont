/* 
 * File:   BMF.cpp
 * Author: ZarakaAngel
 * 
 * Created on 23. prosinec 2012, 13:05
 */

#include <iostream>

#include "BMF.h"
#include "BMF_Char.h"

using namespace std;

BMF::BMF() {
    this->add_space = 0;
    this->count_of_colors = 0;
    this->info = "";
    this->line_height = 0;
    this->number_of_chars = 0;
    this->size_inner = 0;
    this->size_over_BL = 0;
    this->size_under_BL = 0;
    this->strlen_info = 0;
}

BMF::BMF(const BMF& orig) {
}

BMF::~BMF() {
    for (map<unsigned char, BMF_Char*>::iterator it = this->chars.begin(); it != this->chars.end(); ++it) {
        delete it->second;
    }
    this->chars.clear();
}

/**
 * 
 * @param filename
 * @return 
 */
bool BMF::load_font(std::string filename) {
    ifstream file;
    file.open(filename, ios::in | ios::binary);
    file.exceptions(ifstream::failbit | ifstream::badbit);
    if (!file.is_open()) {
        //throw EngineException("Cannot open BMF file!");
        Logger::Instance()->log(LOG_ERROR, "Cannot open BMF file '%s'\n", filename.c_str());
        return false;
    } else {
        //magic header
        char nothing[5];
        file.read(nothing, 4);
        //version
        file.read(nothing, 1);
        char buffer3;
        file.read(&buffer3, 1);
        this->line_height = (unsigned char) buffer3;
        file.read(&this->size_over_BL, 1);
        file.read(&this->size_under_BL, 1);
        file.read(&this->add_space, 1);
        file.read(&this->size_inner, 1);
        file.read(&buffer3, 1);
        this->count_of_colors = (unsigned char) buffer3;
        file.read(&buffer3, 1);
        this->highest_color = (unsigned char) buffer3;
        //works fine
        //reserved
        file.read(nothing, 4);
        //colors
        file.read(&buffer3, 1);
        this->rgb_entries = (unsigned char) buffer3;
        //insert blank color
        SDL_Color blank;
        blank.r = 255;
        blank.g = 0;
        blank.b = 255;
        this->colors.push_back(blank);
        for (int i = 0; i < this->rgb_entries; i++) {
            SDL_Color color;
            file.read(&buffer3, 1);
            color.r = (unsigned char) buffer3 * 4;
            file.read(&buffer3, 1);
            color.g = (unsigned char) buffer3 * 4;
            file.read(&buffer3, 1);
            color.b = (unsigned char) buffer3 * 4;
            this->colors.push_back(color);
        }
        //info string
        file.read(&buffer3, 1);
        this->strlen_info = (unsigned char) buffer3;
        char *buffer = (char*) malloc(this->strlen_info + 1);
        if (buffer == NULL) {
            //throw EngineException("Not enough memory");
            Logger::Instance()->log(LOG_ERROR, "Not Enough memory\n");
            return false;
        }
        file.read(buffer, this->strlen_info);
        buffer[this->strlen_info] = '\0';
        ;
        file.read(&buffer3, 1);
        this->number_of_chars = (unsigned char) buffer3;
        file.read(&buffer3, 1);
        this->number_of_chars += (buffer3 * 256);
        for (int i = 0; i < (int) this->number_of_chars; i++) {
            unsigned char character;
            unsigned char width;
            unsigned char height;
            char rel_x;
            char rel_y;
            unsigned char shift;
            SDL_Surface *char_surf;

            file.read(&buffer3, 1);
            character = (unsigned char) buffer3;
            file.read(&buffer3, 1);
            width = (unsigned char) buffer3;
            file.read(&buffer3, 1);
            height = (unsigned char) buffer3;
            file.read(&rel_x, 1);
            file.read(&rel_y, 1);
            //Logger::Instance()->log(LOG_DEBUG, "rel_x = %d, rel_y = %d\n", rel_x, rel_y);
            file.read(&buffer3, 1);
            shift = (unsigned char) buffer3;
            
            //now i need to create surface
            Uint32 rmask, gmask, bmask, amask;
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
            rmask = 0xff000000;
            gmask = 0x00ff0000;
            bmask = 0x0000ff00;
            amask = 0x00000000;
#else
            rmask = 0x000000ff;
            gmask = 0x0000ff00;
            bmask = 0x00ff0000;
            amask = 0x00000000;
#endif
            char_surf = SDL_CreateRGBSurface(0, width, height, 32, rmask, gmask, bmask, amask);
            if (char_surf == NULL) {
                //throw EngineException("Cannot create surface for character");
                Logger::Instance()->log(LOG_ERROR, "Cannot create Surface for '%c' character: %s\n", character, SDL_GetError());
                return false;
            }

            //and now I will fill the surface in loop..
            for (int h = 0; h < height; h++) {
                for (int w = 0; w < width; w++) {
                    unsigned char index_color;
                    file.read(&buffer3, 1);
                    index_color = (unsigned char) buffer3;
                    SDL_Rect rect;
                    rect.h = 1;
                    rect.w = 1;
                    rect.x = w;
                    rect.y = h;
                    //Logger::Instance()->log(LOG_DEBUG, "Filling %d %d with %d color\n", w, h, index_color);
                    if (SDL_FillRect(char_surf, &rect, SDL_MapRGB(char_surf->format, this->colors[index_color].r, this->colors[index_color].g, this->colors[index_color].b)) == -1) {
                        Logger::Instance()->log(LOG_ERROR, "Cannot SDL_FillRect on '%c' character: %s\n", character, SDL_GetError());
                    }
                }
            }
            BMF_Char *bmf_char = new BMF_Char(character, width, height, rel_x, rel_y, shift, char_surf);

            this->chars.insert(pair<unsigned char, BMF_Char*>(character, bmf_char));
        }
        file.close();
        return true;
    }
}

/**
 * @EngineInit
 * needed
 */
void BMF::printAllChars() {
    Logger::Instance()->log(LOG_INFO, "Font has %d characters\n", this->number_of_chars);
    for (map<unsigned char, BMF_Char*>::iterator it = this->chars.begin(); it != this->chars.end(); ++it) {
        Logger::Instance()->log(LOG_INFO, "Font has '%c'\n", it->first);
    }
}

int BMF::getTextLineHeight(std::string text) const {
    int height = 0;
    for (int i = 0; i < (signed int) text.length(); i++) {
        BMF_Char* character = this->chars.find(text.c_str()[i])->second;
        if ((character->getHeight() + character->getRelY()) > height) {
            height = character->getHeight() + character->getRelY();
        }
    }
    return height;
}

int BMF::getTextLineWidth(std::string text) const {
    int width = 0;
    for (int i = 0; i < (signed int) text.length(); i++) {
        BMF_Char* character = this->chars.find(text.c_str()[i])->second;
        width += character->getShift() + this->add_space;
    }
    return width;
}

SDL_Texture* BMF::getCharTexture(unsigned char character) {
    BMF_Char* bmf_char = this->chars.find(character)->second;
    SDL_Surface* final_surf = NULL;
    int width = bmf_char->getWidth(); //tested it's non zero
    int height = bmf_char->getHeight(); //tested its' non zero
    Uint32 rmask, gmask, bmask, amask;
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    rmask = 0xff000000;
    gmask = 0x00ff0000;
    bmask = 0x0000ff00;
    amask = 0x00000000;
#else
    rmask = 0x000000ff;
    gmask = 0x0000ff00;
    bmask = 0x00ff0000;
    amask = 0x00000000;
#endif
    final_surf = SDL_CreateRGBSurface(0, width, height, 32, rmask, gmask, bmask, amask); //this will pass with no errors

    SDL_Rect dest;
    dest.x = 0;
    dest.y = 0;
    dest.h = height;
    dest.w = width;

    SDL_BlitSurface(bmf_char->getSurf(), NULL, final_surf, &dest); //same, pass with no errors
    SDL_Texture* text = SDL_CreateTextureFromSurface(Engine::Instance()->getRenderer(), final_surf);
    return text;
}

SDL_Surface* BMF::getSurfLine(std::string text) const {
    SDL_Surface *final_surf = NULL;
    int width = this->getTextLineWidth(text);
    int height = this->getTextLineHeight(text);
    Uint32 rmask, gmask, bmask, amask;
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    rmask = 0xff000000;
    gmask = 0x00ff0000;
    bmask = 0x0000ff00;
    amask = 0x00000000;
#else
    rmask = 0x000000ff;
    gmask = 0x0000ff00;
    bmask = 0x00ff0000;
    amask = 0x00000000;
#endif
    final_surf = SDL_CreateRGBSurface(0, width, height, 32, rmask, gmask, bmask, amask);
    if (final_surf == NULL) {
        Logger::Instance()->log(LOG_ERROR, "Cannot create surface for text\n");
        //throw EngineException("Cannot create surface for text");
    }
    SDL_FillRect(final_surf, NULL, SDL_MapRGB(final_surf->format, 255, 0, 255));
    int start = 0;
    for (int i = 0; i < (signed int) text.length(); i++) {
        if(this->chars.find(text.c_str()[i]) == this->chars.end()){
            Logger::Instance()->log(LOG_WARN, "Character '%c' doesnt exist in this font\n", text.c_str()[i]);
            continue;
        }
        BMF_Char* character = this->chars.find(text.c_str()[i])->second;

        SDL_Rect dest;
        dest.x = start + character->getRelX();
        dest.y = character->getRelY();
        dest.w = character->getWidth();
        dest.h = character->getHeight();
        //Logger::Instance()->Instance()->log(LOG_DEBUG, "Drawing %dx%d '%c' at %d %d\n", dest.w, dest.h, text.c_str()[i], dest.x, dest.y);
        if (SDL_BlitSurface(character->getSurf(), NULL, final_surf, &dest) == -1) {
            Logger::Instance()->log(LOG_ERROR, "Cannot SDL_Blit: %s\n", SDL_GetError());
        }
        start += character->getShift() + this->add_space;
    }
    
    //text will be transparent
    if (SDL_SetColorKey(final_surf, SDL_TRUE, SDL_MapRGB(final_surf->format, 255, 0, 255)) == -1) {
        Logger::Instance()->log(LOG_ERROR, "Cannot SDL_SetColorKey: %s\n", SDL_GetError());
    }

    return final_surf;
}

SDL_Texture* BMF::getTextLine(std::string text) const {
    SDL_Surface *final_surf = this->getSurfLine(text);
    SDL_Texture* result = SDL_CreateTextureFromSurface(Engine::Instance()->getRenderer(), final_surf);
    if (result == 0) {
        Logger::Instance()->log(LOG_ERROR, "Cannot SDL_CreateTextureFromSurface: %s\n", SDL_GetError());
    }
    SDL_FreeSurface(final_surf);
    return result;
}