#include "Precompiled.h"
#include "GameObjectFactory.h"
#include "GameObject.h"
#include "CameraComponent.h"
#include "ColliderComponent.h"
#include "FPSCameraControllerComponent.h"
#include "TransformComponent.h"
#include "ModelComponent.h"
#include "MeshComponent.h"
#include "AnimatorComponent.h"
#include "RigidBodyComponent.h"

using namespace NULLEngine;
using namespace NULLEngine::Graphics;
using namespace NULLEngine::EMath;

namespace rj = rapidjson;

namespace
{
    CustomMake TryMake;
}

void NULLEngine::GameObjectFactory::SetCustomMake(CustomMake customMake)
{
    TryMake = customMake;
}


void GameObjectFactory::Make(const std::filesystem::path& templateFile, GameObject& gameObject)
{
    FILE* file = nullptr;
    auto err = fopen_s(&file, templateFile.u8string().c_str(), "r");
    ASSERT(err == 0 && file != nullptr, "GameObjectFactory: failed to open template file %s", templateFile.u8string().c_str());

    char readBuffer[60000];
    rj::FileReadStream readStream(file, readBuffer, sizeof(readBuffer));
    fclose(file);

    rj::Document doc;
    doc.ParseStream(readStream);

    auto components = doc["Components"].GetObj();

    for (auto& component : components)
    {
        const char* componentName = component.name.GetString();

        if (TryMake(componentName, component.value, gameObject))
        {
            // custom component used this

        }
        else if (strcmp(componentName, "CameraComponent") == 0)
        {
            NULLEngine::CameraComponent* cameraComponent = gameObject.AddComponent<NULLEngine::CameraComponent>();
            if (component.value.HasMember("Position"))
            {
                const auto& position = component.value["Position"].GetArray();
                const float x = position[0].GetFloat();
                const float y = position[1].GetFloat();
                const float z = position[2].GetFloat();
                cameraComponent->GetCamera().SetPosition({ x,y,z });
            }

            if (component.value.HasMember("LookAt"))
            {
                const auto& lookAt = component.value["LookAt"].GetArray();
                const float x = lookAt[0].GetFloat();
                const float y = lookAt[1].GetFloat();
                const float z = lookAt[2].GetFloat();
                cameraComponent->GetCamera().SetLookAt({ x,y,z });
            }
        }
        else if (strcmp(componentName, "FPSCameraControllerComponent") == 0)
        {
            FPSCameraControllerComponent* fpsCamera = gameObject.AddComponent<FPSCameraControllerComponent>();
            if (component.value.HasMember("MoveSpeed"))
            {
                const float moveSpeed = component.value["MoveSpeed"].GetFloat();
                fpsCamera->SetMoveSpeed(moveSpeed);
            }

            if (component.value.HasMember("TurnSpeed"))
            {
                const float turnSpeed = component.value["TurnSpeed"].GetFloat();
                fpsCamera->SetTurnSpeed(turnSpeed);
            }
        }
        else if (strcmp(componentName, "TransformComponent") == 0)
        {
            TransformComponent* transform = gameObject.AddComponent<TransformComponent>();

            if (component.value.HasMember("Position"))
            {
                const auto& position = component.value["Position"].GetArray();
                const float x = position[0].GetFloat();
                const float y = position[1].GetFloat();
                const float z = position[2].GetFloat();

                transform->position = EMath::Vector3(x, y, z);
            }

            if (component.value.HasMember("Rotation"))
            {
                const auto& rotation = component.value["Rotation"].GetArray();
                const float rot_x = rotation[0].GetFloat();
                const float rot_y = rotation[1].GetFloat();
                const float rot_z = rotation[2].GetFloat();
                const float rot_w = rotation[3].GetFloat();

                transform->rotation = Quaternion(rot_x, rot_y, rot_z, rot_w);
            }

            if (component.value.HasMember("Scale"))
            {
                const auto& scale = component.value["Scale"].GetArray();
                const float scale_x = scale[0].GetFloat();
                const float scale_y = scale[1].GetFloat();
                const float scale_z = scale[2].GetFloat();

                transform->scale = EMath::Vector3(scale_x, scale_y, scale_z);
            }
        }
        else if (strcmp(componentName, "ModelComponent") == 0)
        {
            ModelComponent* modelComponent = gameObject.AddComponent<ModelComponent>();
            if (component.value.HasMember("FileName"))
            {
                const char* fileName = component.value["FileName"].GetString();
                modelComponent->SetFileName(fileName);
            }

            if (component.value.HasMember("Animations"))
            {
                const auto& animationNames = component.value["Animations"].GetArray();

                for (const auto& animationName : animationNames)
                {
                    const char* animation = animationName.GetString();
                    modelComponent->AddAnimation(animation);
                }
            }
        }
        else if (strcmp(componentName, "AnimatorComponent") == 0)
        {
            AnimatorComponent* animatorComponent = gameObject.AddComponent<AnimatorComponent>();
            // read in starting animation
        }
        else if (strcmp(componentName, "MeshComponent") == 0)
        {
            MeshComponent* meshComponent = gameObject.AddComponent<MeshComponent>();
            if (component.value.HasMember("Shape"))
            {
                const auto& shapeData = component.value["Shape"].GetObj();
                if (shapeData.HasMember("Type"))
                {
                    const char* shapeType = shapeData["Type"].GetString();
                    if (strcmp(shapeType, "Cube") == 0)
                    {
                        const float size = shapeData["Size"].GetFloat();
                        meshComponent->CreateCube(size);
                    }
                    else if (strcmp(shapeType, "Sphere") == 0)
                    {
                        const int slices = shapeData["Slices"].GetInt();
                        const int rings = shapeData["Rings"].GetInt();
                        const float radius = shapeData["Radius"].GetFloat();
                        meshComponent->CreateSphere(slices, rings, radius);
                    }
                    else if (strcmp(shapeType, "Plane") == 0)
                    {
                        const int rows = shapeData["Rows"].GetInt();
                        const int columns = shapeData["Columns"].GetInt();
                        const float spacing = shapeData["Spacing"].GetFloat();
                        meshComponent->CreatePlane(rows, columns, spacing);
                    }
                    else if (strcmp(shapeType, "RevertedPlane") == 0)
                    {
                        const int rows = shapeData["Rows"].GetInt();
                        const int columns = shapeData["Columns"].GetInt();
                        const float spacing = shapeData["Spacing"].GetFloat();
                        meshComponent->CreateRevertedPlane(rows, columns, spacing);
                    }
                    else
                    {
                        ASSERT(false, "ShapeType: %s is not valid", shapeType);
                    }
                }

            }
            if (component.value.HasMember("Material"))
            {
                Material material;
                const auto& materialData = component.value["Material"].GetObj();
                if (materialData.HasMember("Ambient"))
                {
                    const auto& color = materialData["Ambient"].GetArray();
                    material.ambient.r = color[0].GetFloat();
                    material.ambient.g = color[1].GetFloat();
                    material.ambient.b = color[2].GetFloat();
                    material.ambient.a = color[3].GetFloat();
                }
                if (materialData.HasMember("Diffues"))
                {
                    const auto& color = materialData["Diffues"].GetArray();
                    material.diffuse.r = color[0].GetFloat();
                    material.diffuse.g = color[1].GetFloat();
                    material.diffuse.b = color[2].GetFloat();
                    material.diffuse.a = color[3].GetFloat();
                }
                if (materialData.HasMember("Specular"))
                {
                    const auto& color = materialData["Specular"].GetArray();
                    material.specular.r = color[0].GetFloat();
                    material.specular.g = color[1].GetFloat();
                    material.specular.b = color[2].GetFloat();
                    material.specular.a = color[3].GetFloat();
                }
                if (materialData.HasMember("Emissive"))
                {
                    const auto& color = materialData["Emissive"].GetArray();
                    material.emissive.r = color[0].GetFloat();
                    material.emissive.g = color[1].GetFloat();
                    material.emissive.b = color[2].GetFloat();
                    material.emissive.a = color[3].GetFloat();
                }
                if (materialData.HasMember("SpecularPower"))
                {
                    material.power = materialData["SpecularPower"].GetFloat();
                }
                meshComponent->SetMaterial(material);
            }
            if (component.value.HasMember("Textures"))
            {
                const auto& textureData = component.value["Textures"].GetObj();
                if (textureData.HasMember("DiffuseMap"))
                {
                    const char* diffuseMapFileName = textureData["DiffuseMap"].GetString();
                    meshComponent->SetDiffuseMap(diffuseMapFileName);
                }
                if (textureData.HasMember("DisplacementMap"))
                {
                    const char* displacementMap = textureData["DisplacementMap"].GetString();
                    meshComponent->SetDisplacementMap(displacementMap);
                }
                if (textureData.HasMember("NormalMap"))
                {
                    const char* normalMap = textureData["NormalMap"].GetString();
                    meshComponent->SetNormalMap(normalMap);
                }
                if (textureData.HasMember("SpecularMap"))
                {
                    const char* specularMapFileName = textureData["SpecularMap"].GetString();
                    meshComponent->SetSpecularMap(specularMapFileName);
                }
            }
        }
        else if (strcmp(componentName, "ColliderComponent") == 0)
        {
            ColliderComponent* colliderComponent = gameObject.AddComponent<ColliderComponent>();
            if (component.value.HasMember("Shape"))
            {
                const auto& shapeData = component.value["Shape"].GetObj();
                if (shapeData.HasMember("Type"))
                {
                    const char* shapeType = shapeData["Type"].GetString();
                    if (strcmp(shapeType, "Box") == 0)
                    {
                        const auto& halfExtents = shapeData["HalfExtents"].GetArray();
                        const float x = halfExtents[0].GetFloat();
                        const float y = halfExtents[1].GetFloat();
                        const float z = halfExtents[2].GetFloat();
                        colliderComponent->SetBoxCollider({ x,y,z });
                    }
                    else if (strcmp(shapeType, "Sphere") == 0)
                    {
                        const float radius = shapeData["Radius"].GetFloat();
                        colliderComponent->SetSphereCollider(radius);
                    }
                    else if (strcmp(shapeType, "Hull") == 0)
                    {
                        const auto& halfExtents = shapeData["HalfExtents"].GetArray();
                        const auto& origin = shapeData["Origin"].GetArray();

                        const float ex = halfExtents[0].GetFloat();
                        const float ey = halfExtents[1].GetFloat();
                        const float ez = halfExtents[2].GetFloat();

                        const float ox = origin[0].GetFloat();
                        const float oy = origin[1].GetFloat();
                        const float oz = origin[2].GetFloat();

                        colliderComponent->SetHullCollider({ ex,ey,ez }, { ox,oy,oz });
                    }
                    else
                    {
                        ASSERT(false, "ShapeType: %s is not valid", shapeType);
                    }
                }

            }
        }
        else if (strcmp(componentName, "RigidBodyComponent") == 0)
        {
            RigidBodyComponent* rigidBodyComponent = gameObject.AddComponent<RigidBodyComponent>();
            if (component.value.HasMember("Mass"))
            {
                const float mass = component.value["Mass"].GetFloat();
                rigidBodyComponent->SetMass(mass);
            }
        }
        else
        {
            ASSERT(false, "ObjectFactory: %s was not defined", componentName);
        }
    }
}