// ---------------------------------------------------------------------------

#ifdef _Windows
#include <Xml.Win.msxmldom.hpp>
#else
#include <Xml.omnixmldom.hpp>
#endif

#ifndef NavXMLH
#define NavXMLH
#include <fmx.h>
#include <Xml.XMLDoc.hpp>
#include "navitem.h"
// ---------------------------------------------------------------------------
// 這是一個讀取導覽文件的物件

class CNavXML
{
private: // User declarations

	String XMLFile; // XML 檔名
	TList * TreeRoot;
	_di_IXMLDocument Document;

	// Parse 過程中需要暫存的資料
	int ThisLevel;
	SNavItem ThisItem;
	// --------------------------

	// 處理 NavItem
	void __fastcall ClearThisItem(); // 清除 ThisItem
	void __fastcall AddThisItem(); // 將目前的 ThisItem 加入 Tree 並清除

	// 處理標記
	void __fastcall tag_nav(_di_IXMLNode Node);
	void __fastcall tag_ol(_di_IXMLNode Node);
	void __fastcall tag_li(_di_IXMLNode Node);
	void __fastcall tag_a(_di_IXMLNode Node);
	void __fastcall tag_cblink(_di_IXMLNode Node);
	void __fastcall tag_navlink(_di_IXMLNode Node);
	void __fastcall tag_default(_di_IXMLNode Node);

	// 處理 XML
	void __fastcall ParseXML(); // 解析 XML , 儲存到 TreeRoot 中
	void __fastcall ParseNode(_di_IXMLNode Node); // 解析 XML Node
	void __fastcall parseChild(_di_IXMLNode Node); // 解析 XML Child

public: // User declarations

	void __fastcall SaveToTree(TList * TreeRoot);

	__fastcall CNavXML(String sFile);
	__fastcall ~CNavXML();
};
/*
	nav.xhtml 標記與 TreeRoot 的關係

	<nav> : 表示是第一層, 結束上一層的資料
	<ol> : 跳到下一層了
	<li> : 結束上一層的資料, 加入 TreeRoot 中
	文字 : 記錄文字

*/
// ---------------------------------------------------------------------------
#endif
