//---------------------------------------------------------------------------

#ifndef HighlightH
#define HighlightH

#include <vector>
#include <map>
#include <utility>  // for std::pair
#include <FMX.DialogService.hpp>
#include "../../Monster/src/Monster.h"
#include "../../Monster/src/PostfixStack.h"
#include "../../Monster/src/Int2List.h"
#include "subutil.h"

using namespace std;

//---------------------------------------------------------------------------
// ���C�⪺����
class CHighlight
{
private: // User declarations

public: // User declarations

	String * HTMLSource;                     // �ӷ��r��

	TmyPostfixStack * PostfixStack;		// �B��Ϊ�

	// �j�M�r�ꤤ "��" ���ƶq, �Ҧp ���� * ù�~ = 2 �ӵ�
	int FoundPosCount;                  // FoundPos ���ƶq

	// �x�s��쪺��m (�o�O�������I����m)
	// �C�@�ӵ����n���@�ӰO����m���Ŷ�, �o�O�@�ӤG���}�C
	// �� : ���� + ù�~
	// FoundPos[0] �O "����" ����m, FoundPos[1] �O "ù�~" ����m.
	// FoundPos[0]->Int2s->Items[n] ��ܬO��n�ӵ��Ī���m, �Y����m�N�O�� TPoint(x,y)
	TmyInt2List ** FoundPos;

	// �Ҧp�Y�r������u�ꪺ�O�զr [��*��],
	// pair ���e�N�O '['��m�Φr����� 5
	map<int, pair<System::WideChar *, int> > PosToReal;  // Pos �ण�������I���u���m


	// �o�T�ӬO�n���Ϊ����n���, �O���C�@�Ӧr�����F���ǩw��, �s��, ���
	// �w��, �᭱�� vector , �T�դ@�ӳ��, ���O�O �� x ��, �� y ���X�{, �U�@���X�{ (�̫�h�^�� 0)
	map <System::WideChar *, vector<int> > mpWordAnchor;
	// �s��, �C�Ӧr�u���@��,
	map <System::WideChar *, pair<int,int> > mpWordLink;
	// ���, �C�Ӧr���i��n�X���C��
	map <System::WideChar *, vector<int> > mpWordClass;
	// �C�@�Ӧr������, �Ҧp�զr���N�|�����
	map <System::WideChar *, int> mpWordLength;

	// �C�@�ӵ������@�Ӧ�C, �o�O�n�O���ثe�w�e�u����C�O�e�줰��a��F.

	int * FoundPosIndex;
	TmyMonster * SearchEngine;   // �����˯�����

	String __fastcall AddHighlight(String * sSource);  // �B�z���

	void __fastcall GetAllFoundPos(void);		// �N�Ҧ���������m�����
	void __fastcall GetOneFoundPos(int iNum);	// �N�Y�ӵ�����m�����
	void __fastcall FindTheRange(void);			// �N�X�G�n�D���d���X�� (�g�L���L�B��F), �Ǧ^��쪺�ռ�
	void __fastcall KeepRangePos(void);			// �N�C�@�ӵ��ŦX�̲צ�m���d�U��.
	void __fastcall GetEveryWordInfo(void);     // ��C�@�Ӧr�n�w��B���P�s������Ƴ��O���U��, �]�A���|���r
	String __fastcall MakeHighlight();  			// ��ڶ��

	// �Y�ӭn��⪺��, �[�W�w�� name ���аO���H�s��, ���X�O�� iNum ���� iTime ���X�{
	void __fastcall AddWordAnchor(vector<System::WideChar> * vOutput, System::WideChar * pPoint);

	// �[�W�s���ζ�⪺ class
	void __fastcall AddWordLink(vector<System::WideChar> * vOutput, System::WideChar * pPoint);

	// ����U�@�ӥi�H�d�ߪ��r
	// �Ҧp�d�ߪ��r��O "ABC XYA �p�O�A�ڻD"
	// �ثe���Хi��b�ťթμ��I�A�n���U�@�ӥi�d�ߪ��r�����
	System::WideChar * NextFindPoint(System::WideChar * pFindWord);

	// ���R�@�� <span class="gaiji"....> �аO
	void AnalysisGiajiTag(System::WideChar ** pPoint, System::WideChar ** pDesPoint, System::WideChar ** pUniPoint, int * iDesLen, int * iUniLen);
	__fastcall CHighlight(TmyMonster * seSearchEngine);
	__fastcall ~CHighlight();
};
// ---------------------------------------------------------------------------
#endif
