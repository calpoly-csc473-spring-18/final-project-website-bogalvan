
// Copyright (C) 2018 Ian Dunn
// For conditions of distribution and use, see the LICENSE file


#include "RayTracer.hpp"

#include <Scene/Scene.hpp>
#include <math.h>
#include <Shading/BlinnPhongBRDF.hpp>
#include <Shading/CookTorranceBRDF.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/transform.hpp>
#include <iostream>

RayTracer::RayTracer(Scene * scene)
{
	this->scene = scene;
}


void RayTracer::SetDebugContext(PixelContext * context)
{
	this->context = context;
}

void RayTracer::SetParams(const Params & params)
{
	this->params = params;

	if (params.useCookTorrance)
	{
		brdf = new CookTorranceBRDF();
	}
	else
	{
		brdf = new BlinnPhongBRDF();
	}

	if (params.useSpatialDataStructure)
	{
		scene->BuildSpatialDataStructure();
	}
}

const Params & RayTracer::GetParams() const
{
	return params;
}

Pixel RayTracer::CastRaysForPixel(const glm::ivec2 & pixel) const
{
	glm::vec3 color = glm::vec3(0.f);

	for (int i = 0; i < params.superSampling; ++ i)
	{
		for (int j = 0; j < params.superSampling; ++ j)
		{
			Ray const ray = scene->GetCamera().GetPixelRay(pixel, params.imageSize, glm::ivec2(i, j), params.superSampling);

			if (context)
			{
				context->iterations.push_back(new PixelContext::Iteration());
				context->iterations.back()->type = PixelContext::IterationType::Primary;
				context->iterations.back()->parent = nullptr;
			}
         float bounce = 64;
			color += CastRay(ray, params.recursiveDepth,bounce).ToColor();
		}
	}

	color /= glm::pow((float) params.superSampling, 2.f);

	return Pixel(color);
}

