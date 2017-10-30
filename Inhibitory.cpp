#include "Inhibitory.hpp"

Inhibitory::Inhibitory()
:Neuron(J_I)
{}

Inhibitory::Inhibitory(double p, double s, std::vector<double> t)
: Neuron(p,s,t,J_I)
{}
