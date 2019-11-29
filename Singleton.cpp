#include"Singleton.h"

//template<class T>
//T* jkSingleton<T>::m_pInstance = nullptr;

//template<class T>
//static T* jkSingleton<T>::GetInstancePtr()


template<class T>
static T& jkSingleton<T>::GetInstance()
{
	if (!m_pInstance)
		jkSingleton<T>::m_pInstance = new T();
	return *m_pInstance;
}
