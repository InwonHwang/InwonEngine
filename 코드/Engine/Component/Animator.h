#pragma once

#include "..\Core\Core.h"

class Transform;
class Animation;
class GameObject;

typedef struct tagAnimationCallBackHandler
{
	int		_startOfCurAnim;
	int		_startOfNextAnim;
	bool	_playedOnce;
	int		_point;
	float	_fadeLength;

	tagAnimationCallBackHandler()
		: _startOfCurAnim(0),
		_startOfNextAnim(0),
		_playedOnce(false),
		_point(0),
		_fadeLength(0)
	{
	}
}ANIMATIONCALLBACKHANDLER;

class Animator : public ComponentBase
{
	typedef vector<shared_ptr<Transform>> BoneVec;
	typedef vector<shared_ptr<Animation>> AnimationVec;

private:
	shared_ptr<ANIMATIONCALLBACKHANDLER> _animationHandler;
	shared_ptr<AnimationVec>			 _animations;
	shared_ptr<BoneVec>					 _bones;
	shared_ptr<Animation>				 _curAnim;
	shared_ptr<Animation>				 _nextAnim;

public:
	Animator(const shared_ptr<GameObject> gameObject);
	~Animator();

	virtual void Update(void* pData) override;
	virtual void Create(void* pData) override;
	virtual void Destroy() override;

	virtual shared_ptr<void> Clone() override;
	virtual void FromJson(const Json::Value& json) override;
	virtual void ToJson(Json::Value& json) override;

	void Play(int index);
	void CrossFade(int index, float fadeLength = 0.3f);

	void SetAnimationCount(int count) { _animations->resize(count); }
	void SetAnimation(int index, const shared_ptr<Animation> anim) { _animations->data()[index] = anim; }

	shared_ptr<ANIMATIONCALLBACKHANDLER> GetAnimationCallbackHandler() const { return _animationHandler; }
	const shared_ptr<BoneVec>			 GetBones() const { return _bones; }
	const shared_ptr<Animation>			 GetCurrentAnimation() const { return _curAnim; }
	const shared_ptr<Animation>			 GetNextAnimation() const { return _nextAnim; }
	const shared_ptr<Animation>			 GetAnimation(int index) const { return _animations->data()[index]; }
	int									 GetAnimationCount() const { return _animations->size(); }
};

