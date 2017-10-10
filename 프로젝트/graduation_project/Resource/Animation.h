#pragma once

#include "..\Core\Core.h"

class Sprite;

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
	int length;
	int start;
	int end;

	string name;
}ANIMATIONKEYFRAME, *LPANIMATIONKEYFRAME;

class AnimationCurve
{
private:
	ANIMATIONKEYFRAME _keyFrame;

private:
	void setKey(vector<VECTORKEY>& keySetVec, int frame, const Vector3& value);
	void setKey(vector<QUATERNIONKEY>& keySetVec, int frame, const Quaternion& value);
	bool getKey(const vector<VECTORKEY>& keySetVec, int frame, Vector3& value) const;
	bool getKey(const vector<QUATERNIONKEY>& keySetVec, int frame, Quaternion& value) const;

public:
	AnimationCurve();
	~AnimationCurve();

	void SetScaleKey(int frame, const Vector3& scale);
	void SetRotationKey(int frame, const  Quaternion& rotation);
	void SetPositionKey(int frame, const  Vector3& position);
	void SetSpriteKey(int frame, const shared_ptr<Sprite> sprite);

	bool GetScaleKey(int frame, Vector3& scale) const;
	bool GetRotationKey(int frame, Quaternion& rotation) const;
	bool GetPositionKey(int frame, Vector3& position) const;
	shared_ptr<Sprite> GetSpriteKey(int frame);

	void GetAnimatedMatrix(int frame, D3DXMATRIX& matrix) const;
	int GetLength() const;

	void SetFrame(); // frame start, end, length 구하기
};


class Animation : public ResourceBase
{
	typedef vector<shared_ptr<AnimationCurve>> AnimCurveVec;
private:
	shared_ptr<AnimCurveVec> _curves;
	bool _loop;

public:
	Animation() {}
	~Animation() {}
	
	virtual bool Create(void* pData, void* pResourceData, const string& name) override;
	virtual void Destroy() override;

	void SetLoop(bool flag) { _loop = flag; }
	bool GetLoop() const { return _loop; }

	int GetAnimCurveCount() const { return _curves->size(); }
	void GetAnimatedMatrix(int index, int frame, D3DXMATRIX& m) const;

	shared_ptr<Sprite> GetSprite(int frame) const;
	int GetLength(int index) const;
};
