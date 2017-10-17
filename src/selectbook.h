//---------------------------------------------------------------------------

#ifndef selectbookH
#define selectbookH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <FMX.Controls.hpp>
#include <FMX.Forms.hpp>
#include <FMX.Controls.Presentation.hpp>
#include <FMX.Layouts.hpp>
#include <FMX.ListBox.hpp>
#include <FMX.StdCtrls.hpp>
#include <FMX.Types.hpp>

#include "main.h"
#include "bookcase.h"
//---------------------------------------------------------------------------
class TfmSelectBook : public TForm
{
__published:	// IDE-managed Components
	TListBox *lbBookcase;
	TLabel *Label1;
	void __fastcall ListBoxItem1Click(TObject *Sender);
private:	// User declarations
public:		// User declarations

	int SelectedBook;   // �ثe�襤���ѥN�X

	void __fastcall Initial(CBookcase * bookcase);  // ��l���d
	__fastcall TfmSelectBook(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfmSelectBook *fmSelectBook;
//---------------------------------------------------------------------------
#endif
