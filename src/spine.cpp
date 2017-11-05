// ---------------------------------------------------------------------------
#pragma hdrstop
#include "spine.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)
// ---------------------------------------------------------------------------
// 建構式
__fastcall CSpine::CSpine()
{
	Files = new TStringList();         // 遍歷文件
	// CBETA 特有的
	Vol = 0;		// 冊
	Sutra = 0;	// 經
	Juan = 0;		// 卷
}
// ---------------------------------------------------------------------------
// 解構函式
__fastcall CSpine::~CSpine()
{
	if(Files) delete Files;
	if(Vol) delete Vol;
	if(Sutra) delete Sutra;
	if(Juan) delete Juan;
}
// ---------------------------------------------------------------------------
// 載入文件
void __fastcall CSpine::LoadSpineFile(String sFile)
{
	if(!TFile::Exists(sFile))
	{
		ShowMessage(u"遍歷文件不存在 : " + sFile);
		return;
	}

	Files->LoadFromFile(sFile);
}
// ---------------------------------------------------------------------------

