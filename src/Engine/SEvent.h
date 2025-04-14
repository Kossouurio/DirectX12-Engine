#pragma once
#include <vector>

template <typename... Args>
class SEvent
{
public:
	struct _WRAPPERBASE
	{
		virtual ~_WRAPPERBASE() = default;
		virtual void Invoke(Args... args) = 0;
	};

	template <typename T>
	struct _WRAPPER : public _WRAPPERBASE
	{
		T* instance;
		void (T::*method)(Args...);
		_WRAPPER(T* inst, void (T::*meth)(Args...)) : instance(inst), method(meth) {}
		void Invoke(Args... args) override
		{
			(instance->*method)(std::forward<Args>(args)...);
		}
	};

public:
	SEvent();
	~SEvent();

	void Clear();
	int GetCount();

	void Call(Args... args);

	template <typename T>
	void Register(T* pReceiver, void (T::*func)(Args...));

	template <typename T>
	void Unregister(T* pReceiver, void (T::*func)(Args...));

private:
	std::vector<_WRAPPERBASE*> m_methods;
};

template <typename... Args>
SEvent<Args...>::SEvent()
{
}

template <typename... Args>
SEvent<Args...>::~SEvent()
{
	Clear();
}

template <typename... Args>
void SEvent<Args...>::Clear()
{
	for ( size_t i=0 ; i<m_methods.size() ; i++ )
		delete m_methods[i];
	m_methods.clear();
}

template <typename... Args>
int SEvent<Args...>::GetCount()
{
	return (int)m_methods.size();
}

template <typename... Args>
void SEvent<Args...>::Call(Args... args)
{
	for ( size_t i=0 ; i<m_methods.size() ; i++ )
		m_methods[i]->Invoke(std::forward<Args>(args)...);
}

template <typename... Args>
template <typename T>
void SEvent<Args...>::Register(T* pReceiver, void (T::*func)(Args...))
{
	m_methods.push_back(new _WRAPPER<T>(pReceiver, func));
}

template <typename... Args>
template <typename T>
void SEvent<Args...>::Unregister(T* pReceiver, void (T::*func)(Args...))
{
	for ( size_t i=0 ; i<m_methods.size() ; i++ )
	{
		_WRAPPER<T>* pWrapper = dynamic_cast<_WRAPPER<T>*>(m_methods[i]);
		if ( pWrapper && pWrapper->instance==pReceiver && pWrapper->method==func )
		{
			delete m_methods[i];
			m_methods.erase(m_methods.begin() + i);
			--i;
		}
	}
}

/*

class Control
{
public:
	void Show(int flag)
	{
		m_visible = true;
		onShow.Call(flag);
	}
	SEvent<int> onShow;
	SEvent<int, float> onHide;
private:
	bool m_visible = false;
};

class MyClass
{
public:
	void OnShow(int flag)
	{
    }
	void OnHide(int x, float time)
	{
    }
	void Register(Control* pCtrl)
	{
		pCtrl->onShow.Register(this, &MyClass::OnShow);
		pCtrl->onHide.Register(this, &MyClass::OnHide);
	}
};

Control ctrl;
MyClass my;
my.Register(&ctrl);
ctrl.Show(182);

*/