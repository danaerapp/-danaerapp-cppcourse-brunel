#include <iostream>
#include <fstream>
#include <vector>
#include <array>

#include "Neurons.hpp"
#include "Experiment.hpp"


using namespace std;

int main(){
	
	Experiment ex;
	
	ex.run(1000); ///< Experiment::run(double t), t is in ms
	
	return 0;
}
