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
	// �x�s���G���r��C��
	TStringList * Titles;   // ����
	TStringList * Results;  // ���G 0 : ���~, 1: ���\, ...
public:		// User declarations

	// �n���ժ�����
	CSample * Sample;

    // �b���յ��G�W�[���D, �H�K�Ϥ�
	void __fastcall ListBoxAddHead(String sHead);
	__fastcall TfmMain(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfmMain *fmMain;
//---------------------------------------------------------------------------
#endif
