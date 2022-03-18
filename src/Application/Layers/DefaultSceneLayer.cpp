
#include "DefaultSceneLayer.h"
// GLM math library
#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <GLM/gtx/common.hpp> // for fmod (floating modulus)

#include <filesystem>

// Graphics
#include "Graphics/Buffers/IndexBuffer.h"
#include "Graphics/Buffers/VertexBuffer.h"
#include "Graphics/VertexArrayObject.h"
#include "Graphics/ShaderProgram.h"
#include "Graphics/Textures/Texture2D.h"
#include "Graphics/Textures/TextureCube.h"
#include "Graphics/VertexTypes.h"
#include "Graphics/Font.h"
#include "Graphics/GuiBatcher.h"
#include "Graphics/Framebuffer.h"

// Utilities
#include "Utils/MeshBuilder.h"
#include "Utils/MeshFactory.h"
#include "Utils/ObjLoader.h"
#include "Utils/ImGuiHelper.h"
#include "Utils/ResourceManager/ResourceManager.h"
#include "Utils/FileHelpers.h"
#include "Utils/JsonGlmHelpers.h"
#include "Utils/StringUtils.h"
#include "Utils/GlmDefines.h"

// Gameplay
#include "Gameplay/Material.h"
#include "Gameplay/GameObject.h"
#include "Gameplay/Scene.h"

// Components
#include "Gameplay/Components/IComponent.h"
#include "Gameplay/Components/Camera.h"
#include "Gameplay/Components/RotatingBehaviour.h"
#include "Gameplay/Components/JumpBehaviour.h"
#include "Gameplay/Components/RenderComponent.h"
#include "Gameplay/Components/MaterialSwapBehaviour.h"
#include "Gameplay/Components/TriggerVolumeEnterBehaviour.h"
#include "Gameplay/Components/SimpleCameraControl.h"

// Physics
#include "Gameplay/Physics/RigidBody.h"
#include "Gameplay/Physics/Colliders/BoxCollider.h"
#include "Gameplay/Physics/Colliders/PlaneCollider.h"
#include "Gameplay/Physics/Colliders/SphereCollider.h"
#include "Gameplay/Physics/Colliders/ConvexMeshCollider.h"
#include "Gameplay/Physics/Colliders/CylinderCollider.h"
#include "Gameplay/Physics/TriggerVolume.h"
#include "Graphics/DebugDraw.h"

// GUI
#include "Gameplay/Components/GUI/RectTransform.h"
#include "Gameplay/Components/GUI/GuiPanel.h"
#include "Gameplay/Components/GUI/GuiText.h"
#include "Gameplay/InputEngine.h"

#include "Application/Application.h"
#include "Gameplay/Components/ParticleSystem.h"
#include "Graphics/Textures/Texture3D.h"
#include "Graphics/Textures/Texture1D.h"

//bool toggles
bool ambient = true;
bool specu = true;
bool lightsOn = true;
bool custShader = true;

bool specularRamp = false;
bool diffuseRamp = false;

//LUTS
bool blood = false;
bool cool = false;
bool warm = false;


bool registerMats = false;



