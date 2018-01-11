//---------------------------------------------------------------------------

#pragma hdrstop

#include "highlight.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

// ---------------------------------------------------------------------------
// �غc��
__fastcall CHighlight::CHighlight(TmyMonster * seSearchEngine)
{
	SearchEngine = seSearchEngine;
	HTMLSource = 0;
	PostfixStack = new TmyPostfixStack;
	FoundPos = 0;					// �x�s��쪺��m
	FoundPosIndex = 0;
	PosToReal.clear();
	PosToReal[0] = make_pair((wchar_t *)0,0);   // �p�G�ǤJ 0 , �]�Ǧ^ 0, �o�b�̫�@����⤧��|�Ψ�
}
// ---------------------------------------------------------------------------
// �Ѻc�禡
__fastcall CHighlight::~CHighlight()
{
	if(FoundPos)
	{
		for(int i=0; i<FoundPosCount; i++)
		{
			if(FoundPos[i]) delete FoundPos[i];
		}
		delete[] FoundPos;
	}
	if(FoundPosIndex) delete[] FoundPosIndex;
	PosToReal.clear();
}
// ---------------------------------------------------------------------------
// ���
String __fastcall CHighlight::AddHighlight(String * sSource)
{
	HTMLSource = sSource;

	GetAllFoundPos();		// �N�Ҧ��������d�򳣧�X��
	FindTheRange();			// �N�X�G�n�D���d���X�� (�g�L���L�B��F)

	if(SearchEngine->OKSentence.Pos0("*")>=0 ||
	   SearchEngine->OKSentence.Pos0("+")>=0 ||
	   SearchEngine->OKSentence.Pos0("-")>=0)
	{
		KeepRangePos();			// �N�C�@�ӵ��ŦX�̲צ�m���d�U��.
	}

	// ��C�@�Ӧr�n�w��B���P�s������Ƴ��O���U��, �]�A���|���r
	GetEveryWordInfo();
	return MakeHighlight();
}

