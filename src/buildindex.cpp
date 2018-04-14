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

	MB->FileType = CMonsterBuilder::ftCBXML;    // ���w���ު��榡

	if(cbNoMsg->IsChecked)
		MB->BuildIndex();
	else
		MB->BuildIndex(ProgressBar1, ProgressBar2, Memo1);

	String sMsg = u"�`�r�� : " + String(MB->TotalWordCount) + u"\n" +
					u"�X�{�r�� : " + MB->ShowWordCount + u"\n" +
					u"�`�ɶ� : " + MB->cBuildAllTime + u" �@��\n" +
					u"�B�z�U�ɮɶ� : " + MB->cBuildOneTime + u" �@��\n" +
					u"�د��ޮɶ� : " + MB->cBuildMainTime + u" �@��\n";
	Memo1->Lines->Add(sMsg);

}
//---------------------------------------------------------------------------



