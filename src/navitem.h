//---------------------------------------------------------------------------

#ifndef NavItemH
#define NavItemH

// NavTree 樹狀結構分枝的每一個項目的總類
typedef enum {
	nit_Title,      // 沒有連結的純文字
	nit_NormalLink, // 一般的連結
	nit_NavLink,    // 連結另一個目錄
	nit_CBLink      // 連結 CBETA 的經文
} ENavItemType;

// NavTree 樹狀結構分枝的每一個項目
typedef struct stNavItem
{
	int Level;      	// 層次, 由 0 開始好了
	ENavItemType Type; 	// 總類
	String Title;   	// 標題
	String URL;     	// 連結的位置
} SNavItem;

//---------------------------------------------------------------------------
#endif
