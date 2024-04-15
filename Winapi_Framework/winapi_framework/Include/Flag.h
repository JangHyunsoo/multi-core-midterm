#pragma once

enum class SCENE_CREATE {
	SC_CURRENT,
	SC_NEXT,
};

enum class TILE_TYPE {
	AIR,
	LAND,
	STONE,
	WATER,
};

enum class ALGORITHM_TYPE {
	NONE,
	PERLIN,
	CELLULAR_KEY,
	CELLULAR_UPDATE,
	MERGE,
	GAME
};

