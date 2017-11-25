#pragma once

#include "..\Core\Core.h"

class Sprite;

class AnimationCurve
{
	typedef struct tagVectorKey	// 애니메이션 키
	{
		int frame;
		Vector3 value;
	}VECTORKEY, *LPVECTORKEY;

	typedef struct tagQuaternionKey	// 애니메이션 키
	{
		int frame;
		Quaternion value;
	}QUATERNIONKEY, *LPQUATERNIONKEY;

	typedef struct tagSpriteKey	// 애니메이션 키
	{
		int frame;
		shared_ptr<Sprite> value;
	}SPRITEKEY, *LPSPRITEKEY;

	typedef struct tagAnimKeyFrame
	{
		tagAnimKeyFrame() {}
		~tagAnimKeyFrame()
		{
			vector<VECTORKEY> emptyData;
			scale.swap(std::vector<VECTORKEY>());
			rotation.swap(std::vector<QUATERNIONKEY>());
			position.swap(std::vector<VECTORKEY>());
		}

		vector<VECTORKEY> scale;
		vector<QUATERNIONKEY> rotation;
		vector<VECTORKEY> position;
		vector<SPRITEKEY> sprite;
	}ANIMATIONKEYFRAME, *LPANIMATIONKEYFRAME;
private:
	shared_ptr<ANIMATIONKEYFRAME> _keyFrame;
	shared_ptr<string> _boneName;
	int _start;
	int _end;
	int _length;

private:
	void setKey(vector<VECTORKEY>& keySetVec, int frame, const Vector3& value);
	void setKey(vector<QUATERNIONKEY>& keySetVec, int frame, const Quaternion& value);
	bool getKey(const vector<VECTORKEY>& keySetVec, int frame, Vector3& value) const;
	bool getKey(const vector<QUATERNIONKEY>& keySetVec, int frame, Quaternion& value) const;
	void setFrame(int frame);

public:
	AnimationCurve();
	~AnimationCurve();

	void SetScale(int frame, const Vector3& scale) { setKey(_keyFrame->scale, frame, scale); }
	void SetRotation(int frame, const  Quaternion& rotation) { setKey(_keyFrame->rotation, frame, rotation); }
	void SetPosition(int frame, const  Vector3& position) { setKey(_keyFrame->position, frame, position); }
	void SetBoneName(const string& name) { _boneName = make_shared<string>(name); }

	bool GetScale(int frame, Vector3& scale) const { return getKey(_keyFrame->scale, frame, scale); }
	bool GetRotation(int frame, Quaternion& rotation) const { return getKey(_keyFrame->rotation, frame, rotation); }
	bool GetPosition(int frame, Vector3& position) const { return getKey(_keyFrame->position, frame, position); }
	
	shared_ptr<string> GetBoneName() const { return _boneName; }
	int GetLength() const { return _length; }

	void SetSprite(int frame, const shared_ptr<Sprite> sprite);
	shared_ptr<Sprite> GetSprite(int frame) const;

	
};


class Animation : public ResourceBase
{
	typedef vector<shared_ptr<AnimationCurve>> AnimCurveVec;

private:
	unique_ptr<AnimCurveVec> _curves;
	bool _loop;

public:
	Animation();
	~Animation();
	
	virtual bool Create(void* pData, void* pResourceData, const string& name) override;
	virtual void Destroy() override;

	void SetLoop(bool flag) { _loop = flag; }
	bool GetLoop() const { return _loop; }

	void AddAnimationCurve(shared_ptr<AnimationCurve> animationCurve) { _curves->push_back(animationCurve); }
	const shared_ptr<AnimationCurve> GetAnimationCurve(int index) const { return _curves->data()[index]; }

	int GetAnimationCurveCount() const { return _curves->size(); }
};
