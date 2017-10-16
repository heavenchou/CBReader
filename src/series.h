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
// ---------------------------------------------------------------------------

class CSeries
{
private: // User declarations

public: // User declarations

	String Dir; 	// ���Ѫ��ؿ�

	String Title;       // ���D
	String Creator;     // �@��

	String NavFile;     // �������
	String TocFile;     // �ؿ����
	String SpineFile;   // �M�����

	void __fastcall LoadMetaData(String sMeta);	// ���J��]���
	__fastcall CSeries(String sDir);    // �ǤJ�ؿ�, �i���Ȥ�
	__fastcall ~CSeries();
};
// ---------------------------------------------------------------------------
#endif
