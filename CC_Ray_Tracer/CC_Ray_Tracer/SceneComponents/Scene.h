#pragma once
#include <vector>
#include <string>
#include <iostream>
#include <fstream>

#include "Camera.h"
#include "Settings.h"
#include "../Utils/3DShapes/Mesh.h"

#include "../Utils/rapidjson/document.h"
#include "../Utils/rapidjson/istreamwrapper.h"
#include "LightSource.h"
#include "../Accelerators/AABBAccelerator.h"

class Scene
{
public:
	Scene(const std::string& fileName)
	{
		parseSceneFile(fileName);
		for (auto& mesh : mObjects)
		{
			mBBox = mBBox.merge(mesh.getBBox());
		}
		accelerator.createTree(mBBox, getTriangles());
	}

	int getWidth() const
	{
		return mSettings.imageWidth;
	}

	int getHeight() const
	{
		return mSettings.imageHeight;
	}

	const Color& getBackgroundColor() const
	{
		return mSettings.backgroundColor;
	}

	const Camera& getCamera() const {
		return mCamera;
	}

	const std::vector<LightSource>& getLights() const
	{
		return mLights;
	}

	void parseSceneFile(const std::string& fileName);
	bool intersects(const Ray& ray, Intersection& intersection) const;

	vector<Triangle> getTriangles() const
	{
		vector<Triangle> triangles;
		for (auto& mesh : mObjects)
		{
			for (auto& triangle : mesh.getTriangles())
			{
				triangle.setMaterial(mMaterials[mesh.getMaterialIndex()]);
				triangles.push_back(triangle);
			}
		}
		return triangles;
	}

	~Scene() {
		mMaterials.clear();
	}

private:
	Settings mSettings;
	Camera mCamera;
	std::vector<Material*> mMaterials;
	std::vector<Mesh> mObjects;
	std::vector<LightSource> mLights;
	BoundingBox mBBox;
	AABBAccelerator accelerator;

	rapidjson::Document getJsonDocument(const std::string& fileName);
	Vector3 loadVector(const rapidjson::Value::ConstArray& arr);
	Matrix loadMatrix(const rapidjson::Value::ConstArray& arr);
	std::vector<Vector3> loadVectorArr(const rapidjson::Value::ConstArray& arr);
	std::vector<TriangleIndices> loadIndicesArr(const rapidjson::Value::ConstArray& arr);
};