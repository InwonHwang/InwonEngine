#include "Animation.h"

AnimationCurve::AnimationCurve()
{
}

AnimationCurve::~AnimationCurve()
{
}

void AnimationCurve::SetScaleKey(int frame, const Vector3& scale)
{
	setKey(_keyFrame.scale, frame, scale);
}
void AnimationCurve::SetRotationKey(int frame, const Quaternion& rotation)
{
	setKey(_keyFrame.rotation, frame, rotation);
}
void AnimationCurve::SetPositionKey(int frame, const Vector3& position)
{
	setKey(_keyFrame.position, frame, position);
}

bool AnimationCurve::GetScaleKey(int frame, Vector3& scale) const
{
	return getKey(_keyFrame.scale, frame, scale);
}
bool AnimationCurve::GetRotationKey(int frame, Quaternion& rotation) const
{
	return getKey(_keyFrame.rotation, frame, rotation);
}
bool AnimationCurve::GetPositionKey(int frame, Vector3& position) const
{
	return getKey(_keyFrame.position, frame, position);
}

void AnimationCurve::GetAnimatedMatrix(int frame, D3DXMATRIX& matrix) const
{
	Vector3 v(0, 0, 0);
	Quaternion q(0, 0, 0, 0);

	D3DXMatrixIdentity(&matrix);

	if (GetRotationKey(frame, q)) {
		D3DXMatrixRotationQuaternion(&matrix, &q);
	}

	if (GetPositionKey(frame, v)) {
		matrix._41 = v.x;
		matrix._42 = v.y;
		matrix._43 = v.z;
	}

	if (GetScaleKey(frame, v)) {
		matrix._11 *= v.x;
		matrix._22 *= v.y;
		matrix._33 *= v.z;
	}
}

int AnimationCurve::GetLength() const
{
	return _keyFrame.length;
}

void AnimationCurve::SetFrame()
{
	if (_keyFrame.scale.size() > 0)
	{
		_keyFrame.start = _keyFrame.scale.front().frame;
		_keyFrame.end = _keyFrame.scale.back().frame;
		_keyFrame.length = _keyFrame.end - _keyFrame.start;
	}
	else if (_keyFrame.sprite.size() > 0)
	{
		_keyFrame.start = _keyFrame.sprite.front().frame;
		_keyFrame.end = _keyFrame.sprite.back().frame;
		_keyFrame.length = _keyFrame.end - _keyFrame.start;
	}
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
	{
		value = keySetVec[index].value;
	}
	else
	{
		Vector3::Lerp(value, keySetVec[index - 1].value, keySetVec[index].value, float(AppTimer->GetDeltaTime() / 1000));
	}

	return true;
}

void AnimationCurve::setKey(std::vector<QUATERNIONKEY>& keySetVec, int frame, const Quaternion& value)
{
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
	{
		value = keySetVec[index].value;
	}
	else
	{
		Quaternion::SLerp(value, keySetVec[index - 1].value, keySetVec[index].value, float(AppTimer->GetDeltaTime() / 1000));

	}

	return true;
}

void AnimationCurve::SetSpriteKey(int frame, const shared_ptr<Sprite> sprite)
{
	SPRITEKEY tempKey;
	tempKey.frame = frame * 60;
	tempKey.value = sprite;

	auto comepare = [](const SPRITEKEY& lhs, const SPRITEKEY& rhs) {
		return lhs.frame < rhs.frame;
	};

	auto it = std::lower_bound(_keyFrame.sprite.begin(), _keyFrame.sprite.end(), tempKey, comepare);
	_keyFrame.sprite.insert(it, tempKey);
}

shared_ptr<Sprite> AnimationCurve::GetSpriteKey(int frame)
{
	if (_keyFrame.sprite.size() == 0 ||					// 안에 키가 없을 때
		frame < _keyFrame.sprite.front().frame ||			// frame값이 keySet 안의 
		frame > _keyFrame.sprite.back().frame)			// 프레임 값들의 범위를 벗어날 때
		return nullptr;

	int index = 0;
	while (_keyFrame.sprite[index].frame < frame)
		index++;

	if (index == 0)
	{
		return _keyFrame.sprite[index].value;
	}

	if (frame - _keyFrame.sprite[index].frame > frame - _keyFrame.sprite[index - 1].frame)
		return _keyFrame.sprite[index - 1].value;

	return _keyFrame.sprite[index].value;
}

//void Animation::SetAnimCurveCount(int count)
//{
//	_curves->resize(count);
//}
//void Animation::SetAnimCurve(int index, const shared_ptr<AnimationCurve> pAnimCurve)
//{
//	_curves->data()[index] = pAnimCurve;
//
//	pAnimCurve->SetFrame();
//}

bool Animation::Create(void* pData, void* pResourceData, const string& name)
{
	_name = make_shared<string>(name);
	/*_curves.reset(new AnimCurveVec);

	vector<shared_ptr<FBXBONEDATA>>* pFbxBoneDataVec = static_cast<vector<shared_ptr<FBXBONEDATA>>*>(pResourceData);

	if (!pFbxBoneDataVec) return true;

	shared_ptr<AnimationCurve> pAnimCurve;
	shared_ptr<KEYFRAME> pKeyFrame;
	Vector3 v(0, 0, 0);
	Quaternion q(0, 0, 0, 0);
	
	for (auto boneData : *pFbxBoneDataVec)
	{
		pAnimCurve.reset(new AnimationCurve);
		
		for (int i = 0; i < boneData->animVec[0].size(); ++i)
		{
			pKeyFrame = boneData->animVec[0][i];

			FbxAMatrix tempMatrix = pKeyFrame->local;
			int frame = (static_cast<int>(pKeyFrame->frame) - boneData->start) * 60;

			v = FbxUtility::ToVector3(tempMatrix.GetS());
			pAnimCurve->SetScaleKey(frame, v);

			q = FbxUtility::ToQuaternion(tempMatrix.GetQ());
			pAnimCurve->SetRotationKey(frame, q);

			v = FbxUtility::ToVector3(tempMatrix.GetT());
			pAnimCurve->SetPositionKey(frame, v);
		}

		pAnimCurve->SetFrame();

		_curves->push_back(pAnimCurve);
	}
*/
	return true;
}

void Animation::Destroy()
{
	_curves.reset();
}

void Animation::GetAnimatedMatrix(int index, int frame, D3DXMATRIX& m) const
{
	assert(index < _curves->size());

	_curves->data()[index]->GetAnimatedMatrix(frame, m);
}

shared_ptr<Sprite> Animation::GetSprite(int frame) const
{
	return _curves->data()[0]->GetSpriteKey(frame);
}

int Animation::GetLength(int index) const
{
	assert(index < _curves->size());

	return _curves->data()[index]->GetLength();
}