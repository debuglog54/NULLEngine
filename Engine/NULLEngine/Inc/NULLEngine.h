#pragma once

#include "Common.h"
#include "App.h"
#include "AppState.h"
#include "GameObject.h"
#include "GameObjectFactory.h"
#include "Component.h"
#include "TypeIds.h"
#include "Service.h"

// Components
#include "TransformComponent.h"
#include "CameraComponent.h"
#include "FPSCameraControllerComponent.h"
#include "GameWorld.h"
#include "GameObjectHandle.h"
#include "ModelComponent.h"
#include "ColliderComponent.h"
#include "RigidBodyComponent.h"

// Services
#include "CameraService.h"
#include "RenderService.h"
#include "MeshComponent.h"
#include "AnimatorComponent.h"



namespace NULLEngine
{
    App& MainApp();
}