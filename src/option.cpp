//---------------------------------------------------------------------------

#include <fmx.h>
#pragma hdrstop

#include "option.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.fmx"
TfmOption *fmOption;
//---------------------------------------------------------------------------
__fastcall TfmOption::TfmOption(TComponent* Owner)
	: TForm(Owner)
{
    Setting = fmMain->Setting;
}
//---------------------------------------------------------------------------
// 由設定載入
void __fastcall TfmOption::LoadFromSetting()
{
    // 經文格式

	cbShowLineFormat->IsChecked = Setting->ShowLineFormat;
	cbShowLineHead->IsChecked = Setting->ShowLineHead;
	//cbShowJKData->Checked  = Setting->ShowJKData;
	//rgCorrSelect->ItemIndex = Setting->CorrSelect;

	cbShowPunc->IsChecked = Setting->ShowPunc;
	cbNoShowLgPunc->IsChecked = Setting->NoShowLgPunc;

	cbNoShowLgPunc->Enabled = cbShowPunc->IsChecked;      // 若不呈現標點, 就不出現 "偈頌呈現標點與否" 的選項

	cbVerticalMode->IsChecked = Setting->VerticalMode;

	// 校勘格式
	if(Setting->CollationType == ctNoCollation) rbNoCollation->IsChecked = true;
	else if(Setting->CollationType == ctOrigCollation) rbOrigCollation->IsChecked = true;
	else if(Setting->CollationType == ctCBETACollation) rbCBETACollation->IsChecked = true;

	/*
	// 文字顏色大小

    edLineHeight->Text = Setting->LineHeight;

    spBgColor->Brush->Color = Setting->BgColor;
    edBgImage->Text = Setting->BgImage;

    spJuanNumFontColor->Brush->Color =  Setting->JuanNumFontColor;
    edJuanNumFontSize->Text = Setting->JuanNumFontSize;

    spJuanNameFontColor->Brush->Color = Setting->JuanNameFontColor;
    edJuanNameFontSize->Text = Setting->JuanNameFontSize;

    spXuFontColor->Brush->Color = Setting->XuFontColor;
    edXuFontSize->Text = Setting->XuFontSize;

    spBodyFontColor->Brush->Color = Setting->BodyFontColor;
    edBodyFontSize->Text = Setting->BodyFontSize;

    spWFontColor->Brush->Color = Setting->WFontColor;
    edWFontSize->Text = Setting->WFontSize;

    spBylineFontColor->Brush->Color = Setting->BylineFontColor;
    edBylineFontSize->Text = Setting->BylineFontSize;

    spHeadNameFontColor->Brush->Color = Setting->HeadNameFontColor;
    edHeadNameFontSize->Text = Setting->HeadNameFontSize;

    spLineHeadFontColor->Brush->Color = Setting->LineHeadFontColor;
    edLineHeadFontSize->Text = Setting->LineHeadFontSize;

    spLgFontColor->Brush->Color = Setting->LgFontColor;
    edLgFontSize->Text = Setting->LgFontSize;

    spDharaniFontColor->Brush->Color = Setting->DharaniFontColor;
    edDharaniFontSize->Text = Setting->DharaniFontSize;

    spCorrFontColor->Brush->Color = Setting->CorrFontColor;
    edCorrFontSize->Text = Setting->CorrFontSize;

    spGaijiFontColor->Brush->Color = Setting->GaijiFontColor;
    edGaijiFontSize->Text = Setting->GaijiFontSize;

	spNoteFontColor->Brush->Color = Setting->NoteFontColor;
	edNoteFontSize->Text = Setting->NoteFontSize;

	spFootFontColor->Brush->Color = Setting->FootFontColor;
	edFootFontSize->Text = Setting->FootFontSize;

    cbJuanNumBold->Checked = Setting->JuanNumBold;
    cbJuanNameBold->Checked = Setting->JuanNameBold;
    cbXuBold->Checked = Setting->XuBold;
    cbBodyBold->Checked = Setting->BodyBold;
    cbWBold->Checked = Setting->WBold;
    cbBylineBold->Checked = Setting->BylineBold;
    cbHeadNameBold->Checked = Setting->HeadNameBold;
    cbLineHeadBold->Checked = Setting->LineHeadBold;
    cbLgBold->Checked = Setting->LgBold;
    cbDharaniBold->Checked = Setting->DharaniBold;
    cbCorrBold->Checked = Setting->CorrBold;
    cbGaijiBold->Checked = Setting->GaijiBold;
	cbNoteBold->Checked = Setting->NoteBold;
	cbFootBold->Checked = Setting->FootBold;

	cbDharaniFontColor->Checked = Setting->UseDharaniFontColor;
    cbDharaniFontSize->Checked = Setting->UseDharaniFontSize;

    cbCorrFontColor->Checked = Setting->UseCorrFontColor;
    cbCorrFontSize->Checked = Setting->UseCorrFontSize;

    cbGaijiFontColor->Checked = Setting->UseGaijiFontColor;
    cbGaijiFontSize->Checked = Setting->UseGaijiFontSize;

	cbNoteFontColor->Checked = Setting->UseNoteFontColor;
	cbNoteFontSize->Checked = Setting->UseNoteFontSize;

	cbFootFontColor->Checked = Setting->UseFootFontColor;
	cbFootFontSize->Checked = Setting->UseFootFontSize;

    cbCSSFile->Checked = Setting->UseCSSFile;
    edCSSFile->Text = Setting->CSSFileName;
	*/

    // 缺字處理

	cbGaijiUseUniExt->IsChecked = Setting->GaijiUseUniExt;    // 是否使用 Unicode Ext
	cbGaijiUseNormal->IsChecked = Setting->GaijiUseNormal;    // 是否使用通用字

	rbGaijiUniExtFirst->IsChecked = Setting->GaijiUniExtFirst;  // 優先使用 Unicode Ext
	rbGaijiNormalFirst->IsChecked = Setting->GaijiNormalFirst;  // 優先使用 通用字

	rbGaijiDesFirst->IsChecked = Setting->GaijiDesFirst;     // 優先使用組字式
	rbGaijiImageFirst->IsChecked = Setting->GaijiImageFirst;   // 優先使用缺字圖檔

	/*
	// 悉曇字處理法 0:悉曇字型(siddam.ttf) 1:羅馬轉寫(unicode) 2:羅馬轉寫(純文字) 3:悉曇圖檔 4:自訂符號 5:CB碼 6:悉曇羅馬對照
	switch( Setting->ShowSiddamWay)
    {
		case 0: rbUseSiddamFont->Checked = true;
			break;
		case 1: rbUseSiddamRomeU->Checked = true;
			break;
		case 2: rbUseSiddamRome->Checked = true;
			break;
		case 3: rbUseSiddamGif->Checked = true;
        	break;
		case 4: rbUseSiddamSign->Checked = true;
			break;
		case 5: rbUseSiddamCB->Checked = true;
        	break;
		case 6: rbUseSiddamAndRome->Checked = true;
        	break;
    }

	edUserSiddamSign->Text = Setting->UserSiddamSign;		// 使用者自訂悉曇字符號

	// 梵巴字處理法 0:Unicode 1:純文字 2:Ent 碼
	switch( Setting->ShowPaliWay)
    {
		case 0: rbUsePaliUnicode->Checked = true;
			break;
		case 1: rbUsePaliText->Checked = true;
			break;
		case 2: rbUsePaliEnt->Checked = true;
			break;
    }

    // 呈現 Unicode 3.1
    cbShowUnicode30->Checked = Setting->ShowUnicode30;

	// 圖檔大小

	edGaijiWidth->Text = Setting->GaijiWidth;		// 缺字圖檔的寬, 0 為不設限
	edGaijiHeight->Text = Setting->GaijiHeight;		// 缺字圖檔的高
	edSDGifWidth->Text = Setting->SDGifWidth;		// 缺字圖檔的寬, 0 為不設限
	edSDGifHeight->Text = Setting->SDGifHeight;		// 缺字圖檔的高
	//edFigureRate->Text = Setting->FigureRate;		// 圖檔的比例

	// 系統資訊

	edXMLSutraPath->Text = Setting->XMLSutraPath;
    edCBETACDPath->Text = Setting->CBETACDPath;
	edBookmarkFile->Text = Setting->BookmarkFile;
	edSearchPath->Text = Setting->SearchPath;
	edGaijiPath->Text = Setting->GaijiPath;
	edSDGifPath->Text = Setting->SDGifPath;
	edRJGifPath->Text = Setting->RJGifPath;
	edFigurePath->Text = Setting->FigurePath;
	edUserDataPath->Text = Setting->UserDataPath;
    cbSaveLastTocFileName->Checked = Setting->SaveLastTocFileName;

	// 全文檢索

	edNearNum->Text = Setting->NearNum;
	edBeforeNum->Text = Setting->BeforeNum;
	edMaxSearchNum->Text = AnsiString(Setting->MaxSearchNum);
    cbOverSearchWarn->Checked  = Setting->OverSearchWarn;
    cbRMPunctuationWarn->Checked  = Setting->RMPunctuationWarn;

	// 引用複製模式

	switch(Setting->CopyMode)
	{
		case 1 : rbCopyMode1->Checked = true; rbCopyMode3->Checked = true; break;
		case 2 : rbCopyMode2->Checked = true; rbCopyMode3->Checked = true; break;
		case 3 : rbCopyMode1->Checked = true; rbCopyMode4->Checked = true; break;
		case 4 : rbCopyMode2->Checked = true; rbCopyMode4->Checked = true; break;
	}
	cbCBCopyWithJuanNum->Checked = Setting->CBCopyWithJuanNum;
	*/
}

