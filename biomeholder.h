/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   BiomeHolder.h
 * Author: tyler
 *
 * Created on August 4, 2017, 6:12 PM
 */

#ifndef BIOMEHOLDER_H
#define BIOMEHOLDER_H

#include "defines.h"
#include "biome.h"

/*
 A way to point to a biome and it's intensity
 */
class BiomeHolder {
private:
    Biome* biome;
    intensityType intensity;
    
public:
    Biome* getBiome();
    void setBiome(Biome* b);
    intensityType getIntensity();
    void setIntensity(intensityType i);
    BiomeHolder();
};

#endif /* BIOMEHOLDER_H */

