#pragma once

#include "Application.h"
#include "GameObjectManager.h"
#include "InputManager.h"
#include "ResourceManager.h"
#include "MaterialManager.h"

#define Resources ResourceManager::Instance()
#define Input InputManager::Instance()