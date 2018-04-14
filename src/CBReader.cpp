// ---------------------------------------------------------------------------

#include <fmx.h>
#ifdef _WIN32
#include <tchar.h>
#endif
#pragma hdrstop
#include <System.StartUpCopy.hpp>
// ---------------------------------------------------------------------------
USEFORM("buildindex.cpp", fmBuildIndex);
USEFORM("option.cpp", fmOption);
USEFORM("searchrange.cpp", fmSearchRange);
USEFORM("selectbook.cpp", fmSelectBook);
USEFORM("main.cpp", fmMain);
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
		Application->CreateForm(__classid(TfmBuildIndex), &fmBuildIndex);
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
