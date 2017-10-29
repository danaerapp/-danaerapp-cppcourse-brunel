#include "Excitatory.hpp"

Excitatory::Excitatory()
: Neuron(0.1)
{}

Excitatory::Excitatory(double p, double s, std::vector<double> t)
: Neuron(p,s,t,0.1)
{}
