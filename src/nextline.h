//---------------------------------------------------------------------------
#ifndef NextLineH
#define NextLineH
//---------------------------------------------------------------------------
#include <fmx.h>
//---------------------------------------------------------------------------

/*
處理隔行 TT 的物件

呈現結果


T10n0299_p0892c01Z##na　ma.h　sa　ma.m　ta　va　dra　ya　vo
T10n0299_p0892c02_##曩　莫([01]入)　三　滿　多　跋　捺囉(二合引)　野　[冒-目+月](引)
T10n0299_p0892c03_##　dhi　sa　tvaa　ya　ma　haa　sa　tvaa
T10n0299_p0892c04_##　地　薩　怛[口*縛](二合引)　野　麼　賀(引)　薩　怛[口*縛](二合引)
T10n0299_p0892c05_##　ya　ma　haa　kaa　ru　.ni　kaa　ya　ta
T10n0299_p0892c06_##　野　麼　賀(引)　迦(引)　嚕　抳　迦(引)　野　怛
T10n0299_p0892c07_##　dya　thaa　o.m　va　ra　.ti　va　ra
T10n0299_p0892c08_##　[仁-二+爾]也(二合引)　他(引)　唵(引)　婆(引)　囉　胝(引)　婆　囉
T10n0299_p0892c09_##　.ti　va　ra　va　[02]ra　ti　[03]saa　haa
T10n0299_p0892c10_##　胝(引)　婆　囉　婆　冷　帝　薩[口*縛](二合引)　賀(引)

XML

<lb n="0892c01"/><p><tt><t lang="san-sd">&SD-A5A9;</t><t lang="chi">曩</t></tt><tt><t lang="san-sd">&SD-A5F0;</t><t lang="chi"><yin><zi>莫</zi><sg><note n="0892001" resp="Taisho" type="orig" place="foot text">〔入〕－【宋】【元】，入＝引【明】</note><app n="0892001"><lem>入</lem><rdg wit="【宋】【元】" resp="Taisho">&lac;</rdg><rdg wit="【明】" resp="Taisho">引</rdg></app></sg></yin></t></tt><tt><t lang="san-sd">&SD-A67A;</t><t lang="chi">三</t></tt><tt><t lang="san-sd">&SD-A5EF;</t><t lang="chi">滿</t></tt><tt><t lang="san-sd">&SD-A557;</t><t lang="chi">多</t></tt><tt><t lang="san-sd">&SD-A656;</t><t lang="chi">跋</t></tt><tt><t lang="san-sd">&SD-AA76;</t><t lang="chi"><yin><zi>捺囉</zi><sg>二合引</sg></yin></t></tt><tt><t lang="san-sd">&SD-A5F1;</t><t lang="chi">野</t></tt><tt><t lang="san-sd">&SD-A65E;</t><t lang="chi"><yin><zi>&CB04393;</zi><sg>引</sg></yin></t></tt>
<lb n="0892c02"/>
<lb n="0892c03"/><tt><t lang="san-sd">&SD-A57D;</t><t lang="chi">地</t></tt><tt><t lang="san-sd">&SD-A67A;</t><t lang="chi">薩</t></tt><tt><t lang="san-sd">&SD-AF5A;</t><t lang="chi"><yin><zi>怛&CB00085;</zi><sg>二合引</sg></yin></t></tt><tt><t lang="san-sd">&SD-A5F1;</t><t lang="chi">野</t></tt><tt><t lang="san-sd">&SD-A5E5;</t><t lang="chi">麼</t></tt><tt><t lang="san-sd">&SD-A6A9;</t><t lang="chi"><yin><zi>賀</zi><sg>引</sg></yin></t></tt><tt><t lang="san-sd">&SD-A67A;</t><t lang="chi">薩</t></tt><tt><t lang="san-sd">&SD-AF5A;</t><t lang="chi"><yin><zi>怛&CB00085;</zi><sg>二合引</sg></yin></t></tt>
<lb n="0892c04"/>
<lb n="0892c05"/><tt><t lang="san-sd">&SD-A5F1;</t><t lang="chi">野</t></tt><tt><t lang="san-sd">&SD-A67A;</t><t lang="chi">麼</t></tt><tt><t lang="san-sd">&SD-A6A9;</t><t lang="chi"><yin><zi>賀</zi><sg>引</sg></yin></t></tt><tt><t lang="san-sd">&SD-A441;</t><t lang="chi"><yin><zi>迦</zi><sg>引</sg></yin></t></tt><tt><t lang="san-sd">&SD-A642;</t><t lang="chi">嚕</t></tt><tt><t lang="san-sd">&SD-A54D;</t><t lang="chi">抳</t></tt><tt><t lang="san-sd">&SD-A441;</t><t lang="chi"><yin><zi>迦</zi><sg>引</sg></yin></t></tt><tt><t lang="san-sd">&SD-A5F1;</t><t lang="chi">野</t></tt><tt><t lang="san-sd">&SD-A557;</t><t lang="chi">怛</t></tt>
<lb n="0892c06"/>
<lb n="0892c07"/><tt><t lang="san-sd">&SD-DA4A;</t><t lang="chi"><yin><zi>&CB00135;也</zi><sg>二合引</sg></yin></t></tt><tt><t lang="san-sd">&SD-A564;</t><t lang="chi"><yin><zi>他</zi><sg>引</sg></yin></t></tt><tt><t lang="san-sd">&SD-E074;</t><t lang="chi"><yin><zi>唵</zi><sg>引</sg></yin></t></tt><tt><t lang="san-sd">&SD-A656;</t><t lang="chi"><yin><zi>婆</zi><sg>引</sg></yin></t></tt><tt><t lang="san-sd">&SD-A5FD;</t><t lang="chi">囉</t></tt><tt><t lang="san-sd">&SD-A4DC;</t><t lang="chi"><yin><zi>胝</zi><sg>引</sg></yin></t></tt><tt><t lang="san-sd">&SD-A656;</t><t lang="chi">婆</t></tt><tt><t lang="san-sd">&SD-A5FD;</t><t lang="chi">囉</t></tt>
<lb n="0892c08"/>
.......

*/
class CNextLine
{
private:	// User declarations
public:		// User declarations

	bool InNextLine;					// 判斷是否在隔行的 <tt> 中
    bool IsOutput;						// 判斷可以呈現了嗎? 如果有資料在 </tt> 後面, 此時可能還不能輸出, 要一併記錄在第二行裡面
	int TCount;							// 第幾個 <t> 標記, 第一個在本行, 第二個在下一行

	String ThisLine;				// 這一行的內容
	String NextLine;				// 下一行的內容

	void __fastcall FindNextLine(void);		// 遇到隔行對照的 tt 標記
	void __fastcall FindNextLineEnd(void);	// 遇到隔行對照的 tt 結束標記
	void __fastcall Add(String sData);		// 處理隔行對照的資料

	__fastcall CNextLine();		// 建構函式
	__fastcall ~CNextLine();	// 解構函式
};
//---------------------------------------------------------------------------
#endif
