//---------------------------------------------------------------------------

#ifndef mainH
#define mainH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <FMX.Controls.hpp>
#include <FMX.Forms.hpp>

#include <FMX.Controls.Presentation.hpp>
#include <FMX.ListView.Adapters.Base.hpp>
#include <FMX.ListView.Appearances.hpp>
#include <FMX.ListView.hpp>
#include <FMX.ListView.Types.hpp>
#include <FMX.StdCtrls.hpp>
#include <FMX.Types.hpp>
#include <FMX.Layouts.hpp>
#include <FMX.ListBox.hpp>
#include <FMX.TreeView.hpp>
#include "sample.h"
#include "navtreetest.h"
#include "cbxmltest.h"
#include "bookcasetest.h"
//---------------------------------------------------------------------------
class TfmMain : public TForm
{
__published:	// IDE-managed Components
	TCornerButton *btShowTestResult;
	TListBox *lbResult;
	TTreeView *TreeView;
	void __fastcall btShowTestResultClick(TObject *Sender);
private:	// User declarations
	// �x�s���G���r��C��
	TStringList * Titles;   // ����
	TStringList * Results;  // ���G 0 : ���~, 1: ���\, ...

	String TestString;  // ���եΦr��
public:		// User declarations

	// �n���ժ�����
	CSample * Sample;
	CNavTreeTest * NavTreeTest;
	CCBXMLTest * CBXMLTest;
    CBookcaseTest * BookcaseTest;

    // �b���յ��G�W�[���D, �H�K�Ϥ�
	void __fastcall ListBoxAddHead(String sHead);


	// Nav �𪬥ؿ� Item Click ���ե\��
	void __fastcall NavTreeItemClick(TObject *Sender);
	// Nav �𪬥ؿ�����
	void __fastcall TreeViewRunAllTest(void);
	// �x�s���յ��G
	void __fastcall LogTest(String sTitle, bool bResult);
	__fastcall TfmMain(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfmMain *fmMain;
//---------------------------------------------------------------------------
#endif
