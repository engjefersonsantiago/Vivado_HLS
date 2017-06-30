/******************************************************************************
* pktBasics: Basics types and structures for packet processing                *
* Jeferson Santiago da Silva (eng.jefersonsantiago@gmail.com)                 *
******************************************************************************/

#include <iostream>
#include <math.h>
#include <string>
#include <array>

#include "defined_types.h"

#ifndef _PKT_BASICS_HPP_
#define _PKT_BASICS_HPP_

// Basic Field Format
template<uint16_t N_Size>
struct FieldFormat {
	ap_uint<numbits(bytes2Bits(N_Size))> Offset;
	ap_uint<numbits(bytes2Bits(N_Size))> Length;
	IF_SOFTWARE(std::string FieldName;)
};

// Parser match key structrure
template<typename T_Key, uint16_t N_MaxSuppHeaders>
struct KeyFormat {
	T_Key KeyVal;
	T_Key KeyMask;
	ap_uint<numbits(N_MaxSuppHeaders)> NextHeader;
	IF_SOFTWARE(std::string NextHeaderName;)
};

// Basic Header Format structure
template<uint16_t N_Size, uint16_t N_Fields, typename T_Key, uint16_t N_Key, uint16_t N_MaxSuppHeaders, uint16_t N_HeaderLenArrSize, uint16_t N_HeaderLenELemBits>
struct HeaderFormat {
	ap_uint<bytes2Bits(N_Size)> PHVMask;
	std::array<FieldFormat<N_Size>, N_Fields> Fields;
	std::array<KeyFormat<T_Key, N_MaxSuppHeaders>, N_Key> Key;
	std::pair<ap_uint<numbits(bytes2Bits(N_Size))>, ap_uint<numbits(bytes2Bits(N_Size))>> KeyLocation;	// First: Offset. Second: Lenght
	bool LastHeader;							// Last header: no transition
	IF_SOFTWARE(std::string HeaderName;)
	bool varSizeHeader;							// Variable size header flag
	std::pair<ap_uint<numbits(bytes2Bits(N_Size))>, ap_uint<numbits(bytes2Bits(N_Size))>> HeaderLengthInd;	// First: Offset. Second: Lenght
	std::array<ap_uint<N_HeaderLenELemBits>, N_HeaderLenArrSize> ArrLenLookup;
	// For fixed sized headers. Needs to be redefined for variable sizes
	void getHeaderSize(ap_uint<numbits(bytes2Bits(N_Size))>& size, const ap_uint<numbits(bytes2Bits(N_Size))>& expr_val) {size = bytes2Bits(N_Size);}

	//HeaderFormat(	ap_uint<bytes2Bits(N_Size)> PHVMask_,
	//				std::array<FieldFormat<N_Size>, N_Fields> Fields_,
	//				std::array<KeyFormat<T_Key, N_MaxSuppHeaders>, N_Key> Key_,
	//				std::pair<ap_uint<numbits(bytes2Bits(N_Size))>, ap_uint<numbits(bytes2Bits(N_Size))>> KeyLocation_,
	//				bool LastHeader_,						
	//				IF_SOFTWARE(std::string HeaderName_,)
	//				bool varSizeHeader_,							
	//				std::pair<ap_uint<numbits(bytes2Bits(N_Size))>, ap_uint<numbits(bytes2Bits(N_Size))>> HeaderLengthInd_):
   	//				
	//				PHVMask{PHVMask_},
	//				Fields {Fields_},
	//				Key {Key_},
	//				KeyLocation {KeyLocation_},
	//				LastHeader {LastHeader_},						
	//				IF_SOFTWARE(HeaderName{HeaderName_},)
	//				varSizeHeader{varSizeHeader_},							
	//				HeaderLengthInd{HeaderLengthInd_}	
	//				{}
};

// Basic packet data type
template<uint16_t N_BusSize, uint16_t N_MaxSuppHeaders, uint8_t N_MaxPktId>
struct PacketData {
	ap_uint<N_BusSize> Data;
	ap_uint<N_MaxPktId> ID;
	bool Start;
	bool Finish;
	ap_uint<numbits(N_MaxSuppHeaders)> HeaderID;
	PacketData() : Data{0}, ID{0}, Start{false}, Finish{false}, HeaderID{0} {}
};

// Basic PHV (packet header vector) type
template<uint16_t N_Size, uint16_t N_MaxSuppHeaders, uint8_t N_MaxPktId>
struct PHVData {
	ap_uint<bytes2Bits(N_Size)> Data;
	ap_uint<numbits(bytes2Bits(N_Size))> ExtractedBitNum;
	bool Valid;
	bool ValidPulse;
	ap_uint<numbits(N_MaxSuppHeaders)> ID;
	ap_uint<N_MaxPktId> PktID;
	IF_SOFTWARE(std::string Name;)
	PHVData () : Data{0}, ExtractedBitNum{0}, Valid{false}, ValidPulse{false}, ID{0}, PktID{0} {}	// Not good for non-static vars (wires)

	template<uint16_t Diff_Size>
	PHVData& operator= (const PHVData<Diff_Size, N_MaxSuppHeaders, N_MaxPktId> & Din)
	{
		this->Data = ap_uint<bytes2Bits(N_Size)>(Din.Data);
		this->Valid = Din.Valid;
		this->ID = Din.ID;
		this->PktID = Din.PktID;
		IF_SOFTWARE(this->Name = Din.Name;)
		return *this;
	}

};

#endif //_PKT_BASICS_HPP_
