// UGMaxProcessBar_H  : plugin class for 3DMAX.
////////////////////////////////////////////////////////////////////////////
//                    _   _   ____   ____                    
//   Project         | | | | / ___) / ___)                   
//                   | | | || | __ | |                       
//                   | |_| || |_\ || \___                    
//                    \____/ \____| \ ___)     6.0           
//                                                           
//!  \file headerfile.h
//!  \brief Class 进度条显示类。
//!  \details 封装WIN32API，用来提供显示Max插件显示进度的功能。
//!  \author sunye。
//!  \attention 
//!   Copyright (c) 1996-2010 SuperMap Software Co., Ltd. <br> 
//!   All Rights Reserved.
//!  \version 6.0
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
#ifndef UGMaxProcessBar_H
#define UGMaxProcessBar_H
#include <ShlObj.h>
#include "Stream/ugdefs.h"
class UGMaxProcessBar 
{

public:
	UGMaxProcessBar(){}

	UGMaxProcessBar(HINSTANCE hInstance,HWND hWndParent);

	~UGMaxProcessBar();
	//! \brief 设置进度条开始工作，此函数会创建一个线程，一个进度窗口
	void Start();
	//! \brief 如果进度条一次工作分阶段运行，每个阶段要分配一个权重，各个权重之和为1。
	void SetWeight(UGC::UGfloat fWeight);
	//! \brief 设置在某个阶段下需要进行更新总次数
	void SetNumInWeight(UGC::UGint nSteps);
	//! \brief 说明在某个阶段下的工作已经完成了一步
	void Update();
	//! \brief 更新相应的增量
	void Update(UGC::UGint nIncrement);
	//! \brief 工作已经完成，此函数会终止线程和窗口
	void End();
private:
	//! \线程函数
	static DWORD _stdcall UpdateBar(LPVOID lpParam);
	HINSTANCE m_hInstance;
	HWND m_hWndParent;
	HWND m_hWnd;
	HWND m_hpw;
	HANDLE m_hThread;
	UGC::UGfloat m_fWeight;
	UGC::UGint m_nSteps;
	UGC::UGfloat m_fAccum;
};

#endif
