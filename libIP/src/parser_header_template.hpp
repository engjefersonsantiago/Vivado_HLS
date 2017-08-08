/******************************************************************************
* parser_header_template: Self-generated template structs for the parser      *
* Jeferson Santiago da Silva (eng.jefersonsantiago@gmail.com)                 *
******************************************************************************/

#include <iostream>
#include <math.h>
#include <string>
#include <array>

#include "pktBasics.hpp"

#ifndef _PARSER_HEADER_TEMP_HPP_
#define _PARSER_HEADER_TEMP_HPP_

#define HEADER_NUM 12

#define MAX_SUPP_HEADERS 12

#define MAX_HEADER_SIZE 40

#define MAX_HEADER_SIZE_BITS bytes2Bits(MAX_HEADER_SIZE)

#define ETHERNET_HEADER_SIZE 14

#define ETHERNET_HEADER_SIZE_BITS bytes2Bits(ETHERNET_HEADER_SIZE)

#define ETHERNET_NUM_FIELDS 3

const HeaderFormat<14, 3, ap_uint<16>, 5, MAX_SUPP_HEADERS , 1, 1> ethernet_t
{
	(ap_uint<112>("5192296858534827628530496329220095")),
	std::array<FieldFormat< 14>,3>{
		{
			{0, 48 IF_SOFTWARE(, "dstAddr")},
			{48, 48 IF_SOFTWARE(, "srcAddr")},
			{96, 16 IF_SOFTWARE(, "etherType")}
		},
	},
	std::array<KeyFormat<ap_uint<16>, 12>,5>{
		{
			{0x0800, 65535, 6 IF_SOFTWARE(, "ipv4")},
			{0x8100, 65535, 3 IF_SOFTWARE(, "inner_vlan")},
			{0x9100, 65535, 2 IF_SOFTWARE(, "outer_vlan")},
			{0x8847, 65535, 4 IF_SOFTWARE(, "outer_mpls")},
			{0x86dd, 65535, 7 IF_SOFTWARE(, "ipv6")}
		},
	},
	std::pair<ap_uint<7>, ap_uint<7>>{96,16},
	(false),
	IF_SOFTWARE("ethernet_t",)
	(false),
	std::pair<ap_uint<7>, ap_uint<7>>{0,1},
	{{1}}
};

#define OUTER_VLAN_HEADER_SIZE 4

#define OUTER_VLAN_HEADER_SIZE_BITS bytes2Bits(OUTER_VLAN_HEADER_SIZE)

#define OUTER_VLAN_NUM_FIELDS 4

const HeaderFormat<4, 4, ap_uint<16>, 1, MAX_SUPP_HEADERS , 1, 1> vlan_outer_t
{
	(ap_uint<32>("4294967295")),
	std::array<FieldFormat< 4>,4>{
		{
			{0, 3 IF_SOFTWARE(, "PCP")},
			{3, 1 IF_SOFTWARE(, "DEI")},
			{4, 12 IF_SOFTWARE(, "VID")},
			{16, 16 IF_SOFTWARE(, "etherType")}
		},
	},
	std::array<KeyFormat<ap_uint<16>, 12>,1>{
		{
			{0x8100, 65535, 3 IF_SOFTWARE(, "inner_vlan")}
		},
	},
	std::pair<ap_uint<6>, ap_uint<6>>{16,16},
	(false),
	IF_SOFTWARE("vlan_outer_t",)
	(false),
	std::pair<ap_uint<6>, ap_uint<6>>{0,1},
	{{1}}
};

#define INNER_VLAN_HEADER_SIZE 4

#define INNER_VLAN_HEADER_SIZE_BITS bytes2Bits(INNER_VLAN_HEADER_SIZE)

#define INNER_VLAN_NUM_FIELDS 4

const HeaderFormat<4, 4, ap_uint<16>, 2, MAX_SUPP_HEADERS , 1, 1> vlan_inner_t
{
	(ap_uint<32>("4294967295")),
	std::array<FieldFormat< 4>,4>{
		{
			{0, 3 IF_SOFTWARE(, "PCP")},
			{3, 1 IF_SOFTWARE(, "DEI")},
			{4, 12 IF_SOFTWARE(, "VID")},
			{16, 16 IF_SOFTWARE(, "etherType")}
		},
	},
	std::array<KeyFormat<ap_uint<16>, 12>,2>{
		{
			{0x0800, 65535, 6 IF_SOFTWARE(, "ipv4")},
			{0x86dd, 65535, 7 IF_SOFTWARE(, "ipv6")}
		},
	},
	std::pair<ap_uint<6>, ap_uint<6>>{16,16},
	(false),
	IF_SOFTWARE("vlan_inner_t",)
	(false),
	std::pair<ap_uint<6>, ap_uint<6>>{0,1},
	{{1}}
};

