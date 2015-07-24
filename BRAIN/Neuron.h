#pragma once

using namespace std;

class Neuron
{
public:
	Neuron(vector<double>&);
	Neuron(vector<double>&,vector<double>&);
	~Neuron(void);
	

private:
	// Contains the weights for the vector
	vector< double > myWeights;

	// I made this reference a part of the neuron so it
	// can access the state at any time
	vector<double>& currentState;

	vector< double > myDeltas;

public:
	// Returns the neurons value, based on the current state
	double fire();
	// The most recent value for the Neuron
	double myValue;
	// Used to adjust the weights for this neuron
	void  backPropagate(double error, double rate, vector< double > & errorVec);
	// Updates the weights with the delta
	void update(double noise);
	//Resets the the Neurons for back propagation
	void reset();
	// prints the neurons wieght to the stream
	int print(ostream & stream);
};