//TOGGLE
void key_callBack(GLFWwindow* window, int keyPressed, int scancode, int action, int mods)
{
	//NOTE:Please press BACKSPACE to see the state of each toggle. Ensure Ambient and Specular are both either on or off before pressing Keys 4 or 5.


	//release key
	if (action == GLFW_RELEASE)
	{
		return;
	}
	//
	// 
	//all toggles for bool, keys according to the assignment details

	//LIGHTING
	if (keyPressed == GLFW_KEY_1)
	{
		lightsOn = !lightsOn;
		//Print
		if (lightsOn == false)
		{
			std::cout << "Toggle Lights(OFF)\n";
		}
		else if (lightsOn == true)
		{
			std::cout << "Toggle Lights(ON)\n";
		}
	}
	//AMBIENT LIGHTING
	if (keyPressed == GLFW_KEY_2)
	{
		ambient = !ambient;
		//Print
		if (ambient == false)
		{
			std::cout << "Toggle Ambient Lighting(OFF)\n";
		}
		else if (ambient == true)
		{
			std::cout << "Toggle Ambient Lighting(ON)\n";
		}
	}
	//SPECULAR LIGHTING
	if (keyPressed == GLFW_KEY_3)
	{
		specu = !specu;
		//Print
		if (specu == false)
		{
			std::cout << "Toggle Specular Lighting(OFF)\n";
		}
		else if (specu == true)
		{
			std::cout << "Toggle Specular Lighting(ON)\n";
		}
	}
	//AMBIENT AND SPECULAR
	if (keyPressed == GLFW_KEY_4)
	{
		ambient = !ambient;
		specu = !specu;

		//Print
		if (ambient == false && specu == false)
		{
			std::cout << "Toggle Ambient and Specular Lighting(OFF)\n";
		}
		else if (ambient == true && specu == true)
		{
			std::cout << "Toggle Ambient and Specular Lighting(ON)\n";
		}
		else if (ambient == true && specu == false)
		{
			std::cout << "Toggle Ambient(ON) and Specular Lighting(OFF)\n";
		}
		else if (ambient == false && specu == true)
		{
			std::cout << "Toggle Ambient(OFF) and Specular Lighting(ON)\n";
		}

	}
	//AMBIENT, SPECULAR, AND CUSTOM SHADER
	if (keyPressed == GLFW_KEY_5)
	{
		ambient = !ambient;
		specu = !specu;
		custShader = !custShader;

		//Print
		if (ambient == false && specu == false && custShader == false)
		{
			std::cout << "Toggle Ambient,Specular Lighting and Custom Shader(OFF)\n";
		}
		else if (ambient == true && specu == true && custShader == true)
		{
			std::cout << "Toggle Ambient,Specular Lighting and Custom Shader(ON)\n";
		}
		else if (ambient == false && specu == false && custShader == true)
		{
			std::cout << "Toggle Ambient,Specular Lighting(OFF) and Custom Shader(ON)\n";
		}
		else if (ambient == true && specu == true && custShader == false)
		{
			std::cout << "Toggle Ambient,Specular Lighting(ON) and Custom Shader(OFF)\n";
		}
	}
	//DIFFUSE RAMP
	if (keyPressed == GLFW_KEY_6)
	{
		diffuseRamp = !diffuseRamp;
		//Print
		if (diffuseRamp == false)
		{
			std::cout << "Toggle Diffuse Ramp(OFF)\n";
		}
		else if (diffuseRamp == true)
		{
			std::cout << "Toggle Diffuse Ramp(ON)\n";
		}
	}
	//SPECULAR RAMP
	if (keyPressed == GLFW_KEY_7)
	{
		specularRamp = !specularRamp;
		//Print
		if (specularRamp == false)
		{
			std::cout << "Toggle Specular Ramp(OFF)\n";
		}
		else if (specularRamp == true)
		{
			std::cout << "Toggle Specular Ramp(ON)\n";
		}
	}
	//COLOR GRADING : WARM
	if (keyPressed == GLFW_KEY_8)
	{
		warm = !warm;
		//Print
		if (warm == false)
		{
			std::cout << "Toggle Warm Color Correction LUT(OFF)\n";
		}
		else if (warm == true)
		{
			std::cout << "Toggle Warm Color Correction LUT(ON)\n";
		}
	}
	//COLOR GRADING : COOL
	if (keyPressed == GLFW_KEY_9)
	{
		cool = !cool;
		//Print
		if (cool == false)
		{
			std::cout << "Toggle Cool Color Correction LUT(OFF)\n";
		}
		else if (cool == true)
		{
			std::cout << "Toggle Cool Color Correction LUT(ON)\n";
		}
	}
	//COLOR GRADING : BLOOD
	if (keyPressed == GLFW_KEY_0)
	{
		blood = !blood;
		//Print
		if (blood == false)
		{
			std::cout << "Toggle Blood Color Correction LUT(OFF)\n";
		}
		else if (blood == true)
		{
			std::cout << "Toggle Blood Color Correction LUT(ON)\n";
		}
	}
	
	//STATUS OF TOGGLES
	if (keyPressed == GLFW_KEY_BACKSPACE)
	{
		std::cout << "=============" << std::endl;
		if (lightsOn == false)
		{
			std::cout << "Lights = (OFF)\n";
		}
		else if (lightsOn == true)
		{
			std::cout << "Lights = (ON)\n";
		}

		if (ambient == false)
		{
			std::cout << "Ambient Lighting = (OFF)\n";
		}
		else if (ambient == true)
		{
			std::cout << "Ambient Lighting = (ON)\n";
		}

		if (specu == false)
		{
			std::cout << "Specular Lighting = (OFF)\n";
		}
		else if (specu == true)
		{
			std::cout << "Specular Lighting = (ON)\n";
		}

		if (ambient == false && specu == false)
		{
			std::cout << "Ambient and Specular Lighting = (OFF)\n";
		}
		else if (ambient == true && specu == true)
		{
			std::cout << "Ambient and Specular Lighting = (ON)\n";
		}


		if (ambient == false && specu == false && custShader == false)
		{
			std::cout << "Ambient,Specular Lighting and Custom Shader = (OFF)\n";
		}
		else if (ambient == true && specu == true && custShader == true)
		{
			std::cout << "Ambient,Specular Lighting and Custom Shader = (ON)\n";
		}
		else if (ambient == true && specu == true && custShader == false)
		{
			std::cout << "Ambient,Specular Lighting (ON), Custom Shader = (OFF)\n";
		}
		else if (ambient == false && specu == false && custShader == true)
		{
			std::cout << "Ambient,Specular Lighting (OFF), Custom Shader = (ON)\n";
		}

		if (diffuseRamp == false)
		{
			std::cout << "Diffuse Ramp = (OFF)\n";
		}
		else if (diffuseRamp == true)
		{
			std::cout << "Diffuse Ramp = (ON)\n";
		}

		if (specularRamp == false)
		{
			std::cout << "Specular Ramp = (OFF)\n";
		}
		else if (specularRamp == true)
		{
			std::cout << "Specular Ramp = (ON)\n";
		}

		if (warm == false)
		{
			std::cout << "Warm Color Correction LUT = (OFF)\n";
		}
		else if (warm == true)
		{
			std::cout << "Warm Color Correction LUT = (ON)\n";
		}

		if (cool == false)
		{
			std::cout << "Cool Color Correction LUT = (OFF)\n";
		}
		else if (cool == true)
		{
			std::cout << "Cool Color Correction LUT = (ON)\n";
		}

		if (blood == false)
		{
			std::cout << "Blood Color Correction LUT = (OFF)\n";
		}
		else if (blood == true)
		{
			std::cout << "Blood Color Correction LUT = (ON)\n";
		}

		std::cout << "=============" << std::endl;
	}

	







}


DefaultSceneLayer::DefaultSceneLayer() :
	ApplicationLayer()
{
	Name = "Default Scene";
	Overrides = AppLayerFunctions::OnAppLoad|AppLayerFunctions::OnUpdate;
}

DefaultSceneLayer::~DefaultSceneLayer() = default;




