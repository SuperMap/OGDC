 #include "UGMenuDec.h"
 #include "UGExportBIMDec.h"
 #include "../Builds/Win_Solution_vc11//OGDC/OGDC.h"
HINSTANCE hInstance;
//下面是插件所需必须几个函数，也是插件与3dsMax进行交互的最开始的接口，特别是LibClassDesc，LibNumberClasses返回和LibClassDesc的类型个数一致

BOOL WINAPI DllMain(HINSTANCE hinstDLL,ULONG fdwReason,LPVOID lpvReserved) {
	//按钮配置文件要删掉，为了第二次启动按钮有效的权宜之计
	OGDC::OgdcString menuFilePath(GetCOREInterface()->GetMenuManager()->GetMenuFile());
	UGC::UGFile::Delete(menuFilePath);

	switch(fdwReason) {
		case DLL_PROCESS_ATTACH:
			hInstance = hinstDLL;
			DisableThreadLibraryCalls(hInstance);
			break;
	}
	return(TRUE);
}
//!\返回dll文件的描述
__declspec( dllexport ) const TCHAR *LibDescription() { 
	return L"SuperMapPlugin"; 
}

//!\返回插件类的描述类的个数
__declspec(dllexport) int LibNumberClasses() { 
	return 2; 
}

__declspec(dllexport) ClassDesc* LibClassDesc(int i){
	switch (i){
	case 0:return new UGMenuDec(hInstance);
	case 1:return new UGExportBIMDec;
		default: return NULL;
	}
}

__declspec( dllexport ) ULONG LibVersion() { 
	return VERSION_3DSMAX;
}