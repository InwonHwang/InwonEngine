#include "AnimationSystem.h"
#include "..\Resource\Resource.h"
#include "..\Component\Component.h"

AnimationSystem::AnimationSystem()
{
}

AnimationSystem::~AnimationSystem()
{
}

void AnimationSystem::Update()
{
	auto animatorObjects = GameObject::FindObjectsOfType<Animator>();

	for (auto animatorObject : *animatorObjects) {
		auto animator = animatorObject->GetComponent<Animator>();

		ANIMATORINFO animatorInfo;
		animatorInfo.curAnim = animator->GetCurrentAnimation();
		animatorInfo.nextAnim = animator->GetNextAnimation();
		animatorInfo.animationHandler = animator->GetAnimationCallbackHandler();
		animatorInfo.bones = animator->GetBones();

		updateAnimationInfo(animatorInfo);

		updateAndBlend(animatorInfo);
		update(animatorInfo);

	}
	//updateSprite();

}


void AnimationSystem::getAnimatedMatrix(shared_ptr<AnimationCurve> animCurve, int frame, D3DXMATRIX& matrix)
{
	Vector3 v(0, 0, 0);
	Quaternion q(0, 0, 0, 0);

	D3DXMatrixIdentity(&matrix);

	if (animCurve->GetRotation(frame, q)) {
		D3DXMatrixRotationQuaternion(&matrix, &q);
	}

	if (animCurve->GetPosition(frame, v)) {
		matrix._41 = v.x;
		matrix._42 = v.y;
		matrix._43 = v.z;
	}

	if (animCurve->GetScale(frame, v)) {
		matrix._11 *= v.x;
		matrix._22 *= v.y;
		matrix._33 *= v.z;
	}
}

void AnimationSystem::updateAnimationInfo(ANIMATORINFO& animatorInfo)
{
	auto curAnim = animatorInfo.curAnim;
	auto nextAnim = animatorInfo.nextAnim;
	auto animHandler = animatorInfo.animationHandler;

	if (!curAnim) return;

	int length = curAnim->GetAnimationCurve(0)->GetLength();
	animHandler->_playedOnce = ((AppTimer->GetElapsedTime() - animHandler->_startOfCurAnim) / length > 0);

	if (!nextAnim) return;
	
	float t = (AppTimer->GetElapsedTime() - animHandler->_point) / length;
	if (t > animHandler->_fadeLength)
	{
		animHandler->_startOfCurAnim = animHandler->_startOfNextAnim;
		animHandler->_startOfNextAnim = 0;
		curAnim = nextAnim;
		nextAnim = nullptr;
	}	
}

void AnimationSystem::updateAndBlend(ANIMATORINFO& animatorInfo)
{
	auto curAnim = animatorInfo.curAnim;
	auto nextAnim = animatorInfo.nextAnim;
	auto animHandler = animatorInfo.animationHandler;
	auto bones = animatorInfo.bones;

	if (!nextAnim) return;

	int length0 = curAnim->GetAnimationCurve(0)->GetLength();
	int length1 = nextAnim->GetAnimationCurve(0)->GetLength();
	int frame0 = (AppTimer->GetElapsedTime() - animHandler->_startOfCurAnim) % length0;
	int frame1 = (AppTimer->GetElapsedTime() - animHandler->_startOfNextAnim) % length1;

	float point = float(AppTimer->GetElapsedTime() - animHandler->_point) / float(length0);
	float weight0 = 2.0f * point / animHandler->_fadeLength;
	float weight1 = 2.0f - weight0;

	D3DXMATRIX m, m0, m1;
	Vector3 s, t;
	Quaternion r;

	int i = 0;
	for (auto bone : *bones)
	{
		auto animCurve0 = curAnim->GetAnimationCurve(i);
		auto animCurve1 = nextAnim->GetAnimationCurve(i++);

		getAnimatedMatrix(animCurve0, frame0, m0);
		getAnimatedMatrix(animCurve1, frame1, m1);

		m = (m0 * weight1 + m1 * weight0) / 2;
		
		D3DXMatrixDecompose(&s, &r, &t, &m);

		bone->SetLocalScale(s);
		bone->SetLocalRotation(r);
		bone->SetLocalPosition(t);
	}
}

void AnimationSystem::update(ANIMATORINFO& animatorInfo)
{
	auto curAnim = animatorInfo.curAnim;
	auto nextAnim = animatorInfo.nextAnim;
	auto animHandler = animatorInfo.animationHandler;
	auto bones = animatorInfo.bones;

	if (nextAnim || !curAnim) return;

	int length = curAnim->GetAnimationCurve(0)->GetLength();
	int frame = !curAnim->GetLoop() && animHandler->_playedOnce ?
		curAnim->GetAnimationCurve(0)->GetLength() - 1 :
		(AppTimer->GetElapsedTime() - animHandler->_startOfCurAnim) % length;

	D3DXMATRIX m;
	Vector3 s, t;
	Quaternion r;
	int i = 0;
	for (auto bone : *bones)
	{
		auto animCurve = curAnim->GetAnimationCurve(i++);
		animCurve->GetScale(frame, s);
		animCurve->GetRotation(frame, r);
		animCurve->GetPosition(frame, t);

		bone->SetLocalScale(s);
		bone->SetLocalRotation(r);
		bone->SetLocalPosition(t);
	}
}

void AnimationSystem::updateSprite(ANIMATORINFO& animatorInfo)
{
	auto curAnim = animatorInfo.curAnim;
	auto nextAnim = animatorInfo.nextAnim;
	auto animHandler = animatorInfo.animationHandler;
	auto bones = animatorInfo.bones;

	/*shared_ptr<SpriteRenderer> pRenderer = _gameObject->GetComponent<SpriteRenderer>();

	if (!pRenderer) return;

	int length = curAnim->GetAnimationCurve(0)->GetLength();
	int frame = (AppTimer->GetElapsedTime() - animHandler->_startOfCurAnim) % length;

	pRenderer->SetSprite(curAnim->GetAnimationCurve(0)->GetSprite(frame));*/
}
