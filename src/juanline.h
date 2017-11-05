//---------------------------------------------------------------------------

#ifndef JuanLineH
#define JuanLineH

#include "System.RegularExpressions.hpp"
#include "spine.h"
#include <map>
// ---------------------------------------------------------------------------
using namespace std;

class CJuanLine
{
private: // User declarations

	/* ��Ƶ��c

	Vol{"T01"} -> ���V 2 TStringList

	1. PageLine �O������� : 0001a01 , 0010b10 , .......
	2. SerialNo �O���b Spine ���� n �� (�y����)

	Spine �n�O���C�@��

	XML/T/T01/T01n0001_001.xml , T01, 0001, 001

	�n�d T01 �� 0100a01 �b���@�g���@��? �d�ߤ覡��:

	1. ���D�O T01
	2. �b Vol{"T01"}->PageLine �̭���� 0100a01 ���e�O���@�� (���]�� 5 ��)
	3. �� Vol{"T01"}->SerialNo[5] = xx (���]�b Spine ���� 120 ��)

	4. �� Spine->Vol[120] = "T01" , ���b T01
	5. �� Spine->Sutra[120] = "0002" , ���b�� 5 �g
	6. �� Spine->Juan[120] = "03" , ���b�� 3 ��

	*/

	struct SPageLineSerialNo {
		TStringList * PageLine;
		vector <int> SerialNo;
    };

	map <String, SPageLineSerialNo *> Vol;

public: // User declarations

	void __fastcall LoadFromSpine(CSpine * Spine);      	// ���J���
	__fastcall CJuanLine();
	__fastcall ~CJuanLine();
};
// ---------------------------------------------------------------------------
#endif
