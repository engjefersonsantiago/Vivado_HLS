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
struct FieldFormat {
	uint_16 Offset;
	uint_16 Length;
	IF_SOFTWARE(std::string FieldName;)
};

// Parser match key structrure
template<typename T_Key>
struct KeyFormat {
	T_Key	KeyVal;
	T_Key	KeyMask;
	uint_16	NextHeader;
	IF_SOFTWARE(std::string NextHeaderName;)
};

// Basic Header Format structure
template<uint_16 N_Size, uint_16 N_Fields, typename T_Key, uint_16 N_Key>
class HeaderFormat {
	public:
	//const uint_16 Size = N_Size;
	ap_uint<bytes2Bits(N_Size)> PHVMask;
	std::array<FieldFormat, N_Fields> Fields;
	std::array<KeyFormat<T_Key>, N_Key> Key;
	std::pair<uint_16, uint_16> KeyLocation;	// First: Offset. Second: Lenght
	bool LastHeader;							// Last header: no transition
	IF_SOFTWARE(std::string HeaderName;)
};

// Basic packet data type
template<uint_16 N_BusSize>
struct PacketData {
	ap_uint<N_BusSize> Data;
	uint_64 ID;
	bool Start;
	bool Finish;
	uint_16 HeaderID;
	PacketData() : Data{0}, ID{0}, Start{false}, Finish{false}, HeaderID{0} {}
};

// Basic PHV (packet header vector) type
template<uint_16 N_Size>
struct PHVData {
	ap_uint<bytes2Bits(N_Size)> Data;
	bool Valid;
	uint_64 ID;
};

#endif //_PKT_BASICS_HPP_
