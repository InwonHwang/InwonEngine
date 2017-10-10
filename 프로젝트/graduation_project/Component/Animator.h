#pragma once

#include "..\Core\Core.h"
#include "..\Resource\Resource.h"

class Transform;

class Animator : public ComponentBase
{
	typedef struct tagAnimationCallBackHandler
	{
		int _startOfCurAnim;
		int _startOfNextAnim;

		tagAnimationCallBackHandler()
			: _startOfCurAnim(0),
			_startOfNextAnim(0)
		{
		}
	}ANIMATIONCALLBACKHANDLER;

	typedef vector<weak_ptr<Transform>> TransformList;
	typedef vector<shared_ptr<Animation>> AnimationVec;

public:
	Animator()
		: 
		_animationHandler(nullptr),
		_animations(nullptr),
		_bones(nullptr),
		_curAnim(nullptr),
		_nextAnim(nullptr),
		_point(0),
		_cycle(0)
	{
	}
	~Animator() {}

	virtual void Update(void* pData) override;
	virtual void Create(void* pData, const shared_ptr<void> gameObject) override;
	virtual void Destroy() override;

	void SetRootBone(const shared_ptr<Transform> pRootBone);
	void SetAnimationCount(int count);
	void SetAnimation(int index, const shared_ptr<Animation> pAnim);

	void Play(int index);
	void CrossFade(int index, float fadeLength = 0.3f);

private:

	void UpdateInfo();
	// 애니메이션이 변경될때 겹치는 부분 보간하기.
	void UpdateBlendedMatrix();
	void UpdateAnimatedMatrix();
	void UpdateFinalAnimatedMatrix();
	void UpdateSprite();

private:
	shared_ptr<ANIMATIONCALLBACKHANDLER>	_animationHandler;
	shared_ptr<AnimationVec>				_animations;
	shared_ptr<TransformList>				_bones;	
	shared_ptr<Animation>					_curAnim;
	shared_ptr<Animation>					_nextAnim;

	int								_point;
	int								_cycle;
	float							_fadeLength;
};