// ---------------------------------------------------------------------------
// �N�Ҧ���������m�����
void __fastcall CHighlight::GetAllFoundPos(void)
{
	// �j�M�r�ꤤ "��" ���ƶq, �Ҧp ���� * ù�~ = 2 �ӵ�
	FoundPosCount = SearchEngine->SearchWordList->Count;

	// �C�@�ӵ����n���@�ӰO����m���Ŷ�, �o�O�@�ӤG���}�C
	FoundPos = new TmyInt2List*[FoundPosCount];

	for(int i=0; i<FoundPosCount; i++)
	{
		FoundPos[i] = new TmyInt2List;		// �ŧi�@�ӪŶ�
		GetOneFoundPos(i);					// ���o�C�@�ӵ�����m
	}
}
// ---------------------------------------------------------------------------
// �N�Y�ӵ�����m�����
void __fastcall CHighlight::GetOneFoundPos(int iNum)
{
/*
	1.�L�o�аO�μаO, ����Ĥ@�Ӧr�X�{
	2.�P�_�O���O�ثe�n�䪺�r.
	3.�O�N�O��, ���~���
	4.���O�N�O��, ���~���

	matching = 0;		// �O�_�b��襤
	pos = 0				// �r��
	pFindWord = Tag;	// Tag �O�n��諸�r��

	while(*point)
	{
		// �����аO�μ��I

		wiile(*point)
		{
			if (tag) skip tag and continue;
			if (sign) skip sign and continue;
			break;
		}

		pos++;

		if(*point == *pFindWord)	// ���r
		{
			if(!matching)
			{
				matching = 1;
				postmp = pos;
				pointtmp = point;
			}
			point++;
			pFindWord++;

			if(!pFindWord)	// ���õ���
			{
				postmp is the answer;
				matching = 0;
				pFindWord = Tag;
			}
		}
		else		// �S���
		{
			if(matching)
			{
				matching = 0;
				point = pointtmp;
				pos = postmp;
				pFindWord = Tag;
			}
			point++;
		}
	}
*/

	String sWord = SearchEngine->SearchWordList->Strings[iNum];
	bool bMatching = false;		// �P�_�O�_�b��襤
	int iPos = -1;		// �h���аO�μ��I���᪺�r��, �Ĥ@�Ӧr�� 0 �}�l (��Ȭ�0�O�]���@�}�l�٥i�ण�O�r, �O�аO�μ��I)
	int iPosTmp;
	wchar_t * pPoint = HTMLSource->FirstChar();		// �C���b HTMLSource ������
	wchar_t * pPointTmp;
	wchar_t * pFindWord = sWord.c_str();		// �n��⪺�r

	// �Ҧp�j�M "�����t + ���t" , �o�Ӧr��|�X�{�b HTML �̤W��.
	// �ĤG�ժ� "���t" �����b�Ĥ@�դ���, �H�K���|
	// �Ҧp�����t����m�O (1~3) , �ҥH "���t" �� iPos ���� > 3
	// �ҥH�n����X�W�@�ղĤ@����������m

	int iAfterThisPos = 0;
	if(iNum > 0)
	{
		TPoint * tpPtr = (TPoint*) FoundPos[iNum-1]->Int2s->Items[0];
		iAfterThisPos = tpPtr->y;
    }

	// ������ <!-- search head --> ���a��

	while(*pPoint)
	{
		if (wcsncmp(pPoint, L"<div id=\"SearchHead\">", 21) == 0)
		{
			pPoint += 21;
			break;
		}
		pPoint++;
	}

	if(!*pPoint)
	{
        ShowMessage(u"�S�����w�аO�A�{���n�A�ˬd�B�z");
    }

	while(*pPoint)
	{
		// �����аO�μ��I
		int iTagNum = 0;
		bool bShowError = false;    // ���S�����~�T��? �p�G���N�]�� true, �N���Τ@���q
		while(*pPoint)
		{
			// �S���p, �J��歺�аO

			if (wcsncmp(pPoint, L"<span class=\"linehead\">", 23) == 0)
			{
                // <span class="linehead">GA009n0008_p0003a01��</span>
            	// <span class="linehead">ZS01n0001_p0001a01��</span>
            	// <span class="linehead">ZW01n0001_p0001a01��</span>
                // <span class="linehead">A001n0001_p0001a01��</span>
				// <span class="linehead">T30n1579_p0279a10��</span>
				// <span class="linehead">[0279a09] </span>

				if(pPoint[23] == u'[') pPoint += 40;
				else if(pPoint[36] == u'p') pPoint += 53;
				else if(pPoint[35] == u'p') pPoint += 52;
				else if(pPoint[34] == u'p') pPoint += 51;
				else if(pPoint[33] == u'p') pPoint += 50;
				else if(pPoint[32] == u'p') pPoint += 49;
				else
                {
                    if(!bShowError)
                    {
						ShowMessage(u"LineHead Error : Please call Heaven!");
                        bShowError = true;
                    }
                    pPoint += 49;
                }
				continue;
			}

			// 1.�J��аO, �n�B�~�B�z

			if(*pPoint == u'<')
			{
				iTagNum++;		// �]���аO���_��, �ҥH�n�p��
				pPoint++;
				continue;
			}

			if(*pPoint == u'>')
			{
				iTagNum--;
				pPoint++;
				continue;
			}

			if(iTagNum && *pPoint)
			{
				pPoint++;
				continue;
			}

			// 2.�������I�Ÿ�

			if(CWordTool::GetWordType(*pPoint) == CWordTool::wtPunc)
			{
				pPoint++;
				continue;
			}

			// 3.�����ť�

			if(CWordTool::GetWordType(*pPoint) == CWordTool::wtSpace)
			{
				pPoint++;
				continue;
			}

			// 4.�P�� [��]
			if (wcsncmp(pPoint, L"[��]", 3) == 0)
			{
				pPoint += 3;
				continue;
			}

			// 5.�¼Ʀr [01] , �o�b�����åX�{
			if(*pPoint == u'[')
			{
				wchar_t * pTmp = pPoint;
				pPoint++;

				if (wcsncmp(pPoint, L"P.", 2) == 0)  // �n�Ǧ� [P.nn] �� PTS ���X
                    pPoint += 2;

				if(*pPoint >= u'0' && *pPoint <= u'9')
				{
					while(*pPoint >= u'0' && *pPoint <= u'9') pPoint++;

					if (*pPoint == u']')
                    {
                        pPoint ++;
                        continue;
                    }
				}

				pPoint = pTmp;
			}

			// 6.�հɼƦr [<a id="a0001005" ....<..�_��..>. >5</a>]
			// ???? �o�ӷs�����|��
			if(*pPoint == u'[')
			{
				wchar_t * pTmp = pPoint;
				pPoint++;

				if(*pPoint == u'<')
				{
                    // ���_�����аO, ����o�˥�
					//while(*pPoint != '>' && *pPoint) pPoint++;

                    int iTagNumTmp = 1;
                    while(iTagNumTmp > 0 && *pPoint)
                    {
                        pPoint++;
						if(*pPoint == u'>') iTagNumTmp--;
						if(*pPoint == u'<') iTagNumTmp++;
                    }

					if(*pPoint == u'>')
					{
						pPoint++;
                        // �����îժ`�� [��XX] or [��XX] or [��XX]
						//if (wcsncmp(pPoint, u'��', 1) == 0)
						if(*pPoint == u'��')
						{
							pPoint += 1;
						}
						//else if (wcsncmp(pPoint, L'��', 1) == 0)
						else if(*pPoint == u'��')
						{
							pPoint += 1;
						}
						//else if (wcsncmp(pPoint, L'��', 1) == 0)
						else if(*pPoint == u'��')
						{
							pPoint += 1;
						}

						while(*pPoint >= u'0' && *pPoint <= u'9') pPoint++;

						if (*pPoint == u'��')
						{
							pPoint += 1;
						}
						if (wcsncmp(pPoint, L"</a>]", 5) == 0)
						{
							pPoint += 5;
							continue;
						}
					}
				}

				pPoint = pTmp;
			}

			// ��ܳ��S���n�L�o���F
			break;
		}
		if(!*pPoint) break;		// �p�G�����N���X�j��

		iPos++;					// �S���@�ӯu�����r�F
		// �Ǧ^�U�@�Ӧr������, �i��Oext-b����=2, �զr>2, ���^�ƲŸ�1

		int i1stWordLen;	// �Ĥ@�Ӧr������, �n�O�_��
		int iThisWordLen = Get1PattenLen(pPoint);

		if (*pFindWord == u'?')		// �U�Φr��
		{
			PosToReal[iPos] = make_pair(pPoint, iThisWordLen);
			// �o�� bMarching �@�w�O true
			pPoint += iThisWordLen;
			pFindWord++;

		}
		else if(iPos>iAfterThisPos && wcsncmp(pPoint,pFindWord,iThisWordLen)==0)	// ���
		{
			PosToReal[iPos] = make_pair(pPoint, iThisWordLen);

			if(!bMatching)				// �ثe�O�Ĥ@�Ӧr
			{
				i1stWordLen = iThisWordLen;
				bMatching = true;
				iPosTmp = iPos;			// �ثe��m�O�_��
				pPointTmp = pPoint;		// �ثe��m�O�_��
			}

			pPoint += iThisWordLen;
			pFindWord += iThisWordLen;

			if(*pFindWord == 0)		// ��粒������, ���H!!!
			{
				bMatching = false;
				FoundPos[iNum]->Add(iPosTmp,iPos);  // �N��쪺��m�s�_�� (�Y, ��)
				pFindWord = sWord.c_str();		// �n��⪺�r
			}
		}
		else	// �䤣��
		{
            bool bFound = false;
            // �S���, �����i��O�ʦr, �ҥH�n�ˬd�᭱�O���O�� <!--gaiji,��,1[��*��],2&#Xxxxx;,3-->
			if(wcsncmp(pPoint+iThisWordLen,L"<!--gaiji,",10)==0)	// �᭱�O�ʦr
			{
				/* ???? �ʦr�ݳB�z
				wchar_t * pLast = pPoint+iThisWordLen;     // �n��̫ᵲ��������
                for(int i=0; i<500; i++)                // �̦h 500 �Ӧr, �o���ӥi��F, �u�O�קK�� while ���J�L�a�j��
                {
                    if(*pLast == '>') break;
                    pLast++;
                }
                AnsiString sGaiji = AnsiString(pPoint+iThisWordLen , pLast +1 - pPoint-iThisWordLen);   // ���X "<!--gaiji,��,1[��*��],2&#Xxxxx;,3-->"
                int iPos1 = sGaiji.AnsiPos(",");
                int iPos2 = sGaiji.AnsiPos(",1");
				int iPos3 = sGaiji.AnsiPos(",2");
                int iPos4 = sGaiji.AnsiPos(",3");

                AnsiString sNor = sGaiji.SubString(iPos1+1,iPos2-iPos1-1);    // ���X�q�Φr
                AnsiString sDes = sGaiji.SubString(iPos2+2,iPos3-iPos2-2);    // ���X�զr��
                AnsiString sUni = sGaiji.SubString(iPos3+2,iPos4-iPos3-2);    // ���X unicode

				if(sNor.Length()>0 && wcsncmp(sNor.c_str(),pFindWord,sNor.Length())==0)	// �M�q�Φr�ۦP
                {
                    if(!bMatching)				// �ثe�O�Ĥ@�Ӧr
        			{
		        		i1stWordLen = iThisWordLen;
				        bMatching = true;
        				iPosTmp = iPos;			// �ثe��m�O�_��
        				pPointTmp = pPoint;		// �ثe��m�O�_��
        			}

		        	pPoint += iThisWordLen;
        			pFindWord = pFindWord + sNor.Length();

			        if(*pFindWord == 0)		// ��粒������, ���H!!!
        			{
        				bMatching = false;
        				FoundPos[iNum]->Add(iPosTmp,iPos);
        				pFindWord = sWord.c_str();		// �n��⪺�r
        			}
                    bFound = true;
                }
                if(sDes.Length()>0 && wcsncmp(sDes.c_str(),pFindWord,sDes.Length())==0)	// �M�զr���ۦP
                {
                    if(!bMatching)				// �ثe�O�Ĥ@�Ӧr
        			{
		        		i1stWordLen = iThisWordLen;
				        bMatching = true;
        				iPosTmp = iPos;			// �ثe��m�O�_��
        				pPointTmp = pPoint;		// �ثe��m�O�_��
        			}

		        	pPoint += iThisWordLen;
        			pFindWord = pFindWord + sDes.Length();

			        if(*pFindWord == 0)		// ��粒������, ���H!!!
        			{
        				bMatching = false;
        				FoundPos[iNum]->Add(iPosTmp,iPos);
        				pFindWord = sWord.c_str();		// �n��⪺�r
        			}
                    bFound = true;
                }
                if(sUni.Length()>0 && wcsncmp(sUni.c_str(),pFindWord,sUni.Length())==0)	// �M�q�Φr�ۦP
                {
                    if(!bMatching)				// �ثe�O�Ĥ@�Ӧr
        			{
		        		i1stWordLen = iThisWordLen;
				        bMatching = true;
        				iPosTmp = iPos;			// �ثe��m�O�_��
        				pPointTmp = pPoint;		// �ثe��m�O�_��
        			}

		        	pPoint += iThisWordLen;
        			pFindWord = pFindWord + sUni.Length();

			        if(*pFindWord == 0)		// ��粒������, ���H!!!
        			{
        				bMatching = false;
						FoundPos[iNum]->Add(iPosTmp,iPos);
        				pFindWord = sWord.c_str();		// �n��⪺�r
        			}
                    bFound = true;
				}
				*/
            }

            if(!bFound)     // �̵M�S���, �s�ʦr���S���ŦX
            {
			    if(bMatching)
    			{
    				bMatching = false;
    				pPoint = pPointTmp;
    				iPos = iPosTmp;
    				pFindWord = sWord.c_str();		// �n��⪺�r
    				pPoint += i1stWordLen;
    			}
				else pPoint += iThisWordLen;
			}
		}
	}
}
// ---------------------------------------------------------------------------
// �N�X�G�n�D���d���X�� (�g�L���L�B��F), �Ǧ^��쪺�ռ�
void __fastcall CHighlight::FindTheRange(void)
{
	/*
	###################################
	# �m�߭p����B��
	#
	# ��h
	#  �p�G�O�Ʀr, �p�G���B��Ÿ�, �B�h�Ƴ��@��, �N�B��, ���G���J query stack
	#  �p�G�O�Ʀr, �p�G���B��Ÿ�, �p�G�h�Ƥ��@��, ���J query stack
	#  �p�G�O�Ʀr, �S���B��Ÿ�, ���J query stack
	#
	#  �p�G�O�B��Ÿ�, ���J op stack , �B�O���ثe�h��
	#
	#  �p�G�O���A��, �ثe�h�� + 1
	#  �p�G�O�k�A��, �h�� - 1 , �åB�B��
	###################################
	*/

	PostfixStack->Initial();
	String sPatten;
	String sOKSentence = SearchEngine->OKSentence;	// �Ȧs���r��		// �x�s��쪺��m

	// �ثe�ĴX�Ӧr��? �Ҧp (S&S)|S  (���&����)|���I  ����O�� 0 ��
	int iPattenNum = 0;

	//while (sOKSentence.Length())
	for(int i=0; i<sOKSentence.Length(); i++)
	{
		sPatten = sOKSentence.SubString0(i,1);		// ���X patten

		if(sPatten.IsDelimiter0(SearchEngine->OpPatten, 0))		// �p�G�O�B��Ÿ�����
		{
			PostfixStack->PushOp(sPatten);
		}
		else
		{
			// ???? �[�t����k, �p�G�O and �N�u��e�@�Ӧ����G��, �p�G�O or �N�u�d�e�@�ӬO�S��쪺
			// ???? �[�t����k, �p�G���έp����, �����N���, ����t�׷|���

			// �B�z�@�Ӧr

			// �Ĥ@�ե����X��, �]���Ĥ@�եu�O�ܽd, ����C�J�p��  (�ª��P�_�k)

            // ���U�O�s��k
			// ���u�Ĥ@�դF... �]���j�M�r��i��O "�٧Q - �٧Q��" , �ҥH "�٧Q" �|�b html �@�}�l�N�X�{�G��
			// �ҥH�P�_�k�n�令 �b�̫�@�ժ��̫�@�Ӧr���e�X�{��, �Ҧ��� "�٧Q" ���n�����X��

			TPoint * tpPtrLast = (TPoint*) FoundPos[FoundPosCount-1]->Int2s->Items[0]; // html �W��ܽd�r��̫�@�ժ���m, �p   "�٧Q ! �٧Q��" , ��� "�٧Q��" ���@��
			int iLastPos = tpPtrLast->y; // "�٧Q - �٧Q��" , "��" ����m, ����m���e�����n������.

			vector<TPoint *> vSavePtr;  // �n��R�����O�����s�U��, �̫�A�s�^�h
			vSavePtr.clear();

			// �C�ӵ����Ĥ@�դ@�w�n�����ðO���_��
			TPoint * tpPtr = (TPoint*) FoundPos[iPattenNum]->Int2s->Items[0];
			FoundPos[iPattenNum]->Int2s->Delete(0);
			vSavePtr.push_back(tpPtr);

			// �A�������ƪ�
			while (FoundPos[iPattenNum]->Int2s->Count > 0)
			{
				// �A�����X�Ĥ@��
				tpPtr = (TPoint*) FoundPos[iPattenNum]->Int2s->Items[0];
				// ��m��̫�@�ӵ����Ĥ@�ժ��̫�@�Ӧr�٤p, �N�n����.
				// �٧Q - �٧Q�� => �� "��" �٤p���N���n�p��, �����O�g��
				if(tpPtr->x <= iLastPos)
				{
					FoundPos[iPattenNum]->Int2s->Delete(0);
					vSavePtr.push_back(tpPtr);
                }
                else
                {
                    break;
                }
			}

			sPatten = SearchEngine->SearchWordList->Strings[iPattenNum];	// ���X�Y�@���r��
			PostfixStack->PushQuery(FoundPos[iPattenNum], sPatten);

			// �A��R�����U�թ�^�h
			for(int i=vSavePtr.size(); i>0; i--)
			{
                tpPtr = vSavePtr[i-1];
				FoundPos[iPattenNum]->Int2s->Insert(0,tpPtr);
			}

			iPattenNum++;
		}
	}
	// ???? �o�O�ռ�, ���O����, �Ҧp ��� near ���� �i���O 1 ��
	//return (PostfixStack->QueryStack[0]->Int2s->Count);
}
// ---------------------------------------------------------------------------
// �N�C�@�ӵ��ŦX�̲צ�m���d�U��.
void __fastcall CHighlight::KeepRangePos(void)
{
	// �C�@�ӵ����n�B�z
	for(int i=0; i<SearchEngine->SearchWordList->Count; i++)
	{
		// �C�@�ӵ����C�@�ӽd�򳣭n���

		int jj = FoundPos[i]->Int2s->Count-1;	// �ѫ᭱��, �R���~���|�}�a�ǦC
		for(int j=jj; j>0; j--)					// j=0 ���B�z, �]���Ĥ@�լO�ܽd, �@�w�n�d��.
		{
			bool bInRange = false;	// �P�_���S���b�d�򤧤�
			TPoint * tpPtr = (TPoint *) FoundPos[i]->Int2s->Items[j];
			int x = tpPtr->x;

			// �P�_���S���b���G���d�򤧤�
			for(int k=0; k<PostfixStack->QueryStack[0]->Int2s->Count; k++)
			{
				TPoint * tpPtr1 = (TPoint *) PostfixStack->QueryStack[0]->Int2s->Items[k];
				if(x >= tpPtr1->x && x <=  tpPtr1->y)
				{
					// ���F�N���X��
					bInRange = true;
					break;
				}
			}

			if(!bInRange)	// �S���, �ҥH�n�R��
			{
				FoundPos[i]->Int2s->Delete(j);
			}
		}
	}
}
//---------------------------------------------------------------------------
// ��X���r��U�@�r������, �~�r�^��Ʀr�� 1 , [��*��] �� 5, &#x1234; �� 8
int __fastcall CHighlight::Get1PattenLen(const wchar_t * pData)
{
	if(*pData == 0) return 0;

	if(*pData >= 0xD800 && *pData <= 0xDFFF)
	{
		return 2;	// ext-b �H�W������r, �Ǧ^ 2
	}
	else if((*pData == u'&') && (*(pData+1) == u'#'))	// &#.....;
	{
		int iFind = 0;					// ��� ; ����m

		for(int i=0; i<50; i++)		// &#x.....; ���������Ӥ��|�W�L 50
		{
			if(*(pData+i) == 0) break;
			if(*(pData+i) == u';')
			{
				iFind = i+1;
				break;
			}
		}

		if(iFind > 0) return iFind;		// �G�M�O &#.....;
		else return 1;					// �䤣�쵲��, �ҥH�Ǧ^ 1
	}
	else if(*pData == u'[')	// �i��O�զr���F
	{
		int iFind = 0;					// ��� ; ����m

		for(int i=1; i<100; i++)		// [?@((�F-��+��)/((�F-��+��)*(�F-��+��)))] ���������Ӥ��|�W�L 100
		{
			if(*(pData+i) == 0) break;
			else if(*(pData+i) == u'[')  // [ �̭��� [ , �ҥH�i��O�o�ح׭q�����ʦr : [<span class="gaiji">&#X4F39;<!--gaiji,,1[��-�G+�B],2&#X4F39;,3--></span>&gt;��]
			{
				iFind = 0;
				break;
			}
			else if(*(pData+i) == u']')
			{
				iFind = i+1;
				break;
			}
		}

		if(iFind > 0) return iFind;		// �G�M�O�զr��
		else return 1;					// �䤣�쵲��, �ҥH�Ǧ^ 1
	}
	else
		return 1;		// �@��~�r�B�^��r�μƦr
}
// ---------------------------------------------------------------------------
// �ª�, ���ΤF, �]�����|���r�L�k���Q���P�s��
// ��ڶ��
String __fastcall CHighlight::MakeHighlight_old()
{
/*
	�s���޿�

	1.�}�ҼȦs�Ŷ�
	2.�L�o�аO�μаO, ����Ĥ@�Ӧr�X�{
	3.�P�_�O���O�U�@�ӭn�ܦ⪺�a�� (HighlightPos)
	4.�O�N���
*/

	vector< wchar_t > vOutput;

	HighlightPos.Begin = 0;		// highlight ���_�I
	HighlightPos.End = 0;		// highlight �����I
	HighlightPos.Num = 0;		// �ĴX�ӵ�
	HighlightPos.Index = 0;		// �ӵ����ĴX���X�{
	HighlightPos.Next = 0 ;		// �U�@�ӵ����s��, �q�`�O Index + 1 , �Y�O�̫�@��, �h�O 0

	wchar_t * pPoint = HTMLSource->FirstChar();		// �C���b HTMLSource ������

	GetNextHighlightPos(true);	// ���o�U�@�ӭn�ܦ⪺����������m, �ǤJtrue �O��Ȥ�

	// �U�@�ӭn��⪺�u��_�l��m (�������аO�B���I)
	pair<wchar_t *, int> prReal = PosToReal[HighlightPos.Begin];

	while(*pPoint)
	{
		// �����n���
		if(prReal.first == pPoint)
		{
			// ����W�s�������� <a name="x_y"></a> // �� x ��, �� y ���X�{
			AddSearchHeadName(&vOutput, HighlightPos.Num, HighlightPos.Index);

			// �v�r�B�z
			for(int i=HighlightPos.Begin; i<=HighlightPos.End; i++)
			{
				prReal = PosToReal[i];  // �ثe�������U�@�ӭn��⪺�r

				// �p�G���r�e�٦��аO�B���I, �N�����L�h
				while(pPoint < prReal.first)
				{
					vOutput.push_back(*pPoint);
					pPoint++;
                }

				// <a href="#0_1" class="SearchWord1">�v</a>   // �Ĥ@����n�Ӧr�s��ĤG���X�{
				// ....
				// <a href="#1_1" class="SearchWord2">��</a>    // �ĤG����n�Ӧr�s��ĤG���X�{

				// �B�z���r
				// 1.�[�W <a name="x_y class="SearchWordx"> �аO
				AddSearchNameTag(&vOutput, HighlightPos.Num, HighlightPos.Next);
				// 2.�[�W���r
				for(int len=0; len<prReal.second; len++)
				{
					vOutput.push_back(*pPoint);
					pPoint++;
				}
				// 3.�[�W </a>
				vOutput.push_back(L'<');
				vOutput.push_back(L'/');
				vOutput.push_back(L'a');
				vOutput.push_back(L'>');
			}

			GetNextHighlightPos();	// ���o�U�@�ӭn�ܦ⪺����������m
			prReal = PosToReal[HighlightPos.Begin];
		}
		else
		{
			vOutput.push_back(*pPoint);
			pPoint++;
        }
	}

	return String(&vOutput[0],vOutput.size());
}
// ---------------------------------------------------------------------------
// (�ª�, ���ΤF, �]�����|���r�L�k���Q���P�s��)
	// �Ҧp : �n�`�N�T�ر��p
	// 1. �٧Q + �٧Q�l
	// �o�ر��p�O�W�L�@�եH�W���P�@�Ӱ_�I, �n��̪������@�դ���n.
	// 2. �����t + ���t
	// �o�ر��p�O��Ĥ@�դ���, �ĤG�ժ� x ���M�p��Ĥ@��, �������,
	// �n�T�w�ĤG�ժ� x �j��Ĥ@�ժ� y �~���.
	// 3. �����t + ���t�g
	// �p�G���ɸg��O "�����t�g" , ��F "�����t" , �ĤG�եu��� "�g" �F.

	// ???? �S�����, �W�@�Ӧp��s��o�@�ӵ�?