#define OUTER_MPLS_HEADER_SIZE 4

#define OUTER_MPLS_HEADER_SIZE_BITS bytes2Bits(OUTER_MPLS_HEADER_SIZE)

#define OUTER_MPLS_NUM_FIELDS 4

const HeaderFormat<4, 4, ap_uint<3>, 1, MAX_SUPP_HEADERS , 1, 1> mpls_outer_t
{
	(ap_uint<32>("4294967295")),
	std::array<FieldFormat< 4>,4>{
		{
			{0, 20 IF_SOFTWARE(, "label")},
			{20, 3 IF_SOFTWARE(, "TC")},
			{23, 1 IF_SOFTWARE(, "S")},
			{24, 8 IF_SOFTWARE(, "TTL")}
		},
	},
	std::array<KeyFormat<ap_uint<3>, 12>,1>{
		{
			{0x00, 7, 5 IF_SOFTWARE(, "inner_mpls")}
		},
	},
	std::pair<ap_uint<6>, ap_uint<6>>{20,3},
	(false),
	IF_SOFTWARE("mpls_outer_t",)
	(false),
	std::pair<ap_uint<6>, ap_uint<6>>{0,1},
	{{1}}
};

#define INNER_MPLS_HEADER_SIZE 4

#define INNER_MPLS_HEADER_SIZE_BITS bytes2Bits(INNER_MPLS_HEADER_SIZE)

#define INNER_MPLS_NUM_FIELDS 4

const HeaderFormat<4, 4, ap_uint<1>, 1, MAX_SUPP_HEADERS , 1, 1> mpls_inner_t
{
	(ap_uint<32>("4294967295")),
	std::array<FieldFormat< 4>,4>{
		{
			{0, 20 IF_SOFTWARE(, "label")},
			{20, 3 IF_SOFTWARE(, "TC")},
			{23, 1 IF_SOFTWARE(, "S")},
			{24, 8 IF_SOFTWARE(, "TTL")}
		},
	},
	std::array<KeyFormat<ap_uint<1>, 12>,1>{
		{
			{1, 1, 0 IF_SOFTWARE(, "Last Header")}
		},
	},
	std::pair<ap_uint<6>, ap_uint<6>>{0,1},
	(true),
	IF_SOFTWARE("mpls_inner_t",)
	(false),
	std::pair<ap_uint<6>, ap_uint<6>>{0,1},
	{{1}}
};

#define IPV4_HEADER_SIZE 20

#define IPV4_HEADER_SIZE_BITS bytes2Bits(IPV4_HEADER_SIZE)

#define IPV4_NUM_FIELDS 12

const HeaderFormat<20, 12, ap_uint<8>, 3, MAX_SUPP_HEADERS , 1, 1> ipv4_t
{
	(ap_uint<160>("1461501637330902918203684832716283019655932542975")),
	std::array<FieldFormat< 20>,12>{
		{
			{0, 4 IF_SOFTWARE(, "version")},
			{4, 4 IF_SOFTWARE(, "ihl")},
			{8, 8 IF_SOFTWARE(, "diffserv")},
			{16, 16 IF_SOFTWARE(, "totalLen")},
			{32, 16 IF_SOFTWARE(, "identification")},
			{48, 3 IF_SOFTWARE(, "flags")},
			{51, 13 IF_SOFTWARE(, "fragOffset")},
			{64, 8 IF_SOFTWARE(, "ttl")},
			{72, 8 IF_SOFTWARE(, "protocol")},
			{80, 16 IF_SOFTWARE(, "hdrChecksum")},
			{96, 32 IF_SOFTWARE(, "srcAddr")},
			{128, 32 IF_SOFTWARE(, "dstAddr")}
		},
	},
	std::array<KeyFormat<ap_uint<8>, 12>,3>{
		{
			{0x01, 255, 12 IF_SOFTWARE(, "icmp")},
			{0x11, 255, 8 IF_SOFTWARE(, "udp")},
			{0x06, 255, 9 IF_SOFTWARE(, "tcp")}
		},
	},
	std::pair<ap_uint<8>, ap_uint<8>>{72,8},
	(false),
	IF_SOFTWARE("ipv4_t",)
	(false),
	std::pair<ap_uint<8>, ap_uint<8>>{0,1},
	{{1}}
};

