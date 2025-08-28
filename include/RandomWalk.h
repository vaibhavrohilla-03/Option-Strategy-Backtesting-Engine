#pragma once

#ifndef RANDOM_WALK_H
#define RANDOM_WALK_H

#include <vector>

class RandomWalkGenerator
{
private:
	
	int m_numSteps;
	double m_stepSize;
	double m_initialPrice;

public:

	RandomWalkGenerator(int size, double start, double step);

	RandomWalkGenerator(const RandomWalkGenerator& other );

	~RandomWalkGenerator();

	RandomWalkGenerator& operator=(const RandomWalkGenerator& other);

	std::vector<double> generateWalk();

	double computeRandomStep(double currentprice);
};
#endif 
