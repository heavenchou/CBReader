//---------------------------------------------------------------------------

#ifndef mainH
#define mainH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <FMX.Controls.hpp>
#include <FMX.Forms.hpp>

#include "sample.h"
#include <FMX.Controls.Presentation.hpp>
#include <FMX.ListView.Adapters.Base.hpp>
#include <FMX.ListView.Appearances.hpp>
#include <FMX.ListView.hpp>
#include <FMX.ListView.Types.hpp>
#include <FMX.StdCtrls.hpp>
#include <FMX.Types.hpp>
#include <FMX.Layouts.hpp>
#include <FMX.ListBox.hpp>
//---------------------------------------------------------------------------
class TfmMain : public TForm
{
__published:	// IDE-managed Components
	TCornerButton *btRunAllTest;
	TListBox *lbResult;
	void __fastcall btRunAllTestClick(TObject *Sender);
private:	// User declarations
	// 儲存結果的字串列表
	TStringList * Titles;   // 說明
	TStringList * Results;  // 結果 0 : 錯誤, 1: 成功, ...
public:		// User declarations

	// 要測試的物件
	CSample * Sample;

    // 在測試結果上加標題, 以便區分
	void __fastcall ListBoxAddHead(String sHead);
	__fastcall TfmMain(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfmMain *fmMain;
//---------------------------------------------------------------------------
#endif