#define IPV6_HEADER_SIZE 40

#define IPV6_HEADER_SIZE_BITS bytes2Bits(IPV6_HEADER_SIZE)

#define IPV6_NUM_FIELDS 8

const HeaderFormat<40, 8, ap_uint<8>, 4, MAX_SUPP_HEADERS , 1, 1> ipv6_t
{
	(ap_uint<320>("2135987035920910082395021706169552114602704522356652769947041607822219725780640550022962086936575")),
	std::array<FieldFormat< 40>,8>{
		{
			{0, 4 IF_SOFTWARE(, "version")},
			{4, 8 IF_SOFTWARE(, "trafficClass")},
			{12, 20 IF_SOFTWARE(, "flowLabel")},
			{32, 16 IF_SOFTWARE(, "payloadLen")},
			{48, 8 IF_SOFTWARE(, "nextHdr")},
			{56, 8 IF_SOFTWARE(, "hopLimit")},
			{64, 128 IF_SOFTWARE(, "srcAddr")},
			{192, 128 IF_SOFTWARE(, "dstAddr")}
		},
	},
	std::array<KeyFormat<ap_uint<8>, 12>,4>{
		{
			{0x00, 255, 10 IF_SOFTWARE(, "ipv6_ext_1")},
			{0x3a, 255, 12 IF_SOFTWARE(, "icmp")},
			{0x11, 255, 8 IF_SOFTWARE(, "udp")},
			{0x06, 255, 9 IF_SOFTWARE(, "tcp")}
		},
	},
	std::pair<ap_uint<9>, ap_uint<9>>{48,8},
	(false),
	IF_SOFTWARE("ipv6_t",)
	(false),
	std::pair<ap_uint<9>, ap_uint<9>>{0,1},
	{{1}}
};

#define IPV6_EXT_1_HEADER_SIZE 32

#define IPV6_EXT_1_HEADER_SIZE_BITS bytes2Bits(IPV6_EXT_1_HEADER_SIZE)

#define IPV6_EXT_1_NUM_FIELDS 4
template<uint16_t N_Size, uint16_t N_Fields, typename T_Key, uint16_t N_Key, uint16_t N_MaxSuppHeaders, uint16_t N_HeaderLenArrSize, uint16_t N_HeaderLenELemBits> 
struct ipv6_ext_1HeaderFormat : public HeaderFormat<N_Size, N_Fields, T_Key, N_Key, N_MaxSuppHeaders, N_HeaderLenArrSize, N_HeaderLenELemBits> { 
	ipv6_ext_1HeaderFormat (ap_uint<bytes2Bits(N_Size)> PHVMask, 
		std::array<FieldFormat<N_Size>, N_Fields> Fields, 
		std::array<KeyFormat<T_Key, N_MaxSuppHeaders>, N_Key> Key, 
		std::pair<ap_uint<numbits(bytes2Bits(N_Size))>, ap_uint<numbits(bytes2Bits(N_Size))>> KeyLocation, 
		bool LastHeader,
		IF_SOFTWARE(std::string HeaderName,)
		bool varSizeHeader,
		std::pair<ap_uint<numbits(bytes2Bits(N_Size))>, ap_uint<numbits(bytes2Bits(N_Size))>> HeaderLengthInd, std::array<ap_uint<N_HeaderLenELemBits>, N_HeaderLenArrSize> ArrLenLookup) : 
	HeaderFormat<N_Size, N_Fields, T_Key, N_Key, N_MaxSuppHeaders, N_HeaderLenArrSize, N_HeaderLenELemBits> ({PHVMask, 
		Fields, 
		Key, 
		KeyLocation, 
		LastHeader,
		IF_SOFTWARE(HeaderName,)
		varSizeHeader,
		HeaderLengthInd,
		ArrLenLookup}) {}

	void getHeaderSize(ap_uint<numbits(bytes2Bits(N_Size))>& size, const ap_uint<8>& totalLen) const {
		size = ((0x8*(totalLen+0x1))*0x8);
	}
};

