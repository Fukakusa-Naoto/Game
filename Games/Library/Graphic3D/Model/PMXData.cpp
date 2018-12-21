#include "PMXData.h"

// 関数の定義 ========================================================
// インデックス値を読み込む
int Library::Graphic3D::Model::ReadIndex(std::istream *stream, int size)
{
	switch (size)
	{
	case 1:
		uint8_t tmp8;
		stream->read((char*)&tmp8, sizeof(uint8_t));
		if (255 == tmp8) return -1;
		else return (int)tmp8;
	case 2:
		uint16_t tmp16;
		stream->read((char*)&tmp16, sizeof(uint16_t));
		if (65535 == tmp16) return -1;
		else return (int)tmp16;
	case 4:
		int tmp32;
		stream->read((char*)&tmp32, sizeof(int));
		return tmp32;
	default:
		return -1;
	}
}

