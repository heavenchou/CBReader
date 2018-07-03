//---------------------------------------------------------------------------

#ifndef createhtmlH
#define createhtmlH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <FMX.Controls.hpp>
#include <FMX.Forms.hpp>
#include <FMX.Controls.Presentation.hpp>
#include <FMX.Edit.hpp>
#include <FMX.StdCtrls.hpp>
#include <FMX.Types.hpp>
//---------------------------------------------------------------------------
class TfmCreateHtml : public TForm
{
__published:	// IDE-managed Components
	TEdit *edBook;
	TLabel *Label1;
	TEdit *edVolNum;
	TLabel *Label2;
	TButton *btStart;
	TButton *btStop;
	TEdit *edTempPath;
	TLabel *Label3;
	TLabel *lbFileName;
	TCheckBox *cbContinue;
	TEdit *edNoBook;
	TLabel *Label4;
	void __fastcall btStartClick(TObject *Sender);
	void __fastcall btStopClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
    bool Stop;
	void __fastcall CreateThisFile(String sFile);
	__fastcall TfmCreateHtml(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfmCreateHtml *fmCreateHtml;
//---------------------------------------------------------------------------
#endif
