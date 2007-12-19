/*
Copyright (c) 2006-2008 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/

#include "fwBase.h"
#include "fwdev.h"
#include "fwVideo.h"
#include "FwSharedCode.h"
#include "TableEntry.h"

using namespace OPT_LEVEL;

/*******************************************************************/


namespace OPT_LEVEL
{

template<typename T>
FwStatus doit(Fw32s const * const pSrcTable, FwVCHuffmanSpec_32s** ppDstSpec);


template<typename T>
typename CheckCodewordPred<T>::result_type CheckCodewordPred<T>::operator()(typename CheckCodewordPred<T>::first_argument_type op1, typename CheckCodewordPred<T>::second_argument_type op2) const
{
	return (op1.getCodeword() == op2.getCodeword());
}



TableEntry::TE::TE()
{
	length = 0;
	codeword = 0;
	value1 = 0;
	value2 = 0;
	nextLevelTable = NULL;
	subtableLengths = NULL;
	offset2End = 1;
}

TableEntry::TE::TE(const TE &t)
{
	length = t.length;
	codeword = t.codeword;
	value1 = t.value1;
	value2 = t.value2;
	nextLevelTable = t.nextLevelTable;
	offset2End = t.offset2End;
	subtableLengths = t.subtableLengths;
}

TableEntry::TE::TE(TE::CodewordType cw)
{
	length = 0;
	codeword = cw;
	value1 = 0;
	value2 = 0;
	nextLevelTable = NULL;
	offset2End = 1;
	subtableLengths = NULL;
}

TableEntry::TE& TableEntry::TE::operator=(const TableEntry::TE &t)
{
	length = t.length;
	codeword = t.codeword;
	value1 = t.value1;
	value2 = t.value2;
	nextLevelTable = t.nextLevelTable;
	offset2End = t.offset2End;
	subtableLengths = t.subtableLengths;

	return *this;
}

TableEntry::TE::~TE()
{
	//the HuffmanTableFree function will reclaim the memory
}
TableEntry::CodewordType TableEntry::TE::getCodeword() const { return codeword; };
bool TableEntry::TE::moreCodewords() const { return (1 < offset2End); };
void TableEntry::TE::noMoreCodewords(){ offset2End = 1; };
const TableEntry::TE* TableEntry::TE::end() const { return (this + offset2End); }; 
bool TableEntry::TE::isLeaf() const { return nextLevelTable == NULL; };





TableEntry::TableEntry()
{
	length = 0;
	codeword = 0;
	value1 = 0;
	value2 = 0;
	nextLevelTable = NULL;
	subtableLengths = NULL;
}

TableEntry::TableEntry(const TableEntry &te)
{
	length = te.length;
	codeword = te.codeword;
	value1 = te.value1;
	value2 = te.value2;
	nextLevelTable = te.nextLevelTable;
	subtableLengths = te.subtableLengths;
}

TableEntry& TableEntry::operator=(const TableEntry &te)
{
	length = te.length;
	codeword = te.codeword;
	value1 = te.value1;
	value2 = te.value2;
	nextLevelTable = te.nextLevelTable;
	subtableLengths = te.subtableLengths;
	return *this;
}

TableEntry::TableEntry(LengthType codewordSize, CodewordType cwt, std::vector<Fw32s> *_stl)
{
	length = codewordSize;
	codeword = cwt;
	subtableLengths = _stl;
	nextLevelTable = NULL;
	value1 = 0;
	value2 = 0;
}

TableEntry::~TableEntry()
{
	//see ReclaimMemory class
}

bool TableEntry::operator==(const TableEntry& t)
{
	return ((length == t.length) && (codeword == t.codeword) && (value1 == t.value1) && (value2 == t.value2) && (nextLevelTable == t.nextLevelTable) && (subtableLengths == t.subtableLengths));
}

bool TableEntry::operator!=(const TableEntry& t)
{
	return !((*this)==t);
}

bool TableEntry::operator<(const TableEntry& t)
{
	bool b1 = (length < t.length);
	bool b1a = (length == t.length);
	bool b2 = (codeword < t.codeword);
	bool b2a = (codeword == t.codeword);
	bool b3 = (value1 < t.value1);
	bool b3a = (value1 == t.value1);
	bool b4 = (value2 < t.value2);
	bool b4a = (value2 == t.value2);
	bool b5 = (nextLevelTable < t.nextLevelTable);
	bool b5a = (nextLevelTable == t.nextLevelTable);
	bool b6 = (subtableLengths < t.subtableLengths);

	return b1 || (b1a && b2) || (b1a && b2a && b3) || (b1a && b2a && b3a && b4) || (b1a && b2a && b3a && b4a && b5) || (b1a && b2a && b3a && b4a && b5a && b6);
}

bool TableEntry::inRange(Fw32s codewordSize)
{
	//does the codeword have more bits than its supposed length?
	return (codeword != 0)? (codeword < (1 << codewordSize)) : true;
}

bool TableEntry::isLeaf() const 
{ 
	return nextLevelTable == NULL; 
};

void STDCALL TableEntry::cleanup(TE* pte, unsigned int depth)
{
	TE* pTraverseTable = pte;
	//free the subtables
	bool bContinue = false;
	do
	{
		if(!pTraverseTable->isLeaf())
		{
			cleanup(pTraverseTable->nextLevelTable, depth+1);
		}
		bContinue = pTraverseTable->moreCodewords() ;
		pTraverseTable++;
	}while(bContinue);

	//free the table
	if(0 == depth)
	{
		delete pte->subtableLengths;
	}
	fwFree(pte);
}

void STDCALL TableEntry::cleanup(std::vector<TableEntry>& rvte)
{
	for(unsigned int I = 0; I < rvte.size(); I++)
	{
		if(!rvte[I].isLeaf())
		{
			cleanup(*rvte[I].nextLevelTable);
			delete rvte[I].nextLevelTable;
		}
	}
}

FwStatus  TableEntry::allocateTable(TE *ppDstSpec, const std::vector<TableEntry>& vte)
{
	assert(ppDstSpec != NULL);
	unsigned int index = 0;
	while(index < vte.size())
	{
		ppDstSpec[index].codeword = vte[index].codeword;
		ppDstSpec[index].length = vte[index].length;
		ppDstSpec[index].value1 = vte[index].value1;
		ppDstSpec[index].value2 = vte[index].value2;
		ppDstSpec[index].offset2End = (vte.size() - index);
		ppDstSpec[index].nextLevelTable = NULL;
		ppDstSpec[index].subtableLengths = vte[index].subtableLengths;

		if(!vte[index].isLeaf())
		{
			ppDstSpec[index].nextLevelTable = static_cast<TE*>(
				fwMalloc((int)vte[index].nextLevelTable->size() * sizeof(TE)));

			if(NULL == ppDstSpec[index].nextLevelTable)
			{
				ppDstSpec[index].nextLevelTable = NULL;
				ppDstSpec[index].noMoreCodewords();
				return fwStsMemAllocErr;
			}
			FwStatus retVal = allocateTable(ppDstSpec[index].nextLevelTable, 
				*vte[index].nextLevelTable);
			if(retVal != fwStsNoErr)
			{
				ppDstSpec[index].noMoreCodewords();
				fwFree(ppDstSpec[index].nextLevelTable);
				return retVal;
			}
		}

		//process the next codeword in this table
		index++;
	}

	return fwStsNoErr;
}

void  TableEntry::insertTableEntry(std::vector<TableEntry>& rvte, int index)
{
	if(length == (*subtableLengths)[index])//codeword length is an exact match for this table
	{
		rvte.push_back(*this);
	}
	else if(length < (*subtableLengths)[index])//we have to padd to match the exact # of bits stored in this table
	{
		for(int I = 0; I < (1 << ((*subtableLengths)[index]-length)); I++)
		{
			TableEntry t;
			t.length = length;
			t.value1 = value1;
			t.value2 = value2;
			t.nextLevelTable = NULL;
			t.codeword = (((codeword) << ((*subtableLengths)[index]-length)) | I);
			t.subtableLengths = subtableLengths;
			rvte.push_back(t);
		}
	}
	else//store parts of the bits in this table and allocate a next level table for the rest
	{
		TableEntry teThisTable, teNextTable;
		teThisTable.length = (*subtableLengths)[index];
		teNextTable.length = length - (*subtableLengths)[index];

		teThisTable.value1 = 0;
		teThisTable.value2 = 0;
		teNextTable.value1 = value1;
		teNextTable.value2 = value2;

		teThisTable.codeword = (codeword >> (length - (*subtableLengths)[index]));
		int mask = (1 << (length - (*subtableLengths)[index])) - 1;
		teNextTable.codeword = (codeword & mask);//get the lower (te.length-subtableLengths[index]) bits

		//fix the subtable length
		teThisTable.subtableLengths = subtableLengths;
		teNextTable.subtableLengths = subtableLengths;

		//check if this prefix already exists
		CheckCodewordPred<TableEntry> tePred;
		std::vector<TableEntry>::iterator it = std::find_if(rvte.begin(), rvte.end(), 
			std::bind2nd(tePred, teThisTable));

		if(rvte.end() == it)
		{
			std::vector<TableEntry> *nextLevelTable_ = new std::vector<TableEntry>();
			if(nextLevelTable_ == NULL)
			{
				teThisTable.nextLevelTable = NULL;
				rvte.push_back(teThisTable);
				throw std::bad_alloc();
			}
			teThisTable.nextLevelTable = nextLevelTable_;//link the tables
			teNextTable.insertTableEntry(*nextLevelTable_, index+1);
			rvte.push_back(teThisTable);
		}
		else
		{
			//the prefix is there, just install the rest
			//but first make sure it is not a codeword that ends here
			assert(!it->isLeaf());
			teNextTable.insertTableEntry(*(it->nextLevelTable), index+1);
		}
	}
}

/********************************************************************/
template<typename T>
 void readValue(TableEntry* const pte, Fw32s const * const pt, unsigned int * const po)
{
	assert(false);
}

template<>
 void readValue<Fw32s>(TableEntry* const pte, Fw32s const * const pt, unsigned int * const po)
{
	pte->setValue1(pt[*po]);
	(*po)++;
}

template<>
 void readValue<std::pair<Fw32s,Fw32s> >(TableEntry* const pte, Fw32s const * const pt, unsigned int * const po)
{
	pte->setValue1(pt[*po]);
	(*po)++;
	pte->setValue2(pt[*po]);
	(*po)++;
}

template<typename T>
FwStatus doit(Fw32s const * const pSrcTable, FwVCHuffmanSpec_32s** ppDstSpec)
{
	//parse the source table
	unsigned int offset = 0;//offset into the source table
	const Fw32s maxCodewordLength = pSrcTable[offset++];//referred to as "maximum length of the code" in the documentation
	const Fw32s numSubtables = pSrcTable[offset++];

	std::vector<Fw32s> *stl = new std::vector<Fw32s>();
	//each subtable holds a number of bits from the codeword
	//TableEntry::subtableLengths[0] tells us how many bits are stored in the first level table
	//TableEntry::subtableLengths[1] tells us how many bits are stored in the second level table
	//etc.
	for(int I = 0; I < numSubtables; I++)
	{
		stl->push_back(pSrcTable[offset++]);
		assert(stl->back() <= maxCodewordLength);
		assert(stl->back() > 0);
	}
	TableEntry::Sum<Fw32s> sum;
	sum = std::for_each(stl->begin(), stl->end(), sum);
	assert(sum.result() == maxCodewordLength);

	std::vector<TableEntry> firstLevelTable;
	ReclaimMemory recMem(&firstLevelTable);
	for(int codewordSize = 1; codewordSize <= maxCodewordLength; codewordSize++)
	{
		int numCodewords = pSrcTable[offset++];//how many codewords of this size?
		for(int J = 0; J < numCodewords; J++)
		{
			TableEntry te(codewordSize, pSrcTable[offset++], stl);
			readValue<T>(&te, pSrcTable, &offset);
			assert(te.inRange(codewordSize));//does the codeword has enough/not more bits for a given codeword size?

			try
			{
				te.insertTableEntry(firstLevelTable, 0);
			}
			catch(std::bad_alloc ba)
			{
				*ppDstSpec = NULL;
				return fwStsMemAllocErr;
			}
		}
	}
	assert(-1 == pSrcTable[offset++]);

	//now allocate the table and initialize
	*ppDstSpec = static_cast<FwVCHuffmanSpec_32s*>(
		fwMalloc((int)(firstLevelTable.size() * sizeof(TableEntry::TE))));

	FwStatus retValue;
	if(NULL == *ppDstSpec)
	{
		retValue = fwStsMemAllocErr;
	}
	else
	{
		retValue = TableEntry::allocateTable(reinterpret_cast<TableEntry::TE*>(*ppDstSpec), 
			firstLevelTable);
		if(fwStsMemAllocErr == retValue)
		{
			fwFree(*ppDstSpec);
			ppDstSpec = 0;
		}
	}
	return retValue;
}

template<typename T,typename M,typename N>
FwStatus STDCALL TableEntry::decode(T** ppBitStream, Fw32s *pOffset, unsigned int depth, const TE *pDecodeTable, M* pFirst, N* pSecond)
{
	assert(ppBitStream != NULL);
	assert(*ppBitStream != NULL);
	assert(pDecodeTable != NULL);
	assert(pOffset != NULL);
	assert(pFirst != NULL);
	assert(pSecond != NULL);

	int numBitsPulledOffBitstream = (*pDecodeTable->subtableLengths)[depth];
	CodewordType data2Bdecoded;
	if(numBitsPulledOffBitstream <= (*pOffset + 1))//the codeword does not spill to the next chunk of the bit stream
	{
		T streamChunk = **ppBitStream;
		data2Bdecoded = (streamChunk >> (*pOffset-numBitsPulledOffBitstream+1));
		data2Bdecoded &= ((1<<numBitsPulledOffBitstream)-1);
	}
	else
	{
		//lower part
		T lowerData2Bdecoded = **ppBitStream;
		lowerData2Bdecoded &= ((1<<(*pOffset+1))-1);

		//upper part
		T upperData2Bdecoded = *((*ppBitStream)+1);
		//offset is BITS_PER_BYTE*sizeof(T)-1 for the upper part
		upperData2Bdecoded >>= (BITS_PER_BYTE*sizeof(T)-1-(numBitsPulledOffBitstream-(*pOffset+1))+1);
		upperData2Bdecoded &= ((1<<(numBitsPulledOffBitstream-(*pOffset+1)))-1);

		data2Bdecoded =(lowerData2Bdecoded << (numBitsPulledOffBitstream - (*pOffset+1))) | upperData2Bdecoded;
	}
	CheckCodewordPred<TableEntry::TE> pred;
	const TableEntry::TE* it = std::find_if(pDecodeTable, pDecodeTable->end(), std::bind2nd(pred, data2Bdecoded));
	if(it == pDecodeTable->end())//codeword not present
	{
		return fwStsH263VLCCodeErr;
	}
	else
	{
		Util::advanceAndNormalize(ppBitStream, pOffset, it->length);//these bits have been consumed
		//go ahead and overwrite.The true values will be the last ones to be written
		*pFirst = CBL_LIBRARY::Limits<M>::Sat(it->value1);
		*pSecond = CBL_LIBRARY::Limits<N>::Sat(it->value2);

		return (it->isLeaf())? fwStsNoErr : decode(ppBitStream, pOffset, depth+1, it->nextLevelTable, pFirst, pSecond);
	}
}

} // namespace OPT_LEVEL

