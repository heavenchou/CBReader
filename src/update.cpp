//---------------------------------------------------------------------------

#include <fmx.h>
#pragma hdrstop

#include "update.h"
#include "main.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.fmx"
TfmUpdate *fmUpdate;
//---------------------------------------------------------------------------
__fastcall TfmUpdate::TfmUpdate(TComponent* Owner)
	: TForm(Owner)
{
	IsShowMessage = false;
	UseLocalhostURL = false;  // �ϥ� localhost �����պ��}
	ServerURL = u"http://www.cbeta.org/cbreader/update.php?";
	LocalhostURL = u"http://localhost/cbreader/update.php?";// �������ժ����}

	//LocalhostURL = u"http://192.168.0.3/cbreader/update.php?";
	slReceive = new TStringList;	// ���������T��
}
//---------------------------------------------------------------------------
// �ˬd�ݤ��ݭn��s
void __fastcall TfmUpdate::CheckUpdate(String sCBRVer, String sDataVer, bool bShowNoUpdate)
{
	IsShowMessage = bShowNoUpdate;

	// �ǹL�h�������榡 ?cbr=0.1.0.0&data=0.1.0.0
	// �@�~�t�� &os=win
    // debug �Ҧ� &type=debug

	String sURL = ServerURL;
	if(UseLocalhostURL) sURL = LocalhostURL;

	sURL = sURL + u"cbr=" + sCBRVer + u"&data=" + sDataVer;
#ifdef _Windows
	sURL += u"&os=win";
#else
	sURL += u"&os=mac";
#endif

	if(fmMain->IsDebug)
	{
		sURL += u"&type=debug";
		TDialogService::ShowMessage(sURL);
	}

	nhrCBR->URL = sURL;
	nhrCBR->Execute();
}
//---------------------------------------------------------------------------
void __fastcall TfmUpdate::nhrCBRRequestCompleted(TObject * const Sender, IHTTPResponse * const AResponse)
{
	// Ū�J��s���
	slReceive->Clear();
	slReceive->LoadFromStream(AResponse->GetContentStream(),TEncoding::UTF8);
	// �P�_�O���O��s�����
	String sStr1 = slReceive->Strings[0];
	if(fmMain->IsDebug) TDialogService::ShowMessage(sStr1);
	if(sStr1.SubString0(0,2) == u"ok")
	{
		// ��ܨS����s���
		if(IsShowMessage)
		{
			TDialogService::ShowMessage(u"�z�� CBReader �O�̷s��!");
		}
	}
	else if(sStr1.SubString0(0,8) == u"message=")
	{
		int iResult = TDialogServiceSync::MessageDialog(u"�o�{��s�ɮסA�z�n�i���s�ܡH��s��Хߨ譫�s�}�ҥ��{���A�H�T�O�{���P��Ưॿ�T�f�t�C",
			TMsgDlgType::mtConfirmation,
			TMsgDlgButtons() << TMsgDlgBtn::mbYes << TMsgDlgBtn::mbNo, TMsgDlgBtn::mbYes ,0);

		if (iResult == System::Uitypes::mrYes)
			ShowModal();
		else
			if(fmMain->IsDebug)
				TDialogService::ShowMessage(u"����s�N��F");
	}
}
//---------------------------------------------------------------------------
void __fastcall TfmUpdate::FormShow(TObject *Sender)
{
	String sMsg = slReceive->Strings[0];
	sMsg.Delete0(0,8);  // ���� "message="

	// ���s�T���e�{�X��
	for(int i=1; i< slReceive->Count; i++)
	{
		if(slReceive->Strings[i].SubString0(0,7) != u"source=")
		{
			sMsg += u"\n";
			sMsg += slReceive->Strings[i];
		}
		else
            break;
	}
	Memo->Text = sMsg;

	edBookcasePath->Text = IncludeTrailingPathDelimiter(fmMain->Bookcase->BookcaseDir);
	lbMessage->Text = u"�ǳƧ�s";
}
//---------------------------------------------------------------------------
void __fastcall TfmUpdate::btUpdateClick(TObject *Sender)
{
	// �}�l��s
	btUpdate->Cursor = crHourGlass;
	Cursor = crHourGlass;
	String sMessage = lbMessage->Text;
	lbMessage->Text = u"���J��...";

	// �����������˯�����
	fmMain->Bookcase->CBETA->FreeSearchEngine();

	for(int i=1; i<slReceive->Count; i++)
	{
		// �ӷ�
		String sSource = slReceive->Strings[i];
		// ���ɷ|���ťզ�, �ҥH�n�[�W�P�_
		if(sSource.Length() > 7 && sSource.SubString0(0,7) == u"source=")
		{
			// �ت�
			String sDest = slReceive->Strings[i+1];

			if(sDest.SubString0(0,5) == u"dest=")
			{
				sSource.Delete0(0,7);
				sDest.Delete0(0,5);
				if(fmMain->IsDebug)
				{
					TDialogService::ShowMessage(sSource);
					TDialogService::ShowMessage(sDest);
				}
				if(sDest.LowerCase() == "cbreader" ||
					sDest.LowerCase() == "bookcase")
				{
					// �u���o�G�ؤ~�n��s, �@�O�D�{��, �@�O���
					DL_UP(sSource,sDest);   // �U���Χ�s
				}
				i++;
            }
        }
	}

	// ���s�إߥ����˯�����

	lbMessage->Text = u"�]�w��...";
	Application->ProcessMessages();

	fmMain->Bookcase->CBETA->LoadSearchEngine();

	lbMessage->Text = u"�]�w��...";
	Application->ProcessMessages();

	fmMain->SetSearchEngine();

	btUpdate->Cursor = crDefault;
	Cursor = crDefault;

	lbMessage->Text = u"��s����";
	TDialogService::ShowMessage(u"��s�����I");
	Close();
}
//---------------------------------------------------------------------------
void __fastcall TfmUpdate::DL_UP(String sSource, String sDest)
{
	DestFile = sDest;
	nhrDownload->URL = sSource;
	nhrDownload->Execute();
}
//---------------------------------------------------------------------------
void __fastcall TfmUpdate::nhrDownloadReceiveData(TObject * const Sender, __int64 AContentLength,
          __int64 AReadCount, bool &Abort)
{
    Application->ProcessMessages();
	float percentageDownloaded;
	if (AContentLength > 0)
	{
		percentageDownloaded = ((float)AReadCount / (float)AContentLength) * 100.f;
		ProgressBar->Value =  percentageDownloaded;
	}
	else
	{
		// the server did not provide the Content-Length header
	}
}
//---------------------------------------------------------------------------
void __fastcall TfmUpdate::nhrDownloadRequestCompleted(TObject * const Sender,
          IHTTPResponse * const AResponse)
{
	lbMessage->Text = u"�]�w��...";
	Application->ProcessMessages();

	TStream * inf = AResponse->GetContentStream();

	// �D�{����s
	if(DestFile.LowerCase() == u"cbreader")
	{
#ifdef _Windows
		// �p�G���Ȧs��, �n���R��
		DestFile = fmMain->MyFullPath + u"CBReader.exe";
		String sTmpFile = DestFile + u".tmp";
		if(TFile::Exists(sTmpFile))
		{
			TFile::Delete(sTmpFile);
		}

		// �A��{�����ɮק�W���Ȧs��
		if(TFile::Exists(DestFile))
		{
			TFile::Move(DestFile,sTmpFile);
		}

		// �A�N���G�ɦs���ت���

		TZipFile * Zip = new TZipFile();
		Zip->Open(inf, TZipMode::zmRead);
		Zip->ExtractAll(fmMain->MyFullPath);
		Zip->Close();
		Zip->Free();
#else
		// Mac ��, ��{����b�ୱ�N�n�F

		// ���o�ɦW
		String sFileName = GetHttpFileName(nhrDownload->URL);
		String sFile = fmMain->MyFullPath + u"Desktop/" + sFileName;
		TFileStream * outf = new TFileStream(sFile, fmCreate);
		outf->CopyFrom(inf, inf->Size);
		outf->Free();
		String sMessage = u"�s�� CBReader �w�x�s�b�z���ୱ�W�A�ɦW�� " + sFileName + u"�A�бz�����Y����N�즳�� CBReader �Y�i�C";
		TDialogService::ShowMessage(sMessage);
#endif
	}
	else
	{
		// ��s���

		TZipFile * Zip = new TZipFile();
		Zip->Open(inf, TZipMode::zmRead);
		Zip->ExtractAll(edBookcasePath->Text);
		Zip->Close();
		Zip->Free();
    }
}
//---------------------------------------------------------------------------
// ���������~�A�q�X���~�T��
void __fastcall TfmUpdate::nhrCBRRequestError(TObject * const Sender, const UnicodeString AError)
{
    // ��ʧ�s���~�n�q���~�T��
	if(IsShowMessage)
		TDialogService::ShowMessage(AError);
}
//---------------------------------------------------------------------------
// �� http ���}���o�̫᪺�ɦW
String __fastcall TfmUpdate::GetHttpFileName(String sURL)
{
	int iPos = sURL.LastDelimiter0("\\/");
	String sFileName = String(sURL.FirstChar()+iPos+1);
	return sFileName;
}
//---------------------------------------------------------------------------

