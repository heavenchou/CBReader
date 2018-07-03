//---------------------------------------------------------------------------

#ifndef updateH
#define updateH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <FMX.Controls.hpp>
#include <FMX.Forms.hpp>
#include <FMX.Controls.Presentation.hpp>
#include <FMX.Memo.hpp>
#include <FMX.ScrollBox.hpp>
#include <FMX.StdCtrls.hpp>
#include <FMX.Types.hpp>
#include <System.Net.HttpClient.hpp>
#include <System.Net.HttpClientComponent.hpp>
#include <System.Net.URLClient.hpp>
#include <FMX.DialogService.hpp>
#include <FMX.DialogService.Sync.hpp>
#include <System.IOUtils.hpp>
#include <System.Zip.hpp>
#include <FMX.Edit.hpp>
//---------------------------------------------------------------------------
class TfmUpdate : public TForm
{
/*
	��s����

	�̷s����s����榡�p�U

	message=CBReader ���s�������A��s��Э��s���楻�{���A�H���M��Ʈ榡�۲šC
	�ĤG��T��
	�ĤT��T��
	source=http://www.cbeta.org/cbreader/update/cbreader_0.2.zip
	dest=cbreader
	source=http://www.cbeta.org/cbreader/update/bookcase_0.2.zip
	dest=bookcase

	�Ĥ@�� message ��ܦ���s, �_�h�|�Ǧ^ ok
	message �O�T��, ���u�@��, ����X�{ source= �~����
	source=xxx ��ܨӷ�, �@�w�O zip ��
	dest=xxx �O�ت�, �u���G��

	1. cbreader , ��ܧ� zip �ɸ����Y��D�{���ؿ� (mac ���A��)
	   �o�����Y�ɸ̭����ӥu���@�� CBReader.exe
	2. bookcase , ��ܧ� zip �ɸ����Y�� bookcase �ؿ�
	   �ҥH�o�����Y�����ӥ� CBETA �ؿ��}�l
*/

__published:	// IDE-managed Components
	TMemo *Memo;
	TButton *btUpdate;
	TProgressBar *ProgressBar;
	TNetHTTPClient *NetHTTPClient;
	TNetHTTPRequest *nhrCBR;
	TNetHTTPRequest *nhrDownload;
	TEdit *edBookcasePath;
	TLabel *Label2;
	TLabel *lbMessage;
	void __fastcall btUpdateClick(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall nhrCBRRequestCompleted(TObject * const Sender, IHTTPResponse * const AResponse);
	void __fastcall nhrDownloadReceiveData(TObject * const Sender, __int64 AContentLength,
          __int64 AReadCount, bool &Abort);
	void __fastcall nhrDownloadRequestCompleted(TObject * const Sender, IHTTPResponse * const AResponse);
	void __fastcall nhrCBRRequestError(TObject * const Sender, const UnicodeString AError);

private:	// User declarations
public:		// User declarations

	String DestFile;

	String ServerURL;  // �n�ˬd��s�������ؿ�
	bool UseLocalhostURL;  // �ϥ� localhost �����պ��}
	String LocalhostURL;    // �������ժ����}

	// �G�Ӫ���
	//String CBRVer;  // CBReader ������
	//String DataVer;	// �g���ƪ�����

	// �P�_�n���n�q�T��
	// �p�G�ǤJ�Ѽ� true , ��ܭn�q�T��
	// �۰��ˬd��s��, �Y�S��s�N�ۦ����}
	// ����ˬd��s, �S��s�N�ݭn�q�T�� "�ثe�O�̷s��".
	bool IsShowMessage;

	TStringList * slReceive;	// ���������T��

	// �ˬd�ݤ��ݭn��s, �ǤJ cbreader ����, ��ƪ���, �H�έn���n�^���ثe�O�̷s�� (��ʧ�s�~�ݭn)
	void __fastcall CheckUpdate(String sCBRVer, String sDataVer, bool bShowNoUpdate=false);
	// �� http ���}���o�̫᪺�ɦW
	String __fastcall GetHttpFileName(String sURL);
	// �U���ɮ�
	void __fastcall DL_UP(String sSource, String sDest);
	__fastcall TfmUpdate(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfmUpdate *fmUpdate;
//---------------------------------------------------------------------------
#endif
