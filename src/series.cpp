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
	TocFile = "";     // �ؿ����
	SpineFile = "";   // �M�����

	Catalog = 0;	  // �ؿ�

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
	if(!TocFile.IsEmpty())
	{
		Catalog->LoadCatalog(Dir + TocFile);
	}
}
// ---------------------------------------------------------------------------
// �Ѻc�禡
__fastcall CSeries::~CSeries()
{

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

	Node = Document->DocumentElement->ChildNodes->Nodes["toc"];
	if(Node->HasAttribute("src"))
	{
		TocFile = Node->GetAttribute("src");
	}

	// Ū Spine

	Node = Document->DocumentElement->ChildNodes->Nodes["spine"];
	if(Node->HasAttribute("src"))
	{
		SpineFile = Node->GetAttribute("src");
	}
}
// ---------------------------------------------------------------------------

