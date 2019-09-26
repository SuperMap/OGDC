// UGReferenced.h: interface for the UGReferenced class.
//
//////////////////////////////////////////////////////////////////////

#ifndef UGREFERENCED_H
#define UGREFERENCED_H
#include "Stream/ugdefs.h"
namespace UGC
{
	  //!  \brief Toolkit
	class UGDeleteHandler;
	//! \brief 与智能指针UGRefPtr配合使用
	class TOOLKIT_API UGReferenced  
	{
	public:
	//! \brief
	  //!  \brief Toolkit
		UGReferenced()
		{
	  //!  \brief Toolkit
			m_nRefCount = 0;
		}
	//! \brief
		UGReferenced(const UGReferenced&)
		{
	  //!  \brief Toolkit
			m_nRefCount = 0;
		}
	//! \brief
		void Ref()const;
	//! \brief
		void Unref() const;
	//! \brief
		void UnrefNoDelete()const;
	//! \brief
		inline UGint GetReferenceCount() const{return m_nRefCount;}
	public:
	//! \brief
	  //!  \brief Toolkit
		friend class UGDeleteHandler;
	//! \brief
	  //!  \brief Toolkit
		static UGDeleteHandler* GetDeleteHandler();
	//! \brief
	  //!  \brief Toolkit
		static void SetDeleteHandler(UGDeleteHandler* handler);
		

	protected:
		virtual ~UGReferenced();
		
		mutable UGint m_nRefCount;
	
	};

	class UGDeleteHandler
	{
	public:
	  //!  \brief Toolkit
		virtual ~UGDeleteHandler(){}
	  //!  \brief Toolkit
		inline void DoDelete(const UGReferenced* pObject){delete pObject;};
	  //!  \brief Toolkit
		virtual void RequestDelete(const UGReferenced* pObject){DoDelete(pObject);}
	};
}


#endif 

