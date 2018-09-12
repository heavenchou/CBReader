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
	PublishDate = ""; // �X�����
	NavFile = "";     // �������
	Nav2File = "";     // �������
	Nav3File = "";     // �������
	CatalogFile = "";     // �ؿ����
	SpineFile = "";   	// �M�����
	BookDataFile = "";   // Bookdata ���
	JSFile = "";          // CBReader �M�Ϊ� js ��
	Version = "";       // ��ƪ���

	Catalog = 0;	  	// �ؿ�
	Spine = 0;    		// �M�����
	JuanLine = 0;   	// �U���P����檺���Y����, CBETA �M��
	BookData = 0;   	// �C���Ѫ���T, �Ҧp T , �j����, 2

	SearchEngine_orig = 0;   // �����˯�����
	SearchEngine_CB = 0;   // �����˯�����

	//--------------

	Dir = IncludeTrailingPathDelimiter(sDir);	// ���Ѫ��ؿ�

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
	if((ID == "CBETA")||(ID == "SEELAND"))
	{
		JuanLine = new CJuanLine();
		JuanLine->LoadFromSpine(Spine);
	}

	// ���J�����˯�
	LoadSearchEngine();
}
// ---------------------------------------------------------------------------
// �Ѻc�禡
__fastcall CSeries::~CSeries()
{
	if(Catalog) delete Catalog;
	if(Spine) delete Spine;
	if(JuanLine) delete JuanLine;
	if(BookData) delete BookData;
	if(SearchEngine_orig)
	{
		delete SearchEngine_orig;
		SearchEngine_orig = 0;
	}
	if(SearchEngine_CB)
	{
		delete SearchEngine_CB;
		SearchEngine_CB = 0;
	}
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

	// Ū date

	Node = Document->DocumentElement->ChildNodes->Nodes["date"];
	if(Node->ChildNodes->Count > 0)
	{
		PublishDate = Node->ChildNodes->Get(0)->Text;
	}

	// Ū nav

	Node = Document->DocumentElement->ChildNodes->Nodes["nav"];
	if(Node->HasAttribute("src"))
	{
		NavFile = Node->GetAttribute("src");
	}

	// Ū nav

	Node = Document->DocumentElement->ChildNodes->Nodes["nav2"];
	if(Node->HasAttribute("src"))
	{
		Nav2File = Node->GetAttribute("src");
	}

	Node = Document->DocumentElement->ChildNodes->Nodes["nav3"];
	if(Node->HasAttribute("src"))
	{
		Nav3File = Node->GetAttribute("src");
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

	// Ū JSFile
	Node = Document->DocumentElement->ChildNodes->Nodes["javascript"];
	if(Node->HasAttribute("src"))
	{
		JSFile = Node->GetAttribute("src");
	}

	// Ū Version

	Node = Document->DocumentElement->ChildNodes->Nodes["version"];
	if(Node->ChildNodes->Count > 0)
	{
		Version = Node->ChildNodes->Get(0)->Text;
	}
}
// ---------------------------------------------------------------------------
// �h�Ѹg���h�� XML �ɦW
String __fastcall CSeries::CBGetFileNameBySutraNumJuan(String sBookID, String sSutraNum, String sJuan, String sPage, String sField, String sLine)
{
	String sFileName = Spine->CBGetFileNameBySutraNumJuan(sBookID, sSutraNum, sJuan);

 	// �ɦW�n�ɤW #p0001a01 �o�خ榡����m

	String sPageLine = CBGetPageLine(sPage, sField, sLine);
	if(sPageLine != "")
	{
		sFileName += "#p" + sPageLine;
	}

	return sFileName;
}
// ---------------------------------------------------------------------------
// �ѥU������g��
String __fastcall CSeries::CBGetFileNameByVolPageFieldLine(String sBook, String sVol, String sPage, String sField, String sLine)
{
	// �ǤJ T, 1 , �Ǧ^ "01" �o�ؼзǪ��U��
	sVol = BookData->GetNormalVolNumString(sBook, sVol);

	int iIndex = JuanLine->CBGetSpineIndexByVolPageFieldLine(sBook, sVol, sPage, sField, sLine);
	if(iIndex == -1) return "";

	String sFileName = Spine->CBGetFileNameBySpineIndex(iIndex);

	// �ɦW�n�ɤW #p0001a01 �o�خ榡����m

	String sPageLine = CBGetPageLine(sPage, sField, sLine);
	if(sPageLine != "")
	{
		sFileName += "#p" + sPageLine;
	}

	return sFileName;
}
// ---------------------------------------------------------------------------
// �ѭ������o�з� 0001a01 �榡���r��
String __fastcall CSeries::CBGetPageLine(String sPage, String sField, String sLine)
{
	if(sPage != "" || sField != "" || sLine != "")
	{
		if(sPage == "") sPage = "0001";
		int iLen = sPage.Length();
		if(iLen < 4)
		{
			// �� 0 , ���L�Y�Ĥ@�Ӧr�O�^��r, �n�O�d
			if(*sPage.begin() >= '0' && *sPage.begin() <= '9')
				sPage = String().StringOfChar(L'0', 4 - iLen) + sPage;
			else
			{
				// �Ĥ@�r���O�Ʀr
				String sFirst = *sPage.begin();
				sPage.Delete0(0,1);
				sPage = sFirst + String().StringOfChar(L'0', 4 - iLen) + sPage;
            }
		}
		else if(iLen > 4)
		{
			sPage.Delete0(0,iLen-4);
		}

		if(sField == "") sField = "a";
		iLen = sField.Length();
		if(iLen > 1)
		{
			sField.Delete0(0,iLen-1);
		}

		if(sLine == "") sLine = "01";
		iLen = sLine.Length();
		if(iLen < 2)
		{
			sLine = String().StringOfChar(L'0', 2 - iLen) + sLine;
		}
		else if(iLen > 2)
		{
			sLine.Delete0(0,iLen-2);
		}

		return sPage + sField + sLine;
	}
	else
	{
		return "";
    }
}
// ---------------------------------------------------------------------------
// ���J�����˯�����
void __fastcall CSeries::LoadSearchEngine()
{
	if(TDirectory::Exists(Dir + "index"))
	{
		// CBETA ��������
		String sWordIndexFile = Dir + "index/wordindex.ndx";
		String sMainIndexFile = Dir + "index/main.ndx";
		if(TFile::Exists(Dir + SpineFile) &&
			TFile::Exists(sWordIndexFile) &&
			TFile::Exists(sMainIndexFile))
		{
			SearchEngine_CB = new TmyMonster(Dir + SpineFile, sWordIndexFile, sMainIndexFile);
		}

		// ��Ѫ�����

		sWordIndexFile = Dir + "index/wordindex_o.ndx";
		sMainIndexFile = Dir + "index/main_o.ndx";
		if(TFile::Exists(Dir + SpineFile) &&
			TFile::Exists(sWordIndexFile) &&
			TFile::Exists(sMainIndexFile))
		{
			SearchEngine_orig = new TmyMonster(Dir + SpineFile, sWordIndexFile, sMainIndexFile);
		}
	}
}
// ---------------------------------------------------------------------------
// ��������˯�����
void __fastcall CSeries::FreeSearchEngine()
{
	if(SearchEngine_orig)
	{
		delete SearchEngine_orig;
		SearchEngine_orig = 0;
	}
	if(SearchEngine_CB)
	{
		delete SearchEngine_CB;
		SearchEngine_CB = 0;
	}
}
// ---------------------------------------------------------------------------



