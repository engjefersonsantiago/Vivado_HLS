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
template<uint16_t N_Size, uint16_t N_Fields, typename T_Key, uint16_t N_Key, uint16_t N_MaxSuppHeaders>
struct HeaderFormat {
	ap_uint<bytes2Bits(N_Size)> PHVMask;
	std::array<FieldFormat<N_Size>, N_Fields> Fields;
	std::array<KeyFormat<T_Key, N_MaxSuppHeaders>, N_Key> Key;
	std::pair<ap_uint<numbits(bytes2Bits(N_Size))>, ap_uint<numbits(bytes2Bits(N_Size))>> KeyLocation;	// First: Offset. Second: Lenght
	bool LastHeader;							// Last header: no transition
	IF_SOFTWARE(std::string HeaderName;)
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
	bool Valid;
	ap_uint<numbits(N_MaxSuppHeaders)> ID;
	ap_uint<N_MaxPktId> PktID;
	IF_SOFTWARE(std::string Name;)
	//PHVData () : Data{0}, Valid{false}, ID{0}, PktID{0} {}	// Not good for non-static vars (wires)
};

#endif //_PKT_BASICS_HPP_
