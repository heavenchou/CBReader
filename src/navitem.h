//---------------------------------------------------------------------------

#ifndef NavItemH
#define NavItemH

// NavTree �𪬵��c���K���C�@�Ӷ��ت��`��
typedef enum {
	nit_Title,      // �S���s�����¤�r
	nit_NormalLink, // �@�몺�s��
	nit_NavLink,    // �s���t�@�ӥؿ�
	nit_CBLink      // �s�� CBETA ���g��
} ENavItemType;

// NavTree �𪬵��c���K���C�@�Ӷ���
typedef struct stNavItem
{
	int Level;      	// �h��, �� 0 �}�l�n�F
	ENavItemType Type; 	// �`��
	String Title;   	// ���D
	String URL;     	// �s������m
} SNavItem;

//---------------------------------------------------------------------------
#endif
