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
	UseLocalhostURL = false;  // 使用 localhost 的測試網址
	ServerURL = u"http://www.cbeta.org/cbreader/update.php?";
	LocalhostURL = u"http://localhost/cbreader/update.php?";// 內部測試的網址

	//LocalhostURL = u"http://192.168.0.3/cbreader/update.php?";
	slReceive = new TStringList;	// 接收網頁訊息
}
//---------------------------------------------------------------------------
// 檢查需不需要更新
void __fastcall TfmUpdate::CheckUpdate(String sCBRVer, String sDataVer, bool bShowNoUpdate)
{
	IsShowMessage = bShowNoUpdate;

	// 判斷有沒有更新, 如果有更新, 就不修改更新時期,
	// 這樣才能再次執行時, 再次檢查有沒有更新的更新
	IsUpdate = false;

	// 傳過去的版本格式 ?cbr=0.1.0.0&data=0.1.0.0
	// 作業系統 &os=win
    // debug 模式 &type=debug

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
	// 讀入更新資料
	slReceive->Clear();
	slReceive->LoadFromStream(AResponse->GetContentStream(),TEncoding::UTF8);
	// 判斷是不是更新資料檔
	String sStr1 = slReceive->Strings[0];
	if(fmMain->IsDebug) TDialogService::ShowMessage(sStr1);
	if(sStr1.SubString0(0,2) == u"ok")
	{
		// 表示沒有更新資料
		if(IsShowMessage)
		{
			TDialogService::ShowMessage(u"您的 CBReader 是最新的!");
		}
	}
	else if(sStr1.SubString0(0,8) == u"message=")
	{
		int iResult = TDialogServiceSync::MessageDialog(u"發現更新檔案，您要進行更新嗎？更新後請立刻重新開啟本程式，以確保程式與資料能正確搭配。",
			TMsgDlgType::mtConfirmation,
			TMsgDlgButtons() << TMsgDlgBtn::mbYes << TMsgDlgBtn::mbNo, TMsgDlgBtn::mbYes ,0);

		if (iResult == System::Uitypes::mrYes)
			ShowModal();
		else
			if(fmMain->IsDebug)
				TDialogService::ShowMessage(u"不更新就算了");
	}
}
//---------------------------------------------------------------------------
void __fastcall TfmUpdate::FormShow(TObject *Sender)
{
	String sMsg = slReceive->Strings[0];
	sMsg.Delete0(0,8);  // 移除 "message="

	// 把更新訊息呈現出來
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
	lbMessage->Text = u"準備更新";
}
//---------------------------------------------------------------------------
void __fastcall TfmUpdate::btUpdateClick(TObject *Sender)
{
	IsUpdate = true;    // 表示有更新, 不要修改更新日期

	// 開始更新
	btUpdate->Cursor = crHourGlass;
	Cursor = crHourGlass;
	String sMessage = lbMessage->Text;
	lbMessage->Text = u"載入中...";

	// 先關掉全文檢索引擎
	fmMain->Bookcase->CBETA->FreeSearchEngine();

	for(int i=1; i<slReceive->Count; i++)
	{
		// 來源
		String sSource = slReceive->Strings[i];
		// 有時會有空白行, 所以要加上判斷
		if(sSource.Length() > 7 && sSource.SubString0(0,7) == u"source=")
		{
			// 目的
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
					// 只有這二種才要更新, 一是主程式, 一是資料
					DL_UP(sSource,sDest);   // 下載及更新
				}
				i++;
            }
        }
	}

	// 重新建立全文檢索引擎

	lbMessage->Text = u"設定中...";
	Application->ProcessMessages();

	fmMain->Bookcase->CBETA->LoadSearchEngine();

	lbMessage->Text = u"設定中...";
	Application->ProcessMessages();

	fmMain->SetSearchEngine();

	btUpdate->Cursor = crDefault;
	Cursor = crDefault;

	lbMessage->Text = u"更新完成";
	TDialogService::ShowMessage(u"更新完成！");
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
	lbMessage->Text = u"設定中...";
	Application->ProcessMessages();

	TStream * inf = AResponse->GetContentStream();

	// 主程式更新
	if(DestFile.LowerCase() == u"cbreader")
	{
#ifdef _Windows
		// 如果有暫存檔, 要先刪除
		DestFile = fmMain->MyFullPath + u"CBReader.exe";
		String sTmpFile = DestFile + u".tmp";
		if(TFile::Exists(sTmpFile))
		{
			TFile::Delete(sTmpFile);
		}

		// 再把現有的檔案改名成暫存檔
		if(TFile::Exists(DestFile))
		{
			TFile::Move(DestFile,sTmpFile);
		}

		// 再將結果檔存成目的檔

		TZipFile * Zip = new TZipFile();
		Zip->Open(inf, TZipMode::zmRead);
		Zip->ExtractAll(fmMain->MyFullPath);
		Zip->Close();
		Zip->Free();
#else
		// Mac 版, 把程式放在桌面就好了

		// 取得檔名
		String sFileName = GetHttpFileName(nhrDownload->URL);
		String sFile = fmMain->MyFullPath + u"Desktop/" + sFileName;
		TFileStream * outf = new TFileStream(sFile, fmCreate);
		outf->CopyFrom(inf, inf->Size);
		outf->Free();
		String sMessage = u"新版 CBReader 已儲存在您的桌面上，檔名為 " + sFileName + u"，請您解壓縮後取代原有的 CBReader 即可。";
		TDialogService::ShowMessage(sMessage);
#endif
	}
	else
	{
		// 更新資料

		TZipFile * Zip = new TZipFile();
		Zip->Open(inf, TZipMode::zmRead);
		Zip->ExtractAll(edBookcasePath->Text);
		Zip->Close();
		Zip->Free();
    }
}
//---------------------------------------------------------------------------
// 接收有錯誤，秀出錯誤訊息
void __fastcall TfmUpdate::nhrCBRRequestError(TObject * const Sender, const UnicodeString AError)
{
    // 手動更新的才要秀錯誤訊息
	if(IsShowMessage)
		TDialogService::ShowMessage(AError);
}
//---------------------------------------------------------------------------
// 由 http 網址取得最後的檔名
String __fastcall TfmUpdate::GetHttpFileName(String sURL)
{
	int iPos = sURL.LastDelimiter0("\\/");
	String sFileName = String(sURL.FirstChar()+iPos+1);
	return sFileName;
}
//---------------------------------------------------------------------------

