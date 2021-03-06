#pragma once
#ifndef JKRENDERTYPES_H_
#define JKRENDERTYPES_H_

#include"Types.h"
#include"Mesh.h"

//Change both this value and MAX_POINT_LIGHTS in fragmentShader 
//to controll max point light number.
const int MAXLIGHTSNUM = 16;

struct Light
{
	Light(const COLOR3& aColor, const COLOR3& dColor, const COLOR3& sColor) : 
		AmbientColor(aColor), DiffuseColor(dColor), SpecularColor(sColor) {};

	virtual ~Light() {};

	COLOR3 AmbientColor;
	COLOR3 DiffuseColor;
	COLOR3 SpecularColor;

};

struct DirectionLight : public Light
{
	DirectionLight(const COLOR3& aColor, const COLOR3& dColor, const COLOR3& sColor, 
		const VEC3& direction) : Light(aColor, dColor, sColor), Direction(direction) {};
	DirectionLight(const VEC3& direction) : 
		Light(VEC3(0.15f), VEC3(0.8f), VEC3(1.5f)), Direction(direction) {};
	DirectionLight() :
		DirectionLight({ -1.f, -1.f, -1.f }) {};

	~DirectionLight() {};

	VEC3 Direction;
};

struct PointLight : public Light
{
	PointLight(const COLOR3& aColor, const COLOR3& dColor, const COLOR3& sColor, const VEC3& position,
		float constant, float linear, float quadratic) : 
		Light(aColor, dColor, sColor), Position(position), 
		Constant(constant), Linear(linear), Quadratic(quadratic) {};
	PointLight(const VEC3& position, float constant, float linear, float quadratic) : 
		Light(VEC3(0.15f), VEC3(0.8f), VEC3(1.f)), Position(position),
		Constant(constant), Linear(linear), Quadratic(quadratic) {};
	PointLight(const VEC3& position) : PointLight(position, 1.f, 0.35f, 0.44f) {};
	PointLight() : PointLight({ 0.f, 5.f, 0.f }) {};

	VEC3 Position;

	// For attenuation.
	float Constant;
	float Linear;
	float Quadratic;
};

struct SpotLight : public Light
{
	SpotLight(const COLOR3& aColor, const COLOR3& dColor, const COLOR3& sColor, const VEC3& position,
		const VEC3& direction, float innerCutOff, float outterCutOff, float constant, float linear, float quadratic) :
		Light(aColor, dColor, sColor), Position(position), Direction(direction),
		InnerCutOffRadian(innerCutOff), OutterCutOffRadian(outterCutOff),
		//Epsilon(cos(innerCutOff) - cos(outterCutOff)), OutterTheta(cos(outterCutOff)),
		Constant(constant), Linear(linear), Quadratic(quadratic) {};
	SpotLight(const VEC3& position, const VEC3& direction, float innerCutOff, float outterCutOff) : 
		Light({ 0.0, 0.0, 0.0 }, { 1.0f, 1.0f, 1.0f }, {1.0f, 1.0f, 1.0f}),
		Position(position), Direction(direction), InnerCutOffRadian(innerCutOff), OutterCutOffRadian(outterCutOff),
		//Epsilon(cos(innerCutOff) - cos(outterCutOff)), OutterTheta(cos(outterCutOff)),
		Constant(1.f), Linear(.09f), Quadratic(.032f) {};
	SpotLight(const VEC3& position) : SpotLight(position, { 0.f, 0.f, -1.f }, PI / 12, PI / 6) {};
	SpotLight() :SpotLight({ 0.f, 0.f, 1.f }, { 0.f, 0.f, -1.f }, PI / 12, PI / 6) {};

	VEC3 Position;
	VEC3 Direction;
	float InnerCutOffRadian;
	float OutterCutOffRadian;
	//float Epsilon;
	//float OutterTheta;

	float Constant;
	float Linear;
	float Quadratic;
};

#endif // !JKRENDERTYPES_H_
