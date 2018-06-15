
// Copyright (C) 2018 Ian Dunn
// For conditions of distribution and use, see the LICENSE file


#pragma once

#include <glm/glm.hpp>
#include <math.h>

#include <Shading/BRDF.hpp>

#include <Scene/Object.hpp>
#include <Scene/Camera.hpp>
#include <Scene/Light.hpp>
#include <Scene/Scene.hpp>

#include "PixelContext.hpp"
#include "Params.hpp"
#include "Pixel.hpp"
#include "PixelContext.hpp"
#include "RayTraceResults.hpp"



struct LightingResults
{
	glm::vec3 diffuse;
	glm::vec3 specular;
};

class RayTracer
{

public:

	RayTracer(Scene * scene);

	void SetDebugContext(PixelContext * context);
	void SetParams(const Params & params);
	const Params & GetParams() const;
  
   glm::vec3 alignSampleVector( glm::vec3 sample,  glm::vec3  up,  glm::vec3  normal) const ;
   glm::vec3 generateCosineWeightedPoint(  float u,  float v, int radius) const ;
   std::vector<glm::vec3> generate_hemisphere_sample_points( int x,const glm::vec3 & normal, glm::vec3 up,int radius) const ;

   
   
	Pixel CastRaysForPixel(const glm::ivec2 & Pixel) const;
	RayTraceResults CastRay(const Ray & ray, const int depth, float bounce) const;
	glm::vec3 GetAmbientResults(const Object * const HhtObject, const glm::vec3 & point, const glm::vec3 & normal, const int depth, int bounce, glm::vec3 orgin) const;
	LightingResults GetLightingResults(const Light * const light, const Material & Material, const glm::vec3 & point, const glm::vec3 & view, const glm::vec3 & normal) const;
	glm::vec3 GetReflectionResults(const glm::vec3 & point, const glm::vec3 & reflection, const int depth, PixelContext::Iteration * currentIteration = nullptr) const;
	glm::vec3 GetRefractionResults(const Material & material, const glm::vec3 & point, const glm::vec3 & transmissionVector, const bool entering, const int depth, PixelContext::Iteration * currentIteration = nullptr) const;

	static glm::vec3 CalculateRefractionVector(const glm::vec3 & view, glm::vec3 normal, const float ior);

protected:

	Params params;
	Scene * scene = nullptr;
	BRDF * brdf = nullptr;

	PixelContext * context = nullptr;

	const float reflectionEpsilon = 0.001f;
	const float refractionEpsilon = 0.001f;

};
