#pragma once

#include "..\Core\Core.h"

class Transform;
class Animation;
class Animator;
class AnimationCurve;
typedef struct tagAnimationCallBackHandler ANIMATIONCALLBACKHANDLER;

class AnimationSystem
{
	typedef struct tagAnimatorInfo
	{
		shared_ptr<ANIMATIONCALLBACKHANDLER>		animationHandler;
		shared_ptr<vector<shared_ptr<Transform>>>	bones;
		shared_ptr<Animation>						curAnim;
		shared_ptr<Animation>						nextAnim;
	}ANIMATORINFO;

private:
	void getAnimatedMatrix(shared_ptr<AnimationCurve> animCurve, int frame, D3DXMATRIX& matrix);

	void updateAnimationInfo(ANIMATORINFO& animatorInfo);
	void updateAndBlend(ANIMATORINFO& animatorInfo);
	void update(ANIMATORINFO& animatorInfo);
	void updateSprite(ANIMATORINFO& animatorInfo);

public:
	AnimationSystem();
	~AnimationSystem();

	void Update();
};

