// ---------------------------------------------------------------------------

#pragma hdrstop

#include "series.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)

// ---------------------------------------------------------------------------
// �غc��
__fastcall CSeries::CSeries(String sDir)
{
	// ���

	Title = "";       // ���D
	Creator = "";     // �@��
	NavFile = "";     // �������
	CatalogFile = "";     // �ؿ����
	SpineFile = "";   // �M�����
	BookDataFile = "";   // Bookdata ���

	Catalog = 0;	  // �ؿ�
	Spine = 0;    		// �M�����
	JuanLine = 0;   	// �U���P����檺���Y����, CBETA �M��
	BookData = 0;   // �C���Ѫ���T, �Ҧp T , �j����, 2

	//--------------

	Dir = sDir + "/";         // ���Ѫ��ؿ�

	// �P�_�ؿ������S�� metadata : index.xml

	String sMeta = Dir + "index.xml";

	String asFileName = FileSearch("index.xml", Dir);
	if (!asFileName.IsEmpty())
	{
		LoadMetaData(asFileName);
	}

	Catalog = new CCatalog(); // ���J�ؿ����
	if(!CatalogFile.IsEmpty())
	{
		Catalog->LoadCatalog(Dir + CatalogFile);
	}

	Spine = new CSpine();
	if(!SpineFile.IsEmpty())
	{
		Spine->LoadSpineFile(Dir + SpineFile);
	}

	Spine = new CSpine();
	if(!SpineFile.IsEmpty())
	{
		Spine->LoadSpineFile(Dir + SpineFile);
	}

	BookData = new CBookData();
	if(!BookDataFile.IsEmpty())
	{
		BookData->LoadBookDataFile(Dir + BookDataFile);
	}

	// CBETA �M��, �n�B�z JuanLine ���
	if(ID == "CBETA")
	{
		JuanLine = new CJuanLine();
		JuanLine->LoadFromSpine(Spine);
	}
}
// ---------------------------------------------------------------------------
// �Ѻc�禡
__fastcall CSeries::~CSeries()
{
	if(Catalog) delete Catalog;
	if(Spine) delete Spine;
	if(JuanLine) delete JuanLine;
	if(BookData) delete BookData;
}
// ---------------------------------------------------------------------------
// ���J��]���
void __fastcall CSeries::LoadMetaData(String sMeta)
{
	// ���R index.xml
	_di_IXMLDocument Document;
	_di_IXMLNode Node;

	Document = interface_cast<Xmlintf::IXMLDocument>(new TXMLDocument(NULL));
	Document->FileName = sMeta;
	Document->Active = true;

	// Ū ID

	Node = Document->DocumentElement->ChildNodes->Nodes["id"];
	if(Node->ChildNodes->Count > 0)
	{
		ID = Node->ChildNodes->Get(0)->Text;
	}

	// Ū Title

	Node = Document->DocumentElement->ChildNodes->Nodes["title"];
	if(Node->ChildNodes->Count > 0)
	{
		Title = Node->ChildNodes->Get(0)->Text;
	}

	// Ū Creator

	Node = Document->DocumentElement->ChildNodes->Nodes["creator"];
	if(Node->ChildNodes->Count > 0)
	{
		Creator = Node->ChildNodes->Get(0)->Text;
	}

	// Ū nav

	Node = Document->DocumentElement->ChildNodes->Nodes["nav"];
	if(Node->HasAttribute("src"))
	{
		NavFile = Node->GetAttribute("src");
	}

	// Ū toc

	Node = Document->DocumentElement->ChildNodes->Nodes["catalog"];
	if(Node->HasAttribute("src"))
	{
		CatalogFile = Node->GetAttribute("src");
	}

	// Ū Spine

	Node = Document->DocumentElement->ChildNodes->Nodes["spine"];
	if(Node->HasAttribute("src"))
	{
		SpineFile = Node->GetAttribute("src");
	}

	// Ū BookData
	Node = Document->DocumentElement->ChildNodes->Nodes["bookdata"];
	if(Node->HasAttribute("src"))
	{
		BookDataFile = Node->GetAttribute("src");
	}
}
// ---------------------------------------------------------------------------
// �h�Ѹg���h�� XML �ɦW
String __fastcall CSeries::CBGetFileNameBySutraNumJuan(String sBookID, String sSutraNum, String sJuan)
{
	return Spine->CBGetFileNameBySutraNumJuan(sBookID, sSutraNum, sJuan);
}
// ---------------------------------------------------------------------------
// �ѥU������g��
String __fastcall CSeries::CBGetFileNameByVolPageFieldLine(String sBook, String sVol, String sPage, String sField, String sLine)
{
	int iIndex = JuanLine->CBGetSpineIndexByVolPageFieldLine(sBook, sVol, sPage, sField, sLine);
	if(iIndex == -1) return "";

	return Spine->CBGetFileNameBySpineIndex(iIndex);
}
// ---------------------------------------------------------------------------



