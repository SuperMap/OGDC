// UGRefPtr.h: interface for the UGRefPtr class.
//
//////////////////////////////////////////////////////////////////////

#ifndef UGREFPTR_H
#define UGREFPTR_H
#include "Stream/ugdefs.h"
namespace UGC
{
	//! \brief 一个智能指针与UGRerenced配合使用
	template <class T>
	class UGEXPORT UGRefPtr  
	{
	public:
	//! \brief
	  //!  \brief Toolkit
		UGRefPtr():m_ptr(0L){}
	//! \brief
	  //!  \brief Toolkit
		UGRefPtr(T* ptr):m_ptr(ptr){if (m_ptr) m_ptr->Ref();}
	//! \brief
	  //!  \brief Toolkit
		UGRefPtr(const UGRefPtr& rp):m_ptr(rp.m_ptr){if (m_ptr) m_ptr->Ref();}
	//! \brief
	  //!  \brief Toolkit
		~UGRefPtr(){if(m_ptr)m_ptr->Unref(); m_ptr = 0L;}
	//! \brief
	  //!  \brief Toolkit
		inline UGRefPtr& operator = (const UGRefPtr& rp )
		{
	  //!  \brief Toolkit
            if (m_ptr==rp.m_ptr) return *this;
	  //!  \brief Toolkit
            T* tmp_ptr = m_ptr;
	  //!  \brief Toolkit
            m_ptr = rp.m_ptr;
	  //!  \brief Toolkit
            if (m_ptr) m_ptr->Ref();
	  //!  \brief Toolkit
            if (tmp_ptr) tmp_ptr->Unref();
	  //!  \brief Toolkit
            return *this;
	  //!  \brief Toolkit
        }
	//! \brief
		inline UGRefPtr& operator = (T* ptr)
		{
	  //!  \brief Toolkit
			if (m_ptr==ptr) return *this;
	  //!  \brief Toolkit
			T* tmp_ptr = m_ptr;
	  //!  \brief Toolkit
			m_ptr = ptr;
	  //!  \brief Toolkit
			if (m_ptr) m_ptr->Ref();
	  //!  \brief Toolkit
			if (tmp_ptr) tmp_ptr->Unref();
	  //!  \brief Toolkit
			return *this;	
		}
	//! \brief
	  //!  \brief Toolkit
		inline UGbool operator == (const UGRefPtr& rp) const { return (m_ptr==rp.m_ptr); }
	//! \brief
	  //!  \brief Toolkit
        inline UGbool operator != (const UGRefPtr& rp) const { return (m_ptr!=rp.m_ptr); }
	//! \brief
	  //!  \brief Toolkit
        inline UGbool operator < (const UGRefPtr& rp) const { return (m_ptr<rp.m_ptr); }
	//! \brief
	  //!  \brief Toolkit
        inline UGbool operator > (const UGRefPtr& rp) const { return (m_ptr>rp.m_ptr); }
	//! \brief
	  //!  \brief Toolkit

	  //!  \brief Toolkit
		inline UGbool operator == (const T* ptr) const { return (m_ptr==ptr); }
	//! \brief
	  //!  \brief Toolkit
        inline UGbool operator != (const T* ptr) const { return (m_ptr!=ptr); }
	//! \brief
	  //!  \brief Toolkit
        inline UGbool operator < (const T* ptr) const { return (m_ptr<ptr); }
	//! \brief
	  //!  \brief Toolkit
        inline UGbool operator > (const T* ptr) const { return (m_ptr>ptr); }
	//! \brief
	  //!  \brief Toolkit

	  //!  \brief Toolkit
		inline T& operator*()  { return *m_ptr; }
	//! \brief
		
	  //!  \brief Toolkit
        inline const T& operator*() const { return *m_ptr; }
	//! \brief
		
	  //!  \brief Toolkit
        inline T* operator->() { return m_ptr; }
	//! \brief
		
	  //!  \brief Toolkit
        inline const T* operator->() const   { return m_ptr; }
	//! \brief
		
	//! \brief
	  //!  \brief Toolkit
		inline UGbool operator!() const	{ return m_ptr==0L; }
		
	//! \brief
	  //!  \brief Toolkit
		inline UGbool IsValid() const	{ return m_ptr!=0L; }
        
	//! \brief
	  //!  \brief Toolkit
        inline T* Get() { return m_ptr; }
		
	//! \brief
	  //!  \brief Toolkit
        inline const T* Get() const { return m_ptr; }

		//慎用

	//! \brief
	  //!  \brief Toolkit
		inline T* Take() { return Release();}
		
	//! \brief
	  //!  \brief Toolkit
        inline T* Release() { T* tmp=m_ptr; if (m_ptr) m_ptr->UnrefNoDelete(); m_ptr=0; return tmp;}

	private:
		T* m_ptr;
	};

}

#endif 

