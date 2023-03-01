#include "RandomNumberGenerator.hpp"
#include "stdlib.h"


RandomNumberGenerator::RandomNumberGenerator()
{
}

int RandomNumberGenerator::GetRandBetweenTwoInts(int integerOne, int integerTwo)
{
	return rand() % 2 == 1 ? integerOne : integerTwo;
}

int RandomNumberGenerator::GetRandomIntLessThan(int maxNotInclusive)
{
	return rand() % maxNotInclusive;
}

int RandomNumberGenerator::GetRandomIntInRange(int minInclusive, int maxInclusive)
{
	return rand() % (maxInclusive - minInclusive + 1) + minInclusive;
}

int RandomNumberGenerator::GetRandomIntInRange(int minInclusive, int maxInclusive, std::vector<int> ignoreListOfInts)
{
	int returnValue = rand() % (maxInclusive - minInclusive + 1) + minInclusive;
	if (std::find(ignoreListOfInts.begin(), ignoreListOfInts.end(), returnValue) != ignoreListOfInts.end())
	{
		returnValue = GetRandomIntInRange(minInclusive, maxInclusive, ignoreListOfInts);
	}
	return returnValue;
}

float RandomNumberGenerator::GetRandomFloatZeroToOne()
{
	return ((float)rand()) / ((float)RAND_MAX);
}

float RandomNumberGenerator::GetRandomFloatInRange(float minInclusive, float maxInclusive)
{
	float random = (float)rand() / (float)RAND_MAX;
	return minInclusive + (random * (maxInclusive - minInclusive));
}

float RandomNumberGenerator::RollRandomFloatZeroToOne()
{
	return GetRandomFloatInRange(0.0f,1.0f);
}
