//---------------------------------------------------------------------------

#ifndef HighlightH
#define HighlightH

#include <vector>
#include <map>
#include "../../Monster/src/Monster.h"
#include "../../Monster/src/PostfixStack.h"
#include "../../Monster/src/Int2List.h"

//---------------------------------------------------------------------------
// ���C�⪺����
class CHighlight
{
private: // User declarations

public: // User declarations

	struct Smy_HighlightPos {
		int Begin;	// highlight ���_�I
		int End;	// highlight �����I
		int Num;	// �ĴX�ӵ�
		int Index;	// �ӵ����ĴX���X�{
		int Next;	// �U�@�ӵ����s��, �q�`�O Index + 1 , �Y�O�̫�@��, �h�O 0
	} HighlightPos;

	String * HTMLSource;                     // �ӷ��r��
	//vector< wchar_t > vText;            // ��m���G��

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
	map<int, pair<wchar_t *, int>> PosToReal;  // Pos �ण�������I���u���m


	// �o�T�ӬO�n���Ϊ����n���, �O���C�@�Ӧr�����F���ǩw��, �s��, ���
	// �w��, �᭱�� vector , �T�դ@�ӳ��, ���O�O �� x ��, �� y ���X�{, �U�@���X�{ (�̫�h�^�� 0)
	map <wchar_t *, vector<int>> mpWordAnchor;
	// �s��, �C�Ӧr�u���@��,
	map <wchar_t *, pair<int,int>> mpWordLink;
	// ���, �C�Ӧr���i��n�X���C��
	map <wchar_t *, vector<int>> mpWordClass;
	// �C�@�Ӧr������, �Ҧp�զr���N�|�����
	map <wchar_t *, int> mpWordLength;

	/////////////////////////////////////

	// �C�@�ӵ������@�Ӧ�C, �o�O�n�O���ثe�w�e�u����C�O�e�줰��a��F.

	int * FoundPosIndex;
	TmyMonster * SearchEngine;   // �����˯�����

	String __fastcall AddHighlight(String * sSource);  // �B�z���

	void __fastcall GetAllFoundPos(void);		// �N�Ҧ���������m�����
	void __fastcall GetOneFoundPos(int iNum);	// �N�Y�ӵ�����m�����
	void __fastcall FindTheRange(void);			// �N�X�G�n�D���d���X�� (�g�L���L�B��F), �Ǧ^��쪺�ռ�
	void __fastcall KeepRangePos(void);			// �N�C�@�ӵ��ŦX�̲צ�m���d�U��.
	void __fastcall GetEveryWordInfo(void);     // ��C�@�Ӧr�n�w��B���P�s������Ƴ��O���U��, �]�A���|���r
	void __fastcall GetNextHighlightPos(bool bInitial = false);	// ���o�U�@�ӭn�ܦ⪺����������m, ����ǤJ�ȳ���ܪ�Ȥ�
	int __fastcall Get1PattenLen(const wchar_t *);		// ��X���r��U�@�r������, �^��Ʀr�� 1 , [��*��] �� 7, &#x1234; �� 8
	String __fastcall MakeHighlight_old();  			// ��ڶ��
	String __fastcall MakeHighlight();  			// ��ڶ��

	// ���Y�ӭn��⪺��, �[�W name ���аO���H�s��, ���X�O�� iNum ���� iTime ���X�{
	// <a name="iNum_iTime"></a>
	void __fastcall AddSearchHeadName(vector<wchar_t> * vOutput, int iNum, int iTime);
	// �[�W <a href="#x_y" class="SearchWordx"> �аO
	void __fastcall AddSearchNameTag(vector<wchar_t> * vOutput, int iNum, int iTime);

	// �Y�ӭn��⪺��, �[�W�w�� name ���аO���H�s��, ���X�O�� iNum ���� iTime ���X�{
	void __fastcall AddWordAnchor(vector<wchar_t> * vOutput, wchar_t * pPoint);

	// �[�W�s���ζ�⪺ class
	void __fastcall AddWordLink(vector<wchar_t> * vOutput, wchar_t * pPoint);

	__fastcall CHighlight(TmyMonster * seSearchEngine);
	__fastcall ~CHighlight();
};
// ---------------------------------------------------------------------------
#endif
