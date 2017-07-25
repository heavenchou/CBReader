#include <DUnitX.TestFramework.hpp>
#include <stdio.h>

#include "main.h"

#pragma option --xrtti

// ----------------------------------------------
void __fastcall CCBReaderTest::SetUp()
{
#ifdef _Windows
	MyFullPath = GetCurrentDir();
#else
	MyFullPath = "/Users/heavenchou/PAServer/scratch-dir/Heaven-macos1012";
#endif

	MyFullPath += "/";

	NavTree = new CNavTree(MyFullPath + "nav.xhtml");
}

void __fastcall CCBReaderTest::TearDown()
{
}

// ----------------------------------------------
// NavTree Ū���|�ظ��
void __fastcall CCBReaderTest::NavTreeReadTitle()
{
	SetUp();

	TList * TreeRoot = NavTree->GetTreeRoot();
	String s = "";

	// �ˬd���G
	SNavItem * it = (SNavItem * ) TreeRoot->Items[0];
	s = String(it->Level) + "," + it->Type + "," + it->Title + "," + it->URL;
	Dunitx::Testframework::Assert::IsTrue(s == "0,0,�ؿ�,");
}

void __fastcall CCBReaderTest::NavTreeReadLink()
{
	TList * TreeRoot = NavTree->GetTreeRoot();
	String s = "";

	// �ˬd���G
	SNavItem * it = (SNavItem * ) TreeRoot->Items[5];
	s = String(it->Level) + "," + it->Type + "," + it->Title + "," + it->URL;
	// s ���ӬO "2,3,CBETA�g��,T01n0001_001#p0001a01"
	Dunitx::Testframework::Assert::IsTrue(s == "1,1,�Ĥ@��,chap1.xhtml");
}

void __fastcall CCBReaderTest::NavTreeReadNavLink()
{
	TList * TreeRoot = NavTree->GetTreeRoot();
	String s = "";

	// �ˬd���G
	SNavItem * it = (SNavItem * ) TreeRoot->Items[3];
	s = String(it->Level) + "," + it->Type + "," + it->Title + "," + it->URL;
	// s ���ӬO "2,3,�s���j���þ������,Taisho.xml"
	Dunitx::Testframework::Assert::IsTrue(s == "2,2,�s���j���þ������,Taisho.xml");
}

void __fastcall CCBReaderTest::NavTreeReadCBLink()
{
	TList * TreeRoot = NavTree->GetTreeRoot();
	String s = "";

	// �ˬd���G
	SNavItem * it = (SNavItem * ) TreeRoot->Items[2];
	s = String(it->Level) + "," + it->Type + "," + it->Title + "," + it->URL;
	// s ���ӬO "2,3,CBETA�g��,T01n0001_001#p0001a01"
	Dunitx::Testframework::Assert::IsTrue(s == "2,3,CBETA�g��,T01n0001_001#p0001a01");
}

static void registerTests()
{
  TDUnitX::RegisterTestFixture(__classid(CCBReaderTest));
}
#pragma startup registerTests 33