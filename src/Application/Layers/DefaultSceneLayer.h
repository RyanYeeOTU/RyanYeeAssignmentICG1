#pragma once
#include "Application/ApplicationLayer.h"
#include "Application/Layers/RenderLayer.h"
#include "Gameplay/Scene.h"
#include "Gameplay/MeshResource.h"
#include "Gameplay/Material.h"
#include "json.hpp"

/**
 * This example layer handles creating a default test scene, which we will use 
 * as an entry point for creating a sample scene
 */
class DefaultSceneLayer final : public ApplicationLayer {
public:
	MAKE_PTRS(DefaultSceneLayer)

	DefaultSceneLayer();
	virtual ~DefaultSceneLayer();

	// Inherited from ApplicationLayer

	virtual void OnAppLoad(const nlohmann::json& config) override;
	virtual void OnUpdate() override;

	Gameplay::Material::Sptr DemonMat;
	Gameplay::Material::Sptr PilarMat;
	Gameplay::Material::Sptr CBowMat;
	Gameplay::Material::Sptr FloorMat;
	Gameplay::Material::Sptr WaterMaterial;


protected:
	void _CreateScene();

	Texture3D::Sptr warmlut;
	Texture3D::Sptr coldlut;
	Texture3D::Sptr bloodlut;
};