RayTraceResults RayTracer::CastRay(const Ray & ray, const int depth, float bounce) const
{
	const float surfaceEpsilon = 0.001f;

	RayTraceResults results;

	const RayHitResults hitResults = scene->GetRayHitResults(ray);
	const Object * hitObject = hitResults.object;

	PixelContext::Iteration * contextIteration = nullptr;

	if (context)
	{
		contextIteration = context->iterations.back();
		contextIteration->ray = ray;
	}

	if (hitObject)
	{
		const Material & material = hitObject->GetMaterial();

		/////////////
		// Vectors //
		/////////////

		const glm::vec3 point = results.intersectionPoint = hitResults.point;
		const glm::vec3 view = -glm::normalize(ray.direction);
		const glm::vec3 normal = glm::normalize(hitResults.normal);

		const bool insideShape = glm::dot(normal, view) < 0.f;
		const glm::vec3 surfacePoint = insideShape ? point - normal*surfaceEpsilon : point + normal*surfaceEpsilon;


		///////////////////
		// Contributions //
		///////////////////

		float const f0 = pow((material.finish.ior - 1) / (material.finish.ior + 1), 2.f);
		float const fresnelReflectance = params.useFresnel ? f0 + (1 - f0) * pow(1 - glm::abs(glm::dot(normal, view)), 5.f) : 0.f;

		float const localContribution = (1.f - material.filter) * (1.f - material.finish.reflection);
		float reflectionContribution = (1.f - material.filter) * (material.finish.reflection) + material.filter * fresnelReflectance;
		float transmissionContribution = material.filter * (1 - fresnelReflectance);


		///////////////////
		// Local Shading //
		///////////////////

		results.ambient = localContribution * GetAmbientResults(hitObject, point, normal, depth,bounce,results.intersectionPoint);
      
      float shadowFactor = 1.0f;
      float radius = params.shadowRadius;
         
		for (Light * light : scene->GetLights())
		{
         //if we are using shadow
         //set in shadow to isLightOccluddded
			 const bool inShadow = params.useShadows ? 
				scene->IsLightOccluded(surfacePoint, light->position, contextIteration) :
				false;
         
            if(params.useSS){ //if soft shadows is on
               //get vector of n random samples
               std::vector<glm::vec3> samplePts; //vector of random samples
               std::vector<glm::vec3> pts; //vector of random samples
               samplePts.push_back(light->position);
               //make random points
               
          
               float u;
               float v;
               
               int nSamples = ceil(params.shadowSamples/9.0);
               for(int i =0; i<nSamples; i++){
                  //0-3.33 x 0-3.33 y
                  u = (float) rand() / (float) RAND_MAX *.33;
                  v = (float) rand() / (float) RAND_MAX *.33;
                  pts.push_back(glm::vec3(u,v,0));
                  //0-3.33 x 3.33-6.66 y
                  u = (float) rand() / (float) RAND_MAX *.33;
                  v = (float) rand() / (float) RAND_MAX *.33 + .33;
                  pts.push_back(glm::vec3(u,v,0));
                  //0-3.33 x 6.66-1 y
                  u = (float) rand() / (float) RAND_MAX *.33;
                  v = (float) rand() / (float) RAND_MAX *.33 + .64;
                  pts.push_back(glm::vec3(u,v,0));
                  //3.33-6.66 x 0-3.33 y
                  u = (float) rand() / (float) RAND_MAX *.33 + .33;
                  v = (float) rand() / (float) RAND_MAX *.33;
                  pts.push_back(glm::vec3(u,v,0));
                  //3.33-6.66 x 3.33-6.66 y
                  u = (float) rand() / (float) RAND_MAX *.33 + .33;
                  v = (float) rand() / (float) RAND_MAX *.33 + .33;
                  pts.push_back(glm::vec3(u,v,0));
                  //3.33-6.66 x 6.66-1 y 
                  u = (float) rand() / (float) RAND_MAX *.33 + .33;
                  v = (float) rand() / (float) RAND_MAX *.33 + .64;
                  pts.push_back(glm::vec3(u,v,0));
                  //6.66-1 x 0-3.33 y
                  u = (float) rand() / (float) RAND_MAX *.33 + .64;
                  v = (float) rand() / (float) RAND_MAX *.33;
                  pts.push_back(glm::vec3(u,v,0));
                  //6.66-1 x 3.33-6.66 y
                  u = (float) rand() / (float) RAND_MAX *.33 + .64;
                  v = (float) rand() / (float) RAND_MAX *.33 + .33;
                  pts.push_back(glm::vec3(u,v,0));
                  //6.66-1 x 6.66-1 y
                  u = (float) rand() / (float) RAND_MAX *.33 + .64;
                  v = (float) rand() / (float) RAND_MAX *.33 + .64;
                  pts.push_back(glm::vec3(u,v,0));
               }

               for(glm::vec3 p : pts){
                  u = p.x;
                  v = p.y;
                  glm::vec3 pt; 
                  float phi = acos(1-2*u);
                  float theta = 2* M_PI* v;
                  pt.x = sin(phi)*cos(theta) *radius +light->position.x;
                  pt.y= sin(phi)*sin(theta) *radius +light->position.y;
                  pt.z = cos(phi) *radius +light->position.z;
                  samplePts.push_back(pt);
               }
                         
               
               float hitLightCount = 0;
               for(int i =0; i < params.shadowSamples; i++){
                  if(!scene->IsLightOccluded(surfacePoint, samplePts[i], contextIteration)){
                     hitLightCount ++;                
                  }          
               }
               shadowFactor = hitLightCount/params.shadowSamples;
            }
         
         
			if((!inShadow && !params.useSS) || params.useSS){
				const LightingResults lighting = GetLightingResults(light, hitObject->GetMaterial(), point, view, normal);

				results.diffuse += localContribution * lighting.diffuse*shadowFactor;
				results.specular += localContribution * lighting.specular*shadowFactor;
		   }
		}


		////////////////
		// Refraction //
		////////////////

		if (params.useRefractions && transmissionContribution > 0.f)
		{
			const glm::vec3 transmissionVector = CalculateRefractionVector(view, normal, material.finish.ior);
			const bool entering = glm::dot(normal, view) >= 0.f;

			if (transmissionVector == glm::vec3(0.f))
			{
				// CalculateRefractionVector() returns the zero-vector in the case of total internal reflection

				reflectionContribution += transmissionContribution;
				transmissionContribution = 0.f;

				if (contextIteration)
				{
					contextIteration->extraInfo += " total-internal-reflection";
				}
			}
			else
			{
				const glm::vec3 transmissionColor = GetRefractionResults(material, entering ? point - normal*surfaceEpsilon : point + normal*surfaceEpsilon, transmissionVector, entering, depth, contextIteration);
				results.refraction = transmissionContribution * transmissionColor;
			}
		}


		/////////////////
		// Reflections //
		/////////////////

		if (params.useReflections && reflectionContribution > 0.f)
		{
			const glm::vec3 reflectionVector = glm::normalize(normal * glm::dot(view, normal) * 2.f - view);
			const glm::vec3 reflectionColor = material.color * GetReflectionResults(surfacePoint, reflectionVector, depth, contextIteration);
			results.reflection = reflectionContribution * reflectionColor;
		}


		//////////////
		// Finalize //
		//////////////

		if (contextIteration)
		{
			contextIteration->hitObject = hitResults.object;
			contextIteration->hitNormal = hitResults.normal;
			contextIteration->hitTime = hitResults.t;
			contextIteration->results = results;
			contextIteration->contributions = glm::vec3(localContribution, reflectionContribution, transmissionContribution);
		}

		if (params.debugNormals)
		{
			results.ambient = glm::vec3(0.f);
			results.specular = glm::vec3(0.f);
			results.reflection = glm::vec3(0.f);
			results.refraction = glm::vec3(0.f);

			results.diffuse = normal / 2.f + 0.5f;
		}
	}

	return results;
}

