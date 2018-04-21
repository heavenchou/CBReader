//---------------------------------------------------------------------------

#ifndef logoH
#define logoH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <FMX.Controls.hpp>
#include <FMX.Forms.hpp>
#include <FMX.Objects.hpp>
#include <FMX.Types.hpp>
//---------------------------------------------------------------------------
class TfmLogo : public TForm
{
__published:	// IDE-managed Components
	TImage *Image1;
private:	// User declarations
public:		// User declarations
	__fastcall TfmLogo(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfmLogo *fmLogo;
//---------------------------------------------------------------------------
#endif
