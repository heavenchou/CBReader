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
#include "juanline.h"
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
	String CatalogFile;     // �ؿ����
	String SpineFile;   // �M�����

	CCatalog * Catalog; // �ؿ�
	CSpine * Spine;    // �M�����
	CJuanLine * JuanLine;   // �U���P����檺���Y����, CBETA �M��

	void __fastcall LoadMetaData(String sMeta);	// ���J��]���
	__fastcall CSeries(String sDir);    // �ǤJ�ؿ�, �i���Ȥ�
	__fastcall ~CSeries();
};
// ---------------------------------------------------------------------------
#endif
