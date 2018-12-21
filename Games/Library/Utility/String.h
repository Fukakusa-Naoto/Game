//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file		String.h
//!
//! @summary	文字列操作に関するヘッダファイル
//!
//! @date		2018.08.15
//!
//! @author 	深草直斗
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/

// 多重インクルードの防止 =========================================================
#pragma once



// ヘッダファイルの読み込み =======================================================
// <標準ヘッダファイル>
#include <string>


// 名前空間の定義 ================================================================
namespace Library
{
	namespace Utility
	{
		namespace String
		{
			// クラスの定義 ======================================================
			class EncodingConverter
			{
				// <メンバ変数>
			private:
				std::vector<char> buffer;


				// <コンストラクタ>
			public:
				//----------------------------------------------------------
				//! @summary   指定したサイズのバッファを持ったコンバータを初期化する
				//!
				//! @parameter [initialBufferSize] 最初のバッファサイズ
				//----------------------------------------------------------
				EncodingConverter(int initialBufferSize = MAX_PATH)
				{
					buffer.resize(initialBufferSize);
				}


				// <デストラクタ>
			public:
				~EncodingConverter() {};


				// <メンバ関数>
			public:
				//----------------------------------------------------------
				//! @summary   UTF8からCP932(std::string)へ変換する
				//!
				//! @parameter [src] 変換する文字列
				//! @parameter [size] 文字列のサイズ
				//! @parameter [out] 変換後の文字列
				//!
				//! @return
				//----------------------------------------------------------
				int Utf8ToCp932(const char* src, int size, std::string *out)
				{
					std::wstring unicode;
					Utf8ToUtf16(src, size, &unicode);
					return Utf16ToCp932(unicode.data(), unicode.length(), out);
				}


				//----------------------------------------------------------
				//! @summary   CP932からUTF8(std::string)へ変換する
				//!
				//! @parameter [src] 変換する文字列
				//! @parameter [length] 文字列の長さ
				//! @parameter [out] 変換後の文字列
				//!
				//! @return
				//----------------------------------------------------------
				int Cp932ToUtf8(const char* src, int length, std::string *out)
				{
					std::wstring unicode;
					Cp932ToUtf16(src, length, &unicode);
					return Utf16ToUtf8(unicode.c_str(), unicode.length(), out);
				}


				//----------------------------------------------------------
				//! @summary   CP932からUTF16(std::wstring)へ変換する
				//!
				//! @parameter [src] 変換する文字列
				//! @parameter [length] 文字列の長さ
				//! @parameter [out] 変換後の文字列
				//!
				//! @return
				//----------------------------------------------------------
				int Cp932ToUtf16(const char *src, int length, std::wstring *out)
				{
					int size;
					size = ::MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, src, length, NULL, NULL);
					buffer.resize(size * sizeof(wchar_t) * 2);
					MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, src, length, (LPWSTR)buffer.data(), buffer.size() * 2);
					out->assign((wchar_t*)buffer.data(), size);
					return size;
				}


				//----------------------------------------------------------
				//! @summary   UTF16からCP932(std::string)へ変換する
				//!
				//! @parameter [src] 変換する文字列
				//! @parameter [length] 文字列の長さ
				//! @parameter [out] 変換後の文字列
				//!
				//! @return
				//----------------------------------------------------------
				int Utf16ToCp932(const wchar_t *src, int length, std::string *out)
				{
					int size;
					size = WideCharToMultiByte(CP_ACP, NULL, src, length, NULL, NULL, NULL, NULL);
					buffer.resize(size);
					WideCharToMultiByte(CP_ACP, NULL, src, length, (LPSTR)buffer.data(), buffer.size(), NULL, NULL);
					out->assign(buffer.data(), size);
					return size;
				}


				//----------------------------------------------------------
				//! @summary   UTF8からUTF16(std::wstring)へ変換する
				//!
				//! @parameter [src] 変換する文字列
				//! @parameter [length] 文字列の長さ
				//! @parameter [out] 変換後の文字列
				//!
				//! @return
				//----------------------------------------------------------
				int Utf8ToUtf16(const char *src, int length, std::wstring *out)
				{
					int size;
					size = ::MultiByteToWideChar(CP_UTF8, 0, src, length, NULL, NULL);
					buffer.resize(size * sizeof(wchar_t));
					MultiByteToWideChar(CP_UTF8, 0, src, length, (LPWSTR)buffer.data(), buffer.size());
					out->swap(std::wstring((wchar_t*)buffer.data(), size));
					return size;
				}


				//----------------------------------------------------------
				//! @summary   UTF16からUTF8(std::string)へ変換する
				//!
				//! @parameter [src] 変換する文字列
				//! @parameter [length] 文字列の長さ
				//! @parameter [out] 変換後の文字列
				//!
				//! @return
				//----------------------------------------------------------
				int Utf16ToUtf8(const wchar_t *src, int length, std::string *out)
				{
					int size;
					size = WideCharToMultiByte(CP_UTF8, NULL, src, length, NULL, NULL, NULL, NULL);
					buffer.resize(size);
					WideCharToMultiByte(CP_UTF8, NULL, src, length, (LPSTR)buffer.data(), buffer.size(), NULL, NULL);
					out->assign(buffer.data(), size);
					return size;
				}
			};


			// 関数の宣言 ========================================================
			//--------------------------------------------------------------
			//! @summary   先頭から指定文字を検索する
			//!
			//! @parameter [str] 検索元の文字列
			//! @parameter [findChar] 検索する文字
			//!
			//! @return    正の数：先頭から数えた文字の場所
			//! @return    -1	 ：見つからなかった
			//--------------------------------------------------------------
			int FindCharFront(const char* str, char findChar);
			int FindCharFront(const wchar_t* str, wchar_t findChar);


			//--------------------------------------------------------------
			//! @summary   指定文字で文字列を切り分ける
			//!
			//! @parameter [source] 切り分ける文字列
			//! @parameter [cutChar] 区切り文字
			//!
			//! @return    分割後の文字列を格納した配列
			//--------------------------------------------------------------
			std::vector<std::string> SliceString(const std::string& source, char cutChar);


			//--------------------------------------------------------------
			//! @summary   指定文字で文字列を切り分ける
			//!
			//! @parameter [source] 切り分ける文字列
			//! @parameter [cutChar] 区切り文字
			//!
			//! @return    分割後の文字列を格納した配列
			//--------------------------------------------------------------
			std::vector<std::wstring> SliceString(const std::wstring& source, wchar_t cutChar);


			//--------------------------------------------------------------
			//! @summary   文字列の中に指定文字が含まれている数を調べる
			//!
			//! @parameter [str] 文字列
			//! @parameter [checkChar] 調べる文字
			//!
			//! @return    文字の数
			//--------------------------------------------------------------
			int CheckChar(const char* str, char checkChar);


			//--------------------------------------------------------------
			//! @summary   文字列の中に指定文字が含まれている数を調べる
			//!
			//! @parameter [str] 文字列
			//! @parameter [checkChar] 調べる文字
			//!
			//! @return    文字の数
			//--------------------------------------------------------------
			int CheckChar(const wchar_t* str, wchar_t checkChar);


			//--------------------------------------------------------------
			//! @summary   stringをwstringに変換する
			//!
			//! @parameter [str] 文字列
			//!
			//! @return    変換した文字列
			//--------------------------------------------------------------
			std::wstring ConvertToWstring(const std::string& str);
		}
	}
}