//---------------------------------------------------------------------------
// 由設定載入
void __fastcall TfmOption::SaveToSetting()
{
	// 經文格式

	Setting->ShowLineFormat = cbShowLineFormat->IsChecked;
	Setting->ShowLineHead = cbShowLineHead->IsChecked;
	//Setting->ShowJKData  = cbShowJKData->Checked;
	//Setting->CorrSelect = rgCorrSelect->ItemIndex;

	Setting->ShowPunc = cbShowPunc->IsChecked;
	Setting->NoShowLgPunc = cbNoShowLgPunc->IsChecked;

	Setting->VerticalMode = cbVerticalMode->IsChecked;

	// 校勘格式
	if(rbNoCollation->IsChecked) Setting->CollationType = ctNoCollation;
	else if(rbOrigCollation->IsChecked) Setting->CollationType = ctOrigCollation;
	else if(rbCBETACollation->IsChecked) Setting->CollationType = ctCBETACollation;

    /*
    Setting->BgColor = spBgColor->Brush->Color;
    Setting->BgImage = edBgImage->Text;

    Setting->LineHeight = edLineHeight->Text.ToInt();

    Setting->JuanNumFontColor = spJuanNumFontColor->Brush->Color;
    Setting->JuanNumFontSize = edJuanNumFontSize->Text.ToInt();

    Setting->JuanNameFontColor = spJuanNameFontColor->Brush->Color;
    Setting->JuanNameFontSize = edJuanNameFontSize->Text.ToInt();

    Setting->XuFontColor = spXuFontColor->Brush->Color;
    Setting->XuFontSize = edXuFontSize->Text.ToInt();

    Setting-> BodyFontColor = spBodyFontColor->Brush->Color;
    Setting-> BodyFontSize = edBodyFontSize->Text.ToInt();

    Setting->WFontColor = spWFontColor->Brush->Color;
    Setting->WFontSize = edWFontSize->Text.ToInt();

    Setting->BylineFontColor = spBylineFontColor->Brush->Color;
    Setting->BylineFontSize = edBylineFontSize->Text.ToInt();

	Setting->HeadNameFontColor = spHeadNameFontColor->Brush->Color;
    Setting->HeadNameFontSize = edHeadNameFontSize->Text.ToInt();

    Setting->LineHeadFontColor = spLineHeadFontColor->Brush->Color;
    Setting->LineHeadFontSize = edLineHeadFontSize->Text.ToInt();

    Setting->LgFontColor = spLgFontColor->Brush->Color;
    Setting->LgFontSize = edLgFontSize->Text.ToInt();

    Setting->DharaniFontColor = spDharaniFontColor->Brush->Color;
	Setting->DharaniFontSize = edDharaniFontSize->Text.ToInt();

    Setting->CorrFontColor = spCorrFontColor->Brush->Color;
    Setting->CorrFontSize = edCorrFontSize->Text.ToInt();

    Setting->GaijiFontColor = spGaijiFontColor->Brush->Color;
    Setting->GaijiFontSize = edGaijiFontSize->Text.ToInt();

    Setting->NoteFontColor = spNoteFontColor->Brush->Color;
    Setting->NoteFontSize = edNoteFontSize->Text.ToInt();

	Setting->FootFontColor = spFootFontColor->Brush->Color;
	Setting->FootFontSize = edFootFontSize->Text.ToInt();

    Setting->JuanNumBold = cbJuanNumBold->Checked;
    Setting->JuanNameBold = cbJuanNameBold->Checked;
    Setting->XuBold = cbXuBold->Checked;
    Setting->BodyBold = cbBodyBold->Checked;
    Setting->WBold = cbWBold->Checked;
    Setting->BylineBold = cbBylineBold->Checked;
    Setting->HeadNameBold = cbHeadNameBold->Checked;
    Setting->LineHeadBold = cbLineHeadBold->Checked;
    Setting->LgBold = cbLgBold->Checked;
    Setting->DharaniBold = cbDharaniBold->Checked;
    Setting->CorrBold = cbCorrBold->Checked;
    Setting->GaijiBold = cbGaijiBold->Checked;
    Setting->NoteBold = cbNoteBold->Checked;
    Setting->FootBold = cbFootBold->Checked;

    Setting->UseDharaniFontColor = cbDharaniFontColor->Checked;
    Setting->UseDharaniFontSize = cbDharaniFontSize->Checked;

    Setting->UseCorrFontColor = cbCorrFontColor->Checked;
    Setting->UseCorrFontSize = cbCorrFontSize->Checked;

    Setting->UseGaijiFontColor = cbGaijiFontColor->Checked;
	Setting->UseGaijiFontSize = cbGaijiFontSize->Checked;

	Setting->UseNoteFontColor = cbNoteFontColor->Checked;
	Setting->UseNoteFontSize = cbNoteFontSize->Checked;

	Setting->UseFootFontColor = cbFootFontColor->Checked;
	Setting->UseFootFontSize = cbFootFontSize->Checked;

	Setting->UseCSSFile = cbCSSFile->Checked;
    Setting->CSSFileName = edCSSFile->Text;
	*/

	// 缺字處理

	Setting->GaijiUseUniExt = cbGaijiUseUniExt->IsChecked;    // 是否使用 Unicode Ext
	Setting->GaijiUseNormal = cbGaijiUseNormal->IsChecked;    // 是否使用通用字

	Setting->GaijiUniExtFirst = rbGaijiUniExtFirst->IsChecked;  // 優先使用 Unicode Ext
	Setting->GaijiNormalFirst = rbGaijiNormalFirst->IsChecked;  // 優先使用 通用字

	Setting->GaijiDesFirst = rbGaijiDesFirst->IsChecked;     // 優先使用組字式
	Setting->GaijiImageFirst = rbGaijiImageFirst->IsChecked;   // 優先使用缺字圖檔


	/*
    // 悉曇字處理法 0:悉曇字型(siddam.ttf) 1:羅馬轉寫(unicode) 2:羅馬轉寫(純文字) 3:悉曇圖檔 4:自訂符號 5:CB碼 6:悉曇羅馬對照
	if(rbUseSiddamFont->Checked)        Setting->ShowSiddamWay = 0;
	else if(rbUseSiddamRomeU->Checked)	Setting->ShowSiddamWay = 1;
	else if(rbUseSiddamRome->Checked)	Setting->ShowSiddamWay = 2;
	else if(rbUseSiddamGif->Checked)	Setting->ShowSiddamWay = 3;
	else if(rbUseSiddamSign->Checked)	Setting->ShowSiddamWay = 4;
	else if(rbUseSiddamCB->Checked)		Setting->ShowSiddamWay = 5;
	else if(rbUseSiddamAndRome->Checked)		Setting->ShowSiddamWay = 6;
    Setting->UserSiddamSign = edUserSiddamSign->Text;		// 使用者自訂悉曇字符號

	if(rbUsePaliUnicode->Checked)	Setting->ShowPaliWay = 0;		// 梵巴字處理法 0:Unicode 1:純文字 2:Ent 碼
	else if(rbUsePaliText->Checked)	Setting->ShowPaliWay = 1;
	else if(rbUsePaliEnt->Checked)	Setting->ShowPaliWay = 2;

    Setting->ShowUnicode30 = cbShowUnicode30->Checked;      // 呈現 Unicode 3.1

	// 圖檔大小

	Setting->GaijiWidth = edGaijiWidth->Text.ToIntDef(0);		// 缺字圖檔的寬, 0 為不設限
	Setting->GaijiHeight = edGaijiHeight->Text.ToIntDef(0);		// 缺字圖檔的高
	Setting->SDGifWidth = edSDGifWidth->Text.ToIntDef(0);		// 缺字圖檔的寬, 0 為不設限
	Setting->SDGifHeight = edSDGifHeight->Text.ToIntDef(0);		// 缺字圖檔的高
	//Setting->FigureRate = edFigureRate->Text.ToIntDef(100);		// 圖檔的比例

	// 系統資料

    Setting->XMLSutraPath = edXMLSutraPath->Text;
    Setting->CBETACDPath = edCBETACDPath->Text;
	Setting->BookmarkFile = edBookmarkFile->Text;
	Setting->SearchPath = edSearchPath->Text;
	Setting->GaijiPath = edGaijiPath->Text;
	Setting->SDGifPath = edSDGifPath->Text;
	Setting->RJGifPath = edRJGifPath->Text;
	Setting->FigurePath = edFigurePath->Text;
	Setting->UserDataPath = edUserDataPath->Text;
    Setting->SaveLastTocFileName = cbSaveLastTocFileName->Checked;

	// 全文檢索

	Setting->NearNum = edNearNum->Text.ToInt();
	Setting->BeforeNum = edBeforeNum->Text.ToInt();
	AnsiString sTmp = AnsiString(edMaxSearchNum->Text);
	Setting->MaxSearchNum = sTmp.ToIntDef(0);
    Setting->OverSearchWarn = cbOverSearchWarn->Checked;
    Setting->RMPunctuationWarn = cbRMPunctuationWarn->Checked;

	// 引用複製模式

	//if(rbCopyMode1->Checked) Setting->CopyMode = 1;
	//else if (rbCopyMode2->Checked) Setting->CopyMode = 2;
	//else if (rbCopyMode3->Checked) Setting->CopyMode = 3;
	//else if (rbCopyMode4->Checked) Setting->CopyMode = 4;

    if(rbCopyMode1->Checked && rbCopyMode3->Checked) Setting->CopyMode = 1;
	else if (!rbCopyMode1->Checked && rbCopyMode3->Checked) Setting->CopyMode = 2;
	else if (rbCopyMode1->Checked && !rbCopyMode3->Checked) Setting->CopyMode = 3;
	else if (!rbCopyMode1->Checked && !rbCopyMode3->Checked) Setting->CopyMode = 4;

    Setting->CBCopyWithJuanNum = cbCBCopyWithJuanNum->Checked;

    if (!DirectoryExists(Setting->UserDataPath))   // 不存在就開啟
    {
        CreateDir(Setting->UserDataPath);
	}
	*/
}
//---------------------------------------------------------------------------

void __fastcall TfmOption::FormShow(TObject *Sender)
{
	LoadFromSetting();
}
//---------------------------------------------------------------------------

void __fastcall TfmOption::btOKClick(TObject *Sender)
{
	SaveToSetting();
	Close();
}
//---------------------------------------------------------------------------

void __fastcall TfmOption::btCancelClick(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------

void __fastcall TfmOption::btSaveClick(TObject *Sender)
{
	SaveToSetting();
    Setting->SaveToFile();
	Close();
}
//---------------------------------------------------------------------------
void __fastcall TfmOption::cbShowPuncChange(TObject *Sender)
{
	cbNoShowLgPunc->Enabled = cbShowPunc->IsChecked;
}
//---------------------------------------------------------------------------

