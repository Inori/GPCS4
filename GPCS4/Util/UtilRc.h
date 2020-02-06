#pragma once

#include <atomic>
#include "GPCS4Types.h"


class RcObject 
{

public:
	RcObject():
	m_nRefCount(0)
	{
	}

	virtual ~RcObject() 
	{
	}

	uint32_t IncRef() 
	{
		return ++m_nRefCount;
	}

	uint32_t DecRef()
	{
		return --m_nRefCount;
	}

private:

	std::atomic<uint32_t> m_nRefCount;
};



template<typename T>
class RcPtr {
	template<typename Tx>
	friend class RcPtr;
public:

	RcPtr() { }
	RcPtr(std::nullptr_t) { }

	RcPtr(T* object)
		: m_pObject(object) 
	{
		this->IncRef();
	}

	RcPtr(const RcPtr& other)
		: m_pObject(other.m_pObject) 
	{
		this->IncRef();
	}

	template<typename Tx>
	RcPtr(const RcPtr<Tx>& other)
		: m_pObject(other.m_pObject) 
	{
		this->IncRef();
	}

	RcPtr(RcPtr&& other)
		: m_pObject(other.m_pObject) 
	{
		other.m_pObject = nullptr;
	}

	template<typename Tx>
	RcPtr(RcPtr<Tx>&& other)
		: m_pObject(other.m_pObject)
	{
		other.m_pObject = nullptr;
	}

	RcPtr& operator = (std::nullptr_t) 
	{
		this->DecRef();
		m_pObject = nullptr;
		return *this;
	}

	RcPtr& operator = (const RcPtr& other) 
	{
		other.IncRef();
		this->DecRef();
		m_pObject = other.m_pObject;
		return *this;
	}

	template<typename Tx>
	RcPtr& operator = (const RcPtr<Tx>& other) 
	{
		other.IncRef();
		this->DecRef();
		m_pObject = other.m_pObject;
		return *this;
	}

	RcPtr& operator = (RcPtr&& other) 
	{
		this->DecRef();
		this->m_pObject = other.m_pObject;
		other.m_pObject = nullptr;
		return *this;
	}

	template<typename Tx>
	RcPtr& operator = (RcPtr<Tx>&& other) 
	{
		this->DecRef();
		this->m_pObject = other.m_pObject;
		other.m_pObject = nullptr;
		return *this;
	}

	~RcPtr() 
	{
		this->DecRef();
	}

	T& operator *  () const { return *m_pObject; }
	T* operator -> () const { return  m_pObject; }
	T* ptr() const { return m_pObject; }

	bool operator == (const RcPtr& other) const { return m_pObject == other.m_pObject; }
	bool operator != (const RcPtr& other) const { return m_pObject != other.m_pObject; }

	bool operator == (std::nullptr_t) const { return m_pObject == nullptr; }
	bool operator != (std::nullptr_t) const { return m_pObject != nullptr; }

	operator bool() const { return m_pObject != nullptr; }

private:

	T* m_pObject = nullptr;

	void IncRef() const 
	{
		if (m_pObject != nullptr)
		{
			m_pObject->IncRef();
		}
	}

	void DecRef() const 
	{
		if (m_pObject != nullptr)
		{
			if (m_pObject->DecRef() == 0)
			{
				delete m_pObject;
			}
		}
	}

};
