//---------------------------------------------------------------------------

#ifndef mainH
#define mainH
//---------------------------------------------------------------------------
#include "../src/navtree.h"


class __declspec(delphirtti) CCBReaderTest : public TObject
{
public:
	String MyFullPath;
	CNavTree * NavTree;

	virtual void __fastcall SetUp();
	virtual void __fastcall TearDown();

__published:
	// NavTree Ū���|�ظ��
	void __fastcall NavTreeReadTitle();
	void __fastcall NavTreeReadLink();
	void __fastcall NavTreeReadNavLink();
	void __fastcall NavTreeReadCBLink();

};

#endif