const ipv6_ext_1HeaderFormat<32, 4, ap_uint<8>, 4, MAX_SUPP_HEADERS , 4, 9> ipv6_ext_1_t
{
	(ap_uint<256>("115792089237316195423570985008687907853269984665640564039457584007913129639935")),
	std::array<FieldFormat< 32>,4>{
		{
			{0, 8 IF_SOFTWARE(, "nextHdr")},
			{8, 8 IF_SOFTWARE(, "totalLen")},
			{16, 16 IF_SOFTWARE(, "stuff")},
			{32, 224 IF_SOFTWARE(, "options")}
		},
	},
	std::array<KeyFormat<ap_uint<8>, 12>,4>{
		{
			{0x3a, 255, 12 IF_SOFTWARE(, "icmp")},
			{0x3c, 255, 11 IF_SOFTWARE(, "ipv6_ext_2")},
			{0x11, 255, 8 IF_SOFTWARE(, "udp")},
			{0x06, 255, 9 IF_SOFTWARE(, "tcp")}
		},
	},
	std::pair<ap_uint<9>, ap_uint<9>>{0,8},
	(false),
	IF_SOFTWARE("ipv6_ext_1_t",)
	(true),
	std::pair<ap_uint<9>, ap_uint<9>>{8,8},
	{{64,128,192,256}}
};

#define IPV6_EXT_2_HEADER_SIZE 32

#define IPV6_EXT_2_HEADER_SIZE_BITS bytes2Bits(IPV6_EXT_2_HEADER_SIZE)

#define IPV6_EXT_2_NUM_FIELDS 4
template<uint16_t N_Size, uint16_t N_Fields, typename T_Key, uint16_t N_Key, uint16_t N_MaxSuppHeaders, uint16_t N_HeaderLenArrSize, uint16_t N_HeaderLenELemBits> 
struct ipv6_ext_2HeaderFormat : public HeaderFormat<N_Size, N_Fields, T_Key, N_Key, N_MaxSuppHeaders, N_HeaderLenArrSize, N_HeaderLenELemBits> { 
	ipv6_ext_2HeaderFormat (ap_uint<bytes2Bits(N_Size)> PHVMask, 
		std::array<FieldFormat<N_Size>, N_Fields> Fields, 
		std::array<KeyFormat<T_Key, N_MaxSuppHeaders>, N_Key> Key, 
		std::pair<ap_uint<numbits(bytes2Bits(N_Size))>, ap_uint<numbits(bytes2Bits(N_Size))>> KeyLocation, 
		bool LastHeader,
		IF_SOFTWARE(std::string HeaderName,)
		bool varSizeHeader,
		std::pair<ap_uint<numbits(bytes2Bits(N_Size))>, ap_uint<numbits(bytes2Bits(N_Size))>> HeaderLengthInd, std::array<ap_uint<N_HeaderLenELemBits>, N_HeaderLenArrSize> ArrLenLookup) : 
	HeaderFormat<N_Size, N_Fields, T_Key, N_Key, N_MaxSuppHeaders, N_HeaderLenArrSize, N_HeaderLenELemBits> ({PHVMask, 
		Fields, 
		Key, 
		KeyLocation, 
		LastHeader,
		IF_SOFTWARE(HeaderName,)
		varSizeHeader,
		HeaderLengthInd,
		ArrLenLookup}) {}

	void getHeaderSize(ap_uint<numbits(bytes2Bits(N_Size))>& size, const ap_uint<8>& totalLen) const {
		size = ((0x8*(totalLen+0x1))*0x8);
	}
};

const ipv6_ext_2HeaderFormat<32, 4, ap_uint<8>, 3, MAX_SUPP_HEADERS , 4, 9> ipv6_ext_2_t
{
	(ap_uint<256>("115792089237316195423570985008687907853269984665640564039457584007913129639935")),
	std::array<FieldFormat< 32>,4>{
		{
			{0, 8 IF_SOFTWARE(, "nextHdr")},
			{8, 8 IF_SOFTWARE(, "totalLen")},
			{16, 16 IF_SOFTWARE(, "stuff")},
			{32, 224 IF_SOFTWARE(, "options")}
		},
	},
	std::array<KeyFormat<ap_uint<8>, 12>,3>{
		{
			{0x3a, 255, 12 IF_SOFTWARE(, "icmp")},
			{0x11, 255, 8 IF_SOFTWARE(, "udp")},
			{0x06, 255, 9 IF_SOFTWARE(, "tcp")}
		},
	},
	std::pair<ap_uint<9>, ap_uint<9>>{0,8},
	(false),
	IF_SOFTWARE("ipv6_ext_2_t",)
	(true),
	std::pair<ap_uint<9>, ap_uint<9>>{8,8},
	{{64,128,192,256}}
};

