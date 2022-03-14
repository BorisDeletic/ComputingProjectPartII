//
// Created by boris on 10/03/2022.
//

#include <iostream>
#include <algorithm>
#include "simulation.h"

using namespace std;

Simulation::Simulation(int n)
    :
    engine(n),
    lattice(n),
    n(n)
{
}



void Simulation::run(int timeSteps)
{
    for (int i = 0; i < timeSteps; i++) {
        Lattice nextLattice = engine.timeStep(lattice);

        float eq = engine.fractionSpinsFlipped(lattice, nextLattice);

        flips.push_back(eq);
        magnetisations.push_back(lattice.magnetisation());

        printf("eq = %f, mag = %f\n", eq, lattice.magnetisation());

        lattice = nextLattice;
    }
}


int Simulation::timeToEquilibrium() {
/*
 * Calculate the number of steps before magnetisations stabilises -> equilibrium
 * We consider magnetisations stabilised when they does not fluctuate more than 1% from mean
 * For window steps.
 */
    const int windowSize = 20;
    const float fluctuationThreshold = 0.01;

    /* for (int i = 0; i < magnetisations.size() - windowSize; i++) {
         auto start = magnetisations.begin() + i;
         auto end = magnetisations.begin() + i + windowSize; */
     for (int i = 0; i < flips.size() - windowSize; i++) {
         auto start = flips.begin() + i;
         auto end = flips.begin() + i + windowSize;

/*        vector<double> absMags;
        for (auto it = start; it < end; it++)
        {
            absMags.push_back(abs(*it));
        }

        float meanFlips = reduce(absMags.begin(), absMags.end()) / windowSize;
        float maxFlips  = *max_element(absMags.begin(), absMags.end());
        float minFlips  = *min_element(absMags.begin(), absMags.end()); */
         float meanFlips = reduce(start, end) / windowSize;
         float maxFlips  = *max_element(start, end);
         float minFlips  = *min_element(start, end);

        if (
                (maxFlips - meanFlips) / meanFlips < fluctuationThreshold &&
                (meanFlips - minFlips) / meanFlips < fluctuationThreshold)
        {
            return i;
        }
    }

    // equilibrium conditions not reached
    printf("Equilibrium not reached");
    throw std::exception();

}




double Simulation::autoCovariance(int t_start, int tau)
{
    double meanMagnetisation = reduce(magnetisations.begin() + t_start, magnetisations.end()) / (magnetisations.size() - t_start);

    vector<double> covariance;

    for (int i = t_start; i < magnetisations.size() - tau; i++)
    {
        double magP = magnetisations[i] - meanMagnetisation;
        double magP_tau = magnetisations[i + tau] - meanMagnetisation;

        covariance.push_back(magP * magP_tau);
    }

    double meanCov = reduce(covariance.begin(), covariance.end()) / covariance.size();

    return meanCov;
}


vector<double> Simulation::autoCorrelations()
{
  //  int t_eq = timeToEquilibrium();
    int t_eq = 0;
    double autoCov0 = autoCovariance(t_eq, 0);

    vector<double> autoCor;

    float maxTau = (magnetisations.size() - t_eq) * correlationCutoff;

    for (int tau = 0; tau < maxTau; tau++)
    {
        autoCor.push_back(autoCovariance(t_eq, tau) / autoCov0);
    }

    return autoCor;
}



ostream &operator<<(ostream &o, const Simulation &s) {
    o << "Simulation Parameters:" << endl;
    o << "n = " << s.n << endl;
    o << "steps = " << s.magnetisations.size() << endl;
    return o;
}
