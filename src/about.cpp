//---------------------------------------------------------------------------

#include <fmx.h>
#pragma hdrstop

#include "about.h"
#include "logo.h"
#include "main.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.fmx"
TfmAbout *fmAbout;
//---------------------------------------------------------------------------
__fastcall TfmAbout::TfmAbout(TComponent* Owner)
	: TForm(Owner)
{

}
//---------------------------------------------------------------------------
void __fastcall TfmAbout::btOKClick(TObject *Sender)
{
    Close();
}
//---------------------------------------------------------------------------