#define UDP_HEADER_SIZE 8

#define UDP_HEADER_SIZE_BITS bytes2Bits(UDP_HEADER_SIZE)

#define UDP_NUM_FIELDS 4

const HeaderFormat<8, 4, ap_uint<1>, 1, MAX_SUPP_HEADERS , 1, 1> udp_t
{
	(ap_uint<64>("18446744073709551615")),
	std::array<FieldFormat< 8>,4>{
		{
			{0, 16 IF_SOFTWARE(, "srcPort")},
			{16, 16 IF_SOFTWARE(, "dstPort")},
			{32, 16 IF_SOFTWARE(, "hdrLength")},
			{48, 16 IF_SOFTWARE(, "chksum")}
		},
	},
	std::array<KeyFormat<ap_uint<1>, 12>,1>{
		{
			{1, 1, 0 IF_SOFTWARE(, "Last Header")}
		},
	},
	std::pair<ap_uint<7>, ap_uint<7>>{0,1},
	(true),
	IF_SOFTWARE("udp_t",)
	(false),
	std::pair<ap_uint<7>, ap_uint<7>>{0,1},
	{{1}}
};

#define TCP_HEADER_SIZE 20

#define TCP_HEADER_SIZE_BITS bytes2Bits(TCP_HEADER_SIZE)

#define TCP_NUM_FIELDS 10

const HeaderFormat<20, 10, ap_uint<1>, 1, MAX_SUPP_HEADERS , 1, 1> tcp_t
{
	(ap_uint<160>("1461501637330902918203684832716283019655932542975")),
	std::array<FieldFormat< 20>,10>{
		{
			{0, 16 IF_SOFTWARE(, "srcPort")},
			{16, 16 IF_SOFTWARE(, "dstPort")},
			{32, 32 IF_SOFTWARE(, "seqNum")},
			{64, 32 IF_SOFTWARE(, "ackNum")},
			{96, 4 IF_SOFTWARE(, "dataOffset")},
			{100, 6 IF_SOFTWARE(, "unused")},
			{106, 6 IF_SOFTWARE(, "flags")},
			{112, 16 IF_SOFTWARE(, "windowSize")},
			{128, 16 IF_SOFTWARE(, "chksum")},
			{144, 16 IF_SOFTWARE(, "urgPtr")}
		},
	},
	std::array<KeyFormat<ap_uint<1>, 12>,1>{
		{
			{1, 1, 0 IF_SOFTWARE(, "Last Header")}
		},
	},
	std::pair<ap_uint<8>, ap_uint<8>>{0,1},
	(true),
	IF_SOFTWARE("tcp_t",)
	(false),
	std::pair<ap_uint<8>, ap_uint<8>>{0,1},
	{{1}}
};

#define ICMP_HEADER_SIZE 6

#define ICMP_HEADER_SIZE_BITS bytes2Bits(ICMP_HEADER_SIZE)

#define ICMP_NUM_FIELDS 4

const HeaderFormat<6, 4, ap_uint<1>, 1, MAX_SUPP_HEADERS , 1, 1> icmp_t
{
	(ap_uint<48>("281474976710655")),
	std::array<FieldFormat< 6>,4>{
		{
			{0, 8 IF_SOFTWARE(, "mtype")},
			{8, 8 IF_SOFTWARE(, "code")},
			{16, 16 IF_SOFTWARE(, "chksum")},
			{32, 16 IF_SOFTWARE(, "body")}
		},
	},
	std::array<KeyFormat<ap_uint<1>, 12>,1>{
		{
			{1, 1, 0 IF_SOFTWARE(, "Last Header")}
		},
	},
	std::pair<ap_uint<6>, ap_uint<6>>{0,1},
	(true),
	IF_SOFTWARE("icmp_t",)
	(false),
	std::pair<ap_uint<6>, ap_uint<6>>{0,1},
	{{1}}
};

#endif //_PARSER_HEADER_TEMP_HPP_
