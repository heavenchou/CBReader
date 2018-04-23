// ---------------------------------------------------------------------------

#ifndef SettingH
#define SettingH

#include "cbxmloption.h"
#include <fmx.h>
#include <System.IniFiles.hpp>
// ---------------------------------------------------------------------------
enum ctCollationType {ctOrigCollation, ctCBETACollation};
class CSetting
{
private: // User declarations

public: // User declarations

	String SettingFile;     // �]�w�ɪ���m

	//CCBXMLOption * CBXMLOption; // CBETA �g�媺�榡

	// �w�] Windows �|�b�D�{�����l�ؿ� Bookcase
	// �w�] Mac �|�b /Library/CBETA/Bookcase
	// �Y�䤣��, �|�߰ݨϥΪ�, �M��s�b BookcaseFullDir
    // BookcaseFullDir �O�Ĥ@�j�M�ؼ�
	String BookcasePath; 	// ���d���ؿ�
	String BookcaseFullPath; 		// ���d������ؿ�

    // �g��榡

	bool ShowLineFormat;	// �O�_�̤j���ä���
	bool ShowLineHead;		// �O�_�歺�[�W�歺��T
    int  CorrSelect;		// �׭q��� 0:�׭q�Φr, 1:�G�̬ҭn [����>�׭q], 2:�����Φr
    int ShowCorrWarning;    // �O�_�n�q�X�׭q��ܪ��`�N�ƶ�, �w�]�O 0
    bool ShowCollation;		// ��ܮհɸ��
	int  CopyMode;			// 1. ���հ�, �g�W�b�e. 2.���հ�, �g�W�b��. 3. �L�հ�, �g�W�b�e. 4.�L�հ�, �g�W�b��.
    bool CBCopyWithJuanNum; // �ޥνƻs�O�_�e�{����

	bool VerticalMode;		// �������
    bool ShowPunc;          // �e�{���I�P�q��
    bool NoShowLgPunc;      // ���e�{�U�|�����I
    int LgType;             // �o�O2016�s���Ȯɥ\��, �]�w�U�|�e�{���覡, 0 ���ª��覡�ΪŮ�, 1 ���D�з��U�|�� <p> �e�{�s��

	// �հɮ榡

	ctCollationType CollationType;      // �հɮ榡 0:���, 1:CBETA

	// �g��e�{���C��, �I��

    TColor BgColor;     			// �e�����I����
    String BgImage;				// �I����
    int    LineHeight;              // ��Z
    TColor JuanNumFontColor;		// �g�����C��
    int    JuanNumFontSize;			// �g������r�j�p
    bool   JuanNumBold;			    // �g������r�O�_����
    TColor JuanNameFontColor;		// ���W���C��
    int    JuanNameFontSize;		// ���W����r�j�p
    bool   JuanNameBold;			// ���W����r�O�_����
    TColor XuFontColor;				// �Ǫ��C��
    int    XuFontSize;				// �Ǫ���r�j�p
    bool   XuBold;			        // �Ǫ���r�O�_����
    TColor BodyFontColor;			// ���媺�C��
    int    BodyFontSize;			// ���媺��r�j�p
    bool   BodyBold;			    // ���媺��r�O�_����
    TColor WFontColor;				// ���媺�C��
    int    WFontSize;				// ���媺��r�j�p
    bool   WBold;			        // ���媺��r�O�_����
    TColor BylineFontColor;			// �@Ķ�̪��C��
    int    BylineFontSize;			// �@Ķ�̪���r�j�p
    bool   BylineBold;			    // �@Ķ�̪���r�O�_����
    TColor HeadNameFontColor;		// ���D���C��
    int    HeadNameFontSize;		// ���D����r�j�p
    bool   HeadNameBold;			// ���D����r�O�_����
    TColor LineHeadFontColor;		// �歺���C��
    int    LineHeadFontSize;		// �歺����r�j�p
    bool   LineHeadBold;			// �歺����r�O�_����
    TColor LgFontColor;				// �U�|���C��
    int    LgFontSize;				// �U�|����r�j�p
    bool   LgBold;			        // �U�|����r�O�_����

    TColor DharaniFontColor;		// �G���C��
    int    DharaniFontSize;			// �G����r�j�p
    bool   UseDharaniFontColor;		// �O�_�ϥΩG���C��
    bool   UseDharaniFontSize;		// �O�_�ϥΩG����r�j�p
    bool   DharaniBold;			    // �G����r�O�_����

    TColor CorrFontColor;		// �ɻ~���C��
    int    CorrFontSize;		// �ɻ~����r�j�p
    bool   UseCorrFontColor;	// �O�_�ϥΰɻ~���C��
    bool   UseCorrFontSize;		// �O�_�ϥΰɻ~����r�j�p
    bool   CorrBold;			// �ɻ~����r�O�_����

