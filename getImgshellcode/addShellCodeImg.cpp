#include "dwBmpSize.h"
#include <iostream>
using namespace std;

CBMPHide::CBMPHide()
{
	sBmpFileName = "";
	pBuf = 0;
	dwBmpSize = 0;
	ptxtBuf = 0;
	pExEBuf = 0;
}

CBMPHide::~CBMPHide()
{

}

bool CBMPHide::setBmpFileName(char* szFileName)
{
	this->sBmpFileName = szFileName;
	if (pBuf)	//����Ѿ����ɾ��ͷŵ�
	{
		delete[]pBuf;
	}

	HANDLE hfile = CreateFileA(szFileName, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, 0);
	if (hfile == INVALID_HANDLE_VALUE)
	{
		return false;
	}

	//��struct BITMAPFILEHEADER bmfh����� bfSize�Ĵ�СӦ����һ���ġ�
	dwBmpSize = GetFileSize(hfile, 0);	//��ȡ�ļ��Ĵ�С
	pBuf = new byte[dwBmpSize + 83968];
	DWORD dwRead = 0;
	ReadFile(hfile, pBuf, dwBmpSize, &dwRead, 0);
	if (dwRead != dwBmpSize)
	{
		delete[]pBuf;
		pBuf = 0;
		return false;
	}
	CloseHandle(hfile);
	m_fileHdr = (BITMAPFILEHEADER*)pBuf;
	m_infoHdr = (BITMAPINFOHEADER*)(pBuf + sizeof(BITMAPFILEHEADER));
	return true;	//�ɹ��������ļ������ݶ�ȡ��pBuf����
}


int CBMPHide::getBmpWidth()
{
	return m_infoHdr->biWidth;
}

int CBMPHide::getBmpHeight()
{
	return m_infoHdr->biHeight;
}

int CBMPHide::getBmpBitCount()
{
	return m_infoHdr->biBitCount;
}

bool CBMPHide::save()
{
	string sDstFileName = "save.bmp";
	HANDLE hfile = CreateFileA(sDstFileName.c_str(),
		GENERIC_READ | GENERIC_WRITE,
		FILE_SHARE_READ | FILE_SHARE_WRITE,
		NULL,
		CREATE_ALWAYS, 0, 0);
	if (hfile == INVALID_HANDLE_VALUE)
	{
		return false;
	}

	DWORD dwWritten = 0;
	WriteFile(hfile, pBuf, dwBmpSize + 83968, &dwWritten, 0);
	if (dwBmpSize != dwWritten)
	{
		return false;
	}
	CloseHandle(hfile);
	return true;
}
//����һ���ַ�����ͼƬ�У����ַ�������ֽڣ�д��ÿ�����ص�alphaͨ����
bool CBMPHide::hideString2BMP(char* szStr2Hide)
{
	LPBYTE pAlpha = pBuf + m_fileHdr->bfOffBits + 3;	//��һ�����ص�ͨ��λ��
	//cout << pAlpha << endl;
	int nHide;	//�ɹ����ص��ֽ���

	//ÿ��ѭ��д��һ���ֽڣ�����alphaͨ��
	//(pAlpha - pBuf) < m_fileHdr->bfSize������ж��ַ�����̫��ͼƬ��������
	for (nHide = 0; (pAlpha - pBuf) < m_fileHdr->bfSize && szStr2Hide[nHide] != 0; nHide++, pAlpha += 4)
	{
		//cout << nHide << endl;
		*pAlpha = szStr2Hide[nHide];	//д��һ���ֽ�
		//cout << pAlpha << endl;
	}

	return true;
}

void CBMPHide::showStringInBmp(char* szBmpFIleName/*=NULL*/)
{
	string sDstFileName = "save.bmp";
	sDstFileName = szBmpFIleName;

	HANDLE hfile = CreateFileA(sDstFileName.c_str(),
		GENERIC_READ | GENERIC_WRITE,
		FILE_SHARE_READ | FILE_SHARE_WRITE,
		NULL,
		OPEN_EXISTING, 0, 0);
	if (hfile == INVALID_HANDLE_VALUE)
	{
		return;
	}
	DWORD dwSize = GetFileSize(hfile, 0);
	LPBYTE pBuf1 = new byte[dwSize];

	DWORD dwRead = 0;

	ReadFile(hfile, pBuf1, dwSize, &dwRead, 0);
	CloseHandle(hfile);

	//�ļ����ݶ�ȡ��pBuf1��
	BITMAPFILEHEADER* pHdr = (BITMAPFILEHEADER*)pBuf1;
	LPBYTE pStr = pBuf1 + pHdr->bfOffBits + 3;
	char szTmp[1280];
	RtlZeroMemory(szTmp, 1280);
	for (int i = 0; i < 1280; i++)
	{
		if (*pStr == 0 || *pStr == 0xFF)
		{
			break;
		}
		szTmp[i] = *pStr;
		pStr += 4;
	}
	printf_s(szTmp);

	delete[]pBuf1;
}

int main(int argc, char* argv[])
{
	if (argc < 2)
	{
		wprintf(L"Command: %S <SHELLCODE> ...\n", argv[0]);
		return -1;
	}


	CBMPHide hide;
	hide.setBmpFileName((char*)"test.bmp");
	printf_s("test.bmp width:%d,height:%d,bitCount%d\n",
		hide.getBmpWidth(),
		hide.getBmpHeight(),
		hide.getBmpBitCount());
	char* shellcode = argv[1];
	hide.hideString2BMP((char*)shellcode);
	hide.save();
	cout << shellcode << endl;
}

