//---------------------------------------------------------------------------

#pragma hdrstop

#include "highlight.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

// ---------------------------------------------------------------------------
// 建構式
__fastcall CHighlight::CHighlight(TmyMonster * seSearchEngine)
{
	SearchEngine = seSearchEngine;
	HTMLSource = 0;
	PostfixStack = new TmyPostfixStack;
	FoundPos = 0;					// 儲存找到的位置
	FoundPosIndex = 0;
	PosToReal.clear();
	PosToReal[0] = make_pair((wchar_t *)0,0);   // 如果傳入 0 , 也傳回 0, 這在最後一筆塗色之後會用到
}
// ---------------------------------------------------------------------------
// 解構函式
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
// 塗色
String __fastcall CHighlight::AddHighlight(String * sSource)
{
	HTMLSource = sSource;

	GetAllFoundPos();		// 將所有的詞的範圍都找出來
	FindTheRange();			// 將合乎要求的範圍找出來 (經過布林運算了)

	if(SearchEngine->OKSentence.Pos0("*")>=0 ||
	   SearchEngine->OKSentence.Pos0("+")>=0 ||
	   SearchEngine->OKSentence.Pos0("-")>=0)
	{
		KeepRangePos();			// 將每一個詞符合最終位置的留下來.
	}

	// 把每一個字要定位、塗色與連結的資料都記錄下來, 包括重疊的字
	GetEveryWordInfo();
	return MakeHighlight();
}

