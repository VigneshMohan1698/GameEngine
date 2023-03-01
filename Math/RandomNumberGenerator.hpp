#pragma once
#include <vector>
class RandomNumberGenerator
{
public:
	RandomNumberGenerator();
	int GetRandBetweenTwoInts(int integerOne, int integerTwo);
	int GetRandomIntLessThan(int maxNotInclusive);
	int GetRandomIntInRange(int minInclusive, int maxInclusive);
	int GetRandomIntInRange(int minInclusive, int maxInclusive, std::vector<int> ignoreListOfInts);
	float GetRandomFloatZeroToOne();
	float GetRandomFloatInRange(float minInclusive, float maxInclusive);
	float RollRandomFloatZeroToOne();
};