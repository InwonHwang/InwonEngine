#pragma once

#include "..\stdafx.h"
#include "Type.h"
#include "SingletonBase.h"

class Timer : public SingletonBase<Timer>
{
private:
	int _millisecondsFrequency;	//ms당 CPU 클럭 수

	int	_startTime;					// 시간 측정 시작 시간
	int	_prevTime;
	int	_curTime;					// 시작 ~ 현재시간
	int	_timeScale;					// 시간 흐름 속도?

private:
	int calculateTime();			// CPU 클럭수로부터 시간 구하기

public:
	Timer();
	~Timer();

	bool Init() override;
	void Release() override;

	int GetDeltaTime() { return _curTime - _prevTime; }
	int GetElapsedTime() { return _curTime - _startTime; }	//경과 시간	
	int SetTimeScale(int value) { _timeScale = value; }

	void Start();	// 측정 시작
	void Update();	// 시작 ~ 현재 시간 변경
};