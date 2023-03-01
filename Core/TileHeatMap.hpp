#pragma once
#include "Engine/Math/IntVec2.hpp"
#include <vector>
class TileHeatMap
{
public:
	TileHeatMap();
	TileHeatMap(IntVec2 const& dimensions);
	bool					isInBounds(IntVec2 const& tileCoords)						const;
	float					GetHeatAt(IntVec2 const& tileCoords)						const;
	float					GetHighestHeat()											const;
	float					GetHighestHeatLessThan(float considerValueOnlyBelowThis)	const;
	void					SetHeatAt(IntVec2 const& tileCoords, float newValue);
	void					AddHeatAt(IntVec2 const& tileCoords, float heatToAdd);
	void					SetHeatEverywhere(float newValueForAllTiles);
	IntVec2					GetRandomPositionInHeatMapLessThan(float maxHeatValue);
	IntVec2					GetNextLowerPosition(IntVec2 referencePosition);
	int					    GetTotalHeatValues();
	IntVec2					GetDimensions()												const ;

protected:
	int						GetTileIndexForTileCoords(IntVec2 const& tileCoords)		const;
	IntVec2					m_dimesnions = IntVec2::ZERO;
	std::vector<float>		m_values;
};