glm::vec3 RayTracer::GetAmbientResults(const Object * const hitObject, const glm::vec3 & point, const glm::vec3 & normal, const int depth,int bounce, glm::vec3 origin) const
{
   if(!params.useGI){
      if (params.useShading)
      {
         return hitObject->GetMaterial().finish.ambient * hitObject->GetMaterial().color;
      }
      else
      {
         return glm::vec3(0.f);
      }
   }
   else{
      glm::vec3 ambient = glm::vec3(0.f);
      //generate random points
      if(bounce >= 16){
         std::vector<glm::vec3> samplePts;
         glm::vec3 up = glm::vec3(0,0,1);
         samplePts = generate_hemisphere_sample_points(bounce,normal,up,1);
         for(int i =0; i < bounce; i ++){
            origin+= (samplePts[i] * .001f);
            Ray r = Ray(origin, samplePts[i]);
            ambient += RayTracer::CastRay(r, depth-1, bounce/4).ToColor() * glm::dot(samplePts[i], normal);
         }
      }else{
        return glm::vec3(0.f); 
      }
      ambient *= (1.0f/bounce);
      return ambient;
   } 
}

std::vector<glm::vec3> RayTracer::generate_hemisphere_sample_points( int x,const glm::vec3 & normal, glm::vec3 up,int radius) const {
   std::vector<glm::vec3> points;
   for(int i =0; i <x; i ++){
      const float u = (rand() / (float) RAND_MAX);
      const float v = (rand() / (float) RAND_MAX);
      glm::vec3 pt  = generateCosineWeightedPoint(u, v, radius);
      pt = alignSampleVector(pt,up , normal);
      points.push_back(pt);
   }
   return points;
}

glm::vec3 RayTracer::generateCosineWeightedPoint(float u,float v, int radius) const {
    glm::vec3 pt;
    
    float radial = sqrt(u);
    float theta = 2.0 * M_PI * v;

    float x = radial * cos(theta);
    float y = radial * sin(theta);

    pt.x = x;
    pt.y = y;
    pt.z = sqrt(1 - u);
    
    return pt;
}

