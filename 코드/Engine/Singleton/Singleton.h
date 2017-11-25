#pragma once

#include "Application.h"
#include "InputManager.h"
#include "ResourceManager.h"
#include "SceneManager.h"

#define Resources ResourceManager::Instance()
#define Input InputManager::Instance()
#define Scenes SceneManager::Instance()