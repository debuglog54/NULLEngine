#include "Precompiled.h"
#include "TransformComponent.h"
#include "GameObject.h"

using namespace NULLEngine;

void TransformComponent::DebugUI()
{
    Graphics::SimpleDraw::AddTransform(GetMatrix4());
}

void TransformComponent::EditorUI()
{
    ImGui::LabelText("Transform","");
    auto uniqueId = GetOwner().GetUniqueId();

    std::string positionStr = "Position##" + std::to_string(uniqueId);
    std::string rotationStr = "Rotation##" + std::to_string(uniqueId);
    std::string scaleStr = "Scale##" + std::to_string(uniqueId);

    ImGui::DragFloat3(positionStr.c_str(), &position.x,0.01);
    ImGui::DragFloat3(rotationStr.c_str(), &rotation.x, 0.01);
    ImGui::DragFloat3(scaleStr.c_str(), &scale.x, 0.01);
}

void TransformComponent::Serialize(rapidjson::Document& doc, rapidjson::Value& value)
{
    rapidjson::Value componentValue(rapidjson::kObjectType);

    rapidjson::Value posArray(rapidjson::kArrayType);
    posArray.PushBack(position.x, doc.GetAllocator());
    posArray.PushBack(position.y, doc.GetAllocator());
    posArray.PushBack(position.z, doc.GetAllocator());
    componentValue.AddMember("Position", posArray, doc.GetAllocator());

    rapidjson::Value rotArray(rapidjson::kArrayType);
    rotArray.PushBack(rotation.x, doc.GetAllocator());
    rotArray.PushBack(rotation.y, doc.GetAllocator());
    rotArray.PushBack(rotation.z, doc.GetAllocator());
    rotArray.PushBack(rotation.w, doc.GetAllocator());
    componentValue.AddMember("Rotation", rotArray, doc.GetAllocator());

    rapidjson::Value scaleArray(rapidjson::kArrayType);
    scaleArray.PushBack(scale.x, doc.GetAllocator());
    scaleArray.PushBack(scale.y, doc.GetAllocator());
    scaleArray.PushBack(scale.z, doc.GetAllocator());
    componentValue.AddMember("Scale", scaleArray, doc.GetAllocator());

    value.AddMember("TransformComponent", componentValue, doc.GetAllocator());
}