// ---------------------------------------------------------------------------

#ifdef _Windows
#include <Xml.Win.msxmldom.hpp>
#else
#include <Xml.omnixmldom.hpp>
#endif

#ifndef CBXMLH
#define CBXMLH
#include <fmx.h>
#include <Xml.XMLDoc.hpp>
#include <System.IOUtils.hpp>
#include "setting.h"

// ---------------------------------------------------------------------------
// 這是一個讀取導覽文件的物件

class CCBXML
{
private: // User declarations

	String XMLFile; // XML 檔名
	UnicodeString HTMLText;	// HTML 的結果
	CSetting * Setting; // 呈現用的設定

	_di_IXMLDocument Document;

	void __fastcall MakeHTMLHead(); 	// 先產生 html 的 head
	// --------------------------

	// 處理標記
	void __fastcall tag_a(_di_IXMLNode Node);
	void __fastcall tag_lb(_di_IXMLNode Node);
	void __fastcall tag_pb(_di_IXMLNode Node);
	void __fastcall tag_p(_di_IXMLNode Node);
	void __fastcall tag_div(_di_IXMLNode Node);
	void __fastcall tag_note(_di_IXMLNode Node);
	void __fastcall tag_default(_di_IXMLNode Node);

	// 處理 XML
	void __fastcall ParseXML(); // 解析 XML , 儲存到 TreeRoot 中
	void __fastcall ParseNode(_di_IXMLNode Node); // 解析 XML Node
	void __fastcall parseChild(_di_IXMLNode Node); // 解析 XML Child

public: // User declarations

	void __fastcall SaveToHTML(String sFile);	// 儲存至 HTML

	// 傳入參數為 XML 檔, 呈現的設定
	__fastcall CCBXML(String sFile, CSetting * csSetting);
	__fastcall ~CCBXML();
};
// ---------------------------------------------------------------------------
#endif
