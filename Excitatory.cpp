#include "Excitatory.hpp"

Excitatory::Excitatory()
: Neuron(Neuron::J_E)
{}

Excitatory::Excitatory(double p, double s, std::vector<double> t)
: Neuron(p,s,t,J_E)
{}