// ���o�U�@�ӭn�ܦ⪺����������m, ����ǤJ�ȳ���ܪ�Ȥ�
void __fastcall CHighlight::GetNextHighlightPos(bool bInitial)
{
/*
	struct Smy_HighlightPos {
		int Begin;	// highlight ���_�I
		int End;	// highlight �����I
		int Num;	// �ĴX�ӵ�
		int Index;	// �ӵ����ĴX���X�{
		int Next;	// �U�@�ӵ����s��, �q�`�O Index + 1 , �Y�O�̫�@��, �h�O 0
	} HighlightPos;
*/
	if(bInitial)	// �i���Ȥ�
	{
		if(FoundPosIndex) delete[] FoundPosIndex;
		FoundPosIndex = new int[SearchEngine->SearchWordList->Count];
		for(int i=0; i<SearchEngine->SearchWordList->Count; i++)
		{
			*(FoundPosIndex+i) = 0;
		}
	}

	int iMin = 99999999;

	int iIndex = -1;

	int iPtrY;  // �o�O�n�Ȧs�������Ӧr
	// �Ҧp�d�� (�٧Q - �٧Q�l) + �٧Q��
	// ���F�קK "�٧Q��" �u��� "�٧Q" �G�r, �ҥH�n��N��̪����r��
	// �]���n�O�� iPtrY
	// ???? ��ڽд��լݬݩ��U�G�̶��O�_ OK?
	// (�٧Q - �٧Q�l) + �٧Q�� vs �٧Q�� + (�٧Q - �٧Q�l)

	// ???? �i��̫�@�� "�٧Q" ���ӭn��b "�٧Q��" , �o�S����, �y���̫�@�� "�٧Q"
	// �L�k�s�^�̫e��, �o�ӦA�ݯण��B�z????

	// ��X�Ҧ���C���̤p�����@��.
	for(int i=0; i<SearchEngine->SearchWordList->Count; i++)
	{
		if(*(FoundPosIndex+i) < FoundPos[i]->Int2s->Count)	// �٨S�W�L�~����
		{
			// i ��ܬO�ĴX�ӵ�
			// *(FoundPosIndex+i) �O�ثe�w�ϥΨ쪺����, �� 0 �}�l
			TPoint * tpPtr = (TPoint *) FoundPos[i]->Int2s->Items[*(FoundPosIndex+i)];
			if(tpPtr->x < iMin)
			{
				iIndex = i;
				iMin = tpPtr->x;
				iPtrY = tpPtr->y;
			}
		}
	}

	if(iIndex != -1)		// ��ܦ����
	{
		// ���F�ȭ���, �ҥH�C�@�ӳ��վ�
		for(int i=0; i<SearchEngine->SearchWordList->Count; i++)
		{
			if(*(FoundPosIndex+i) < FoundPos[i]->Int2s->Count)	// �٨S�W�L�~����
			{
				// i ��ܬO�ĴX�ӵ�
				// *(FoundPosIndex+i) �O�ثe�w�ϥΨ쪺����, �� 0 �}�l
				TPoint * tpPtr = (TPoint *) FoundPos[i]->Int2s->Items[*(FoundPosIndex+i)];
				if(tpPtr->x == iMin)
				{
					if(tpPtr->y >= iPtrY)	// �n��o�Ӥj�~�n�O��
					{
						iPtrY = tpPtr->y;
						HighlightPos.Begin = tpPtr->x;
						HighlightPos.End = tpPtr->y;
						HighlightPos.Num = i;	// �ĴX�ӵ�
						HighlightPos.Index = *(FoundPosIndex+i);	// �ӵ����ĴX���X�{
						HighlightPos.Next = HighlightPos.Index + 1; // �U�@�ӵ����s��, �q�`�O Index + 1 , �Y�O�̫�@��, �h�O 0
						if(HighlightPos.Next == FoundPos[i]->Int2s->Count)	HighlightPos.Next = 0;
					}
					*(FoundPosIndex+i) = *(FoundPosIndex+i)+1;		// �ӵ������ްh��@��
				}
			}
		}
	}
	else
	{
		// �䤣��F
		HighlightPos.Begin = 0;
		HighlightPos.End = 0;
		HighlightPos.Num = 0;	// �ĴX�ӵ�
		HighlightPos.Index = 0;	// �ӵ����ĴX���X�{
	}
}
// ---------------------------------------------------------------------------
// ���Y�ӭn��⪺��, �[�W name ���аO���H�s��, ���X�O�� iNum ���� iTime ���X�{
// <a name="iNum_iTime"></a>
void __fastcall CHighlight::AddSearchHeadName(vector<wchar_t> * vOutput, int iNum, int iTime)
{
	String sTag = u"<a name=\"" + String(iNum) + u"_" + String(iTime) + u"\"></a>";
	wchar_t * wc = sTag.FirstChar();
	while(*wc)
	{
		vOutput->push_back(*wc);
		wc++;
	}
}
// ---------------------------------------------------------------------------
// �[�W <a href="#x_y" class="SearchWordx"> �аO
void __fastcall CHighlight::AddSearchNameTag(vector<wchar_t> * vOutput, int iNum, int iTime)
{
	int iMod = iNum % 5;	// �@�������C��, ���ۨϥ�
	String sTag = u"<a href=\"#" + String(iNum) + u"_" + String(iTime)
		+ u"\" class=\"SearchWord" + String(iMod) + "\">";
	wchar_t * wc = sTag.FirstChar();
	while(*wc)
	{
		vOutput->push_back(*wc);
		wc++;
	}
}
// ---------------------------------------------------------------------------
// �s��, �i�H�B�z���|���r�����P�s��
// ��ڶ��
String __fastcall CHighlight::MakeHighlight()
{
/*
	�s���޿�

	1.�}�ҼȦs�Ŷ�
	2.�v�r�B�z
	3.�Y���r���w��, �N�B�z�w��, ���s���B���, �]�@�@�B�z
*/

	vector< wchar_t > vOutput;
	wchar_t * pPoint = HTMLSource->FirstChar();		// �C���b HTMLSource ������

	while(*pPoint)
	{
		// �P�_�n���n�w��
		if(mpWordAnchor.find(pPoint) != mpWordAnchor.end())
		{
			// �w��, �᭱�� vector , �T�դ@�ӳ��, ���O�O �� x ��, �� y ���X�{, �U�@���X�{ (�̫�h�^�� 0)
			// map <wchar_t *, vector<int>> mpWordAnchor;
			AddWordAnchor(&vOutput, pPoint);
		}

		// �P�_�n���n���P�s��
		if(mpWordLink.find(pPoint) != mpWordLink.end())
		{
			// �s��, �C�Ӧr�u���@��,
			// map <wchar_t *, pair<int,int>> mpWordLink;
			// ���, �C�Ӧr���i��n�X���C��
			// map <wchar_t *, vector<int>> mpWordClass;
			AddWordLink(&vOutput, pPoint);

			// �N���r��W�h

			// �C�@�Ӧr������, �Ҧp�զr���N�|�����
			int iLen = mpWordLength[pPoint];

			for(int i=0; i<iLen; i++)
			{
				vOutput.push_back(*pPoint);
				pPoint++;
            }

			// ����

			vOutput.push_back(L'<');
			vOutput.push_back(L'/');
			vOutput.push_back(L'a');
			vOutput.push_back(L'>');
		}
		else
		{
			vOutput.push_back(*pPoint);
			pPoint++;
		}
	}

	return String(&vOutput[0],vOutput.size());
}
// ---------------------------------------------------------------------------
// �Y�ӭn��⪺��, �[�W�w�� name ���аO���H�s��, ���X�O�� iNum ���� iTime ���X�{
// �ïd�U�U�ӳs������m, ���ݩʥi�d�� javascript ����
// <a name="Search_iNum_iTime" href="#Search_iNum_iNext"></a>
// �ǤJ�� vector �T�դ@�ӳ��, ���O�O �� x ��, �� y ���X�{, �U�@���X�{ (�̫�h�^�� 0)
void __fastcall CHighlight::AddWordAnchor(vector<wchar_t> * vOutput, wchar_t * pPoint)
{
	vector<int> * vData = &(mpWordAnchor[pPoint]);
	for(int i=0; i<vData->size(); i+=3)
	{
		String sTag = u"<a name=\"Search_" + String((*vData)[i]) + u"_" +
			String((*vData)[i+1]) + u"\" href=\"Search_" + String((*vData)[i]) +
			u"_" + String((*vData)[i+2]) + "\"></a>";
		wchar_t * wc = sTag.FirstChar();
		while(*wc)
		{
			vOutput->push_back(*wc);
			wc++;
		}
	}
}
// ---------------------------------------------------------------------------
// �[�W�s���ζ�⪺ class , �Ҧp:
// <a href="#Search_x_y" class="SearchWord1 SearchWord2 SearchWord3...">
// �s��, �C�Ӧr�u���@��,
// map <wchar_t *, pair<int,int>> mpWordLink;
// ���, �C�Ӧr���i��n�X���C��
// map <wchar_t *, vector<int>> mpWordClass;
void __fastcall CHighlight::AddWordLink(vector<wchar_t> * vOutput, wchar_t * pPoint)
{
	int iNum = mpWordLink[pPoint].first;    // �ĴX�ӵ���
	int iTime = mpWordLink[pPoint].second;  // �s����ĴX���X�{

	// ���]�w�s�� <a href="

	String sTag = u"<a href=\"#Search_" + String(iNum) + u"_" + String(iTime)
		+ u"\" class=\"";

	// �]�w class

	vector<int> * vClass = &(mpWordClass[pPoint]);

	for(int i=0; i<vClass->size(); i++)
	{
		int iMod = (*vClass)[i];

		if(i) sTag += u" ";   // �Ĥ@�Ӥ��ΪŮ�
		sTag = sTag + u"SearchWord" + String(iMod);
	}

	sTag = sTag + u"\">";

	wchar_t * wc = sTag.FirstChar();
	while(*wc)
	{
		vOutput->push_back(*wc);
		wc++;
	}
}
// ---------------------------------------------------------------------------
// ��C�@�Ӧr�n�w��B���P�s������Ƴ��O���U��, �]�A���|���r
// �Ҧp�Ĥ@�յ� AB , �ĤG�յ� BC , ����O "ABC"
// �h A: �O���w�� <a name="0_0" href="#0_1"></a> ,�o�̪� href �i�H�d���{����U�@�ӳs��
//    A: �O�� class word0 , �s���U�@���I.
//    B: �O�� class word0 , �s���U�@���I.
// �ĤG����
//    B: �O���w�� <a name="1_0" href="#1_1"></a>
//    B: �O�� class word1 (���� class) , �]���w���s��, �N���]�w�s���F.
//    C: �O�� class word0 , �s���U�@���I.