using namespace OPT_LEVEL;

FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiHuffmanTableInitAlloc_32s)(const Fw32s* pSrcTable, FwVCHuffmanSpec_32s** ppDstSpec)
{
	if(FW_REF::PtrNotOK(pSrcTable, ppDstSpec))return fwStsNullPtrErr;

	switch(Dispatch::Type<DT_SSE2>())
	{
	case DT_SSE3:
	case DT_SSE2:
	case DT_REFR:
	default:
		return doit<Fw32s>(pSrcTable, ppDstSpec);
		break;
	}
}

FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiHuffmanRunLevelTableInitAlloc_32s)(const Fw32s* pSrcTable, FwVCHuffmanSpec_32s** ppDstSpec)
{
	if(FW_REF::PtrNotOK(pSrcTable, ppDstSpec))return fwStsNullPtrErr;
	switch(Dispatch::Type<DT_SSE2>())
	{
	case DT_SSE3:
	case DT_SSE2:
	case DT_REFR:
	default:
		return doit<std::pair<Fw32s,Fw32s> >(pSrcTable, ppDstSpec);
		break;
	}
}

FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiHuffmanTableFree_32s)(FwVCHuffmanSpec_32s **ppDecodeTable)
{
	if(FW_REF::PtrNotOK(ppDecodeTable))return fwStsNullPtrErr;

	TableEntry::cleanup(reinterpret_cast<TableEntry::TE*>(*ppDecodeTable));

	return fwStsNoErr;
}

FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiDecodeHuffmanOne_1u32s)(Fw32u** ppBitStream, Fw32s* pOffset, Fw32s* pDst, const FwVCHuffmanSpec_32s *pDecodeTable)
{
	if(FW_REF::PtrNotOK(ppBitStream, pOffset, pDst, pDecodeTable))return fwStsNullPtrErr;
	switch(Dispatch::Type<DT_SSE2>())
	{
	case DT_SSE3:
	case DT_SSE2:
	case DT_REFR:
	default:
		Fw32s tmp;
		return TableEntry::decode(ppBitStream, pOffset, 0, 
			reinterpret_cast<const TableEntry::TE*>(pDecodeTable), pDst, &tmp);
		break;
	}
}

FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiDecodeHuffmanPair_1u16s)(Fw32u** ppBitStream, Fw32s* pOffset, const FwVCHuffmanSpec_32s *pDecodeTable, Fw8s *pFirst, Fw16s *pSecond)
{
	if(FW_REF::PtrNotOK(ppBitStream, pOffset, pFirst, pSecond, pDecodeTable))return fwStsNullPtrErr;
	switch(Dispatch::Type<DT_SSE2>())
	{
	case DT_SSE3:
	case DT_SSE2:
	case DT_REFR:
	default:
		return TableEntry::decode(ppBitStream, pOffset, 0, 
			reinterpret_cast<const TableEntry::TE*>(pDecodeTable), pFirst, pSecond);
		break;
	}
}


// Please do NOT remove the above line for CPP files that need to be multipass compiled
// OREFR OSSE2 
