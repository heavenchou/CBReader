//---------------------------------------------------------------------------

#include <fmx.h>
#pragma hdrstop

#include "buildindex.h"
#include "main.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.fmx"
TfmBuildIndex *fmBuildIndex;
//---------------------------------------------------------------------------
__fastcall TfmBuildIndex::TfmBuildIndex(TComponent* Owner)
	: TForm(Owner)
{

}
//---------------------------------------------------------------------------
void __fastcall TfmBuildIndex::btBuildClick(TObject *Sender)
{
	String sBuildListDir = edBuildListDir->Text;
	String sBuildList = edBuildList->Text;
	String sWordIndex = edWordIndex->Text;
	String sMainIndex = edMainIndex->Text;


	// 這個 map 可以傳入 "T", 傳回 "【大】"
	map<String, String> mpBookToVerName;

	CMonsterBuilder * MB = new CMonsterBuilder(sBuildListDir,
			sBuildList, sWordIndex, sMainIndex);

	MB->FileType = CMonsterBuilder::ftCBXML;    // 指定索引的格式
	// 索引總類

	if(rbIndexTypeCBETA->IsChecked)
		MB->IndexType = CMonsterBuilder::itCBETA;
	else if(rbIndexTypeOrig->IsChecked)
	{
		MB->IndexType = CMonsterBuilder::itOrig;
		// 原書索引需要 "T" -> "【大】" 這個 map
		mpBookToVerName.clear();
		fmMain->Bookcase->CBETA->BookData->BookToVerNameMap(mpBookToVerName);
		MB->mpBookToVerName = &mpBookToVerName;
	}

	if(cbNoMsg->IsChecked)
		MB->BuildIndex();
	else
		MB->BuildIndex(ProgressBar1, ProgressBar2, Memo1);

	String sMsg = u"總字數 : " + String(MB->TotalWordCount) + u"\n" +
					u"出現字數 : " + MB->ShowWordCount + u"\n" +
					u"總時間 : " + MB->cBuildAllTime + u" 毫秒\n" +
					u"處理各檔時間 : " + MB->cBuildOneTime + u" 毫秒\n" +
					u"建索引時間 : " + MB->cBuildMainTime + u" 毫秒\n";
	Memo1->Lines->Add(sMsg);

	mpBookToVerName.clear();

}
//---------------------------------------------------------------------------



