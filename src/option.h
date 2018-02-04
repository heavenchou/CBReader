//---------------------------------------------------------------------------

#ifndef optionH
#define optionH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <FMX.Controls.hpp>
#include <FMX.Forms.hpp>
#include <FMX.Controls.Presentation.hpp>
#include <FMX.Layouts.hpp>
#include <FMX.StdCtrls.hpp>
#include <FMX.TabControl.hpp>
#include <FMX.TreeView.hpp>
#include <FMX.Types.hpp>
#include "main.h"
#include "setting.h"
//---------------------------------------------------------------------------
class TfmOption : public TForm
{
__published:	// IDE-managed Components
	TTreeView *TreeView1;
	TTabControl *TabControl1;
	TTabItem *TabItem1;
	TTabItem *TabItem2;
	TTabItem *TabItem3;
	TSplitter *Splitter1;
	TTreeViewItem *TreeViewItem1;
	TTreeViewItem *TreeViewItem2;
	TTreeViewItem *TreeViewItem3;
	TPanel *Panel1;
	TGroupBox *gbSutraFormat;
	TCheckBox *cbShowLineFormat;
	TCheckBox *cbShowLineHead;
	TCheckBox *cbShowPunc;
	TCheckBox *cbNoShowLgPunc;
	TCheckBox *cbVerticalMode;
	TButton *btOK;
	TButton *btCancel;
	TGroupBox *gbCollationType;
	TRadioButton *rbNoCollation;
	TRadioButton *rbOrigCollation;
	TRadioButton *rbCBETACollation;
	TGroupBox *GroupBox1;
	TCheckBox *cbGaijiUseUniExt;
	TCheckBox *cbGaijiUseNormal;
	TGroupBox *GroupBox2;
	TRadioButton *rbGaijiUniExtFirst;
	TRadioButton *rbGaijiNormalFirst;
	TGroupBox *GroupBox3;
	TRadioButton *rbGaijiDesFirst;
	TRadioButton *rbGaijiImageFirst;
	void __fastcall FormShow(TObject *Sender);
	void __fastcall btOKClick(TObject *Sender);
	void __fastcall btCancelClick(TObject *Sender);
private:	// User declarations
public:		// User declarations

	CSetting * Setting; // �]�w��

	void __fastcall LoadFromSetting();  // �ѳ]�w���J
	void __fastcall SaveToSetting();  	// �ѳ]�w���J

	__fastcall TfmOption(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfmOption *fmOption;
//---------------------------------------------------------------------------
#endif
