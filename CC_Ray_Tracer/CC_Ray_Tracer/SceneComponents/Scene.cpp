#include "Scene.h"

using std::vector;
using std::string;
using std::ifstream;
using std::cout;

using rapidjson::Document;
using rapidjson::Value;
using rapidjson::IStreamWrapper;

const char* SETTINGS_FIELD = "settings";
const char* BG_COLOR_FIELD = "background_color";
const char* IMG_SETTINGS_FIELD = "image_settings";
const char* IMG_WIDTH_FIELD = "width";
const char* IMG_HIGHT_FIELD = "height";

const char* CAMERA_FIELD = "camera";
const char* ROT_MATRIX_FIELD = "matrix";
const char* ORIGIN_FIELD = "position";

const char* LIGHTS_FIELD = "lights";
const char* INTENSITY_FIELD = "intensity";
const char* LIGHT_POS_FIELD = "position";

const char* MATERIALS_FIELD = "materials";
const char* TYPE_FIELD = "type";
const char* ALBEDO_FIELD = "albedo";
const char* SHADING_FIELD = "smooth_shading";
const char* IOR_FIELD = "ior";

const char* OBJECTS_FIELD = "objects";
const char* MATERIAL_INDEX_FIELD = "material_index";
const char* VERTICES_FIELD = "vertices";
const char* INDICES_FIELD = "triangles";

void Scene::parseSceneFile(const string& fileName)
{
	Document document = getJsonDocument(fileName);

	const Value& settingsValue = document.FindMember(SETTINGS_FIELD)->value;
	if (!settingsValue.IsNull() && settingsValue.IsObject())
	{
		const Value& backgroundColor = settingsValue.FindMember(BG_COLOR_FIELD)->value;
		assert(!backgroundColor.IsNull() && backgroundColor.IsArray());
		mSettings.backgroundColor = loadVector(backgroundColor.GetArray());
		
		const Value& imageSettings = settingsValue.FindMember(IMG_SETTINGS_FIELD)->value;
		if (!imageSettings.IsNull() && imageSettings.IsObject())
		{
			const Value& imageWidth = imageSettings.FindMember(IMG_WIDTH_FIELD)->value;
			const Value& imageHeight = imageSettings.FindMember(IMG_HIGHT_FIELD)->value;
			assert(!imageWidth.IsNull(), imageWidth.IsInt());
			assert(!imageHeight.IsNull(), imageHeight.IsInt());

			mSettings.imageWidth = imageWidth.GetInt();
			mSettings.imageHeight = imageHeight.GetInt();
			mCamera.setImageWidth(mSettings.imageWidth);
			mCamera.setImageHeight(mSettings.imageHeight);
		}
	}

	const Value& cameraValue = document.FindMember(CAMERA_FIELD)->value;
	if (!cameraValue.IsNull() && cameraValue.IsObject())
	{
		const Value& rotationMatrix = cameraValue.FindMember(ROT_MATRIX_FIELD)->value;
		assert(!rotationMatrix.IsNull() && rotationMatrix.IsArray());
		Matrix matrix = loadMatrix(rotationMatrix.GetArray());
		mCamera.setRotationMatrix(matrix);

		const Value& origin = cameraValue.FindMember(ORIGIN_FIELD)->value;
		assert(!origin.IsNull() && origin.IsArray());
		mCamera.setOrigin(loadVector(origin.GetArray()));
	}

	const Value& lightsValue = document.FindMember(LIGHTS_FIELD)->value;
	if (!lightsValue.IsNull() && lightsValue.IsArray())
	{
		const Value::ConstArray& lightsArr = lightsValue.GetArray();
		for (int i = 0; i < lightsArr.Size(); ++i)
		{
			const Value& intensity = lightsArr[i].FindMember(INTENSITY_FIELD)->value;
			assert(!intensity.IsNull() && intensity.IsInt());

			const Value& position = lightsArr[i].FindMember(LIGHT_POS_FIELD)->value;
			assert(!position.IsNull() && position.IsArray());

			mLights.push_back(LightSource(loadVector(position.GetArray()), intensity.GetFloat()));
		}
	}

	const Value& materialsValue = document.FindMember(MATERIALS_FIELD)->value;
	if (!materialsValue.IsNull() && materialsValue.IsArray())
	{
		const Value::ConstArray& materialsArr = materialsValue.GetArray();
		for (int i = 0; i < materialsArr.Size(); ++i)
		{
			const Value& typeVal = materialsArr[i].FindMember(TYPE_FIELD)->value;

			const Value& albedo = materialsArr[i].FindMember(ALBEDO_FIELD)->value;

			const Value& shading = materialsArr[i].FindMember(SHADING_FIELD)->value;

			const Value& iorVal = materialsArr[i].FindMember(IOR_FIELD)->value;

			MaterialType type = MaterialType::Diffuse;
			if (!typeVal.IsNull() && typeVal.IsString())
			{
				string typeString = typeVal.GetString();
				if (typeString == "reflective")
				{
					type = MaterialType::Reflective;
				}
				else if (typeString == "refractive")
				{
					type = MaterialType::Refractive;
				}
				else if (typeString == "constant")
				{
					type = MaterialType::Constant;
				}
			}
			
			Vector3 albed = Vector3(1,1,1);
			if (!albedo.IsNull() && albedo.IsArray())
			{
				albed = loadVector(albedo.GetArray());
			}

			float ior = 1.0f;
			if (!iorVal.IsNull() && iorVal.IsFloat())
			{
				ior = iorVal.GetFloat();
			}

			mMaterials.push_back(new Material(albed, type, shading.GetBool(), ior));
		}
	}

	const Value& objectsValue = document.FindMember(OBJECTS_FIELD)->value;
	if (!objectsValue.IsNull() && objectsValue.IsArray())
	{
		const Value::ConstArray& meshArr = objectsValue.GetArray();
		for (int i = 0; i < meshArr.Size(); ++i)
		{
			const Value& mat_idx = meshArr[i].FindMember(MATERIAL_INDEX_FIELD)->value;
			assert(!mat_idx.IsNull() && mat_idx.IsInt());

			const Value& vertices = meshArr[i].FindMember(VERTICES_FIELD)->value;
			assert(!vertices.IsNull() && vertices.IsArray());

			const Value& indices = meshArr[i].FindMember(INDICES_FIELD)->value;
			assert(!indices.IsNull() && indices.IsArray());

			mObjects.push_back(Mesh(loadVectorArr(vertices.GetArray()),
				loadIndicesArr(indices.GetArray()), mat_idx.GetInt()));
		}
	}
}

