
// Copyright (C) 2018 Ian Dunn
// For conditions of distribution and use, see the LICENSE file


#pragma once

#include <glm/glm.hpp>


struct Params
{
	glm::ivec2 imageSize;

	bool useShading = true;
	bool useShadows = true;
	bool useCookTorrance = false;

	bool useReflections = true;
	bool useRefractions = true;
	bool useFresnel = false;
	bool useBeers = false;
	bool useGI = false;
   bool useSS = false;

	int recursiveDepth = 6;
	int superSampling = 1;
   int shadowSamples = 0;
   int shadowRadius = 1;

	bool useSpatialDataStructure = false;

	bool debugNormals = false;
};
