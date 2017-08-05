/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#ifndef BASEENT_CPP
#define BASEENT_CPP

using namespace std;

#include <cassert>
#include "biome.h"
#include "tyler.h"
#include "baseent.h"

Biome::Biome(Biome* inParent){
    parent = inParent;
    if(inParent != NULL){
        depth = inParent->depth + 1;
        tSrand(inParent->seed);
        seed = tRand();
    }
    
    red = green = blue = bumpiness = seed = 0;
}

vector< Biome* > Biome::getChildBiomes(){
    if(childBiomes.size() == 0){
        // Generate the child biomes randomly!
        // Currently to keep things simple, these are just completely random
        tSrand(seed);
        Biome biome0 = new Biome(this);
        biome0.red = rnd();
        biome0.green = rnd();
        biome0.blue = rnd();
        biome0.bumpiness = rnd();
        childBiomes.push_back(&biome0);

        Biome biome1 = new Biome(this);
        biome1.red = rnd();
        biome1.green = rnd();
        biome1.blue = rnd();
        biome1.bumpiness = rnd();
        childBiomes.push_back(&biome1);
    }
    return childBiomes;
}

void Biome::makeInitialBiome(){
    red = .6;
    green = .5;
    blue = .2;
    bumpiness = 1.0;
    seed = 0;
}



#endif
