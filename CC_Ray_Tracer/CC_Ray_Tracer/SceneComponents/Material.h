#pragma once
#include "../Utils/Math/Color.h"
#include "Ray.h"

enum class MaterialType
{
	Diffuse,
	Reflective
};

class Material
{
protected:
	Color mAlbedo;
	MaterialType mType;
	bool mIsSmooth;

	public:
	Material(Color albedo, MaterialType type, bool isSmooth)
	{
		mAlbedo = albedo;
		mType = type;
		mIsSmooth = isSmooth;
	}

	const Color& getAlbedo() const
	{
		return mAlbedo;
	}

	const MaterialType& getType() const
	{
		return mType;
	}

	bool isSmooth() const
	{
		return mIsSmooth;
	}
};