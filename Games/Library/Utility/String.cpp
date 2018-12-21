//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file		String.cpp
//!
//! @summary	文字列操作に関するソースファイル
//!
//! @date		2018.08.15
//!
//! @author 	深草直斗
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/

// ヘッダファイルの読み込み =================================================
// <標準ヘッダファイル>
#include <Windows.h>
#include <vector>

// <自作ヘッダファイル>
#include "String.h"


// usingディレクティブ =====================================================
using namespace std;
using namespace Library;


// 関数の定義 ==============================================================
//--------------------------------------------------------------------
//! @summary   先頭から指定文字を検索する
//!
//! @parameter [str] 検索元の文字列
//! @parameter [findChar] 検索する文字
//!
//! @return    正の数：先頭から数えた文字の場所
//! @return    -1	 ：見つからなかった
//--------------------------------------------------------------------
int Utility::String::FindCharFront(const char* str, char findChar)
{
	int count = 0;
	while (*str != '\0')
	{
		if (*str == findChar) return count;
		++count;
		++str;
	}
	return -1;
}



//--------------------------------------------------------------------
//! @summary   先頭から指定文字を検索する
//!
//! @parameter [str] 検索元の文字列
//! @parameter [findChar] 検索する文字
//!
//! @return    正の数：先頭から数えた文字の場所
//! @return    -1	 ：見つからなかった
//--------------------------------------------------------------------
int Utility::String::FindCharFront(const wchar_t* str, wchar_t findChar)
{
	int count = 0;
	while (*str != '\0')
	{
		if (*str == findChar) return count;
		++count;
		++str;
	}
	return -1;
}



//--------------------------------------------------------------------
//! @summary   指定文字で文字列を切り分ける
//!
//! @parameter [source] 切り分ける文字列
//! @parameter [cutChar] 区切り文字
//!
//! @return    分割後の文字列を格納した配列
//--------------------------------------------------------------------
vector<string> Utility::String::SliceString(const string& source, char cutChar)
{
	// 区切り文字がなかった
	if (FindCharFront(source.c_str(), cutChar) == -1) return vector<string>();

	int stringCount = 0;
	int queueCount = 0;

	// 分割後の文字列を格納する配列
	vector<string> queue = vector<string>(CheckChar(source.c_str(), cutChar) + 1);

	while (source[stringCount] != '\0')
	{
		if (source[stringCount] == cutChar)
		{
			++stringCount;
			++queueCount;
		}
		else queue[queueCount] += source[stringCount++];
	}

	return queue;
}



//--------------------------------------------------------------------
//! @summary   指定文字で文字列を切り分ける
//!
//! @parameter [source] 切り分ける文字列
//! @parameter [cutChar] 区切り文字
//!
//! @return    分割後の文字列を格納した配列
//--------------------------------------------------------------------
vector<wstring> Utility::String::SliceString(const wstring& source, wchar_t cutChar)
{
	// 区切り文字がなかった
	if (FindCharFront(source.c_str(), cutChar) == -1) return vector<wstring>();

	int stringCount = 0;
	int queueCount = 0;

	// 分割後の文字列を格納する配列
	vector<wstring> queue = vector<wstring>(CheckChar(source.c_str(), cutChar) + 1);

	while (source[stringCount] != '\0')
	{
		if (source[stringCount] == cutChar)
		{
			++stringCount;
			++queueCount;
		}
		else queue[queueCount] += source[stringCount++];
	}

	return queue;
}



//--------------------------------------------------------------------
//! @summary   文字列の中に指定文字が含まれている数を調べる
//!
//! @parameter [str] 文字列
//! @parameter [checkChar] 調べる文字
//!
//! @return    文字の数
//--------------------------------------------------------------------
int Utility::String::CheckChar(const char* str, char checkChar)
{
	if (FindCharFront(str, checkChar) == -1) return 0;

	int count = 0;
	while (*str != '\0')
	{
		if (*str == checkChar) ++count;
		++str;
	}
	return count;
}



//--------------------------------------------------------------------
//! @summary   文字列の中に指定文字が含まれている数を調べる
//!
//! @parameter [str] 文字列
//! @parameter [checkChar] 調べる文字
//!
//! @return    文字の数
//--------------------------------------------------------------------
int Utility::String::CheckChar(const wchar_t* str, wchar_t checkChar)
{
	if (FindCharFront(str, checkChar) == -1) return 0;

	int count = 0;
	while (*str != '\0')
	{
		if (*str == checkChar) ++count;
		++str;
	}
	return count;
}



//--------------------------------------------------------------------
//! @summary   stringをwstringに変換する
//!
//! @parameter [str] 文字列
//!
//! @return    変換した文字列
//--------------------------------------------------------------------
wstring Utility::String::ConvertToWstring(const string& str)
{
	// 文字列がない
	if (str.size() == 0) return wstring();

	size_t charsNeeded = ::MultiByteToWideChar(CP_UTF8, 0,
		str.data(), (int)str.size(), NULL, 0);
	if (charsNeeded == 0)
		throw runtime_error("Failed converting UTF-8 string to UTF-16");

	vector<wchar_t> buffer(charsNeeded);
	int charsConverted = ::MultiByteToWideChar(CP_UTF8, 0,
		str.data(), (int)str.size(), &buffer[0], buffer.size());
	if (charsConverted == 0)
		throw runtime_error("Failed converting UTF-8 string to UTF-16");

	return wstring(&buffer[0], charsConverted);
}
