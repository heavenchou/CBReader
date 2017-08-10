//---------------------------------------------------------------------------

#ifndef CBXMLOptionH
#define CBXMLOptionH

// ---------------------------------------------------------------------------
// CBETA XML 經文的各種呈現設定

class CCBXMLOption
{
private: // User declarations

public: // User declarations

	bool ShowLineHead;  // 呈現行首資訊 (段首資料)
	bool ShowLineFormat; // 是否呈現原書格式 (反之為段落呈現)

	__fastcall CCBXMLOption();
};
//---------------------------------------------------------------------------
#endif