// ---------------------------------------------------------------------------
// 將所有的詞的位置先找到
void __fastcall CHighlight::GetAllFoundPos(void)
{
	// 搜尋字串中 "詞" 的數量, 例如 菩薩 * 羅漢 = 2 個詞
	FoundPosCount = SearchEngine->SearchWordList->Count;

	// 每一個詞都要有一個記錄位置的空間, 這是一個二維陣列
	FoundPos = new TmyInt2List*[FoundPosCount];

	for(int i=0; i<FoundPosCount; i++)
	{
		FoundPos[i] = new TmyInt2List;		// 宣告一個空間
		GetOneFoundPos(i);					// 取得每一個詞的位置
	}
}
// ---------------------------------------------------------------------------
// 將某個詞的位置先找到
void __fastcall CHighlight::GetOneFoundPos(int iNum)
{
/*
	1.過濾標記及標記, 直到第一個字出現
	2.判斷是不是目前要找的字.
	3.是就記錄, 並繼續找
	4.不是就記錄, 並繼續找

	matching = 0;		// 是否在比對中
	pos = 0				// 字數
	pFindWord = Tag;	// Tag 是要比對的字串

	while(*point)
	{
		// 移除標記及標點

		wiile(*point)
		{
			if (tag) skip tag and continue;
			if (sign) skip sign and continue;
			break;
		}

		pos++;

		if(*point == *pFindWord)	// 找到字
		{
			if(!matching)
			{
				matching = 1;
				postmp = pos;
				pointtmp = point;
			}
			point++;
			pFindWord++;

			if(!pFindWord)	// 找到並結束
			{
				postmp is the answer;
				matching = 0;
				pFindWord = Tag;
			}
		}
		else		// 沒找到
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
	bool bMatching = false;		// 判斷是否在比對中
	int iPos = -1;		// 去掉標記及標點之後的字數, 第一個字由 0 開始 (初值為0是因為一開始還可能不是字, 是標記或標點)
	int iPosTmp;
	wchar_t * pPoint = HTMLSource->FirstChar();		// 遊走在 HTMLSource 的指標
	wchar_t * pPointTmp;
	wchar_t * pFindWord = sWord.c_str();		// 要塗色的字

	// 例如搜尋 "雜阿含 + 阿含" , 這個字串會出現在 HTML 最上方.
	// 第二組的 "阿含" 必須在第一組之後, 以免重疊
	// 例如雜阿含的位置是 (1~3) , 所以 "阿含" 的 iPos 必須 > 3
	// 所以要先找出上一組第一筆的結束位置

	int iAfterThisPos = 0;
	if(iNum > 0)
	{
		TPoint * tpPtr = (TPoint*) FoundPos[iNum-1]->Int2s->Items[0];
		iAfterThisPos = tpPtr->y;
    }

	// 先移至 <!-- search head --> 的地方

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
        ShowMessage(u"沒找到指定標記，程式要再檢查處理");
    }

	while(*pPoint)
	{
		// 移除標記及標點
		int iTagNum = 0;
		bool bShowError = false;    // 有沒有錯誤訊息? 如果有就設為 true, 就不用一直秀
		while(*pPoint)
		{
			// 特殊狀況, 遇到行首標記

			if (wcsncmp(pPoint, L"<span class=\"linehead\">", 23) == 0)
			{
                // <span class="linehead">GA009n0008_p0003a01║</span>
            	// <span class="linehead">ZS01n0001_p0001a01║</span>
            	// <span class="linehead">ZW01n0001_p0001a01║</span>
                // <span class="linehead">A001n0001_p0001a01║</span>
				// <span class="linehead">T30n1579_p0279a10║</span>
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

			// 1.遇到標記, 要額外處理

			if(*pPoint == u'<')
			{
				iTagNum++;		// 因為標記有巢狀, 所以要計數
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

			// 2.忽略標點符號

			if(CWordTool::GetWordType(*pPoint) == CWordTool::wtPunc)
			{
				pPoint++;
				continue;
			}

			// 3.忽略空白

			if(CWordTool::GetWordType(*pPoint) == CWordTool::wtSpace)
			{
				pPoint++;
				continue;
			}

			// 4.星號 [＊]
			if (wcsncmp(pPoint, L"[＊]", 3) == 0)
			{
				pPoint += 3;
				continue;
			}

			// 5.純數字 [01] , 這在卍續藏出現
			if(*pPoint == u'[')
			{
				wchar_t * pTmp = pPoint;
				pPoint++;

				if (wcsncmp(pPoint, L"P.", 2) == 0)  // 南傳有 [P.nn] 的 PTS 頁碼
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

			// 6.校勘數字 [<a id="a0001005" ....<..巢狀..>. >5</a>]
			// ???? 這個新版的會改
			if(*pPoint == u'[')
			{
				wchar_t * pTmp = pPoint;
				pPoint++;

				if(*pPoint == u'<')
				{
                    // 有巢狀的標記, 不能這樣用
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
                        // 卍續藏校注有 [科XX] or [標XX] or [解XX]
						//if (wcsncmp(pPoint, u'科', 1) == 0)
						if(*pPoint == u'科')
						{
							pPoint += 1;
						}
						//else if (wcsncmp(pPoint, L'標', 1) == 0)
						else if(*pPoint == u'標')
						{
							pPoint += 1;
						}
						//else if (wcsncmp(pPoint, L'解', 1) == 0)
						else if(*pPoint == u'解')
						{
							pPoint += 1;
						}

						while(*pPoint >= u'0' && *pPoint <= u'9') pPoint++;

						if (*pPoint == u'＊')
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

			// 表示都沒有要過濾的了
			break;
		}
		if(!*pPoint) break;		// 如果結束就跳出迴圈

		iPos++;					// 又找到一個真正的字了
		// 傳回下一個字的長度, 可能是ext-b中文=2, 組字>2, 中英數符號1

		int i1stWordLen;	// 第一個字的長度, 要記起來
		int iThisWordLen = Get1PattenLen(pPoint);

		if (*pFindWord == u'?')		// 萬用字元
		{
			PosToReal[iPos] = make_pair(pPoint, iThisWordLen);
			// 這時 bMarching 一定是 true
			pPoint += iThisWordLen;
			pFindWord++;

		}
		else if(iPos>iAfterThisPos && wcsncmp(pPoint,pFindWord,iThisWordLen)==0)	// 找到
		{
			PosToReal[iPos] = make_pair(pPoint, iThisWordLen);

			if(!bMatching)				// 目前是第一個字
			{
				i1stWordLen = iThisWordLen;
				bMatching = true;
				iPosTmp = iPos;			// 目前位置記起來
				pPointTmp = pPoint;		// 目前位置記起來
			}

			pPoint += iThisWordLen;
			pFindWord += iThisWordLen;

			if(*pFindWord == 0)		// 比對完全結束, 恭禧!!!
			{
				bMatching = false;
				FoundPos[iNum]->Add(iPosTmp,iPos);  // 將找到的位置存起來 (頭, 尾)
				pFindWord = sWord.c_str();		// 要塗色的字
			}
		}
		else	// 找不到
		{
            bool bFound = false;
            // 沒找到, 但它可能是缺字, 所以要檢查後面是不是有 <!--gaiji,缽,1[金*本],2&#Xxxxx;,3-->
			if(wcsncmp(pPoint+iThisWordLen,L"<!--gaiji,",10)==0)	// 後面是缺字
			{
				/* ???? 缺字待處理
				wchar_t * pLast = pPoint+iThisWordLen;     // 要找最後結尾的指標
                for(int i=0; i<500; i++)                // 最多 500 個字, 這不太可能了, 只是避免用 while 陷入無窮迴圈
                {
                    if(*pLast == '>') break;
                    pLast++;
                }
                AnsiString sGaiji = AnsiString(pPoint+iThisWordLen , pLast +1 - pPoint-iThisWordLen);   // 取出 "<!--gaiji,缽,1[金*本],2&#Xxxxx;,3-->"
                int iPos1 = sGaiji.AnsiPos(",");
                int iPos2 = sGaiji.AnsiPos(",1");
				int iPos3 = sGaiji.AnsiPos(",2");
                int iPos4 = sGaiji.AnsiPos(",3");

                AnsiString sNor = sGaiji.SubString(iPos1+1,iPos2-iPos1-1);    // 取出通用字
                AnsiString sDes = sGaiji.SubString(iPos2+2,iPos3-iPos2-2);    // 取出組字式
                AnsiString sUni = sGaiji.SubString(iPos3+2,iPos4-iPos3-2);    // 取出 unicode

				if(sNor.Length()>0 && wcsncmp(sNor.c_str(),pFindWord,sNor.Length())==0)	// 和通用字相同
                {
                    if(!bMatching)				// 目前是第一個字
        			{
		        		i1stWordLen = iThisWordLen;
				        bMatching = true;
        				iPosTmp = iPos;			// 目前位置記起來
        				pPointTmp = pPoint;		// 目前位置記起來
        			}

		        	pPoint += iThisWordLen;
        			pFindWord = pFindWord + sNor.Length();

			        if(*pFindWord == 0)		// 比對完全結束, 恭禧!!!
        			{
        				bMatching = false;
        				FoundPos[iNum]->Add(iPosTmp,iPos);
        				pFindWord = sWord.c_str();		// 要塗色的字
        			}
                    bFound = true;
                }
                if(sDes.Length()>0 && wcsncmp(sDes.c_str(),pFindWord,sDes.Length())==0)	// 和組字式相同
                {
                    if(!bMatching)				// 目前是第一個字
        			{
		        		i1stWordLen = iThisWordLen;
				        bMatching = true;
        				iPosTmp = iPos;			// 目前位置記起來
        				pPointTmp = pPoint;		// 目前位置記起來
        			}

		        	pPoint += iThisWordLen;
        			pFindWord = pFindWord + sDes.Length();

			        if(*pFindWord == 0)		// 比對完全結束, 恭禧!!!
        			{
        				bMatching = false;
        				FoundPos[iNum]->Add(iPosTmp,iPos);
        				pFindWord = sWord.c_str();		// 要塗色的字
        			}
                    bFound = true;
                }
                if(sUni.Length()>0 && wcsncmp(sUni.c_str(),pFindWord,sUni.Length())==0)	// 和通用字相同
                {
                    if(!bMatching)				// 目前是第一個字
        			{
		        		i1stWordLen = iThisWordLen;
				        bMatching = true;
        				iPosTmp = iPos;			// 目前位置記起來
        				pPointTmp = pPoint;		// 目前位置記起來
        			}

		        	pPoint += iThisWordLen;
        			pFindWord = pFindWord + sUni.Length();

			        if(*pFindWord == 0)		// 比對完全結束, 恭禧!!!
        			{
        				bMatching = false;
						FoundPos[iNum]->Add(iPosTmp,iPos);
        				pFindWord = sWord.c_str();		// 要塗色的字
        			}
                    bFound = true;
				}
				*/
            }

            if(!bFound)     // 依然沒找到, 連缺字都沒有符合
            {
			    if(bMatching)
    			{
    				bMatching = false;
    				pPoint = pPointTmp;
    				iPos = iPosTmp;
    				pFindWord = sWord.c_str();		// 要塗色的字
    				pPoint += i1stWordLen;
    			}
				else pPoint += iThisWordLen;
			}
		}
	}
}
// ---------------------------------------------------------------------------
// 將合乎要求的範圍找出來 (經過布林運算了), 傳回找到的組數
void __fastcall CHighlight::FindTheRange(void)
{
	/*
	###################################
	# 練習計算機運算
	#
	# 原則
	#  如果是數字, 如果有運算符號, 且層數都一樣, 就運算, 結果推入 query stack
	#  如果是數字, 如果有運算符號, 如果層數不一樣, 推入 query stack
	#  如果是數字, 沒有運算符號, 推入 query stack
	#
	#  如果是運算符號, 推入 op stack , 且記錄目前層數
	#
	#  如果是左括號, 目前層數 + 1
	#  如果是右括號, 層數 - 1 , 並且運算
	###################################
	*/

	PostfixStack->Initial();
	String sPatten;
	String sOKSentence = SearchEngine->OKSentence;	// 暫存的字串		// 儲存找到的位置

	// 目前第幾個字串? 例如 (S&S)|S  (佛陀&阿難)|布施  佛陀是第 0 個
	int iPattenNum = 0;

	//while (sOKSentence.Length())
	for(int i=0; i<sOKSentence.Length(); i++)
	{
		sPatten = sOKSentence.SubString0(i,1);		// 取出 patten

		if(sPatten.IsDelimiter0(SearchEngine->OpPatten, 0))		// 如果是運算符號的話
		{
			PostfixStack->PushOp(sPatten);
		}
		else
		{
			// ???? 加速的方法, 如果是 and 就只算前一個有結果的, 如果是 or 就只查前一個是沒找到的
			// ???? 加速的方法, 如果不統計次數, 有找到就算數, 那麼速度會更快

			// 處理一個字

			// 第一組先取出來, 因為第一組只是示範, 不能列入計算  (舊的判斷法)

            // 底下是新方法
			// 不只第一組了... 因為搜尋字串可能是 "舍利 - 舍利弗" , 所以 "舍利" 會在 html 一開始就出現二次
			// 所以判斷法要改成 在最後一組的最後一個字之前出現的, 所有的 "舍利" 都要先取出來

			TPoint * tpPtrLast = (TPoint*) FoundPos[FoundPosCount-1]->Int2s->Items[0]; // html 上方示範字串最後一組的位置, 如   "舍利 ! 舍利弗" , 表示 "舍利弗" 那一組
			int iLastPos = tpPtrLast->y; // "舍利 - 舍利弗" , "弗" 的位置, 此位置之前的都要先移除.

			vector<TPoint *> vSavePtr;  // 要把刪除的記錄先存下來, 最後再存回去
			vSavePtr.clear();

			// 每個詞的第一組一定要移除並記錄起來
			TPoint * tpPtr = (TPoint*) FoundPos[iPattenNum]->Int2s->Items[0];
			FoundPos[iPattenNum]->Int2s->Delete(0);
			vSavePtr.push_back(tpPtr);

			// 再移除重複的
			while (FoundPos[iPattenNum]->Int2s->Count > 0)
			{
				// 再次取出第一組
				tpPtr = (TPoint*) FoundPos[iPattenNum]->Int2s->Items[0];
				// 位置比最後一個詞的第一組的最後一個字還小, 就要移除.
				// 舍利 - 舍利弗 => 比 "弗" 還小的就不要計算, 那不是經文
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

			sPatten = SearchEngine->SearchWordList->Strings[iPattenNum];	// 取出某一筆字串
			PostfixStack->PushQuery(FoundPos[iPattenNum], sPatten);

			// 再把刪掉的各組放回去
			for(int i=vSavePtr.size(); i>0; i--)
			{
                tpPtr = vSavePtr[i-1];
				FoundPos[iPattenNum]->Int2s->Insert(0,tpPtr);
			}

			iPattenNum++;
		}
	}
	// ???? 這是組數, 不是筆數, 例如 佛陀 near 阿難 可能算是 1 組
	//return (PostfixStack->QueryStack[0]->Int2s->Count);
}
// ---------------------------------------------------------------------------
// 將每一個詞符合最終位置的留下來.
void __fastcall CHighlight::KeepRangePos(void)
{
	// 每一個詞都要處理
	for(int i=0; i<SearchEngine->SearchWordList->Count; i++)
	{
		// 每一個詞的每一個範圍都要比較

		int jj = FoundPos[i]->Int2s->Count-1;	// 由後面來, 刪除才不會破壞序列
		for(int j=jj; j>0; j--)					// j=0 不處理, 因為第一組是示範, 一定要留著.
		{
			bool bInRange = false;	// 判斷有沒有在範圍之中
			TPoint * tpPtr = (TPoint *) FoundPos[i]->Int2s->Items[j];
			int x = tpPtr->x;

			// 判斷有沒有在結果的範圍之中
			for(int k=0; k<PostfixStack->QueryStack[0]->Int2s->Count; k++)
			{
				TPoint * tpPtr1 = (TPoint *) PostfixStack->QueryStack[0]->Int2s->Items[k];
				if(x >= tpPtr1->x && x <=  tpPtr1->y)
				{
					// 找到了就跳出來
					bInRange = true;
					break;
				}
			}

			if(!bInRange)	// 沒找到, 所以要刪除
			{
				FoundPos[i]->Int2s->Delete(j);
			}
		}
	}
}
//---------------------------------------------------------------------------
// 找出此字串下一字的長度, 漢字英文數字為 1 , [金*本] 為 5, &#x1234; 為 8
int __fastcall CHighlight::Get1PattenLen(const wchar_t * pData)
{
	if(*pData == 0) return 0;

	if(*pData >= 0xD800 && *pData <= 0xDFFF)
	{
		return 2;	// ext-b 以上的中文字, 傳回 2
	}
	else if((*pData == u'&') && (*(pData+1) == u'#'))	// &#.....;
	{
		int iFind = 0;					// 找到 ; 的位置

		for(int i=0; i<50; i++)		// &#x.....; 的長度應該不會超過 50
		{
			if(*(pData+i) == 0) break;
			if(*(pData+i) == u';')
			{
				iFind = i+1;
				break;
			}
		}

		if(iFind > 0) return iFind;		// 果然是 &#.....;
		else return 1;					// 找不到結尾, 所以傳回 1
	}
	else if(*pData == u'[')	// 可能是組字式了
	{
		int iFind = 0;					// 找到 ; 的位置

		for(int i=1; i<100; i++)		// [?@((乏-之+虫)/((乏-之+虫)*(乏-之+虫)))] 的長度應該不會超過 100
		{
			if(*(pData+i) == 0) break;
			else if(*(pData+i) == u'[')  // [ 裡面有 [ , 所以可能是這種修訂內有缺字 : [<span class="gaiji">&#X4F39;<!--gaiji,,1[仁-二+且],2&#X4F39;,3--></span>&gt;但]
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

		if(iFind > 0) return iFind;		// 果然是組字式
		else return 1;					// 找不到結尾, 所以傳回 1
	}
	else
		return 1;		// 一般漢字、英文字或數字
}
// ---------------------------------------------------------------------------
// 舊版, 不用了, 因為重疊的字無法順利塗色與連結
// 實際塗色
String __fastcall CHighlight::MakeHighlight_old()
{
/*
	新的邏輯

	1.開啟暫存空間
	2.過濾標記及標記, 直到第一個字出現
	3.判斷是不是下一個要變色的地方 (HighlightPos)
	4.是就塗色
*/

	vector< wchar_t > vOutput;

	HighlightPos.Begin = 0;		// highlight 的起點
	HighlightPos.End = 0;		// highlight 的終點
	HighlightPos.Num = 0;		// 第幾個詞
	HighlightPos.Index = 0;		// 該詞的第幾次出現
	HighlightPos.Next = 0 ;		// 下一個詞的編號, 通常是 Index + 1 , 若是最後一筆, 則是 0

	wchar_t * pPoint = HTMLSource->FirstChar();		// 遊走在 HTMLSource 的指標

	GetNextHighlightPos(true);	// 取得下一個要變色的詞的相關位置, 傳入true 是初值化

	// 下一個要塗色的真實起始位置 (不忽略標記、標點)
	pair<wchar_t *, int> prReal = PosToReal[HighlightPos.Begin];

	while(*pPoint)
	{
		// 此詞要塗色
		if(prReal.first == pPoint)
		{
			// 先放上連結的標籤 <a name="x_y"></a> // 第 x 詞, 第 y 次出現
			AddSearchHeadName(&vOutput, HighlightPos.Num, HighlightPos.Index);

			// 逐字處理
			for(int i=HighlightPos.Begin; i<=HighlightPos.End; i++)
			{
				prReal = PosToReal[i];  // 目前此詞的下一個要塗色的字

				// 如果此字前還有標記、標點, 就先略過去
				while(pPoint < prReal.first)
				{
					vOutput.push_back(*pPoint);
					pPoint++;
                }

				// <a href="#0_1" class="SearchWord1">宗</a>   // 第一詞第n個字連到第二次出現
				// ....
				// <a href="#1_1" class="SearchWord2">極</a>    // 第二詞第n個字連到第二次出現

				// 處理此字
				// 1.加上 <a name="x_y class="SearchWordx"> 標記
				AddSearchNameTag(&vOutput, HighlightPos.Num, HighlightPos.Next);
				// 2.加上此字
				for(int len=0; len<prReal.second; len++)
				{
					vOutput.push_back(*pPoint);
					pPoint++;
				}
				// 3.加上 </a>
				vOutput.push_back(L'<');
				vOutput.push_back(L'/');
				vOutput.push_back(L'a');
				vOutput.push_back(L'>');
			}

			GetNextHighlightPos();	// 取得下一個要變色的詞的相關位置
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
// (舊版, 不用了, 因為重疊的字無法順利塗色與連結)
	// 例如 : 要注意三種情況
	// 1. 舍利 + 舍利子
	// 這種情況是超過一組以上有同一個起點, 要塗最長的那一組比較好.
	// 2. 雜阿含 + 阿含
	// 這種情況是塗完第一組之後, 第二組的 x 雖然小於第一組, 但不能塗,
	// 要確定第二組的 x 大於第一組的 y 才能塗.
	// 3. 雜阿含 + 阿含經
	// 如果此時經文是 "雜阿含經" , 塗完了 "雜阿含" , 第二組只能塗 "經" 了.

	// ???? 沒有塗的詞, 上一個如何連到這一個詞?

// 取得下一個要變色的詞的相關位置, 任何傳入值都表示初值化
void __fastcall CHighlight::GetNextHighlightPos(bool bInitial)
{
/*
	struct Smy_HighlightPos {
		int Begin;	// highlight 的起點
		int End;	// highlight 的終點
		int Num;	// 第幾個詞
		int Index;	// 該詞的第幾次出現
		int Next;	// 下一個詞的編號, 通常是 Index + 1 , 若是最後一筆, 則是 0
	} HighlightPos;
*/
	if(bInitial)	// 進行初值化
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

	int iPtrY;  // 這是要暫存結尾那個字
	// 例如查詢 (舍利 - 舍利子) + 舍利弗
	// 為了避免 "舍利弗" 只塗色 "舍利" 二字, 所以要塗就塗最長的字串
	// 因此要記錄 iPtrY
	// ???? 實際請測試看看底下二者塗色是否 OK?
	// (舍利 - 舍利子) + 舍利弗 vs 舍利弗 + (舍利 - 舍利子)

	// ???? 可能最後一個 "舍利" 本來要塗在 "舍利弗" , 卻沒有塗, 造成最後一個 "舍利"
	// 無法連回最前面, 這個再看能不能處理????

	// 找出所有串列中最小的那一個.
	for(int i=0; i<SearchEngine->SearchWordList->Count; i++)
	{
		if(*(FoundPosIndex+i) < FoundPos[i]->Int2s->Count)	// 還沒超過才能比較
		{
			// i 表示是第幾個詞
			// *(FoundPosIndex+i) 是目前已使用到的索引, 由 0 開始
			TPoint * tpPtr = (TPoint *) FoundPos[i]->Int2s->Items[*(FoundPosIndex+i)];
			if(tpPtr->x < iMin)
			{
				iIndex = i;
				iMin = tpPtr->x;
				iPtrY = tpPtr->y;
			}
		}
	}

	if(iIndex != -1)		// 表示有找到
	{
		// 為了怕重覆, 所以每一個都調整
		for(int i=0; i<SearchEngine->SearchWordList->Count; i++)
		{
			if(*(FoundPosIndex+i) < FoundPos[i]->Int2s->Count)	// 還沒超過才能比較
			{
				// i 表示是第幾個詞
				// *(FoundPosIndex+i) 是目前已使用到的索引, 由 0 開始
				TPoint * tpPtr = (TPoint *) FoundPos[i]->Int2s->Items[*(FoundPosIndex+i)];
				if(tpPtr->x == iMin)
				{
					if(tpPtr->y >= iPtrY)	// 要比這個大才要記錄
					{
						iPtrY = tpPtr->y;
						HighlightPos.Begin = tpPtr->x;
						HighlightPos.End = tpPtr->y;
						HighlightPos.Num = i;	// 第幾個詞
						HighlightPos.Index = *(FoundPosIndex+i);	// 該詞的第幾次出現
						HighlightPos.Next = HighlightPos.Index + 1; // 下一個詞的編號, 通常是 Index + 1 , 若是最後一筆, 則是 0
						if(HighlightPos.Next == FoundPos[i]->Int2s->Count)	HighlightPos.Next = 0;
					}
					*(FoundPosIndex+i) = *(FoundPosIndex+i)+1;		// 該詞的索引退後一位
				}
			}
		}
	}
	else
	{
		// 找不到了
		HighlightPos.Begin = 0;
		HighlightPos.End = 0;
		HighlightPos.Num = 0;	// 第幾個詞
		HighlightPos.Index = 0;	// 該詞的第幾次出現
	}
}
// ---------------------------------------------------------------------------
// 找到某個要塗色的詞, 加上 name 的標記讓人連結, 指出是第 iNum 詞第 iTime 次出現
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
// 加上 <a href="#x_y" class="SearchWordx"> 標記
void __fastcall CHighlight::AddSearchNameTag(vector<wchar_t> * vOutput, int iNum, int iTime)
{
	int iMod = iNum % 5;	// 共有五種顏色, 輪著使用
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
// 新版, 可以處理重疊的字的塗色與連結
// 實際塗色
String __fastcall CHighlight::MakeHighlight()
{
/*
	新的邏輯

	1.開啟暫存空間
	2.逐字處理
	3.若此字有定位, 就處理定位, 有連結、塗色, 也一一處理
*/

	vector< wchar_t > vOutput;
	wchar_t * pPoint = HTMLSource->FirstChar();		// 遊走在 HTMLSource 的指標

	while(*pPoint)
	{
		// 判斷要不要定位
		if(mpWordAnchor.find(pPoint) != mpWordAnchor.end())
		{
			// 定位, 後面接 vector , 三組一個單位, 分別是 第 x 組, 第 y 次出現, 下一次出現 (最後則回到 0)
			// map <wchar_t *, vector<int>> mpWordAnchor;
			AddWordAnchor(&vOutput, pPoint);
		}

		// 判斷要不要塗色與連結
		if(mpWordLink.find(pPoint) != mpWordLink.end())
		{
			// 連結, 每個字只有一組,
			// map <wchar_t *, pair<int,int>> mpWordLink;
			// 塗色, 每個字都可能好幾個顏色
			// map <wchar_t *, vector<int>> mpWordClass;
			AddWordLink(&vOutput, pPoint);

			// 將此字填上去

			// 每一個字的長度, 例如組字式就會比較長
			int iLen = mpWordLength[pPoint];

			for(int i=0; i<iLen; i++)
			{
				vOutput.push_back(*pPoint);
				pPoint++;
            }

			// 結束

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
// 某個要塗色的詞, 加上定位 name 的標記讓人連結, 指出是第 iNum 詞第 iTime 次出現
// 並留下下個連結的位置, 此屬性可留給 javascript 應用
// <a name="Search_iNum_iTime" href="#Search_iNum_iNext"></a>
// 傳入的 vector 三組一個單位, 分別是 第 x 組, 第 y 次出現, 下一次出現 (最後則回到 0)
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
// 加上連結及塗色的 class , 例如:
// <a href="#Search_x_y" class="SearchWord1 SearchWord2 SearchWord3...">
// 連結, 每個字只有一組,
// map <wchar_t *, pair<int,int>> mpWordLink;
// 塗色, 每個字都可能好幾個顏色
// map <wchar_t *, vector<int>> mpWordClass;
void __fastcall CHighlight::AddWordLink(vector<wchar_t> * vOutput, wchar_t * pPoint)
{
	int iNum = mpWordLink[pPoint].first;    // 第幾個詞組
	int iTime = mpWordLink[pPoint].second;  // 連結到第幾次出現

	// 先設定連結 <a href="

	String sTag = u"<a href=\"#Search_" + String(iNum) + u"_" + String(iTime)
		+ u"\" class=\"";

	// 設定 class

	vector<int> * vClass = &(mpWordClass[pPoint]);

	for(int i=0; i<vClass->size(); i++)
	{
		int iMod = (*vClass)[i];

		if(i) sTag += u" ";   // 第一個不用空格
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
// 把每一個字要定位、塗色與連結的資料都記錄下來, 包括重疊的字
// 例如第一組詞 AB , 第二組詞 BC , 內文是 "ABC"
// 則 A: 記錄定位 <a name="0_0" href="#0_1"></a> ,這裡的 href 可以留給程式找下一個連結
//    A: 記錄 class word0 , 連結下一個點.
//    B: 記錄 class word0 , 連結下一個點.
// 第二詞的
//    B: 記錄定位 <a name="1_0" href="#1_1"></a>
//    B: 記錄 class word1 (重覆 class) , 因為已有連結, 就不設定連結了.
//    C: 記錄 class word0 , 連結下一個點.

// 做法 : 第一組 (ABC... 其實是 RealPos 真實位置)
// map 定位[A] => 0_0
// map 連結[A] => 0_1
// map 連結[B] => 0_1
// map 塗色[A] => word0
// map 塗色[B] => word0
// 第二組
// map 定位[B] => 1_0
// map 連結[B] => 0_1
// map 連結[C] => 0_1
// map 塗色[B] => word1
// map 塗色[C] => word1

// 實作
	// 這三個是要塗色用的重要資料, 記錄每一個字對應了哪些定位, 連結, 塗色

	// 定位, 後面接 vector , 二組一個單位, 分別是 第 x 組, 第 y 次出現
	// map <wchar_t *, vector<int>> mpWordAnchor;

	// 連結, 每個字只有一組,
	// map <wchar_t *, pair<int,int>> mpWordLink;

	// 塗色, 每個字都可能好幾個顏色
	// map <wchar_t *, vector<int>> mpWordClass;

	// 每一個字的長度, 例如組字式就會比較長
	// map <wchar_t *, int> mpWordLength;

// 記錄每一字的定位、連結、塗色
void __fastcall CHighlight::GetEveryWordInfo(void)
{
	mpWordAnchor.clear();
	mpWordLink.clear();
	mpWordClass.clear();
	mpWordLength.clear();

	// 每一個詞依次處理
	for(int i=0; i<SearchEngine->SearchWordList->Count; i++)
	{
		// 每一個詞所有出現的地方
		for(int j=0; j<FoundPos[i]->Int2s->Count; j++)
		{
			// i 是第 n 個詞, j 是出現第 j 次
			TPoint * tpPtr = (TPoint *) FoundPos[i]->Int2s->Items[j];

			// 1. 一個詞出現, 先定位, 找出此詞第一個字的真實位置
			wchar_t * wc = PosToReal[tpPtr->x].first;

			int iNext = j+1;    // 這是下一次出現的次序, 若下一個是最後, 就回到 0
			if(iNext == FoundPos[i]->Int2s->Count) iNext = 0;

			mpWordAnchor[wc].push_back(i);	// 先推入第 i 個詞
			mpWordAnchor[wc].push_back(j);  // 再推入第 j 次出現
			mpWordAnchor[wc].push_back(iNext);  // 再推入第 j 次出現

			int iMod = i % 5;   // 塗色的順序 class = "SearchWord x" 的那個 x

			// 每次出現的詞首至詞尾
			for(int k=tpPtr->x; k<=tpPtr->y; k++)
			{
				// 此詞每個字的真實位置
				pair<wchar_t *, int> word = PosToReal[k];
				wc = word.first;

				// 2. 每個字都要記錄下一個連結, 這只能記錄一次
				if(mpWordLink.find(wc) == mpWordLink.end())
				{
					mpWordLink[wc] = make_pair(i,iNext);
                }

				// 3. 每個字都要記錄塗色的流水序
				mpWordClass[wc].push_back(iMod);

				// 4. 記錄此字的長度 (組字式就比較長了)
				mpWordLength[wc] = word.second;
            }
		}
	}
}
// ---------------------------------------------------------------------------
