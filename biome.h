/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   biome.h
 * Author: tyler
 *
 * Created on August 4, 2017, 4:28 PM
 */

#ifndef BIOME_H
#define BIOME_H

#include "templates.h"
#include "defines.h"
using namespace std;

// These constants are to make things simple, by propagating all biomes exactly the same way.

// A biome will apply to this many generations of vertices when they split
#define APPLY_NUM_GENERATIONS = 3

// A biome's children will start to apply after this many generations
#define GENERATIONS_BEFORE_CHILD_RULES_APPLY = 2

class Biome
{
public:

    // The parent biome
    Biome* parent;
    // The depth in the biome tree. Root biome is 0, first level children are 1, etc.
    int depth;
    // Store a seed for random generation
    int seed;
    // Use a getter to return the child biomes. So that we can generate them if they don't exist yet.
    vector< Biome* > getChildBiomes();

    Biome(Biome* parent);
    
    void makeInitialBiome();

    // Stuff that affects the way the biome looks:
    float red, green, blue;
    // TODO: Make this a lot more flexible
    float bumpiness;
    
private:
    // Child biomes. To keep it simple for now, there are always just two.
    vector< Biome* > childBiomes;
};



#endif /* BIOME_H */

