#include "Animator.h"
#include "Transform.h"
#include "SpriteRenderer.h"
#include "SkinnedMeshRenderer.h"
#include "..\Resource\Resource.h"

Animator::Animator(const shared_ptr<GameObject> gameObject)
	: ComponentBase(gameObject),
	_animationHandler(nullptr),
	_animations(nullptr),
	_bones(nullptr),
	_curAnim(nullptr),
	_nextAnim(nullptr)
{
	_animationHandler = make_shared<ANIMATIONCALLBACKHANDLER>();
	_animations = make_shared<AnimationVec>();
	_bones = make_shared<BoneVec>();
}

Animator::~Animator()
{
}

void Animator::Update(void* pData)
{
}

void Animator::Create(void* pData)
{
	auto skinnedMeshRenderer = _gameObject->GetComponentInChildren<SkinnedMeshRenderer>();

	if (!skinnedMeshRenderer) return;

	auto bones = skinnedMeshRenderer->GetBones();

	_bones->assign(bones->begin(), bones->end());
}

void Animator::Destroy()
{
}

shared_ptr<void> Animator::Clone() 
{ 
	shared_ptr<Animator> animator = make_shared<Animator>(nullptr);
	animator->_animations->assign(_animations->begin(), _animations->end());

	return animator;
}

void Animator::FromJson(const Json::Value& json)
{
	SetEnable(json["is_enabled"].asBool());

	auto transform = GetComponent<Transform>();

	for (auto boneName : json["bones"]) {
		auto bone = transform->FindChild(boneName.asString());

		if (!bone) continue;

		_bones->push_back(bone);
	}

	for (auto animationName : json["animations"])
	{
		auto animation = Resources->Create<Animation>(animationName.asString());

		if (!animation) continue;

		_animations->push_back(animation);
	}
}

void Animator::ToJson(Json::Value& json)
{
	json["is_enable"] = _isEnabled;

	for (auto bone : *_bones)
		json["bones"].append(*bone->GetGameObject()->GetName());

	for (auto animation : *_animations)
		json["animations"].append(*animation->GetName());
}

void Animator::Play(int index)
{
	if (index > _animations->size() - 1 || 
		_animations->data()[index] == _curAnim) return;

	_curAnim = _animations->data()[index];
	_nextAnim = nullptr;
	_animationHandler->_startOfCurAnim = AppTimer->GetElapsedTime();
}

void Animator::CrossFade(int index, float fadeLength)
{
	if (index > _animations->size() - 1 ||
		_curAnim == _animations->data()[index] ||
		_nextAnim) return;

	_nextAnim = _animations->data()[index];
	_animationHandler->_startOfNextAnim = AppTimer->GetElapsedTime();
	// 현재시간
	_animationHandler->_point = AppTimer->GetElapsedTime();
	_animationHandler->_fadeLength = fadeLength;
}