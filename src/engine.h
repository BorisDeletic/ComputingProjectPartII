//
// Created by boris on 10/03/2022.
//

#ifndef ISINGMODEL_ENGINE_H
#define ISINGMODEL_ENGINE_H

#include <random>
#include "lattice.h"

class Engine {
public:
    Engine(int n);

    Lattice timeStep(Lattice& old);

    void setTemperature(float temperature) {T = temperature; };
    void setHField(float field) {H = field; };

    float fractionSpinsFlipped(Lattice& oldLattice, Lattice& newLattice);
private:
    void calculateNewSpinsMC(vector<vector<int>>& newSpins);
    int flipSpin(int s0, vector<int> neighbours);

    int n;
    float T;
    float H;

    random_device rd;  // Will be used to obtain a seed for the random number engine
    mt19937 gen; // Standard mersenne_twister_engine seeded with rd()
    uniform_real_distribution<> tempRNG; //random numbers for temperature effect
    uniform_int_distribution<> spinRNG; //rng for selecting lattice site
};


#endif //ISINGMODEL_ENGINE_H
