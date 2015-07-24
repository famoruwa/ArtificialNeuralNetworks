#include "stdafx.h"
#include "Neuron.h"
#include ".\neuron.h"

// just a note here
// using a reference instead of a pointer is only a matter of taste
Neuron::Neuron(vector<double>& stateVec) : myValue(0), currentState(stateVec)
{
	myWeights.resize(stateVec.size());

	// randomize weights
	int index;
	for(index = 0; index < myWeights.size(); ++index)
	{
		if(rng() > 0.5)
		{
			myWeights[index] = 4.0 * rng();
		} else {
			myWeights[index] = - 4.0 * rng();
		}

		/*if(rng() > 0.5)
		{
			myWeights[index] = 1;
		} else {
			myWeights[index] = -1;
		}*/
	}
	
}


Neuron::~Neuron(void)
{
}

// Returns the neurons value, based on the current state
double Neuron::fire()
{
	Neuron::myValue = 0;

	int i;
	for(i = 0; i < myWeights.size(); i++) {
		/*if(myWeights[i] > 1000) {
			cout << "Weight: " << myWeights[i] << endl;
		}*/
		/*if(currentState[i] > 1) 
		{
			myValue += myWeights[i];
		} else if(currentState[i] < -1)
		{
			myValue -= myWeights[i];
		} else 
		{
			myValue += myWeights[i]*currentState[i];
		}*/
		myValue += myWeights[i]*currentState[i];
	}

	//myValue = 2.0/(exp(-1.0*myValue) + 1.0) - 1.0;

	if(myValue > 1) {
		myValue = 1;
	}
	if(myValue < -1) {
		myValue = -1;
	}

	return myValue;
}

// Used to adjust the weights for this neuron
void  Neuron::backPropagate(double error, double rate, vector< double > & errorVec)
{
	// back propogate the error to the weights... and beyond

	/*

	E = incoming error

	Val = Weights*IncomingState

	if 

	E^2 = |Val - ActualVal|^2

	D(E^2)/DW = 2*E * D(Val)/DW = 2*E*IncomingState

	So we want to purturb each weight by an amounr rate * E * IncomingState

	To go the extra step we need to propogate the error back

	If we write out one more iteration

	Val = W_i * Sigma( N_i)

	where N_i is the neural state of the previous time step 
	(I am sticking in the Sigmoid function for clarity)
	so if we want to apply the chain rule

	D(Val)/D(N_i) = W_i * Sigma'

	This is the error we should pass back to the Neurons.
	*/

	// delta Weight[i]

	myDeltas.resize(myWeights.size());
	
	int ii;
	for(ii = 0; ii < myWeights.size(); ++ii)
	{
		double state = currentState[ii];
		double delta = error * rate * state;

		myDeltas[ii] += delta;

		/*double rand = (delta/10)*rng();
		if(delta <= 0) 
		{
			myDeltas[ii] += delta - rand;
		} else 
		{
			myDeltas[ii] += delta + rand;
		}*/
	}

	// compute error to back propogate sigma == identity for these neurons
	// NOTE we add to the error
	double sigma = 0;
	//sigma = ((exp(myValue/2)) + (exp((-1)*(myValue/2))));
	if(myValue < 0.99999 && myValue > -0.99999) 
	{
		sigma = 1;
	}
	for(ii = 0; ii < myWeights.size(); ++ii)
	{
		errorVec[ii] += error  * myWeights[ii] * sigma; // * sigma'(myValue) if we had a sigma
	}
}

void Neuron::update(double noise) {
	for(int i = 0; i < myWeights.size(); i++)
	{
		double delta = myDeltas[i];
		delta += delta*((rng()+rng()+rng()+rng()+rng())/5-1/2)*noise;
		myWeights[i] += delta; 
	}
}

void Neuron::reset() {
	for(int i = 0; i < myDeltas.size(); i++)\
	{
		myDeltas[i] = 0;
	}
}

// prints the neurons wieght to the stream
int Neuron::print(ostream & stream)
{
	for(int i = 0; i != myWeights.size(); i++)
	{
		stream << "W " << myWeights[i] << endl;
	}
	return 0;
}

Neuron::Neuron(vector< double > & weights, vector< double > & stateVec) : myValue(0), currentState(stateVec)
{
	myWeights.resize(stateVec.size());
	if(weights.size() != myWeights.size())
	{
		cerr << "YIKES" << endl;
		return;
	}

	// randomize weights
	int index;
	for(index = 0; index < myWeights.size(); ++index)
	{
		myWeights[index] = weights[index];
	}
}