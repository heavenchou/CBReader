//---------------------------------------------------------------------------

#ifndef buildindexH
#define buildindexH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <FMX.Controls.hpp>
#include <FMX.Forms.hpp>
#include <FMX.Controls.Presentation.hpp>
#include <FMX.Edit.hpp>
#include <FMX.Types.hpp>
#include <FMX.StdCtrls.hpp>
#include <FMX.Memo.hpp>
#include <FMX.ScrollBox.hpp>


#include "../../Monster/src/builder.h"
//---------------------------------------------------------------------------
class TfmBuildIndex : public TForm
{
__published:	// IDE-managed Components
	TEdit *edBuildListDir;
	TEdit *edBuildList;
	TEdit *edWordIndex;
	TEdit *edMainIndex;
	TButton *btBuild;
	TLabel *Label1;
	TLabel *Label2;
	TLabel *Label3;
	TLabel *Label4;
	TStyleBook *StyleBook1;
	TMemo *Memo1;
	TProgressBar *ProgressBar1;
	TProgressBar *ProgressBar2;
	TLabel *Label5;
	TLabel *Label6;
	TLabel *Label7;
	TCheckBox *cbNoMsg;
	TGroupBox *GroupBox1;
	TRadioButton *rbIndexTypeOrig;
	TRadioButton *rbIndexTypeCBETA;
	void __fastcall btBuildClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TfmBuildIndex(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfmBuildIndex *fmBuildIndex;
//---------------------------------------------------------------------------
#endif
