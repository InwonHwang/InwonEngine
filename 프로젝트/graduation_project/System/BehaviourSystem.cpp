#include "BehaviourSystem.h"
#include "..\Resource\Resource.h"
#include "..\Component\Component.h"

BehaviourSystem::BehaviourSystem()
{
}


BehaviourSystem::~BehaviourSystem()
{
}

void BehaviourSystem::Update()
{
	auto behaviourObjects = GameObject::FindObjectsOfType<Behaviour>();

	for (auto behaviourObject : *behaviourObjects) {
		behaviourObject->GetComponent<Behaviour>()->Update();
	}	
}