    TColor GaijiFontColor;		// �ʦr���C��
    int    GaijiFontSize;		// �ʦr����r�j�p
    bool   UseGaijiFontColor;	// �O�_�ϥίʦr���C��
    bool   UseGaijiFontSize;	// �O�_�ϥίʦr����r�j�p
    bool   GaijiBold;			// �ʦr����r�O�_����

    TColor NoteFontColor;		// ����p�����C��
    int    NoteFontSize;		// ����p������r�j�p
    bool   UseNoteFontColor;	// �O�_�ϥ�����p�����C��
    bool   UseNoteFontSize;		// �O�_�ϥ�����p������r�j�p
    bool   NoteBold;			// ����p������r�O�_����

	TColor FootFontColor;		// �հɪ��C��
	int    FootFontSize;		// �հɪ���r�j�p
	bool   UseFootFontColor;	// �O�_�ϥήհɪ��C��
	bool   UseFootFontSize;		// �O�_�ϥήհɪ���r�j�p
    bool   FootBold;			// �հɪ���r�O�_����

	bool   UseCSSFile;     		// �ϥ� CSS �ɮ�
	String CSSFileName;

	// �ʦr�B�z

	bool GaijiUseUniExt;    // �O�_�ϥ� Unicode Ext
	bool GaijiUseNormal;    // �O�_�ϥγq�Φr

	bool GaijiUniExtFirst;  // �u���ϥ� Unicode Ext
	bool GaijiNormalFirst;  // �u���ϥ� �q�Φr

	bool GaijiDesFirst;     // �u���ϥβզr��
	bool GaijiImageFirst;   // �u���ϥίʦr����

	//int GaijiID[4];			// 4�سB�z�ʦr����k, ���O�O"�q�Φr","�զr��","Unicode", "�ϫ�"
	//bool GaijiUse[4];		// 4�دʦr�O�_�ϥ�

	int ShowSiddamWay;		// �x��r�B�z�k 0:�x��r��(siddam.ttf) 1:ù����g(unicode) 2:ù����g(�¤�r) 3:�x����� 4:�ۭq�Ÿ� 5:CB�X
	int ShowPaliWay;		// ��ڦr�B�z�k 0:Unicode 1:�¤�r 2:Ent �X

	String UserSiddamSign;		// �ϥΪ̦ۭq�x��r�Ÿ�
    bool ShowUnicode30;     // �e�{ Unicode 3.1

	// ���ɤj�p

	int GaijiWidth;			// �ʦr���ɪ��e, 0 �����]��
	int GaijiHeight;		// �ʦr���ɪ���
	int SDGifWidth;			// �ʦr���ɪ��e, 0 �����]��
	int SDGifHeight;		// �ʦr���ɪ���
	int FigureRate;			// ���ɪ����

    // �t�θ�T

    String CBETACDPath;		// CD �g�媺��m
    String XMLSutraPath;	// �g�媺��m
	String BookmarkFile;	// Bookmark ����m
	String SearchPath;		// �����˯�����m
	String GaijiPath;		// �ʦr���ɪ���m
	String SDGifPath;		// �x��r���ɪ���m
	String RJGifPath;		// �����r���ɪ���m
	String FigurePath;		// ���ɪ���m
    String UserDataPath;    // �ӤH��ƥؿ�
    bool SaveLastTocFileName;	// �O�_�x�s�̫�}�Ҫ��ѥ�
    String LastTocFileName;	// �̫�}�Ҫ��ѥ�

	// �����˯�

	int NearNum;				// �����˯� Near ���r�Z
	int BeforeNum;				// �����˯� Before ���r�Z
	int MaxSearchNum;			// ��X�̦h������, 0 ��ܤ��]��
    bool OverSearchWarn; 		// �W�X�����˯��ɮ׼ƪ������, �O�_���Xĵ�i?
    bool RMPunctuationWarn; 	// �����ϥΪ̿�J�˯��r�ꤤ�����I�ɡA�O�_���Xĵ�i�T���H

    // �~���s�������

    int CalloutCount;                   // �~���s���{�����ƥ�
	//TTntStringList * slCalloutTitle;       // �~���s�������D
	//TTntStringList * slCalloutProgram;     // �~���s�����{��

	// �٨S�B�z (�Τ��B�z?)

    String XMLJuanPosPath;		// �C�@���g�岾�쪺�����
	String JuanLinePath;		// �C�@���g��Ĥ@��歺����T

	void __fastcall LoadFromFile();
	void __fastcall SaveToFile();
	void __fastcall LoadFromFile(String sFile);
	void __fastcall SaveToFile(String sFile);

	__fastcall CSetting(String sFile);
	__fastcall ~CSetting();
};
// ---------------------------------------------------------------------------
#endif
