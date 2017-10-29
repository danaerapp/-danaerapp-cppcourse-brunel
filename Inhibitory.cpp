#include "Inhibitory.hpp"

Inhibitory::Inhibitory()
:Neuron(0.5)
{}

Inhibitory::Inhibitory(double p, double s, std::vector<double> t)
: Neuron(p,s,t,0.5)
{}
