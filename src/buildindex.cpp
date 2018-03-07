//---------------------------------------------------------------------------

#include <fmx.h>
#pragma hdrstop

#include "buildindex.h"
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

	CMonsterBuilder * MB = new CMonsterBuilder(sBuildListDir,
			sBuildList, sWordIndex, sMainIndex);

	MB->FileType = CMonsterBuilder::ftCBXML;    // 指定索引的格式

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

}
//---------------------------------------------------------------------------



