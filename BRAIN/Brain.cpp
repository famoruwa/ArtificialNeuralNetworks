#include "stdafx.h"
#include "Neuron.h"
#include "Brain.h"
#include ".\brain.h"

void Brain::readInputFile(char* file, vector< vector< double > > & input)
{
	ifstream infile;
	infile.open (file, ifstream::in);

	int numInputs;
	int numValues;
	int value;
	char c;
	infile >> c;
	if(c == 'I')
	{
		infile >> numInputs;
	} else
	{
		return;
	}
	infile >> c;
	if(c == 'N')
	{
		infile >> numValues;
	} else
	{
		return;
	}

	int iter = 0;
	int inputNum = 0;
	input.resize(numInputs);
	for(int i = 0; i < numInputs; i++)
	{
		input[i] = vector<double>(numValues);
		for(int j = 0; j < numValues; j++)
		{
			infile >> input[i][j];
		}
	}
}

void Brain::readTargetFile(char* file, vector< vector< double > > & target)
{
	ifstream infile;
	infile.open (file, ifstream::in);

	int numTargets;
	int numValues;
	int value;
	char c;
	infile >> c;
	if(c == 'O')
	{
		infile >> numTargets;
	} else
	{
		return;
	}
	infile >> c;
	if(c == 'N')
	{
		infile >> numValues;
	} else
	{
		return;
	}

	int iter = 0;
	int targetNum = 0;
	target.resize(numTargets);
	for(int i = 0; i < numTargets; i++)
	{
		target[i] = vector<double>(numValues);
		for(int j = 0; j < numValues; j++)
		{
			infile >> target[i][j];
		}
	}
}

void randomize(vector< vector < double > > & vec, vector< vector< double > > & vec2)
{
	vector< double > temp;
	vector< double > temp2;
	for(int i = 0; i < vec.size()*1000; i++)
	{
		int x = floor(rng()*vec.size());
		int y = floor(rng()*vec.size());
		temp = vec[x];
		temp2 = vec2[x];
		vec[x] = vec[y];
		vec2[x] = vec2[y];
		vec[y] = temp;
		vec2[y] = temp2;
	}
}

Brain::Brain(void)
: myNumNeurons(0)
, myNumInputs(0)
{
}

Brain::Brain(int numNeurons, int numInputs, int numOutputs) 
{
	// numOutputs <= numNeurons
	myNumNeurons = numNeurons;
	myNumInputs = numInputs + 1;
	myNumOutputs = numOutputs;

	// first part of state is input, then comes neural state
	myCurrentState.resize(numNeurons + myNumInputs);

	int i;
	for(i = 0; i < numNeurons; i++) 
	{
		// note: Neurons do not care what came from input
		// or what came from another neuron
		Brain::myNeurons.push_back(new Neuron(myCurrentState));
	}
}

Brain::~Brain(void)
{
	int i;
	for(i = 0; i < Brain::myNeurons.size(); i++) 
	{
		delete Brain::myNeurons[i];
	}
}

// Function for forward propagating the network
void Brain::forwardPropagate(int numSteps, vector< double > input)
{
	myPreviousStates.clear();
	int i;
	for(i = 0; i < myNumInputs-1; i++)
	{
		// write over the inputs part of state vector
		myCurrentState[i] = input[i];
	}
	myCurrentState[myNumInputs-1] = 1;

	int step;
	for(step = 0; step < numSteps; step++)
	{
		myPreviousStates.push_back(myCurrentState);

		for(int i = 0; i < myNeurons.size(); i++)
		{
			myNeurons[i]->fire();
		}


		for(i = 0; i < myNeurons.size(); i++)
		{
			myCurrentState[i + myNumInputs] = myNeurons[i]->myValue;
		}

	}
}

