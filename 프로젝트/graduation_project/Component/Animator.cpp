#include "Animator.h"
#include "Transform.h"
#include "SpriteRenderer.h"

void Animator::Update(void* pData)
{
	if (!_curAnim) return;

	UpdateInfo();

	if ((_curAnim && !_curAnim->GetLoop() && _cycle > 0))
	{
		UpdateFinalAnimatedMatrix();
	}
	else if (_nextAnim)
	{
		UpdateBlendedMatrix();
	}
	else
	{
		UpdateAnimatedMatrix();
	}

	UpdateSprite();
}

void Animator::Create(void* pData, const shared_ptr<void> gameObject)
{
	_gameObject = static_pointer_cast<GameObject>(gameObject);
	_bones.reset(new TransformList);
	_animationHandler.reset(new ANIMATIONCALLBACKHANDLER);
	_animations.reset(new AnimationVec);
}

void Animator::Destroy()
{
	//ComponentPool<Animator>* pMeshRendererPool = static_cast<ComponentPool<Animator>*>(_pComponentPool);
	//pMeshRendererPool->Destroy(shared_from_this());
	
	_gameObject.reset();
	_bones.reset();
	_animationHandler.reset();
	_animations.reset();
}

void Animator::SetRootBone(const shared_ptr<Transform> pRootBone)
{
	/*stack<shared_ptr<Transform>> s;

	s.push(pRootBone);

	while (!s.empty())
	{
		shared_ptr<Transform> temp = s.top();
		s.pop();

		if (temp->IsBone())
			_bones->push_back(temp);

		for (int i = 0; i < temp->GetChildCount(); ++i)
		{
			s.push(temp->GetChild(i));
		}
	}*/
}

void Animator::SetAnimationCount(int count)
{
	_animations->resize(count);
}

void Animator::SetAnimation(int index, const shared_ptr<Animation> pAnim)
{
	assert(index < _animations->size());

	_animations->data()[index] = pAnim;
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
	_point = AppTimer->GetElapsedTime();
	_fadeLength = fadeLength;
}

void Animator::UpdateInfo()
{
	if (!_curAnim) return;

	_cycle = (AppTimer->GetElapsedTime() - _animationHandler->_startOfCurAnim) / _curAnim->GetLength(0);

	if (_nextAnim)
	{
		float t = float(AppTimer->GetElapsedTime() - _point) / float(_curAnim->GetLength(0));
		if (t > _fadeLength)
		{
			_animationHandler->_startOfCurAnim = _animationHandler->_startOfNextAnim;
			_animationHandler->_startOfNextAnim = 0;
			_curAnim = _nextAnim;
			_nextAnim = nullptr;
		}
	}
}

void Animator::UpdateBlendedMatrix()
{
	int animCurveCount = _curAnim->GetAnimCurveCount();

	if (animCurveCount != _bones->size() ||
		_nextAnim->GetAnimCurveCount() != _bones->size())
		return;

	int frame0 = (AppTimer->GetElapsedTime() - _animationHandler->_startOfCurAnim) % _curAnim->GetLength(0);
	int frame1 = (AppTimer->GetElapsedTime() - _animationHandler->_startOfNextAnim) % _nextAnim->GetLength(0);

	float point = float(AppTimer->GetElapsedTime() - _point) / float(_curAnim->GetLength(0));
	float weight0 = 2.0f * point / _fadeLength;
	float weight1 = 2.0f - weight0;
	D3DXMATRIX m, m0, m1;

	for (int i = 0; i < animCurveCount; ++i)
	{
		_curAnim->GetAnimatedMatrix(i, frame0, m0);
		_nextAnim->GetAnimatedMatrix(i, frame1, m1);

		m = (m0 * weight1 + m1 * weight0) / 2;

		_bones->data()[i].lock()->SetLocalMatrix(m);
	}
}

void Animator::UpdateAnimatedMatrix()
{
	int animCurveCount = _curAnim->GetAnimCurveCount();

	if (animCurveCount != _bones->size())
		return;

	int frame = (AppTimer->GetElapsedTime() - _animationHandler->_startOfCurAnim) % _curAnim->GetLength(0);

	D3DXMATRIX m;
	for (int i = 0; i < animCurveCount; ++i)
	{
		_curAnim->GetAnimatedMatrix(i, frame, m);

		_bones->data()[i].lock()->SetLocalMatrix(m);
	}
}

void Animator::UpdateFinalAnimatedMatrix()
{
	int animCurveCount = _curAnim->GetAnimCurveCount();

	if (animCurveCount != _bones->size())
		return;

	int frame = _curAnim->GetLength(0) - 1;

	D3DXMATRIX m;
	for (int i = 0; i < animCurveCount; ++i)
	{
		_curAnim->GetAnimatedMatrix(i, frame, m);

		_bones->data()[i].lock()->SetLocalMatrix(m);
	}
}

void Animator::UpdateSprite()
{
	shared_ptr<SpriteRenderer> pRenderer = _gameObject->GetComponent<SpriteRenderer>();

	if (!pRenderer) return;

	int frame = (AppTimer->GetElapsedTime() - _animationHandler->_startOfCurAnim) % _curAnim->GetLength(0);

	pRenderer->SetSprite(_curAnim->GetSprite(frame));
}