Document Scene::getJsonDocument(const string& fileName)
{
	ifstream file(fileName);
	assert(file.is_open());

	IStreamWrapper isw(file);
	Document document;
	document.ParseStream(isw);

	if (document.HasParseError())
	{
		std::cout << "Error: " << document.GetParseError() << '\n';
		std::cout << "Offset: " << document.GetErrorOffset() << '\n';
	}

	assert(document.IsObject());
	return document;
}

bool Scene::intersects(const Ray& ray, Intersection& intersection) const
{
	float currentT = intersection.getT();
	float boxT = INT_MAX;
	bool hit = false;
	for (const Mesh& mesh : mObjects)
	{
		if (ray.getType() == RayType::Shadow
			&& mMaterials[mesh.getMaterialIndex()]->getType() == MaterialType::Refractive)
		{
			continue;
		}

		if (mesh.intersects(ray, intersection))
		{
			hit = true;
			if (currentT > intersection.getT())
			{
				intersection.updateMaterial(mMaterials[mesh.getMaterialIndex()]);
				currentT = intersection.getT();
			}
		}
	}
	return hit;
}

Vector3 Scene::loadVector(const Value::ConstArray& arr)
{
	assert(arr.Size() == 3);
	Vector3 vec(
		(float)arr[0].GetFloat(),
		(float)arr[1].GetFloat(),
		(float)arr[2].GetFloat()
	);
	return vec;
}

Matrix Scene::loadMatrix(const Value::ConstArray& arr)
{
	assert(arr.Size() == 9);
	Matrix mat(
		arr[0].GetFloat(), arr[1].GetFloat(), arr[2].GetFloat(),
		arr[3].GetFloat(), arr[4].GetFloat(), arr[5].GetFloat(),
		arr[6].GetFloat(), arr[7].GetFloat(), arr[8].GetFloat()
	);
	return mat;
}

vector<Vector3> Scene::loadVectorArr(const Value::ConstArray& arr)
{
	assert(arr.Size() % 3 == 0);
	vector<Vector3> vecArr;
	for (int i = 0; i < arr.Size(); i+=3)
	{
		vecArr.push_back(Vector3(
			(float)arr[i].GetFloat(),
			(float)arr[i + 1].GetFloat(),
			(float)arr[i + 2].GetFloat()
		));
	}
	return vecArr;
}

vector<TriangleIndices> Scene::loadIndicesArr(const Value::ConstArray& arr)
{
	assert(arr.Size() % 3 == 0);
	vector<TriangleIndices> indicesArr;
	for (int i = 0; i < arr.Size(); i += 3)
	{
		indicesArr.push_back(TriangleIndices(
			(float)arr[i].GetFloat(),
			(float)arr[i + 1].GetFloat(),
			(float)arr[i + 2].GetFloat()
		));
	}
	return indicesArr;
}
