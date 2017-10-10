#include "Timer.h"

Timer::Timer()
	: _startTime(0),
	_prevTime(0),
	_timeScale(1)
{
}

Timer::~Timer()
{
}

bool Timer::Init()
{
	LARGE_INTEGER frequency;
	QueryPerformanceFrequency(&frequency);

	_millisecondsFrequency = frequency.LowPart / 1000;

	return true;
}

void Timer::Release()
{
}

int Timer::calculateTime()
{
	LARGE_INTEGER temp;
	QueryPerformanceCounter(&temp);
	return (temp.LowPart / _millisecondsFrequency) * _timeScale; // ms시간 리턴
}


void Timer::Start()
{
	if (_startTime == 0)
	{
		_startTime = calculateTime();
		_curTime = _startTime;
		_prevTime = _startTime;
	}
}

void Timer::Update()
{
	_prevTime = _curTime;
	_curTime = calculateTime();
}