void DefaultSceneLayer::OnAppLoad(const nlohmann::json& config) {
	_CreateScene();

}
void DefaultSceneLayer::OnUpdate()
{
	Application& app = Application::Get();
	glfwSetKeyCallback(app.GetWindow(), key_callBack);
	float incre = glfwGetTime();


	if (warm == true)
	{
		app.CurrentScene()->SetColorLUT(warmlut);
	}
	else if (cool == true)
	{
		app.CurrentScene()->SetColorLUT(coldlut);
	}
	else if (blood == true)
	{
		app.CurrentScene()->SetColorLUT(bloodlut);
	}
	else
	{
		app.GetLayer<RenderLayer>()->SetRenderFlags(RenderFlags::None);
	}
	


	if (!warm && !cool && !blood && lightsOn) // no colour correction
	{
	app.GetLayer<RenderLayer>()->SetRenderFlags(RenderFlags::EnableLights);
	}
	else if (!warm && !cool && !blood) //if there is nothing
	{
		app.GetLayer<RenderLayer>()->SetRenderFlags(RenderFlags::None);
	}
	else if (!lightsOn)// if lights are not on and coloor correction is on
	{
		app.GetLayer<RenderLayer>()->SetRenderFlags(RenderFlags::EnableColorCorrection);
	}
	else //if both
	{
		app.GetLayer<RenderLayer>()->SetRenderFlags(RenderFlags::EnableLights|RenderFlags::EnableColorCorrection);
	}



	//togglemats


	if (registerMats == false)
	{
		registerMats = true; // one register

		//register materials from .h file
		DemonMat = app.CurrentScene()->FindObjectByName("demon")->Get<RenderComponent>()->GetMaterial();
		PilarMat = app.CurrentScene()->FindObjectByName("pillar")->Get<RenderComponent>()->GetMaterial();
		CBowMat = app.CurrentScene()->FindObjectByName("Crossbow")->Get<RenderComponent>()->GetMaterial();
		FloorMat = app.CurrentScene()->FindObjectByName("Plane")->Get<RenderComponent>()->GetMaterial();
		WaterMaterial = app.CurrentScene()->FindObjectByName("Water")->Get<RenderComponent>()->GetMaterial();
	}



	if (!custShader)
	{
		WaterMaterial->Set("delta", 0.0f);
	}
	else
	{
		WaterMaterial->Set("delta", incre);
	}

	if (!diffuseRamp)
	{
		DemonMat->Set("diffuseRamp", false);
		CBowMat->Set("diffuseRamp", false);
		PilarMat->Set("diffuseRamp", false);
		FloorMat->Set("diffuseRamp", false);
		WaterMaterial->Set("diffuseRamp", false);
	}
	else
	{
		DemonMat->Set("diffuseRamp", true);
		CBowMat->Set("diffuseRamp", true);
		PilarMat->Set("diffuseRamp", true);
		FloorMat->Set("diffuseRamp", true);
		WaterMaterial->Set("diffuseRamp", true);
	}

	if (!specularRamp)
	{
		DemonMat->Set("specularRamp", false);
		CBowMat->Set("specularRamp", false);
		PilarMat->Set("specularRamp", false);
		FloorMat->Set("specularRamp", false);
		WaterMaterial->Set("specularRamp", false);
	}
	else
	{
		DemonMat->Set("specularRamp", true);
		CBowMat->Set("specularRamp", true);
		PilarMat->Set("specularRamp", true);
		FloorMat->Set("specularRamp", true);
		WaterMaterial->Set("specularRamp", true);

	}

	if (!ambient)
	{
		DemonMat->Set("ambient", false);
		CBowMat->Set("ambient", false);
		PilarMat->Set("ambient", false);
		FloorMat->Set("ambient", false);
		WaterMaterial->Set("ambient", false);
	}
	else
	{
		DemonMat->Set("ambient", true);
		CBowMat->Set("ambient", true);
		PilarMat->Set("ambient", true);
		WaterMaterial->Set("ambient", true);

	}

	if (!specu)
	{
		DemonMat->Set("specular", false);
		CBowMat->Set("specular", false);
		PilarMat->Set("specular", false);
		FloorMat->Set("specular", false);
		WaterMaterial->Set("specular", false);

	}
	else
	{
		DemonMat->Set("specular", true);
		CBowMat->Set("specular", true);
		PilarMat->Set("specular", true);
		FloorMat->Set("specular", true);
		WaterMaterial->Set("specular", true);

	}



}