// ���k : �Ĥ@�� (ABC... ���O RealPos �u���m)
// map �w��[A] => 0_0
// map �s��[A] => 0_1
// map �s��[B] => 0_1
// map ���[A] => word0
// map ���[B] => word0
// �ĤG��
// map �w��[B] => 1_0
// map �s��[B] => 0_1
// map �s��[C] => 0_1
// map ���[B] => word1
// map ���[C] => word1

// ��@
	// �o�T�ӬO�n���Ϊ����n���, �O���C�@�Ӧr�����F���ǩw��, �s��, ���

	// �w��, �᭱�� vector , �G�դ@�ӳ��, ���O�O �� x ��, �� y ���X�{
	// map <wchar_t *, vector<int>> mpWordAnchor;

	// �s��, �C�Ӧr�u���@��,
	// map <wchar_t *, pair<int,int>> mpWordLink;

	// ���, �C�Ӧr���i��n�X���C��
	// map <wchar_t *, vector<int>> mpWordClass;

	// �C�@�Ӧr������, �Ҧp�զr���N�|�����
	// map <wchar_t *, int> mpWordLength;

// �O���C�@�r���w��B�s���B���
void __fastcall CHighlight::GetEveryWordInfo(void)
{
	mpWordAnchor.clear();
	mpWordLink.clear();
	mpWordClass.clear();
	mpWordLength.clear();

	// �C�@�ӵ��̦��B�z
	for(int i=0; i<SearchEngine->SearchWordList->Count; i++)
	{
		// �C�@�ӵ��Ҧ��X�{���a��
		for(int j=0; j<FoundPos[i]->Int2s->Count; j++)
		{
			// i �O�� n �ӵ�, j �O�X�{�� j ��
			TPoint * tpPtr = (TPoint *) FoundPos[i]->Int2s->Items[j];

			// 1. �@�ӵ��X�{, ���w��, ��X�����Ĥ@�Ӧr���u���m
			wchar_t * wc = PosToReal[tpPtr->x].first;

			int iNext = j+1;    // �o�O�U�@���X�{������, �Y�U�@�ӬO�̫�, �N�^�� 0
			if(iNext == FoundPos[i]->Int2s->Count) iNext = 0;

			mpWordAnchor[wc].push_back(i);	// �����J�� i �ӵ�
			mpWordAnchor[wc].push_back(j);  // �A���J�� j ���X�{
			mpWordAnchor[wc].push_back(iNext);  // �A���J�� j ���X�{

			int iMod = i % 5;   // ��⪺���� class = "SearchWord x" ������ x

			// �C���X�{�������ܵ���
			for(int k=tpPtr->x; k<=tpPtr->y; k++)
			{
				// �����C�Ӧr���u���m
				pair<wchar_t *, int> word = PosToReal[k];
				wc = word.first;

				// 2. �C�Ӧr���n�O���U�@�ӳs��, �o�u��O���@��
				if(mpWordLink.find(wc) == mpWordLink.end())
				{
					mpWordLink[wc] = make_pair(i,iNext);
                }

				// 3. �C�Ӧr���n�O����⪺�y����
				mpWordClass[wc].push_back(iMod);

				// 4. �O�����r������ (�զr���N������F)
				mpWordLength[wc] = word.second;
            }
		}
	}
}
// ---------------------------------------------------------------------------
