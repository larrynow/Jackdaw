#pragma once
#ifndef JKRENDERTYPES_H_
#define JKRENDERTYPES_H_

#include"Types.h"
#include"Mesh.h"

struct Light
{
	Light(const COLOR3& aColor, const COLOR3& dColor, const COLOR3& sColor) : AmbientColor(aColor), DiffuseColor(dColor), SpecluarColor(sColor) {};
	Light() :Light({ .2f }, { .5f }, { 1.f }) {};

	virtual ~Light() {};

	COLOR3 AmbientColor;
	COLOR3 DiffuseColor;
	COLOR3 SpecluarColor;

};

struct DirectionLight : public Light
{
	DirectionLight(const COLOR3& aColor, const COLOR3& dColor, const COLOR3& sColor, const VEC3& direction) : Light(aColor, dColor, sColor), Direction(direction) {};
	DirectionLight() : Light(), Direction({ -1.f, -1.f, -1.f }) {};
	DirectionLight(const VEC3& direction) : Light(), Direction(direction) {};
	~DirectionLight() {};

	VEC3 Direction;
};

struct PointLight : public Light
{
	PointLight(const COLOR3& aColor, const COLOR3& dColor, const COLOR3& sColor, const VEC3& position,
		float constant, float linear, float quadratic) : Light(aColor, dColor, sColor), Position(position), Constant(constant), Linear(linear), Quadratic(quadratic) {};
	PointLight(const VEC3& position, float constant, float linear, float quadratic) : Light(), Position(position),
		Constant(constant), Linear(linear), Quadratic(quadratic) {};
	PointLight(const VEC3& position) : Light(), Position(position), Constant(1.f), Linear(.35f), Quadratic(.44f) {};
	PointLight() : PointLight({ 0.f, 10.f, 0.f }) {};

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
		Epsilon(cos(innerCutOff) - cos(outterCutOff)), OutterTheta(cos(outterCutOff)),
		Constant(constant), Linear(linear), Quadratic(quadratic) {};
	SpotLight(const VEC3& position, const VEC3& direction, float innerCutOff, float outterCutOff) : Light(),
		Position(position), Direction(direction), InnerCutOffRadian(innerCutOff), OutterCutOffRadian(outterCutOff),
		Epsilon(cos(innerCutOff) - cos(outterCutOff)), OutterTheta(cos(outterCutOff)),
		Constant(1.f), Linear(.35f), Quadratic(.44f) {};
	SpotLight(const VEC3& position) : SpotLight(position, { 0.f, 0.f, 1.f }, PI / 12, PI / 6) {};
	SpotLight() :SpotLight({ 0.f, 0.f, -1.f }, { 0.f, 0.f, 1.f }, PI / 12, PI / 6) {};

	VEC3 Position;
	VEC3 Direction;
	float InnerCutOffRadian;
	float OutterCutOffRadian;
	float Epsilon;
	float OutterTheta;

	float Constant;
	float Linear;
	float Quadratic;
};

#endif // !JKRENDERTYPES_H_