// Function for back propagating the network, updates neural weights based on error vector
// note this depends on the current state of the network
void Brain::backPropagate(int numSteps, vector< double > & error, double learningRate, double noise)
{
	
	if (error.size() + myNumInputs > myCurrentState.size())
	{
		// this is bad
		return;
	}

	if( myPreviousStates.size() != numSteps )
	{
		return;
	}

	// error vector should match the currentState in size
	// this should become a state variable as well
	vector<double> errorVec;
	vector<double> outErrorVec;

	errorVec.resize(myCurrentState.size());
	outErrorVec.resize(myCurrentState.size());

	// no error on inputs I hope
	// we set outErrorVec to zero along the way
	int ii;
	for(ii = 0; ii < myNumInputs; ii++)
	{
		outErrorVec[ii] = 0.0;
		errorVec[ii] = 0.0;
	}

	// increment both ii and jj
	for(int jj = 0;  jj < error.size(); jj++, ii++)
	{
		outErrorVec[ii] = 0.0;
		errorVec[ii] = error[jj];
	}

	// no errors
	for(; ii < errorVec.size(); ii++)
	{
		outErrorVec[ii] = 0.0;
		errorVec[ii] = 0.0;
	}

	for(int i  = 0; i < myNeurons.size(); i++) 
	{
		myNeurons[i]->reset();
	}

	// note this really only valid for one step
	for( int step = 0; step < numSteps; ++step)
	{

		myCurrentState = myPreviousStates[numSteps - 1 - step];

		int i;

		for(i = 0; i < myNeurons.size(); i++) 
		{
			myNeurons[i]->myValue = myCurrentState[myNumInputs + i];
			// note each neuron will contribute to the out put error
			myNeurons[i]->backPropagate(errorVec[myNumInputs + i], learningRate, outErrorVec);
		}

		for(i = 0; i < errorVec.size(); ++i)
		{
			errorVec[i] = outErrorVec[i];
			outErrorVec[i] = 0.0;
		}

		
	}
	for(int i  = 0; i < myNeurons.size(); i++) 
	{
		myNeurons[i]->update(noise);
	}
}

// returns the state of the output neurons
void Brain::getOutput(vector< double> & output)
{
	output.resize(myNumOutputs);

	int i = 0;
	for(i = 0 ; i < myNumOutputs; i++)
	{
		if(myCurrentState[myNumInputs + i] > 1)
		{
			output[i] = 1;
		} else if(myCurrentState[myNumInputs + i] < -1)
		{
			output[i] = -1;
		} else
		{
			output[i] = myCurrentState[myNumInputs + i];
		}
	}
}

// Returns the current state of the brain
vector< double > & Brain::getCurrentState(void)
{
	return Brain::myCurrentState;
}

void Brain::setStates(double state) {
	for(int i = 0; i < myNeurons.size(); i++)
	{
		myNeurons[i]->myValue = state;
		myCurrentState[myNumInputs + i] = state;
	}
}

// prints the brain to the given stream
int Brain::print(ostream & stream)
{
	stream << "N " << myNumNeurons << endl;
	stream << "I " << myNumInputs << endl;
	stream << "O " << myNumOutputs << endl;
	for(int i = 0; i < myNeurons.size(); i++)
	{
		stream << "N " << i << endl;
		myNeurons[i]->print(stream);
	}
	return 0;
}

Brain::Brain(char* file) 
{
	ifstream infile;
	infile.open (file, ifstream::in);

	char c;

	infile >> c;
	if(c == 'N')
	{
		infile >> myNumNeurons;
	} else 
	{
		myNumNeurons = 0;
		cerr << "YIKES" << endl;
	}
	infile >> c;
	if(c == 'I')
	{
		infile >> myNumInputs;
	} else 
	{
		myNumInputs = 0;
		cerr << "YIKES" << endl;
	}
	infile >> c;
	if(c == 'O')
	{
		infile >> myNumOutputs;
	} else 
	{
		myNumOutputs = 0;
		cerr << "YIKES" << endl;
	}

	// first part of state is input, then comes neural state
	myCurrentState.resize(myNumNeurons + myNumInputs);

	vector<double> weights = vector<double>(myCurrentState.size(),0);
	int neuron = 0;

	while(infile >> c) 
	{
		if(c == 'N')
		{
			infile >> neuron;
			if(neuron >= myNumNeurons) {
				cerr << "YIKES" << endl;
				return;
			}
			for(int i = 0; i < weights.size(); i++)
			{
				infile >> c;
				if(c == 'W')
				{
					infile >> weights[i];
				} else
				{
					cerr << "YIKES" << endl;
					return;
				}
			}
			myNeurons.push_back(new Neuron(weights,myCurrentState));
		}

	}
}
