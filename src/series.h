// ---------------------------------------------------------------------------


#ifndef SeriesH
#define SeriesH


#ifdef _Windows
#include <Xml.Win.msxmldom.hpp>
#else
#include <Xml.omnixmldom.hpp>
#endif

#include <Xml.XMLDoc.hpp>
#include <fmx.h>

#include "catalog.h"
#include "spine.h"
#include "bookdata.h"
#include "juanline.h"
#include <utility>  // for std::pair
// ---------------------------------------------------------------------------

class CSeries
{
private: // User declarations

public: // User declarations

	String Dir; 	// ���Ѫ��ؿ�

    String ID;          // ID �N�X, CBETA �����w�� CBEAT
	String Title;       // ���D
	String Creator;     // �@��

	String NavFile;     // �������
	String CatalogFile;	// �ؿ����
	String SpineFile;   // �M�����
	String BookDataFile;   // BookData ���

	CCatalog * Catalog; // �ؿ�
	CSpine * Spine;		// �M�����
	CJuanLine * JuanLine;	// �U���P����檺���Y����, CBETA �M��
	CBookData * BookData;   // �C���Ѫ���T, �Ҧp T , �j����, 2

	void __fastcall LoadMetaData(String sMeta);	// ���J��]���

	// �Ѹg���h��g��
	String __fastcall CBGetFileNameBySutraNumJuan(String sBookID, String sSutraNum, String sJuan = "");
	// �ѥU������g��
	String __fastcall CBGetFileNameByVolPageFieldLine(String sBook, String sVol = "", String sPage = "", String sField = "", String sLine = "");
	__fastcall CSeries(String sDir);    // �ǤJ�ؿ�, �i���Ȥ�
	__fastcall ~CSeries();
};
// ---------------------------------------------------------------------------
#endif
