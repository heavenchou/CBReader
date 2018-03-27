// ---------------------------------------------------------------------------

#pragma hdrstop

#include "setting.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)

// ---------------------------------------------------------------------------
__fastcall CSetting::CSetting(String sFile) // �غc�禡
{
	SettingFile = sFile;            // �]�w�ɪ���m

	// �]�w�w�]��

	BookcaseDir = "Bookcase"; 		// ���d���ؿ�

    // �g��榡

	ShowLineFormat = false;         // �O�_�̤j���ä���
	ShowLineHead = true;			// �O�_�歺�[�W�歺��T
	//CorrSelect = 0;				// �ɻ~��� 0:�׭q�Φr, 1:�G�̬ҭn [����>�׭q], 2:�����Φr
	//ShowCorrWarning = 0;          // �O�_�n�q�X�׭q��ܪ��`�N�ƶ�, �w�]�O 0
	//ShowJKData = true;			// ��ܮհɸ��
	VerticalMode = false;			// �������
	ShowPunc = true;                // �e�{���I
	NoShowLgPunc = false;           // ���e�{�U�|�����I
	//LgType = 1;                   // �o�O2016�s���Ȯɥ\��, �]�w�U�|�e�{���覡, 0 ���ª��覡�ΪŮ�, 1 ���D�з��U�|�� <p> �e�{�s��

	CollationType = ctCBETACollation;      // �հɮ榡 0:�L, 1:���, 2:CBETA

    // ��r���C��P�j�p

	BgColor = TColor(14479837);				// �w�]�ȬO�զ�
    BgImage = "";
    LineHeight=20;                  // ��Z
	JuanNumFontColor = TColor(32768);
    JuanNumFontSize = 16;
    JuanNumBold = false;
	JuanNameFontColor = TColor(16711680);
    JuanNameFontSize = 18;
    JuanNameBold = true;
	XuFontColor = TColor(10485760);
    XuFontSize = 16;
    XuBold = false;
	BodyFontColor = TColor(0);
    BodyFontSize = 16;
    BodyBold = false;
	WFontColor = TColor(10485760);
    WFontSize = 16;
    WBold = false;
	BylineFontColor = TColor(8421440);
    BylineFontSize = 16;
    BylineBold = false;
	HeadNameFontColor = TColor(10485760);
    HeadNameFontSize = 18;
    HeadNameBold = true;
	LineHeadFontColor = TColor(10485760);
    LineHeadFontSize = 14;
    LineHeadBold = false;
	LgFontColor = TColor(4227072);
    LgFontSize = 16;
    LgBold = false;
	DharaniFontColor = TColor(4227072);
    DharaniFontSize = 16;
    DharaniBold = false;
	CorrFontColor = TColor(255);
    CorrFontSize = 16;
    CorrBold = false;
	GaijiFontColor = TColor(16711680);
    GaijiFontSize = 16;
    GaijiBold = false;
	NoteFontColor = TColor(20639); // 6684774;
    NoteFontSize = 14;
    NoteBold = false;
	FootFontColor = TColor(8421631);
	FootFontSize = 16;
    FootBold = false;

    UseDharaniFontColor = false;	// �O�_�ϥΩG���C��
    UseDharaniFontSize = false;	// �O�_�ϥΩG����r�j�p
    UseCorrFontColor = true;	// �O�_�ϥΰɻ~���C��
    UseCorrFontSize = false;	// �O�_�ϥΰɻ~����r�j�p
    UseGaijiFontColor = false;	// �O�_�ϥίʦr���C��
    UseGaijiFontSize = false;	// �O�_�ϥίʦr����r�j�p
    UseNoteFontColor = true;	// �O�_�ϥ�����p�����C��
	UseNoteFontSize = true;		// �O�_�ϥ�����p������r�j�p
	UseFootFontColor = false;	// �O�_�ϥήհɪ��C��
	UseFootFontSize = false;		// �O�_�ϥήհɪ���r�j�p

    UseCSSFile = false;     		// �ϥ� CSS �ɮ�
    CSSFileName = "cbreader.css";	// CSS �ɦW

    // �ʦr�B�z

	GaijiUseUniExt = true;    // �O�_�ϥ� Unicode Ext
	GaijiUseNormal = true;    // �O�_�ϥγq�Φr

	GaijiUniExtFirst = true;  // �u���ϥ� Unicode Ext
	GaijiNormalFirst = false;  // �u���ϥ� �q�Φr

	GaijiDesFirst = true;     // �u���ϥβզr��
	GaijiImageFirst = false;   // �u���ϥίʦr����

	ShowSiddamWay = 6;		// �x��r�B�z�k 0:�x��r��(siddam.ttf) 1:ù����g(unicode) 2:ù����g(�¤�r) 3:�x����� 4:�ۭq�Ÿ� 5:CB�X 6:�x��ù�����
	UserSiddamSign = "��";	// �ϥΪ̦ۭq�x��r�Ÿ�
    ShowPaliWay = 0;		// ��ڦr�B�z�k 0:Unicode 1:�¤�r 2:Ent �X
    ShowUnicode30 = true;  // �e�{ Unicode 3.0 , �b P5 XML , �o�ӥi�H�w�]��

	// ���ɤj�p

	GaijiWidth  = 0;		// �ʦr���ɪ��e, 0 �����]��
	GaijiHeight = 0;		// �ʦr���ɪ���, 0 �����]��
	SDGifWidth  = 0;		// �ʦr���ɪ��e, 0 �����]��
	SDGifHeight = 0;		// �ʦr���ɪ���, 0 �����]��
	FigureRate  = 100;		// ���ɪ����

    // �t�θ�T

	CBETACDPath = "D:\\";		// CBETA ���Ъ���m
    XMLSutraPath = "XML\\";	    // d:\\cbeta.src\\release\\xml\\";	// �g�媺��m
	//BookmarkFile = fmMain->BookmarkFile;
	//SearchPath = fmMain->SearchPath;
	GaijiPath  = "gaiji-CB\\";
	SDGifPath  = "sd-gif\\";		// �x��r���ɪ���m
	RJGifPath  = "rj-gif\\";		// �����r���ɪ���m
	FigurePath = "Figures\\";
	//UserDataPath = fmMain->UserDataPath;   // �ӤH��ƥؿ�
    SaveLastTocFileName = true;		// �O�_�x�s�̫�}�Ҫ��ѥ�
    LastTocFileName = "00All.toc";

	// �����˯�

	NearNum = 30;				// �����˯� Near ���r�Z
	BeforeNum = 30;				// �����˯� Before ���r�Z
	MaxSearchNum = 500;			// ��X�̦h������, 0 ��ܤ��]��
    OverSearchWarn = true; 		// �W�X�����˯��ɮ׼ƪ������, �O�_���Xĵ�i?
    RMPunctuationWarn = true;	// �����ϥΪ̿�J�˯��r�ꤤ�����I�ɡA�O�_���Xĵ�i�T���H

	// �ޥνƻs�Ҧ�

	CopyMode = 1;			    // 1. ���հ�, �g�W�b�e. 2.���հ�, �g�W�b��. 3. �L�հ�, �g�W�b�e. 4.�L�հ�, �g�W�b��.
    CBCopyWithJuanNum = true;   // �ޥνƻs�O�_�e�{����

    // �~���s�������

    CalloutCount = 0;               // �~���s���{�����ƥ�
	//slCalloutTitle = new TTntStringList();      // �~���s�������D
	//slCalloutProgram = new TTntStringList();    // �~���s�����{��

    // ���諸��T (�άO������, �n�| ini)
    XMLJuanPosPath = "JuanPos\\";	// �C�@���g�岾�쪺�����
	JuanLinePath = "JuanLine\\";	// �C�@���g��Ĥ@��歺����T

    LoadFromFile(SettingFile);  // ���J�]�w��
}

