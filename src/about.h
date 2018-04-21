//---------------------------------------------------------------------------

#ifndef aboutH
#define aboutH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <FMX.Controls.hpp>
#include <FMX.Forms.hpp>
#include <FMX.Controls.Presentation.hpp>
#include <FMX.Objects.hpp>
#include <FMX.StdCtrls.hpp>
#include <FMX.Types.hpp>
//---------------------------------------------------------------------------
class TfmAbout : public TForm
{
__published:	// IDE-managed Components
	TPanel *Panel1;
	TImage *Image1;
	TPanel *Panel2;
	TLabel *Label1;
	TStyleBook *StyleBook1;
	TLabel *Label2;
	TLabel *Label3;
	TLabel *Label4;
	TLabel *Label5;
	TLabel *Label6;
	TLabel *Label7;
	TButton *btOK;
	void __fastcall btOKClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TfmAbout(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfmAbout *fmAbout;
//---------------------------------------------------------------------------
#endif