glm::vec3 RayTracer::alignSampleVector(glm::vec3 sample,glm::vec3  up,glm::vec3  normal) const {
    glm::vec3 hemUp = up;
    
    if(hemUp == normal){
       return sample;
    }else if(hemUp == (-1.0f * normal)){
       return -1.0f * sample;
    }else{
    
       float angle = acos(glm::dot(hemUp, normal));
       glm::vec3 axis = glm::cross(hemUp, normal);
       glm::vec4 pt = glm::vec4(sample.x,sample.y,sample.z,0);
       glm::mat4 matrix = glm::rotate(angle, axis);
       pt = (matrix*pt);
       glm::vec3 pt2  = glm::vec3(pt.x,pt.y,pt.z);
       return pt2;
    }
}


LightingResults RayTracer::GetLightingResults(const Light * const light, const Material & material, const glm::vec3 & point, const glm::vec3 & view, const glm::vec3 & normal) const
{
	LightingResults Results;

	if (params.useShading)
	{
		SurfaceVectors surface;
		surface.normal = normal;
		surface.view = view;
		surface.light = glm::normalize(light->position - point);

		if (brdf)
		{
			Results.diffuse  = light->color * material.finish.diffuse  * material.color * brdf->CalculateDiffuse(material, surface);
			Results.specular = light->color * material.finish.specular * material.color * brdf->CalculateSpecular(material, surface);
		}
	}
	else
	{
		Results.diffuse = material.color;
	}

	return Results;
}

glm::vec3 RayTracer::GetReflectionResults(const glm::vec3 & point, const glm::vec3 & reflectionVector, const int depth, PixelContext::Iteration * currentIteration) const
{
	if (depth > 0)
	{
		if (context)
		{
			context->iterations.push_back(new PixelContext::Iteration());
			context->iterations.back()->type = PixelContext::IterationType::Reflection;
			context->iterations.back()->parent = currentIteration;
		}

		return CastRay(Ray(point, reflectionVector), depth - 1,1).ToColor();
	}

	return glm::vec3(0.f);
}

glm::vec3 RayTracer::GetRefractionResults(const Material & material, const glm::vec3 & point, glm::vec3 const & transmissionVector, const bool entering, int const depth, PixelContext::Iteration * currentIteration) const
{
	if (depth > 0)
	{
		if (context)
		{
			context->iterations.push_back(new PixelContext::Iteration());
			context->iterations.back()->type = PixelContext::IterationType::Refraction;
			context->iterations.back()->parent = currentIteration;

			if (entering)
			{
				context->iterations.back()->extraInfo += " into-object";
			}
			else
			{
				context->iterations.back()->extraInfo += " into-air";
			}
		}

		const RayTraceResults results = CastRay(Ray(point, transmissionVector), depth - 1,1);

		if (entering)
		{
			if (params.useBeers)
			{
				// Beer's Law
				const float distance = glm::distance(point, results.intersectionPoint);

				const float density = 0.15f;
				const glm::vec3 absorbance = (glm::vec3(1.f) - material.color) * 0.15f * -distance;
				const glm::vec3 attenuation = glm::vec3(expf(absorbance.x), expf(absorbance.y), expf(absorbance.z));

				return results.ToColor() * attenuation;
			}
			else
			{
				return results.ToColor() * material.color;
			}
		}
		else
		{
			return results.ToColor();
		}
	}

	return glm::vec3(0.f);
}

glm::vec3 RayTracer::CalculateRefractionVector(const glm::vec3 & view, glm::vec3 normal, const float ior)
{
	float iorRatio;

	if (glm::dot(view, normal) >= 0.f)
	{
		iorRatio = 1.f / ior;
	}
	else
	{
		iorRatio = ior / 1.f;
		normal = -normal;
	}

	const float c1 = glm::dot(view, normal);
	const float c2 = 1 - (iorRatio * iorRatio) * (1 - (c1 * c1));

	if (c2 < 0.f)
	{
		return glm::vec3(0.f, 0.f, 0.f);
	}

	return glm::normalize((-view * iorRatio) + normal * (iorRatio * c1 - glm::sqrt(c2)));
}
