#include "RandomWalk.h"

RandomWalkGenerator::RandomWalkGenerator(int size, double start, double step): m_numSteps(size), m_initialPrice(start), m_stepSize(step) {}

RandomWalkGenerator::RandomWalkGenerator(const RandomWalkGenerator& other): m_numSteps(other.m_numSteps), m_initialPrice(other.m_initialPrice), m_stepSize(other.m_stepSize) {}

RandomWalkGenerator::~RandomWalkGenerator(){}

RandomWalkGenerator& RandomWalkGenerator::operator=(const RandomWalkGenerator& other)
{
	if (this != &other)
	{
		m_numSteps = other.m_numSteps;
		m_initialPrice = other.m_initialPrice;
		m_stepSize = other.m_stepSize;
	}

	return *this;
}

double RandomWalkGenerator::computeRandomStep(double currentprice)
{
	const int num_directions = 2;
	int r = rand() % num_directions;
	double val = currentprice;
	if (r == 0)
	{
		val += (m_stepSize * val);
	}
	else if (r == 1)
	{
		val -= (m_stepSize * val);
	}
	return val;
}

std::vector<double> RandomWalkGenerator::generateWalk()
{
	std::vector<double> walk;
	double prev = m_initialPrice;

	for(int i = 0; i < m_numSteps; ++i)
	{
		double val = computeRandomStep(prev);
		walk.push_back(val);
		prev = val;
	}
	return walk;
}