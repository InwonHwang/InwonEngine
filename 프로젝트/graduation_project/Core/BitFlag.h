#pragma once

// enum ���� �÷��� ���� �������� �ʰ� ����ϵ��� �ϱ� ����
#define FLAG(x) (1<<(x)) 

template <typename T>
class BitFlag
{
private:
	T _value;

public:
	BitFlag() : _value(0) {}
	~BitFlag() {}

	bool Test(T place) const;	// �ش� �ڸ����� ��Ʈ�� 1���� Ȯ��
	void Set(T place);			// �ش� �ڸ����� ��Ʈ�� 1�� ����
	void Reset(T place);		// �ش� �ڸ����� ��Ʈ�� 0���� ����
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
