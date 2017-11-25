#include "Animation.h"

AnimationCurve::AnimationCurve() :
	_keyFrame(nullptr),
	_boneName(nullptr),
	_start(INT_MAX),
	_end(0)
{
	_keyFrame = make_shared<ANIMATIONKEYFRAME>();
	_boneName = make_shared<string>("");
}

AnimationCurve::~AnimationCurve()
{
}

void AnimationCurve::setKey(std::vector<VECTORKEY>& keySetVec, int frame, const Vector3& value)
{
	VECTORKEY tempKey;
	tempKey.frame = frame;
	tempKey.value = value;

	auto comepare = [](const VECTORKEY& lhs, const VECTORKEY& rhs) {
		return lhs.frame < rhs.frame;
	};

	auto it = std::lower_bound(keySetVec.begin(), keySetVec.end(), tempKey, comepare);
	keySetVec.insert(it, tempKey);
}

bool AnimationCurve::getKey(const std::vector<VECTORKEY>& keySetVec, int frame, Vector3& value) const
{
	if (keySetVec.size() == 0 ||					// 안에 키가 없을 때
		frame < keySetVec.front().frame ||			// frame값이 keySet 안의 
		frame > keySetVec.back().frame)			// 프레임 값들의 범위를 벗어날 때
		return false;


	int index = 0;
	while (keySetVec[index].frame < frame)
		index++;

	if (index == 0)
		value = keySetVec[index].value;	
	else
		Vector3::Lerp(value, keySetVec[index - 1].value, keySetVec[index].value, float(AppTimer->GetDeltaTime() / 1000));	

	return true;
}

void AnimationCurve::setKey(std::vector<QUATERNIONKEY>& keySetVec, int frame, const Quaternion& value)
{
	setFrame(frame);

	QUATERNIONKEY tempKey;
	tempKey.frame = frame;
	tempKey.value = value;

	auto comepare = [](const QUATERNIONKEY& lhs, const QUATERNIONKEY& rhs) {
		return lhs.frame < rhs.frame;
	};

	auto it = std::lower_bound(keySetVec.begin(), keySetVec.end(), tempKey, comepare);
	keySetVec.insert(it, tempKey);
}

bool AnimationCurve::getKey(const std::vector<QUATERNIONKEY>& keySetVec, int frame, Quaternion& value) const
{
	if (keySetVec.size() == 0 ||					// 안에 키가 없을 때
		frame < keySetVec.front().frame ||			// frame값이 keySet 안의 
		frame > keySetVec.back().frame)			// 프레임 값들의 범위를 벗어날 때
		return false;

	int index = 0;
	while (keySetVec[index].frame < frame)
		index++;

	if (index == 0)	
		value = keySetVec[index].value;	
	else	
		Quaternion::SLerp(value, keySetVec[index - 1].value, keySetVec[index].value, float(AppTimer->GetDeltaTime() / 1000));

	return true;
}

void AnimationCurve::setFrame(int frame)
{
	if (frame < _start) _start = frame;
	if (frame > _end) _end = frame;
	_length = _end - _start;
}

void AnimationCurve::SetSprite(int frame, const shared_ptr<Sprite> sprite)
{
	SPRITEKEY tempKey;
	tempKey.frame = frame * 60;
	tempKey.value = sprite;

	auto comepare = [](const SPRITEKEY& lhs, const SPRITEKEY& rhs) {
		return lhs.frame < rhs.frame;
	};

	auto it = std::lower_bound(_keyFrame->sprite.begin(), _keyFrame->sprite.end(), tempKey, comepare);
	_keyFrame->sprite.insert(it, tempKey);
}

shared_ptr<Sprite> AnimationCurve::GetSprite(int frame) const
{
	if (_keyFrame->sprite.size() == 0 ||					// 안에 키가 없을 때
		frame < _keyFrame->sprite.front().frame ||			// frame값이 keySet 안의 
		frame > _keyFrame->sprite.back().frame)			// 프레임 값들의 범위를 벗어날 때
		return nullptr;

	int index = 0;
	while (_keyFrame->sprite[index].frame < frame)
		index++;

	if (index == 0)
		return _keyFrame->sprite[index].value;

	if (frame - _keyFrame->sprite[index].frame > frame - _keyFrame->sprite[index - 1].frame)
		return _keyFrame->sprite[index - 1].value;

	return _keyFrame->sprite[index].value;
}

Animation::Animation()
	: ResourceBase(),
	_curves(nullptr)
{
	_name = make_shared<string>();
	_curves = make_unique<AnimCurveVec>();
}

Animation::~Animation()
{
}

bool Animation::Create(void* pData, void* pResourceData, const string& name)
{
	string* pPath = static_cast<string*>(pResourceData);
	*_name = name;

	Json::Value json;
	JsonUtility::ReadFromFile(*pPath, json);

	Json::Value keyFrames = json["key_frames"];

	for (auto keyFrame : keyFrames)
	{
		auto animCurve = make_shared<AnimationCurve>();
		animCurve->SetBoneName(keyFrame["bone_name"].asString());

		for (auto key : keyFrame["keys"])
		{
			int frame = key["frame"].asInt() * 60;
			animCurve->SetScale(frame,
				Vector3(key["scale"]["x"].asFloat(),
				key["scale"]["y"].asFloat(),
				key["scale"]["z"].asFloat()));
			animCurve->SetRotation(frame,
				Quaternion(key["rotation"]["x"].asFloat(),
					key["rotation"]["y"].asFloat(),
					key["rotation"]["z"].asFloat(),
					key["rotation"]["w"].asFloat()));
			animCurve->SetPosition(frame,
				Vector3(key["translation"]["x"].asFloat(),
					key["translation"]["y"].asFloat(),
					key["translation"]["z"].asFloat()));
		}

		AddAnimationCurve(animCurve);
	}

	return true;
}

void Animation::Destroy()
{
}