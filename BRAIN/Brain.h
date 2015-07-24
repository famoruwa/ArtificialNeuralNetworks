#pragma once

using namespace std;

#include "Neuron.h"

class Brain
{
public:
	Brain(void);
	Brain(int numNeurons, int numInputs, int numOutputs);
	Brain(char* file);
	~Brain(void);
private:
	// vector of Neurons
	vector< Neuron* > myNeurons;
	// The current state of the brain
	vector< double > myCurrentState;
	// Contains pointers to the output neurons
	vector< vector< double > > myPreviousStates;
public:
	// Function for forward propagating the network
	void forwardPropagate(int numSteps, vector< double > input);
	// Function for back propagating the network, updates neural weights based on error vector
	void backPropagate(int numSteps, vector< double > & error, double learningRate, double noise);
	// returns the state of the output neurons
	void getOutput(vector< double > & output);
	// sets the neurons states
	void setStates(double state);
	// Number of Neurons is the brain
	int myNumNeurons;
	// Number of inputs to the brain
	int myNumInputs;
	// Number of Neurons used for output
	int myNumOutputs;
	void static readInputFile(char* file, vector< vector< double > > & input);
	void static readTargetFile(char* file, vector< vector< double > > & target);
public:
	// Returns the current state of the brain
	vector< double > & getCurrentState(void);
	// prints the brain to the given stream
	int print(ostream & stream);
};