// ---------------------------------------------------------------------------
__fastcall CSetting::~CSetting(void) // �Ѻc�禡
{
}
// ---------------------------------------------------------------------------
// ���J�]�w��
void __fastcall CSetting::LoadFromFile()
{
	LoadFromFile(SettingFile);
}
// ---------------------------------------------------------------------------
// ���J�]�w��
void __fastcall CSetting::LoadFromFile(String sFile)
{
	TIniFile *IniFile = new TIniFile(sFile);

	String Section;

	// Ini file ���c�O
    // [section]
    // Ident = Value

    Section = "ShowFormat";

	ShowLineFormat = IniFile->ReadBool(Section, "ShowLineFormat", ShowLineFormat);
	ShowLineHead = IniFile->ReadBool(Section, "ShowLineHead", ShowLineHead);
	ShowPunc = IniFile->ReadBool(Section, "ShowPunc", ShowPunc);
	NoShowLgPunc = IniFile->ReadBool(Section, "NoShowLgPunc", NoShowLgPunc);
	VerticalMode = IniFile->ReadBool(Section, "VerticalMode", VerticalMode);

	CollationType = (ctCollationType)IniFile->ReadInteger(Section, "CollationType", CollationType);

	// �ʦr�B�z

	Section = "Gaiji";

	GaijiUseUniExt = IniFile->ReadBool(Section, "GaijiUseUniExt" ,GaijiUseUniExt);    // �O�_�ϥ� Unicode Ext
	GaijiUseNormal = IniFile->ReadBool(Section, "GaijiUseNormal" ,GaijiUseNormal);      // �O�_�ϥγq�Φr

	GaijiUniExtFirst = IniFile->ReadBool(Section, "GaijiUniExtFirst" ,GaijiUniExtFirst);  // �u���ϥ� Unicode Ext
	GaijiNormalFirst = !GaijiUniExtFirst;  // �u���ϥ� �q�Φr

	GaijiDesFirst = IniFile->ReadBool(Section, "GaijiDesFirst" ,GaijiDesFirst);     // �u���ϥβզr��
	GaijiImageFirst = !GaijiDesFirst;   // �u���ϥίʦr����

	// �t�θ�T

	Section = "SystemInfo";

	BookcaseDir = IniFile->ReadString(Section, "BookcaseDir", BookcaseDir);

	delete IniFile;
}
// ---------------------------------------------------------------------------
// �x�s�]�w
void __fastcall CSetting::SaveToFile()
{
	SaveToFile(SettingFile);
}
// ---------------------------------------------------------------------------
// �x�s�]�w
void __fastcall CSetting::SaveToFile(String sFile)
{
	TIniFile *IniFile = new TIniFile(sFile);

    AnsiString Section;

    // Ini file ���c�O
    // [section]
    // Ident = Value

	Section = "Version";
    try
    {
		IniFile->WriteString(Section, "Version", "0.1");
    }
    catch(...)
    {
		delete IniFile;     // �������A��

		return;
	}

    Section = "ShowFormat";

	IniFile->WriteBool(Section, "ShowLineFormat", ShowLineFormat);
	IniFile->WriteBool(Section, "ShowLineHead", ShowLineHead);
	IniFile->WriteBool(Section, "ShowPunc", ShowPunc);
	IniFile->WriteBool(Section, "NoShowLgPunc", NoShowLgPunc);
	IniFile->WriteBool(Section, "VerticalMode", VerticalMode);
	IniFile->WriteInteger(Section, "CollationType", CollationType);

	// �ʦr�B�z

	Section = "Gaiji";

	IniFile->WriteBool(Section, "GaijiUseUniExt", GaijiUseUniExt);
	IniFile->WriteBool(Section, "GaijiUseNormal", GaijiUseNormal);
	IniFile->WriteBool(Section, "GaijiUniExtFirst", GaijiUniExtFirst);
	IniFile->WriteBool(Section, "GaijiDesFirst", GaijiDesFirst);

	// �t�θ�T

	Section = "SystemInfo";

	IniFile->WriteString(Section, "BookcaseDir", BookcaseDir);

	delete IniFile;
}
// ---------------------------------------------------------------------------

