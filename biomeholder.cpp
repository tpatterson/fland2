/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   BiomeHolder.cpp
 * Author: tyler
 * 
 * Created on August 4, 2017, 6:12 PM
 */

#include "biomeholder.h"

Biome* BiomeHolder::getBiome(){ return biome; }
void BiomeHolder::setBiome(Biome* b){ biome = b; }
intensityType BiomeHolder::getIntensity(){ return intensity; }
void BiomeHolder::setIntensity(intensityType i){ intensity = i; }
BiomeHolder::BiomeHolder(){}
