#define  _CRT_SECURE_NO_WARNINGS
#pragma  once
#include <string>
#include <Windows.h>
using namespace std;

class CBMPHide
{
public:
	CBMPHide();
	~CBMPHide();

	bool setBmpFileName(char* szFileName);	//����Bmp�ļ���

	int getBmpWidth();	//��ȡ���
	int getBmpHeight();	//��ȡ�߶�
	int getBmpBitCount();	//��ȡBit����
	bool save();

	bool hideString2BMP(char* szStr2Hide);	//����String��BMP�ļ���
	void showStringInBmp(char* szBmpFIleName = NULL);	//չʾ

	void savetxtFile(char* FileName);	//����txt�ļ���bmpͼ����
	void showtxtFile(char* szBmpFIleName = NULL);	//���ܳ�txtFile

	void saveExeFile(char* FileName);
	void showExeFile(char* szBmpFIleName = NULL);

private:
	DWORD dwBmpSize;	//ͼƬ�ļ���С
	DWORD dwTxTSize;
	DWORD dwExESize;

	string sBmpFileName;
	string sTxTFileName;
	string sExEFileName;

	LPBYTE pBuf;	//���ڴ��ͼƬ��Ϣ���ڴ�
	LPBYTE ptxtBuf;	//���ڴ��txt��Ϣ���ڴ�
	LPBYTE pExEBuf;	//���ڴ��exe��Ϣ���ڴ�

	BITMAPFILEHEADER* m_fileHdr;
	BITMAPINFOHEADER* m_infoHdr;
};