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
	更新說明

	最新版更新收到格式如下

	message=CBReader 有新的版本，更新後請重新執行本程式，以期和資料格式相符。
	第二行訊息
	第三行訊息
	source=http://www.cbeta.org/cbreader/update/cbreader_0.2.zip
	dest=cbreader
	source=http://www.cbeta.org/cbreader/update/bookcase_0.2.zip
	dest=bookcase

	第一行 message 表示有更新, 否則會傳回 ok
	message 是訊息, 不只一行, 直到出現 source= 才結束
	source=xxx 表示來源, 一定是 zip 檔
	dest=xxx 是目的, 只有二種

	1. cbreader , 表示把 zip 檔解壓縮到主程式目錄 (mac 不適用)
	   這類壓縮檔裡面應該只有一個 CBReader.exe
	2. bookcase , 表示把 zip 檔解壓縮到 bookcase 目錄
	   所以這類壓縮檔應該由 CBETA 目錄開始
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

	String ServerURL;  // 要檢查更新的網頁目錄
	bool UseLocalhostURL;  // 使用 localhost 的測試網址
	String LocalhostURL;    // 內部測試的網址

	// 二個版本
	//String CBRVer;  // CBReader 的版本
	//String DataVer;	// 經文資料的版本

	// 判斷要不要秀訊息
	// 如果傳入參數 true , 表示要秀訊息
	// 自動檢查更新時, 若沒更新就自行離開
	// 手動檢查更新, 沒更新就需要秀訊息 "目前是最新版".
	bool IsShowMessage;

	TStringList * slReceive;	// 接收網頁訊息

	// 檢查需不需要更新, 傳入 cbreader 版本, 資料版本, 以及要不要回應目前是最新的 (手動更新才需要)
	void __fastcall CheckUpdate(String sCBRVer, String sDataVer, bool bShowNoUpdate=false);
	// 由 http 網址取得最後的檔名
	String __fastcall GetHttpFileName(String sURL);
	// 下載檔案
	void __fastcall DL_UP(String sSource, String sDest);
	__fastcall TfmUpdate(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfmUpdate *fmUpdate;
//---------------------------------------------------------------------------
#endif
