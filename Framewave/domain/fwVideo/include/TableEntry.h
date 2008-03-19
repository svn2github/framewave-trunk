/*
Copyright (c) 2006-2008 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/

#ifndef _TABLE_ENTRY_H
#define _TABLE_ENTRY_H
/*************************************************************/
#include <algorithm>
#include <exception>
#include <functional>
#include <iostream>
#include <sstream>
#include <stack>
#include <utility>
#include <vector>

/*************************************************************/
#define BITS_PER_BYTE 8

/*************************************************************/

namespace OPT_LEVEL
{
class TableEntry;

template<typename T>
class CheckCodewordPred
{
public:
	typedef T first_argument_type;
	typedef T second_argument_type;
	typedef bool result_type;
	result_type operator()(first_argument_type, second_argument_type) const;
};

class Util
{
public:
	/*
	 *
	 * @param pOffset offset inside the doubleword. Belongs to [0,BITS_PER_BYTE*sizeof(Fw32u)-1]
	 */
	template<typename T>
	static void advanceAndNormalize(T** ppBitStream, Fw32s *pOffset, int amountBits2Advance)
	{
		if(amountBits2Advance <= *pOffset)
		{
			*pOffset -= amountBits2Advance;
		}
		else
		{
			//this code assumes that the amount to advance cannot be more than BITS_PER_BYTE*sizeof(T)
			//this assumption is made because the amount to advance is derived from the length
			//of a codeword, which currently is a BITS_PER_BYTE*sizeof(T)-bit number
			assert(amountBits2Advance < BITS_PER_BYTE*sizeof(T));
			(*ppBitStream)++;
			*pOffset = BITS_PER_BYTE*sizeof(T) -(amountBits2Advance - (*pOffset));
		}
	}
};

/*************************************************************/
class TableEntry
{
public:
	typedef Fw32s LengthType;//since all codewords are contained in pSrcTable which is points to Fw32s
	//the maximum length of a codeword cannot be more than 32, therefore a Fw8u could be enough. This
	//type will be tweaked to achieve better performance.
	typedef Fw32s CodewordType;
	typedef Fw32s ValueType;

	template<class T>
	class Sum
	{
		T res;
	public:
		Sum(T i = 0) : res(i) {}
		void operator()(T x) { res += x; }
		T result() const { return res; }
	};

	struct TE
	{
		typedef TableEntry::CodewordType CodewordType;
		LengthType length;
		CodewordType codeword;
		ValueType value1;
		ValueType value2;
		struct TE *nextLevelTable;
		size_t offset2End;//1 indicates last TE in this subtable, 2 second to last, etc
		std::vector<Fw32s> *subtableLengths;

		TE();
		TE(const TE &t);
		TE(TE::CodewordType cw);
		TE& operator=(const TE &t);
		~TE();
		CodewordType getCodeword() const;
		bool moreCodewords() const;
		void noMoreCodewords();
		const TE* end() const; 
		bool isLeaf() const;
	};

	typedef struct TE TE;

	TableEntry();
	TableEntry(const TableEntry &te);
	TableEntry& operator=(const TableEntry &te);
	TableEntry(LengthType codewordSize, CodewordType cwt, std::vector<Fw32s> *_stl);
	virtual ~TableEntry();
	bool operator==(const TableEntry& t);
	bool operator!=(const TableEntry& t);
	bool operator<(const TableEntry& t);

	void insertTableEntry(std::vector<TableEntry>& rvte, int index);
	static FwStatus allocateTable(TE*, const std::vector<TableEntry>&);
	static void STDCALL cleanup(TE*, unsigned int depth = 0);
	static void STDCALL cleanup(std::vector<TableEntry>&);
	template<typename T,typename M,typename N>
	static FwStatus STDCALL decode(T**, Fw32s*, unsigned int,const TE*, M*, N*);

	LengthType getLength() const {return length;};
	CodewordType getCodeword() const {return codeword;};
	ValueType getValue1() const { return value1; };
	ValueType getValue2() const { return value2; };
	std::vector<TableEntry>* getNextLevelTable() const { return nextLevelTable; };
	void setValue1(ValueType v){value1 = v;};
	void setValue2(ValueType v){value2 = v;};
	void setNextLevelTable(std::vector<TableEntry>* pvte){ nextLevelTable = pvte; };
	bool inRange(Fw32s codewordSize);
	bool isLeaf() const;

private:
	LengthType length;
	CodewordType codeword;
	ValueType value1;
	ValueType value2;
	std::vector<TableEntry> *nextLevelTable;
	//each subtable holds a number of bits from the codeword
	//subtableLengts[0] tells us how many bits are stored in the first level table
	//subtableLengths[1] tells us how many bits are stored in the second level table
	//etc.
	std::vector<Fw32s> *subtableLengths;

};

class ReclaimMemory
{
private:
	std::vector<TableEntry>* rvte;
public:
	ReclaimMemory(std::vector<TableEntry>* _rvte):rvte(_rvte){};
	ReclaimMemory(const ReclaimMemory& rrm):rvte(rrm.rvte){};
	ReclaimMemory& operator=(const ReclaimMemory& rrm){rvte = rrm.rvte; return *this;};
	~ReclaimMemory(){TableEntry::cleanup(*rvte);};
};

}

#endif
