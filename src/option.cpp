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
// �ѳ]�w���J
void __fastcall TfmOption::LoadFromSetting()
{
    // �g��榡

	cbShowLineFormat->IsChecked = Setting->ShowLineFormat;
	cbShowLineHead->IsChecked = Setting->ShowLineHead;
	//cbShowJKData->Checked  = Setting->ShowJKData;
	//rgCorrSelect->ItemIndex = Setting->CorrSelect;

	cbShowPunc->IsChecked = Setting->ShowPunc;
	cbNoShowLgPunc->IsChecked = Setting->NoShowLgPunc;

	cbNoShowLgPunc->Enabled = cbShowPunc->IsChecked;      // �Y���e�{���I, �N���X�{ "�U�|�e�{���I�P�_" ���ﶵ

	cbVerticalMode->IsChecked = Setting->VerticalMode;

	// �հɮ榡
	if(Setting->CollationType == ctNoCollation) rbNoCollation->IsChecked = true;
	else if(Setting->CollationType == ctOrigCollation) rbOrigCollation->IsChecked = true;
	else if(Setting->CollationType == ctCBETACollation) rbCBETACollation->IsChecked = true;

	/*
	// ��r�C��j�p

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

    // �ʦr�B�z

	cbGaijiUseUniExt->IsChecked = Setting->GaijiUseUniExt;    // �O�_�ϥ� Unicode Ext
	cbGaijiUseNormal->IsChecked = Setting->GaijiUseNormal;    // �O�_�ϥγq�Φr

	rbGaijiUniExtFirst->IsChecked = Setting->GaijiUniExtFirst;  // �u���ϥ� Unicode Ext
	rbGaijiNormalFirst->IsChecked = Setting->GaijiNormalFirst;  // �u���ϥ� �q�Φr

	rbGaijiDesFirst->IsChecked = Setting->GaijiDesFirst;     // �u���ϥβզr��
	rbGaijiImageFirst->IsChecked = Setting->GaijiImageFirst;   // �u���ϥίʦr����

	/*
	// �x��r�B�z�k 0:�x��r��(siddam.ttf) 1:ù����g(unicode) 2:ù����g(�¤�r) 3:�x����� 4:�ۭq�Ÿ� 5:CB�X 6:�x��ù�����
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

	edUserSiddamSign->Text = Setting->UserSiddamSign;		// �ϥΪ̦ۭq�x��r�Ÿ�

	// ��ڦr�B�z�k 0:Unicode 1:�¤�r 2:Ent �X
	switch( Setting->ShowPaliWay)
    {
		case 0: rbUsePaliUnicode->Checked = true;
			break;
		case 1: rbUsePaliText->Checked = true;
			break;
		case 2: rbUsePaliEnt->Checked = true;
			break;
    }

    // �e�{ Unicode 3.1
    cbShowUnicode30->Checked = Setting->ShowUnicode30;

	// ���ɤj�p

	edGaijiWidth->Text = Setting->GaijiWidth;		// �ʦr���ɪ��e, 0 �����]��
	edGaijiHeight->Text = Setting->GaijiHeight;		// �ʦr���ɪ���
	edSDGifWidth->Text = Setting->SDGifWidth;		// �ʦr���ɪ��e, 0 �����]��
	edSDGifHeight->Text = Setting->SDGifHeight;		// �ʦr���ɪ���
	//edFigureRate->Text = Setting->FigureRate;		// ���ɪ����

	// �t�θ�T

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

	// �����˯�

	edNearNum->Text = Setting->NearNum;
	edBeforeNum->Text = Setting->BeforeNum;
	edMaxSearchNum->Text = AnsiString(Setting->MaxSearchNum);
    cbOverSearchWarn->Checked  = Setting->OverSearchWarn;
    cbRMPunctuationWarn->Checked  = Setting->RMPunctuationWarn;

	// �ޥνƻs�Ҧ�

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
// �ѳ]�w���J
void __fastcall TfmOption::SaveToSetting()
{
	// �g��榡

	Setting->ShowLineFormat = cbShowLineFormat->IsChecked;
	Setting->ShowLineHead = cbShowLineHead->IsChecked;
	//Setting->ShowJKData  = cbShowJKData->Checked;
	//Setting->CorrSelect = rgCorrSelect->ItemIndex;

	Setting->ShowPunc = cbShowPunc->IsChecked;
	Setting->NoShowLgPunc = cbNoShowLgPunc->IsChecked;

	Setting->VerticalMode = cbVerticalMode->IsChecked;

	// �հɮ榡
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

	// �ʦr�B�z

	Setting->GaijiUseUniExt = cbGaijiUseUniExt->IsChecked;    // �O�_�ϥ� Unicode Ext
	Setting->GaijiUseNormal = cbGaijiUseNormal->IsChecked;    // �O�_�ϥγq�Φr

	Setting->GaijiUniExtFirst = rbGaijiUniExtFirst->IsChecked;  // �u���ϥ� Unicode Ext
	Setting->GaijiNormalFirst = rbGaijiNormalFirst->IsChecked;  // �u���ϥ� �q�Φr

	Setting->GaijiDesFirst = rbGaijiDesFirst->IsChecked;     // �u���ϥβզr��
	Setting->GaijiImageFirst = rbGaijiImageFirst->IsChecked;   // �u���ϥίʦr����


	/*
    // �x��r�B�z�k 0:�x��r��(siddam.ttf) 1:ù����g(unicode) 2:ù����g(�¤�r) 3:�x����� 4:�ۭq�Ÿ� 5:CB�X 6:�x��ù�����
	if(rbUseSiddamFont->Checked)        Setting->ShowSiddamWay = 0;
	else if(rbUseSiddamRomeU->Checked)	Setting->ShowSiddamWay = 1;
	else if(rbUseSiddamRome->Checked)	Setting->ShowSiddamWay = 2;
	else if(rbUseSiddamGif->Checked)	Setting->ShowSiddamWay = 3;
	else if(rbUseSiddamSign->Checked)	Setting->ShowSiddamWay = 4;
	else if(rbUseSiddamCB->Checked)		Setting->ShowSiddamWay = 5;
	else if(rbUseSiddamAndRome->Checked)		Setting->ShowSiddamWay = 6;
    Setting->UserSiddamSign = edUserSiddamSign->Text;		// �ϥΪ̦ۭq�x��r�Ÿ�

	if(rbUsePaliUnicode->Checked)	Setting->ShowPaliWay = 0;		// ��ڦr�B�z�k 0:Unicode 1:�¤�r 2:Ent �X
	else if(rbUsePaliText->Checked)	Setting->ShowPaliWay = 1;
	else if(rbUsePaliEnt->Checked)	Setting->ShowPaliWay = 2;

    Setting->ShowUnicode30 = cbShowUnicode30->Checked;      // �e�{ Unicode 3.1

	// ���ɤj�p

	Setting->GaijiWidth = edGaijiWidth->Text.ToIntDef(0);		// �ʦr���ɪ��e, 0 �����]��
	Setting->GaijiHeight = edGaijiHeight->Text.ToIntDef(0);		// �ʦr���ɪ���
	Setting->SDGifWidth = edSDGifWidth->Text.ToIntDef(0);		// �ʦr���ɪ��e, 0 �����]��
	Setting->SDGifHeight = edSDGifHeight->Text.ToIntDef(0);		// �ʦr���ɪ���
	//Setting->FigureRate = edFigureRate->Text.ToIntDef(100);		// ���ɪ����

	// �t�θ��

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

	// �����˯�

	Setting->NearNum = edNearNum->Text.ToInt();
	Setting->BeforeNum = edBeforeNum->Text.ToInt();
	AnsiString sTmp = AnsiString(edMaxSearchNum->Text);
	Setting->MaxSearchNum = sTmp.ToIntDef(0);
    Setting->OverSearchWarn = cbOverSearchWarn->Checked;
    Setting->RMPunctuationWarn = cbRMPunctuationWarn->Checked;

	// �ޥνƻs�Ҧ�

	//if(rbCopyMode1->Checked) Setting->CopyMode = 1;
	//else if (rbCopyMode2->Checked) Setting->CopyMode = 2;
	//else if (rbCopyMode3->Checked) Setting->CopyMode = 3;
	//else if (rbCopyMode4->Checked) Setting->CopyMode = 4;

    if(rbCopyMode1->Checked && rbCopyMode3->Checked) Setting->CopyMode = 1;
	else if (!rbCopyMode1->Checked && rbCopyMode3->Checked) Setting->CopyMode = 2;
	else if (rbCopyMode1->Checked && !rbCopyMode3->Checked) Setting->CopyMode = 3;
	else if (!rbCopyMode1->Checked && !rbCopyMode3->Checked) Setting->CopyMode = 4;

    Setting->CBCopyWithJuanNum = cbCBCopyWithJuanNum->Checked;

    if (!DirectoryExists(Setting->UserDataPath))   // ���s�b�N�}��
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

