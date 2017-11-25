#pragma once

#include "..\stdafx.h"
#include "Type.h"
#include "SingletonBase.h"

class Timer : public SingletonBase<Timer>
{
private:
	int _millisecondsFrequency;	//ms�� CPU Ŭ�� ��

	int	_startTime;					// �ð� ���� ���� �ð�
	int	_prevTime;
	int	_curTime;					// ���� ~ ����ð�
	int	_timeScale;					// �ð� �帧 �ӵ�?

private:
	int calculateTime();			// CPU Ŭ�����κ��� �ð� ���ϱ�

public:
	Timer();
	~Timer();

	bool Init() override;
	void Release() override;

	int GetDeltaTime() { return _curTime - _prevTime; }
	int GetElapsedTime() { return _curTime - _startTime; }	//��� �ð�	
	int SetTimeScale(int value) { _timeScale = value; }

	void Start();	// ���� ����
	void Update();	// ���� ~ ���� �ð� ����
};