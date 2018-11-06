// WhatsAppSender.cpp : a c++ class for sending WhatsApp messages
// using the WhatsAppMate API https://www.whatsmate.net/whatsapp-gateway-api.html 
// By Michael Haephrati, Secured Globe, Inc.

#include "stdafx.h"
#include "SGWhatsApp.h"

#include <string>

#include <Wininet.h>
#include <Wincrypt.h>
#pragma comment(lib,"wininet.lib")
#pragma comment(lib,"crypt32.lib")

#define BUFFER_LENGTH 1024


SGWhatsApp::SGWhatsApp()
{
}


SGWhatsApp::~SGWhatsApp()
{
}

bool SGWhatsApp::SendGroupMessage(LPCTSTR ClientID, LPCTSTR ClientSecret, LPCTSTR groupAdmin, LPCTSTR groupName, LPCTSTR message)
{
	BOOL bResults = FALSE;
	HINTERNET hOpenHandle, hConnectHandle;
	const TCHAR* szHeaders = _T("Content-Type:application/json; charset=utf-8\r\n");


	hOpenHandle = InternetOpen(_T("HTTP"), INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
	if (hOpenHandle == NULL)
	{
		return bResults;
	}


	hConnectHandle = InternetConnect(hOpenHandle,
		GROUP_API_SERVER,
		INTERNET_DEFAULT_HTTP_PORT,
		NULL, NULL, INTERNET_SERVICE_HTTP,
		0, 1);

	if (hConnectHandle == NULL)
	{
		InternetCloseHandle(hOpenHandle);
		return bResults;
	}

	const wchar_t *AcceptTypes[] = { _T("application/json"),NULL };
	HINTERNET hRequest = HttpOpenRequest(hConnectHandle, _T("POST"), GROUP_API_PATH, NULL, NULL, AcceptTypes, 0, 0);

	if (hRequest == NULL)
	{
		InternetCloseHandle(hConnectHandle);
		InternetCloseHandle(hOpenHandle);
		return bResults;
	}

	std::wstring HeaderData;

	HeaderData += _T("X-WM-CLIENT-ID: ");
	HeaderData += ClientID;
	HeaderData += _T("\r\nX-WM-CLIENT-SECRET: ");
	HeaderData += ClientSecret;
	HeaderData += _T("\r\n");
	HttpAddRequestHeaders(hRequest, HeaderData.c_str(), HeaderData.size(), NULL);

	std::wstring WJsonData;
	WJsonData += _T("{");
	WJsonData += _T("\"group_admin\":\"");
	WJsonData += groupAdmin;
	WJsonData += _T("\",");
	WJsonData += _T("\"group_name\":\"");
	WJsonData += groupName;
	WJsonData += _T("\",");
	WJsonData += _T("\"message\":\"");
	WJsonData += message;
	WJsonData += _T("\"");
	WJsonData += _T("}");

	const std::string JsonData(WJsonData.begin(), WJsonData.end());

	bResults = HttpSendRequest(hRequest, NULL, 0, (LPVOID)(JsonData.c_str()), JsonData.size());

	TCHAR StatusText[BUFFER_LENGTH] = { 0 };
	DWORD StatusTextLen = BUFFER_LENGTH;
	HttpQueryInfo(hRequest, HTTP_QUERY_STATUS_TEXT, &StatusText, &StatusTextLen, NULL);
	bResults = (StatusTextLen && wcscmp(StatusText, L"OK") == FALSE);


	InternetCloseHandle(hConnectHandle);
	InternetCloseHandle(hOpenHandle);

	return bResults;
}


bool SGWhatsApp::SendGroupDocument(LPCTSTR ClientID, LPCTSTR ClientSecret, LPCTSTR groupAdmin, LPCTSTR groupName, /*LPCTSTR message,*/ LPCTSTR filename, LPBYTE attachment, int length)
{
    BOOL bResults = FALSE;
    HINTERNET hOpenHandle, hConnectHandle;
    const TCHAR* szHeaders = _T("Content-Type:application/json; charset=utf-8\r\n");


    hOpenHandle = InternetOpen(_T("HTTP"), INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
    if (hOpenHandle == NULL)
    {
        return bResults;
    }


    hConnectHandle = InternetConnect(hOpenHandle,
        DOCUMENT_API_SERVER,
        INTERNET_DEFAULT_HTTP_PORT,
        NULL, NULL, INTERNET_SERVICE_HTTP,
        0, 1);

    if (hConnectHandle == NULL)
    {
        InternetCloseHandle(hOpenHandle);
        return bResults;
    }

    const wchar_t *AcceptTypes[] = { _T("application/json"),NULL };
    HINTERNET hRequest = HttpOpenRequest(hConnectHandle, _T("POST"), DOCUMENT_API_PATH, NULL, NULL, AcceptTypes, 0, 0);

    if (hRequest == NULL)
    {
        InternetCloseHandle(hConnectHandle);
        InternetCloseHandle(hOpenHandle);
        return bResults;
    }

    std::wstring HeaderData;

    HeaderData += _T("X-WM-CLIENT-ID: ");
    HeaderData += ClientID;
    HeaderData += _T("\r\nX-WM-CLIENT-SECRET: ");
    HeaderData += ClientSecret;
    HeaderData += _T("\r\n");
    HttpAddRequestHeaders(hRequest, HeaderData.c_str(), HeaderData.size(), NULL);

    std::wstring document = _T("");
    DWORD desiredLength = 0;
    CryptBinaryToString(attachment, length, CRYPT_STRING_BASE64, NULL, &desiredLength);
    desiredLength++;
    TCHAR* base64content = (TCHAR*)malloc(desiredLength * sizeof(TCHAR));
    CryptBinaryToString(attachment, length, CRYPT_STRING_BASE64, base64content, &desiredLength);

    std::wstring WJsonData;
    WJsonData += _T("{");
    WJsonData += _T("\"group_admin\":\"");
    WJsonData += groupAdmin;
    WJsonData += _T("\",");
    WJsonData += _T("\"group_name\":\"");
    WJsonData += groupName;
    WJsonData += _T("\",");
    //WJsonData += _T("\"message\":\"");
    //WJsonData += message;
    //WJsonData += _T("\"");

    WJsonData += _T("\"filename\":\"");
    WJsonData += filename;
    WJsonData += _T("\",");

    WJsonData += _T("\"document\":\"");    
    // Needed to remove CRLF and all spaces symbols
    for(size_t i=0; i<lstrlen(base64content); i++)
        if (!isspace(base64content[i]))
            WJsonData += base64content[i];
    WJsonData += _T("\"");

    WJsonData += _T("}");

    free(base64content);
    const std::string JsonData(WJsonData.begin(), WJsonData.end());

    bResults = HttpSendRequest(hRequest, NULL, 0, (LPVOID)(JsonData.c_str()), JsonData.size());

    TCHAR StatusText[BUFFER_LENGTH] = { 0 };
    DWORD StatusTextLen = BUFFER_LENGTH;
    HttpQueryInfo(hRequest, HTTP_QUERY_STATUS_TEXT, &StatusText, &StatusTextLen, NULL);
    bResults = (StatusTextLen && wcscmp(StatusText, L"OK") == FALSE);

    DWORD availableBytes = 0;
    DWORD downloadedBytes = 0;
    LPBYTE nextBytes = (LPBYTE)malloc((availableBytes +1) * sizeof(TCHAR));
    memset(nextBytes, 0, (availableBytes + 1) * sizeof(TCHAR));
    InternetQueryDataAvailable(hRequest, &availableBytes, 0, 0);
    InternetReadFile(hRequest, nextBytes, availableBytes, &downloadedBytes);
    free(nextBytes);

    InternetCloseHandle(hConnectHandle);
    InternetCloseHandle(hOpenHandle);

    return bResults;
}

bool SGWhatsApp::SendGroupImage(LPCTSTR ClientID, LPCTSTR ClientSecret, LPCTSTR groupAdmin, LPCTSTR groupName, LPCTSTR caption, LPBYTE attachment, int length)
{
    BOOL bResults = FALSE;
    HINTERNET hOpenHandle, hConnectHandle;
    const TCHAR* szHeaders = _T("Content-Type:application/json; charset=utf-8\r\n");


    hOpenHandle = InternetOpen(_T("HTTP"), INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
    if (hOpenHandle == NULL)
    {
        return bResults;
    }


    hConnectHandle = InternetConnect(hOpenHandle,
        IMAGE_API_SERVER,
        INTERNET_DEFAULT_HTTP_PORT,
        NULL, NULL, INTERNET_SERVICE_HTTP,
        0, 1);

    if (hConnectHandle == NULL)
    {
        InternetCloseHandle(hOpenHandle);
        return bResults;
    }

    const wchar_t *AcceptTypes[] = { _T("application/json"),NULL };
    HINTERNET hRequest = HttpOpenRequest(hConnectHandle, _T("POST"), IMAGE_API_PATH, NULL, NULL, AcceptTypes, 0, 0);

    if (hRequest == NULL)
    {
        InternetCloseHandle(hConnectHandle);
        InternetCloseHandle(hOpenHandle);
        return bResults;
    }

    std::wstring HeaderData;

    HeaderData += _T("X-WM-CLIENT-ID: ");
    HeaderData += ClientID;
    HeaderData += _T("\r\nX-WM-CLIENT-SECRET: ");
    HeaderData += ClientSecret;
    HeaderData += _T("\r\n");
    HttpAddRequestHeaders(hRequest, HeaderData.c_str(), HeaderData.size(), NULL);

    std::wstring document = _T("");
    DWORD desiredLength = 0;
    CryptBinaryToString(attachment, length, CRYPT_STRING_BASE64, NULL, &desiredLength);
    desiredLength++;
    TCHAR* base64content = (TCHAR*)malloc(desiredLength * sizeof(TCHAR));
    CryptBinaryToString(attachment, length, CRYPT_STRING_BASE64, base64content, &desiredLength);

    std::wstring WJsonData;
    WJsonData += _T("{");
    WJsonData += _T("\"group_admin\":\"");
    WJsonData += groupAdmin;
    WJsonData += _T("\",");
    WJsonData += _T("\"group_name\":\"");
    WJsonData += groupName;
    WJsonData += _T("\",");
    //WJsonData += _T("\"message\":\"");
    //WJsonData += message;
    //WJsonData += _T("\"");

    WJsonData += _T("\"caption\":\"");
    WJsonData += caption;
    WJsonData += _T("\",");

    WJsonData += _T("\"image\":\"");
    // Needed to remove CRLF and all spaces symbols
    for (size_t i = 0; i<lstrlen(base64content); i++)
        if (!isspace(base64content[i]))
            WJsonData += base64content[i];
    WJsonData += _T("\"");

    WJsonData += _T("}");

    free(base64content);
    const std::string JsonData(WJsonData.begin(), WJsonData.end());

    bResults = HttpSendRequest(hRequest, NULL, 0, (LPVOID)(JsonData.c_str()), JsonData.size());

    TCHAR StatusText[BUFFER_LENGTH] = { 0 };
    DWORD StatusTextLen = BUFFER_LENGTH;
    HttpQueryInfo(hRequest, HTTP_QUERY_STATUS_TEXT, &StatusText, &StatusTextLen, NULL);
    bResults = (StatusTextLen && wcscmp(StatusText, L"OK") == FALSE);

    DWORD availableBytes = 0;
    DWORD downloadedBytes = 0;
    LPBYTE nextBytes = (LPBYTE)malloc((availableBytes + 1) * sizeof(TCHAR));
    memset(nextBytes, 0, (availableBytes + 1) * sizeof(TCHAR));
    InternetQueryDataAvailable(hRequest, &availableBytes, 0, 0);
    InternetReadFile(hRequest, nextBytes, availableBytes, &downloadedBytes);
    free(nextBytes);

    InternetCloseHandle(hConnectHandle);
    InternetCloseHandle(hOpenHandle);

    return bResults;
}