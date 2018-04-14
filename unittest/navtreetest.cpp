#include <DUnitX.TestFramework.hpp>
#include <stdio.h>

// ----------------------------------------------
// NavTree 讀取四種資料
void __fastcall CCBReaderTest::NavTreeReadTitle()
{
	NavTree = new CNavTree("nav.xhtml");
	TList * TreeRoot = NavTree->GetTreeRoot();
	String s = "";

	// 檢查結果
	SNavItem * it = (SNavItem * ) TreeRoot->Items[0];
	s = String(it->Level) + "," + it->Type + "," + it->Title + "," + it->URL;
	Dunitx::Testframework::Assert::IsTrue(s == "0,0,目錄,");
}

void __fastcall CCBReaderTest::NavTreeReadLink()
{
	TList * TreeRoot = NavTree->GetTreeRoot();
	String s = "";

	// 檢查結果
	SNavItem * it = (SNavItem * ) TreeRoot->Items[5];
	s = String(it->Level) + "," + it->Type + "," + it->Title + "," + it->URL;
	// s 應該是 "2,3,CBETA經文,T01n0001_001#p0001a01"
	Dunitx::Testframework::Assert::IsTrue(s == "1,1,第一卷,chap1.xhtml");
}

void __fastcall CCBReaderTest::NavTreeReadNavLink()
{
	TList * TreeRoot = NavTree->GetTreeRoot();
	String s = "";

	// 檢查結果
	SNavItem * it = (SNavItem * ) TreeRoot->Items[3];
	s = String(it->Level) + "," + it->Type + "," + it->Title + "," + it->URL;
	// s 應該是 "2,3,連結大正藏導覽文件,Taisho.xml"
	Dunitx::Testframework::Assert::IsTrue(s == "2,2,連結大正藏導覽文件,Taisho.xml");
}

void __fastcall CCBReaderTest::NavTreeReadCBLink()
{
	TList * TreeRoot = NavTree->GetTreeRoot();
	String s = "";

	// 檢查結果
	SNavItem * it = (SNavItem * ) TreeRoot->Items[2];
	s = String(it->Level) + "," + it->Type + "," + it->Title + "," + it->URL;
	// s 應該是 "2,3,CBETA經文,T01n0001_001#p0001a01"
	//Dunitx::Testframework::Assert::IsTrue(s == "2,3,CBETA經文,T01n0001_001#p0001a01");
	Dunitx::Testframework::Assert::IsTrue(aaa == 3);

}
