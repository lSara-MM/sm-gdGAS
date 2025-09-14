//#include "smBitSet.h"
//
//void sm::BitSet::Set(int pos, bool value)
//{
//	constexpr int blockSize = sizeof(T);
//
//	int block = pos / blockSize;
//
//	int subBlockPos = pos - (block * blockSize);
//	data[block] |= (1 << subBlockPos);
//}