// ---------------------------------------------------------------------------

#include <fmx.h>
#ifdef _WIN32
#include <tchar.h>
#endif
#pragma hdrstop
#include <System.StartUpCopy.hpp>
// ---------------------------------------------------------------------------
USEFORM("option.cpp", fmOption);
USEFORM("selectbook.cpp", fmSelectBook);
USEFORM("main.cpp", fmMain);
USEFORM("searchrange.cpp", fmSearchRange);
//---------------------------------------------------------------------------
extern "C" int FMXmain()
{
	try
	{
		Application->Initialize();
		Application->CreateForm(__classid(TfmMain), &fmMain);
		Application->CreateForm(__classid(TfmSelectBook), &fmSelectBook);
		Application->CreateForm(__classid(TfmOption), &fmOption);
		Application->CreateForm(__classid(TfmSearchRange), &fmSearchRange);
		Application->Run();
	}
	catch (Exception &exception)
	{
		Application->ShowException(&exception);
	}
	catch (...)
	{
		try
		{
			throw Exception("");
		}
		catch (Exception &exception)
		{
			Application->ShowException(&exception);
		}
	}
	return 0;
}
// ---------------------------------------------------------------------------
