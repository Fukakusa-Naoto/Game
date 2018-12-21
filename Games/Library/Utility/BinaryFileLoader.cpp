//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file		BinaryFileLoader.cpp
//!
//! @summary	バイナリ・ファイルの読み込みに関する処理を記述したソースファイル
//!
//! @date		2018.09.18
//!
//! @author 	深草直斗
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/

// ヘッダファイルの読み込み =================================================
// <標準ヘッダファイル>
#include <windows.h>
#include <fstream>

// <自作ヘッダファイル>
#include "BinaryFileLoader.h"


// usingディレクティブ =====================================================
using namespace std;
using namespace Library;


// 関数の定義 ==============================================================
//------------------------------------------------------------------
//! @summary   バイナリファイルの読み込み
//!
//! @parameter [fileName] 読み込むファイル名
//!
//! @return    読み込んだバイナリデータ
//------------------------------------------------------------------
Utility::BinaryData Utility::LoadBinaryFile(const wchar_t* fileName)
{
	// 読み込んだデータの入れ物
	Utility::BinaryData vessel;
	// ファイル操作
	ifstream ifs;

	// ファイルを開く
	ifs.open(fileName, ios::in | ios::binary);

	// 読み込み失敗時、強制終了
	if (!ifs)
	{
		wstring message = fileName + wstring(L"が開けませんでした");
		MessageBox(NULL, message.c_str(), TEXT("エラー"), MB_OK);
		PostQuitMessage(0);
	}

	// ファイルサイズを取得
	ifs.seekg(0, fstream::end);
	streamoff eofPos = ifs.tellg();
	ifs.clear();
	ifs.seekg(0, std::fstream::beg);
	streamoff begPos = ifs.tellg();
	vessel.size = (unsigned int)(eofPos - begPos);

	// 読み込むためのメモリを確保
	vessel.data.reset(new char[vessel.size]);

	// ファイル先頭からバッファへコピー
	ifs.read(vessel.GetData(), vessel.size);

	// ファイルクローズ
	ifs.close();

	return move(vessel);
}
