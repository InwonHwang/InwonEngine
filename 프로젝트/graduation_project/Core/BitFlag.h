#pragma once

// enum 에서 플래그 값을 정의하지 않고 계산하도록 하기 위해
#define FLAG(x) (1<<(x)) 

template <typename T>
class BitFlag
{
private:
	T _value;

public:
	BitFlag() : _value(0) {}
	~BitFlag() {}

	bool Test(T place) const;	// 해당 자리수의 비트가 1인지 확인
	void Set(T place);			// 해당 자리수의 비트를 1로 변경
	void Reset(T place);		// 해당 자리수의 비트를 0으로 변경
	void Clear();
};

typedef BitFlag<unsigned char>		flags8;		// 8 bit flag
typedef BitFlag<unsigned short>		flags16;	// 16 bit flag
typedef BitFlag<unsigned long>		flag32;		// 32 bit flag

template<typename T>
inline bool BitFlag<T>::Test(T place) const
{
	return (_value & FLAG(place)) != 0;
}

template<typename T>
void BitFlag<T>::Set(T place)
{
	_value |= FLAG(place);
}

template<typename T>
inline void BitFlag<T>::Reset(T place)
{
	_value &= ~FLAG(place);
}

template<typename T>
inline void BitFlag<T>::Clear()
{
	_value = 0;
}
