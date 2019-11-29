#pragma once
#ifndef JKISINGLETON_H_
#define JKISINGLETON_H_

template<class T>
class jkSingleton
{
public:

	static T* GetInstancePtr() {
		if (!m_pInstance)
			m_pInstance = new T();
		return m_pInstance;
	};
	static T& GetInstance();

protected:

	jkSingleton() {};
	jkSingleton(const jkSingleton&) {};

	static T* m_pInstance;
};

#endif // !JKSINGLETON_H_