void DefaultSceneLayer::_CreateScene()
{
	using namespace Gameplay;
	using namespace Gameplay::Physics;

	Application& app = Application::Get();
	bool loadScene = false;

	// For now we can use a toggle to generate our scene vs load from file
	if (loadScene && std::filesystem::exists("scene.json")) {
		app.LoadScene("scene.json");
	} else {
		// This time we'll have 2 different shaders, and share data between both of them using the UBO
		// This shader will handle reflective materials 
		ShaderProgram::Sptr reflectiveShader = ResourceManager::CreateAsset<ShaderProgram>(std::unordered_map<ShaderPartType, std::string>{
			{ ShaderPartType::Vertex, "shaders/vertex_shaders/basic.glsl" },
			{ ShaderPartType::Fragment, "shaders/fragment_shaders/frag_environment_reflective.glsl" }
		});
		reflectiveShader->SetDebugName("Reflective");

		// This shader handles our basic materials without reflections (cause they expensive)
		ShaderProgram::Sptr basicShader = ResourceManager::CreateAsset<ShaderProgram>(std::unordered_map<ShaderPartType, std::string>{
			{ ShaderPartType::Vertex, "shaders/vertex_shaders/basic.glsl" },
			{ ShaderPartType::Fragment, "shaders/fragment_shaders/frag_blinn_phong_textured.glsl" }
		});
		basicShader->SetDebugName("Blinn-phong");

		// This shader handles our basic materials without reflections (cause they expensive)
		ShaderProgram::Sptr specShader = ResourceManager::CreateAsset<ShaderProgram>(std::unordered_map<ShaderPartType, std::string>{
			{ ShaderPartType::Vertex, "shaders/vertex_shaders/basic.glsl" },
			{ ShaderPartType::Fragment, "shaders/fragment_shaders/textured_specular.glsl" }
		});
		specShader->SetDebugName("Textured-Specular");

		// This shader handles our foliage vertex shader example
		ShaderProgram::Sptr foliageShader = ResourceManager::CreateAsset<ShaderProgram>(std::unordered_map<ShaderPartType, std::string>{
			{ ShaderPartType::Vertex, "shaders/vertex_shaders/foliage.glsl" },
			{ ShaderPartType::Fragment, "shaders/fragment_shaders/screendoor_transparency.glsl" }
		});
		ShaderProgram::Sptr waterShader = ResourceManager::CreateAsset<ShaderProgram>(std::unordered_map<ShaderPartType, std::string>{
			{ ShaderPartType::Vertex, "shaders/vertex_shaders/Water_Vert.glsl" },
			{ ShaderPartType::Fragment, "shaders/fragment_shaders/Water_Frag.glsl" }
		});

		/*
		foliageShader->SetDebugName("Foliage");

		// This shader handles our cel shading example
		ShaderProgram::Sptr toonShader = ResourceManager::CreateAsset<ShaderProgram>(std::unordered_map<ShaderPartType, std::string>{
			{ ShaderPartType::Vertex, "shaders/vertex_shaders/basic.glsl" },
			{ ShaderPartType::Fragment, "shaders/fragment_shaders/toon_shading.glsl" }
		});
		toonShader->SetDebugName("Toon Shader");
		
		// This shader handles our displacement mapping example
		ShaderProgram::Sptr displacementShader = ResourceManager::CreateAsset<ShaderProgram>(std::unordered_map<ShaderPartType, std::string>{
			{ ShaderPartType::Vertex, "shaders/vertex_shaders/displacement_mapping.glsl" },
			{ ShaderPartType::Fragment, "shaders/fragment_shaders/frag_tangentspace_normal_maps.glsl" }
		});
		displacementShader->SetDebugName("Displacement Mapping");

		// This shader handles our tangent space normal mapping
		ShaderProgram::Sptr tangentSpaceMapping = ResourceManager::CreateAsset<ShaderProgram>(std::unordered_map<ShaderPartType, std::string>{
			{ ShaderPartType::Vertex, "shaders/vertex_shaders/basic.glsl" },
			{ ShaderPartType::Fragment, "shaders/fragment_shaders/frag_tangentspace_normal_maps.glsl" }
		});
		tangentSpaceMapping->SetDebugName("Tangent Space Mapping");

		// This shader handles our multitexturing example
		ShaderProgram::Sptr multiTextureShader = ResourceManager::CreateAsset<ShaderProgram>(std::unordered_map<ShaderPartType, std::string>{
			{ ShaderPartType::Vertex, "shaders/vertex_shaders/vert_multitextured.glsl" },
			{ ShaderPartType::Fragment, "shaders/fragment_shaders/frag_multitextured.glsl" }
		});
		multiTextureShader->SetDebugName("Multitexturing");
		
		*/
		// Load in the meshes
		MeshResource::Sptr monkeyMesh = ResourceManager::CreateAsset<MeshResource>("Monkey.obj");
		MeshResource::Sptr crossMesh = ResourceManager::CreateAsset<MeshResource>("Crossbow.obj");
		MeshResource::Sptr pillarMesh = ResourceManager::CreateAsset<MeshResource>("PILLARFIX.obj");
		MeshResource::Sptr demonMesh = ResourceManager::CreateAsset<MeshResource>("demon.obj");
		MeshResource::Sptr planeObjMesh = ResourceManager::CreateAsset<MeshResource>("plane.obj");
		

		// Load in some textures
		Texture2D::Sptr		pillarTexture = ResourceManager::CreateAsset<Texture2D>("Textures/PillarVar3.png");
		Texture2D::Sptr    demonTexture = ResourceManager::CreateAsset<Texture2D>("textures/demon2.png");
		Texture2D::Sptr		crossBowTexture = ResourceManager::CreateAsset<Texture2D>("Textures/CrossbowTex.png");
		Texture2D::Sptr    floorTexture = ResourceManager::CreateAsset<Texture2D>("textures/FloorVar8.png");
		Texture2D::Sptr    boxTexture   = ResourceManager::CreateAsset<Texture2D>("textures/box-diffuse.png");
		//Texture2D::Sptr    boxSpec      = ResourceManager::CreateAsset<Texture2D>("textures/box-specular.png");
		Texture2D::Sptr    monkeyTex    = ResourceManager::CreateAsset<Texture2D>("textures/monkey-uvMap.png");
		Texture2D::Sptr    leafTex      = ResourceManager::CreateAsset<Texture2D>("textures/leaves.png");
		Texture2D::Sptr		waterTex = ResourceManager::CreateAsset<Texture2D>("textures/FloorVar8.png");
		leafTex->SetMinFilter(MinFilter::Nearest);
		leafTex->SetMagFilter(MagFilter::Nearest);


		// Loading in a 1D LUT
		Texture1D::Sptr toonLut = ResourceManager::CreateAsset<Texture1D>("luts/toon-1D.png"); 
		toonLut->SetWrap(WrapMode::ClampToEdge);

		warmlut = ResourceManager::CreateAsset<Texture3D>("luts/warmCC.CUBE");
		coldlut = ResourceManager::CreateAsset<Texture3D>("luts/coolCC.CUBE");
		bloodlut = ResourceManager::CreateAsset<Texture3D>("luts/bloodCC.CUBE");

		
		// Here we'll load in the cubemap, as well as a special shader to handle drawing the skybox
		TextureCube::Sptr testCubemap = ResourceManager::CreateAsset<TextureCube>("cubemaps/ocean/ocean.jpg");
		ShaderProgram::Sptr skyboxShader = ResourceManager::CreateAsset<ShaderProgram>(std::unordered_map<ShaderPartType, std::string>{
			{ ShaderPartType::Vertex, "shaders/vertex_shaders/skybox_vert.glsl" },
			{ ShaderPartType::Fragment, "shaders/fragment_shaders/skybox_frag.glsl" }
		});

		// Create an empty scene
		Scene::Sptr scene = std::make_shared<Scene>(); 

		// Setting up our enviroment map
		scene->SetSkyboxTexture(testCubemap); 
		scene->SetSkyboxShader(skyboxShader);
		// Since the skybox I used was for Y-up, we need to rotate it 90 deg around the X-axis to convert it to z-up 
		scene->SetSkyboxRotation(glm::rotate(MAT4_IDENTITY, glm::half_pi<float>(), glm::vec3(1.0f, 0.0f, 0.0f)));
 

		// Configure the color correction LUT
		//scene->SetColorLUT(lut);

		// Create our materials
		// This will be our box material, with no environment reflections
		/* Material::Sptr boxMaterial = ResourceManager::CreateAsset<Material>(basicShader);
		{
			boxMaterial->Name = "Box";
			boxMaterial->Set("u_Material.Diffuse", boxTexture);
			boxMaterial->Set("u_Material.Shininess", 0.1f);
		}
		*/
		Material::Sptr floorMaterial = ResourceManager::CreateAsset<Material>(basicShader);
		{
			floorMaterial->Name = "DirtFloor";
			floorMaterial->Set("u_Material.Diffuse", floorTexture);
			floorMaterial->Set("u_Material.Shininess", 0.1f);
		}
		
		// This will be the reflective material, we'll make the whole thing 90% reflective
		/*Material::Sptr monkeyMaterial = ResourceManager::CreateAsset<Material>(reflectiveShader);
		{
			monkeyMaterial->Name = "Monkey";
			monkeyMaterial->Set("u_Material.Diffuse", monkeyTex);
			monkeyMaterial->Set("u_Material.Shininess", 0.5f);
		}
		*/
		Material::Sptr crossbowMat = ResourceManager::CreateAsset<Material>(specShader);
		{
			crossbowMat->Name = "Crossbow";
			crossbowMat->Set("u_Material.Diffuse", crossBowTexture);
			crossbowMat->Set("u_Material.Shininess", 0.5f);
		}
		Material::Sptr demonMat = ResourceManager::CreateAsset<Material>(specShader);
		{
			demonMat->Name = "Demon";
			demonMat->Set("u_Material.Diffuse", demonTexture);
			demonMat->Set("u_Material.Shininess", 0.5f);
		}

		Material::Sptr pillarMat = ResourceManager::CreateAsset<Material>(specShader);
		{
			pillarMat->Name = "Pillar";
			pillarMat->Set("u_Material.Diffuse", pillarTexture);
			pillarMat->Set("u_Material.Shininess", 0.5f);
		}


		Material::Sptr waterMat = ResourceManager::CreateAsset<Material>(waterShader);
		{
		
			waterMat->Name = "Water";
			waterMat->Set("u_Material.Diffuse", waterTex);
			waterMat->Set("u_Material.Shininess", 0.5f);
			waterMat->Set("u_Scale", 0.1f);
			waterMat->Set("delta", 0.0f);
		}
		// This will be the reflective material, we'll make the whole thing 90% reflective
/*		Material::Sptr testMaterial = ResourceManager::CreateAsset<Material>(specShader);
		{
			testMaterial->Name = "Box-Specular";
			testMaterial->Set("u_Material.Diffuse", boxTexture);
			testMaterial->Set("u_Material.Specular", boxSpec);
		}
		*/
		// Our foliage vertex shader material
		/*Material::Sptr foliageMaterial = ResourceManager::CreateAsset<Material>(foliageShader);
		{
			foliageMaterial->Name = "Foliage Shader";
			foliageMaterial->Set("u_Material.Diffuse", leafTex);
			foliageMaterial->Set("u_Material.Shininess", 0.1f);
			foliageMaterial->Set("u_Material.Threshold", 0.1f);

			foliageMaterial->Set("u_WindDirection", glm::vec3(1.0f, 1.0f, 0.0f));
			foliageMaterial->Set("u_WindStrength", 0.5f);
			foliageMaterial->Set("u_VerticalScale", 1.0f);
			foliageMaterial->Set("u_WindSpeed", 1.0f);
		}

		// Our toon shader material
		Material::Sptr toonMaterial = ResourceManager::CreateAsset<Material>(toonShader);
		{
			toonMaterial->Name = "Toon";
			toonMaterial->Set("u_Material.Diffuse", boxTexture); 
			toonMaterial->Set("s_ToonTerm", toonLut);
			toonMaterial->Set("u_Material.Shininess", 0.1f);
			toonMaterial->Set("u_Material.Steps", 8);
		}


		Material::Sptr displacementTest = ResourceManager::CreateAsset<Material>(displacementShader);
		{
			Texture2D::Sptr displacementMap = ResourceManager::CreateAsset<Texture2D>("textures/displacement_map.png");
			Texture2D::Sptr normalMap       = ResourceManager::CreateAsset<Texture2D>("textures/normal_map.png");
			Texture2D::Sptr diffuseMap      = ResourceManager::CreateAsset<Texture2D>("textures/bricks_diffuse.png");

			displacementTest->Name = "Displacement Map";
			displacementTest->Set("u_Material.Diffuse", diffuseMap);
			displacementTest->Set("s_Heightmap", displacementMap);
			displacementTest->Set("s_NormalMap", normalMap);
			displacementTest->Set("u_Material.Shininess", 0.5f);
			displacementTest->Set("u_Scale", 0.1f);
		}
		
		Material::Sptr normalmapMat = ResourceManager::CreateAsset<Material>(tangentSpaceMapping);
		{
			Texture2D::Sptr normalMap       = ResourceManager::CreateAsset<Texture2D>("textures/normal_map.png");
			Texture2D::Sptr diffuseMap      = ResourceManager::CreateAsset<Texture2D>("textures/bricks_diffuse.png");

			normalmapMat->Name = "Tangent Space Normal Map";
			normalmapMat->Set("u_Material.Diffuse", diffuseMap);
			normalmapMat->Set("s_NormalMap", normalMap);
			normalmapMat->Set("u_Material.Shininess", 0.5f);
			normalmapMat->Set("u_Scale", 0.1f);
		}
/*
		Material::Sptr multiTextureMat = ResourceManager::CreateAsset<Material>(multiTextureShader);
		{
			Texture2D::Sptr sand  = ResourceManager::CreateAsset<Texture2D>("textures/terrain/sand.png");
			Texture2D::Sptr grass = ResourceManager::CreateAsset<Texture2D>("textures/terrain/grass.png");

			multiTextureMat->Name = "Multitexturing";
			multiTextureMat->Set("u_Material.DiffuseA", sand);
			multiTextureMat->Set("u_Material.DiffuseB", grass);
			multiTextureMat->Set("u_Material.Shininess", 0.5f);
			multiTextureMat->Set("u_Scale", 0.1f);
		}
		*/
		// Create some lights for our scene
		scene->Lights.resize(3);
		scene->Lights[0].Position = glm::vec3(0.0f, 1.0f, 3.0f);
		scene->Lights[0].Color = glm::vec3(1.0f, 1.0f, 1.0f);
		scene->Lights[0].Range = 300.0f;

		scene->Lights[1].Position = glm::vec3(1.0f, 0.0f, 3.0f);
		scene->Lights[1].Color = glm::vec3(0.2f, 0.8f, 0.1f);

		scene->Lights[2].Position = glm::vec3(0.0f, 1.0f, 3.0f);
		scene->Lights[2].Color = glm::vec3(1.0f, 0.2f, 0.1f);

		// We'll create a mesh that is a simple plane that we can resize later
		MeshResource::Sptr planeMesh = ResourceManager::CreateAsset<MeshResource>();
		planeMesh->AddParam(MeshBuilderParam::CreatePlane(ZERO, UNIT_Z, UNIT_X, glm::vec2(1.0f)));
		planeMesh->GenerateMesh();

		MeshResource::Sptr sphere = ResourceManager::CreateAsset<MeshResource>();
		sphere->AddParam(MeshBuilderParam::CreateIcoSphere(ZERO, ONE, 5));
		sphere->GenerateMesh();

		// Set up the scene's camera
		GameObject::Sptr camera = scene->MainCamera->GetGameObject()->SelfRef();
		{
			camera->SetPostion({ -0.400, -5.930, 9.400 });
			camera->SetRotation({63.305f,-0.f,0.f });
			//camera->LookAt(glm::vec3(63.305f,-0.f,0.f));

			camera->Add<SimpleCameraControl>();

			// This is now handled by scene itself!
			//Camera::Sptr cam = camera->Add<Camera>();
			// Make sure that the camera is set as the scene's main camera!
			//scene->MainCamera = cam;
		}

		// Set up all our sample objects
		GameObject::Sptr plane = scene->CreateGameObject("Plane");
		{
			// Make a big tiled mesh
			MeshResource::Sptr tiledMesh = ResourceManager::CreateAsset<MeshResource>();
			tiledMesh->AddParam(MeshBuilderParam::CreatePlane(ZERO, UNIT_Z, UNIT_X, glm::vec2(100.0f), glm::vec2(20.0f)));
			tiledMesh->GenerateMesh();
			
			// Create and attach a RenderComponent to the object to draw our mesh
			RenderComponent::Sptr renderer = plane->Add<RenderComponent>();
			renderer->SetMesh(tiledMesh);
			renderer->SetMaterial(floorMaterial);
			
			// Attach a plane collider that extends infinitely along the X/Y axis
			RigidBody::Sptr physics = plane->Add<RigidBody>(/*static by default*/);
			physics->AddCollider(BoxCollider::Create(glm::vec3(50.0f, 50.0f, 1.0f)))->SetPosition({ 0,0,-1 });
		}

		GameObject::Sptr water = scene->CreateGameObject("Water");
		{

			// Create and attach a RenderComponent to the object to draw our mesh
			RenderComponent::Sptr renderer = water->Add<RenderComponent>();
			renderer->SetMesh(planeObjMesh);
			renderer->SetMaterial(waterMat);
			water->SetScale(glm::vec3{ 20.450f,27.600f,1.00f });


		}
		/*GameObject::Sptr monkey1 = scene->CreateGameObject("Monkey 1");
		{
			// Set position in the scene
			monkey1->SetPostion(glm::vec3(1.5f, 0.0f, 1.0f));

			// Add some behaviour that relies on the physics body
			monkey1->Add<JumpBehaviour>();

			// Create and attach a renderer for the monkey
			RenderComponent::Sptr renderer = monkey1->Add<RenderComponent>();
			renderer->SetMesh(monkeyMesh);
			renderer->SetMaterial(monkeyMaterial);

			// Example of a trigger that interacts with static and kinematic bodies as well as dynamic bodies
			TriggerVolume::Sptr trigger = monkey1->Add<TriggerVolume>();
			trigger->SetFlags(TriggerTypeFlags::Statics | TriggerTypeFlags::Kinematics);
			trigger->AddCollider(BoxCollider::Create(glm::vec3(1.0f)));

			monkey1->Add<TriggerVolumeEnterBehaviour>();
		}
		*/
		GameObject::Sptr demoBase = scene->CreateGameObject("Demo Parent"); // parent example

		GameObject::Sptr crossbow = scene->CreateGameObject("Crossbow");
		{

			// Set and rotation position in the scene
			crossbow->SetPostion(glm::vec3{ -0.4f, -5.930f, 9.400f });
			crossbow->SetRotation(glm::vec3{60.0f, 0, 0});
			crossbow->SetScale(glm::vec3{ 0.950f, 1.f, 1.f });
			// Add a render component
			RenderComponent::Sptr renderer = crossbow->Add<RenderComponent>();
			renderer->SetMesh(crossMesh);
			renderer->SetMaterial(crossbowMat);

			demoBase->AddChild(crossbow);
		}

		GameObject::Sptr demon = scene->CreateGameObject("demon");
		{

			// Set and rotation position in the scene
			demon->SetPostion(glm::vec3{ 0.710f,3.150f,1.0f });
			demon->SetRotation(glm::vec3{97.0f, 0.0f, -179.0f});
			demon->SetScale(glm::vec3{ 0.460f,0.5f,0.630f });
			// Add a render component
			RenderComponent::Sptr renderer = demon->Add<RenderComponent>();
			renderer->SetMesh(demonMesh);
			renderer->SetMaterial(demonMat);

			demoBase->AddChild(demon);
		}

		GameObject::Sptr pillar = scene->CreateGameObject("pillar");
		{

			// Set and rotation position in the scene
			pillar->SetPostion(glm::vec3{ -7.320f, -.430f, 2.560f });
			pillar->SetRotation(glm::vec3{64.f, -0.f, 87.f});
			pillar->SetScale(glm::vec3{ 2.220f, 1.920f, 1.0f});
			// Add a render component
			RenderComponent::Sptr renderer = pillar->Add<RenderComponent>();
			renderer->SetMesh(pillarMesh);
			renderer->SetMaterial(pillarMat);
			RotatingBehaviour::Sptr behaviour = pillar->Add<RotatingBehaviour>();
			behaviour->RotationSpeed = glm::vec3(6.0f, 0.0f, -80.0f);
			demoBase->AddChild(pillar);
		}
		GameObject::Sptr pillar2 = scene->CreateGameObject("pillar2");
		{

			// Set and rotation position in the scene
			pillar2->SetPostion(glm::vec3{ 5.300f,10.110f, 1.f });
			pillar2->SetRotation(glm::vec3{ -36.f,0.f,0.f });
			pillar2->SetScale(glm::vec3{ 1.810f,2.530f,1.f});
			// Add a render component
			RenderComponent::Sptr renderer = pillar2->Add<RenderComponent>();
			renderer->SetMesh(pillarMesh);
			renderer->SetMaterial(pillarMat);
			RotatingBehaviour::Sptr behaviour = pillar2->Add<RotatingBehaviour>();
			behaviour->RotationSpeed = glm::vec3(6.0f, 0.0f, -40.0f);
			demoBase->AddChild(pillar2);
		}
		GameObject::Sptr pillar3 = scene->CreateGameObject("pillar3");
		{

			// Set and rotation position in the scene
			pillar3->SetPostion(glm::vec3{ 5.830f,2.780f, 1.f });
			pillar3->SetRotation(glm::vec3{ 94.f,41.f,0.f });
			pillar3->SetScale(glm::vec3{ 1.f,1.f,1.f });
			// Add a render component
			RenderComponent::Sptr renderer = pillar3->Add<RenderComponent>();
			renderer->SetMesh(pillarMesh);
			renderer->SetMaterial(pillarMat);
			RotatingBehaviour::Sptr behaviour = pillar3->Add<RotatingBehaviour>();
			behaviour->RotationSpeed = glm::vec3(20.0f, 0.0f, -50.0f);
			demoBase->AddChild(pillar3);
		}
		GameObject::Sptr pillar4 = scene->CreateGameObject("pillar4");
		{

			// Set and rotation position in the scene
			pillar4->SetPostion(glm::vec3{ -3.710f,9.780f, 1.f });
			pillar4->SetRotation(glm::vec3{ -115.f,0.f,0.f });
			pillar4->SetScale(glm::vec3{ 3.270f,3.450f,1.f });

			// Add a render component
			RenderComponent::Sptr renderer = pillar4->Add<RenderComponent>();
			renderer->SetMesh(pillarMesh);
			renderer->SetMaterial(pillarMat);
			RotatingBehaviour::Sptr behaviour = pillar4->Add<RotatingBehaviour>();
			behaviour->RotationSpeed = glm::vec3(0.0f, 0.0f, -60.0f);
			demoBase->AddChild(pillar4);
		}







		// Box to showcase the specular material
	/*	GameObject::Sptr specBox = scene->CreateGameObject("Specular Object");
		{
			MeshResource::Sptr boxMesh = ResourceManager::CreateAsset<MeshResource>();
			boxMesh->AddParam(MeshBuilderParam::CreateCube(ZERO, ONE));
			boxMesh->GenerateMesh();

			// Set and rotation position in the scene
			specBox->SetPostion(glm::vec3(0, -4.0f, 1.0f));

			// Add a render component
			RenderComponent::Sptr renderer = specBox->Add<RenderComponent>();
			renderer->SetMesh(boxMesh);
			renderer->SetMaterial(testMaterial);

			demoBase->AddChild(specBox);
		}

		// sphere to showcase the foliage material
		GameObject::Sptr foliageBall = scene->CreateGameObject("Foliage Sphere");
		{
			// Set and rotation position in the scene
			foliageBall->SetPostion(glm::vec3(-4.0f, -4.0f, 1.0f));

			// Add a render component
			RenderComponent::Sptr renderer = foliageBall->Add<RenderComponent>();
			renderer->SetMesh(sphere);
			renderer->SetMaterial(foliageMaterial);

			demoBase->AddChild(foliageBall);
		}

		// Box to showcase the foliage material
		GameObject::Sptr foliageBox = scene->CreateGameObject("Foliage Box");
		{
			MeshResource::Sptr box = ResourceManager::CreateAsset<MeshResource>();
			box->AddParam(MeshBuilderParam::CreateCube(glm::vec3(0, 0, 0.5f), ONE));
			box->GenerateMesh();

			// Set and rotation position in the scene
			foliageBox->SetPostion(glm::vec3(-6.0f, -4.0f, 1.0f));

			// Add a render component
			RenderComponent::Sptr renderer = foliageBox->Add<RenderComponent>();
			renderer->SetMesh(box);
			renderer->SetMaterial(foliageMaterial);

			demoBase->AddChild(foliageBox);
		}

		// Box to showcase the specular material
		GameObject::Sptr toonBall = scene->CreateGameObject("Toon Object");
		{
			// Set and rotation position in the scene
			toonBall->SetPostion(glm::vec3(-2.0f, -4.0f, 1.0f));

			// Add a render component
			RenderComponent::Sptr renderer = toonBall->Add<RenderComponent>();
			renderer->SetMesh(sphere);
			renderer->SetMaterial(toonMaterial);

			demoBase->AddChild(toonBall);
		}

		GameObject::Sptr displacementBall = scene->CreateGameObject("Displacement Object");
		{
			// Set and rotation position in the scene
			displacementBall->SetPostion(glm::vec3(2.0f, -4.0f, 1.0f));

			// Add a render component
			RenderComponent::Sptr renderer = displacementBall->Add<RenderComponent>();
			renderer->SetMesh(sphere);
			renderer->SetMaterial(displacementTest);

			demoBase->AddChild(displacementBall);
		}

		GameObject::Sptr multiTextureBall = scene->CreateGameObject("Multitextured Object");
		{
			// Set and rotation position in the scene 
			multiTextureBall->SetPostion(glm::vec3(4.0f, -4.0f, 1.0f));

			// Add a render component 
			RenderComponent::Sptr renderer = multiTextureBall->Add<RenderComponent>();
			renderer->SetMesh(sphere);
			renderer->SetMaterial(multiTextureMat);

			demoBase->AddChild(multiTextureBall);
		}

		GameObject::Sptr normalMapBall = scene->CreateGameObject("Normal Mapped Object");
		{
			// Set and rotation position in the scene 
			normalMapBall->SetPostion(glm::vec3(6.0f, -4.0f, 1.0f));

			// Add a render component 
			RenderComponent::Sptr renderer = normalMapBall->Add<RenderComponent>();
			renderer->SetMesh(sphere);
			renderer->SetMaterial(normalmapMat);

			demoBase->AddChild(normalMapBall);
		}

		// Create a trigger volume for testing how we can detect collisions with objects!
		GameObject::Sptr trigger = scene->CreateGameObject("Trigger");
		{
			TriggerVolume::Sptr volume = trigger->Add<TriggerVolume>();
			CylinderCollider::Sptr collider = CylinderCollider::Create(glm::vec3(3.0f, 3.0f, 1.0f));
			collider->SetPosition(glm::vec3(0.0f, 0.0f, 0.5f));
			volume->AddCollider(collider);

			trigger->Add<TriggerVolumeEnterBehaviour>();
		}
*/
		/////////////////////////// UI //////////////////////////////
		/*
		GameObject::Sptr canvas = scene->CreateGameObject("UI Canvas");
		{
			RectTransform::Sptr transform = canvas->Add<RectTransform>();
			transform->SetMin({ 16, 16 });
			transform->SetMax({ 256, 256 });

			GuiPanel::Sptr canPanel = canvas->Add<GuiPanel>();


			GameObject::Sptr subPanel = scene->CreateGameObject("Sub Item");
			{
				RectTransform::Sptr transform = subPanel->Add<RectTransform>();
				transform->SetMin({ 10, 10 });
				transform->SetMax({ 128, 128 });

				GuiPanel::Sptr panel = subPanel->Add<GuiPanel>();
				panel->SetColor(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));

				panel->SetTexture(ResourceManager::CreateAsset<Texture2D>("textures/upArrow.png"));

				Font::Sptr font = ResourceManager::CreateAsset<Font>("fonts/Roboto-Medium.ttf", 16.0f);
				font->Bake();

				GuiText::Sptr text = subPanel->Add<GuiText>();
				text->SetText("Hello world!");
				text->SetFont(font);

				monkey1->Get<JumpBehaviour>()->Panel = text;
			}

			canvas->AddChild(subPanel);
		}
		

		GameObject::Sptr particles = scene->CreateGameObject("Particles");
		{
			ParticleSystem::Sptr particleManager = particles->Add<ParticleSystem>();  
			particleManager->AddEmitter(glm::vec3(0.0f), glm::vec3(0.0f, -1.0f, 10.0f), 10.0f, glm::vec4(0.0f, 1.0f, 0.0f, 1.0f)); 
		}
		*/
		GuiBatcher::SetDefaultTexture(ResourceManager::CreateAsset<Texture2D>("textures/ui-sprite.png"));
		GuiBatcher::SetDefaultBorderRadius(8);

		// Save the asset manifest for all the resources we just loaded
		ResourceManager::SaveManifest("scene-manifest.json");
		// Save the scene to a JSON file
		scene->Save("scene.json");

		// Send the scene to the application
		app.LoadScene(scene);


	}
}

