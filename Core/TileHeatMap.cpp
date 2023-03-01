#include "TileHeatMap.hpp"
#include "Engine/Math/RandomNumberGenerator.hpp"

TileHeatMap::TileHeatMap()
{
}

TileHeatMap::TileHeatMap(IntVec2 const& dimensions)
{
    m_dimesnions = dimensions;
	for (int i = 0; i < m_dimesnions.x * m_dimesnions.y; i++)
	{
		m_values.push_back(999.0f);
	}
}

bool TileHeatMap::isInBounds(IntVec2 const& tileCoords) const
{
    return  tileCoords.x >=0 && tileCoords.y >= 0 && tileCoords.x < m_dimesnions.x && tileCoords.y < m_dimesnions.y;
}

float TileHeatMap::GetHeatAt(IntVec2 const& tileCoords) const
{
	if (isInBounds(tileCoords))
	{
		return m_values[GetTileIndexForTileCoords(tileCoords)];
	}
	else return 999.0f;
}

float TileHeatMap::GetHighestHeat() const
{
	float highestHeat = 0.0f;
	for (int i = 0; i < m_dimesnions.x * m_dimesnions.y; i++)
	{
		if (m_values[i] > highestHeat)
		{
			highestHeat = m_values[i];
		}
	}
	return highestHeat;
}

float TileHeatMap::GetHighestHeatLessThan(float considerValueOnlyBelowThis) const
{
	float highestHeat = 0.0f;
	for (int i = 0; i < m_dimesnions.x * m_dimesnions.y; i++)
	{
		if (m_values[i] > highestHeat && m_values[i] < considerValueOnlyBelowThis)
		{
			highestHeat = m_values[i];
		}
	}
	return highestHeat;
}

void TileHeatMap::SetHeatAt(IntVec2 const& tileCoords, float newValue)
{
	if (isInBounds(tileCoords))
	{
		m_values[GetTileIndexForTileCoords(tileCoords)] = newValue;
	}

}

void TileHeatMap::AddHeatAt(IntVec2 const& tileCoords, float heatToAdd)
{
	m_values[GetTileIndexForTileCoords(tileCoords)] += heatToAdd;
}

void TileHeatMap::SetHeatEverywhere(float newValueForAllTiles)
{
	for (int i = 0; i < m_dimesnions.x * m_dimesnions.y; i++)
	{
		m_values[i] = newValueForAllTiles;
	}
}

IntVec2 TileHeatMap::GetRandomPositionInHeatMapLessThan(float maxHeatValue)
{
	RandomNumberGenerator rng;
	int  x = rng.GetRandomIntInRange(1, m_dimesnions.x - 1);
	int  y = rng.GetRandomIntInRange(1, m_dimesnions.y - 1);
	float heatValue = GetHeatAt(IntVec2(x, y));
	while (heatValue == maxHeatValue)
	{
		x = rng.GetRandomIntInRange(1, m_dimesnions.x - 1);
		y = rng.GetRandomIntInRange(1, m_dimesnions.y - 1);
		heatValue = GetHeatAt(IntVec2(x, y));
	}
	return IntVec2(x,y);
}

IntVec2 TileHeatMap::GetNextLowerPosition(IntVec2 referencePosition)
{
	IntVec2 returnValue;

	returnValue = referencePosition;

	IntVec2 top = IntVec2(0, 1);
	IntVec2 bottom = IntVec2(0, -1);
	IntVec2 left = IntVec2(-1, 0);
	IntVec2 right = IntVec2(1, 0);

	IntVec2 TopTile = referencePosition + top;
	IntVec2 bottomTile = referencePosition + bottom;
	IntVec2 leftTile = referencePosition + left;
	IntVec2 rightTile = referencePosition + right;
	returnValue = referencePosition;

	if (GetHeatAt(TopTile) <= GetHeatAt(referencePosition))
		returnValue = TopTile;
	else if (GetHeatAt(bottomTile) <= GetHeatAt(referencePosition))
		returnValue = bottomTile;
	else if (GetHeatAt(leftTile) <= GetHeatAt(referencePosition))
		returnValue = leftTile;
	else if (GetHeatAt(rightTile) <= GetHeatAt(referencePosition))
		returnValue = rightTile;

	return returnValue;
}

int TileHeatMap::GetTileIndexForTileCoords(IntVec2 const& tileCoords) const
{
	int index = 0;
	for (int y = 0; y < m_dimesnions.y ; y++ )
	{
		for (int x = 0; x < m_dimesnions.x;x++)
		{
			if (x == tileCoords.x && y == tileCoords.y)
			{
				return index;
			}
			index++;
		}
		
	}
	return index;

}
int TileHeatMap::GetTotalHeatValues()
{
	return (int)m_values.size();
}

IntVec2 TileHeatMap::GetDimensions() const
{
	return m_dimesnions;
}
