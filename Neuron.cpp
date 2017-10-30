#include "Neuron.hpp"

double Neuron::getPotential() const{
	return potential;
}

double Neuron::getSpikesNumber() const{
	return spikesNumber;
}

double Neuron::getTime(unsigned int i) const{
	return times[i];
}

double Neuron::getJ() const{
	return J;
}

void Neuron::setPotential(double i){
	potential = i;
}

void Neuron::addSpike(){
	spikesNumber+=1;
}

void Neuron::addTime(double t){
	times.push_back(t);
}
	
bool Neuron::isRefractory() const{
	if ( (temps_pause > 0) and (potential > Vth) and (temps_pause < (tauRef/h))){ // Si temps_pause == tauRef, on veut encore que le neurone puisse transmettre sosn potentiel (et donc être considéré comme non-refractaire)
		return true;
	}
	return false;
}
				
void Neuron::update(){
	
	++clock_;
	
	//Background noise
	static std::random_device rd;
	static std::mt19937 gen(rd());
	static std::poisson_distribution<> d(nu_ext*Neuron::h); ///< random external connection
	
	///Réfléchir à faire (clock_-tspike < tauRef), avec tspike=times.back()
	if (temps_pause > 0){ //Neuron is refractory 
		--temps_pause; //h est notre pas de temps
		
		if (temps_pause <= 0){
			potential=Vreset;
			
		}
		
	}else if (buffer[clock_%Dmax] > 0.0){ //Temps actuel modulo delay max+1 nous donne le nb de steps avant de recevoir le spike
		potential=Neuron::exphtau*potential + buffer[clock_%Dmax] + d(gen);
		
		buffer[clock_%Dmax] =0.0 ; //Reset the PSP
	}else{
		//Calcul du potentiel
		double new_potential(0.0);
		new_potential=exphtau*potential + d(gen);
		
		if (new_potential >= Vth){ //Le neurone spike
			addTime(clock_);
			addSpike();
			temps_pause=(tauRef/h);
		}
		
		potential=new_potential;
	}
}

void Neuron::receive(double J){
	buffer[(Dmax+clock_)%Dmax]+=J; //The neurone receives the potential of an other neuron's spike, D/H : transforms in nb of steps
}

Neuron::Neuron(double J)
: potential(Vreset), J(J), spikesNumber(0.0), temps_pause(0.0), clock_(0), Dmax(delay/h), buffer(Dmax,0.0)
{
	assert(buffer.size() == Dmax);
}

Neuron::Neuron(double p, double s, std::vector<double> t, double J)
:potential(p), spikesNumber(s), temps_pause(0.0), clock_(0), Dmax(delay/h), buffer(Dmax,0.0), J(J)
{
	for (unsigned int i(0); i<t.size();++i){
		times[i]=t[i];
	}
}
