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

#define HEADER_NUM 6

#define MAX_SUPP_HEADERS 6

#define MAX_HEADER_SIZE 60

#define MAX_HEADER_SIZE_BITS bytes2Bits(MAX_HEADER_SIZE)

#define ETHERNET_HEADER_SIZE 14

#define ETHERNET_HEADER_SIZE_BITS bytes2Bits(ETHERNET_HEADER_SIZE)

#define ETHERNET_NUM_FIELDS 3

const HeaderFormat<14, 3, ap_uint<16>, 3, MAX_SUPP_HEADERS> ethernet_t
{
	(ap_uint<112>("5192296858534827628530496329220095")),
	{
		{
			{0, 48 IF_SOFTWARE(, "dstAddr")},
			{48, 48 IF_SOFTWARE(, "srcAddr")},
			{96, 16 IF_SOFTWARE(, "etherType")}
		},
	},
	{
		{
			{0x0800, 65535, 3 IF_SOFTWARE(, "ipv4_var")},
			{0x8100, 65535, 2 IF_SOFTWARE(, "vlan")},
			{0x86dd, 65535, 4 IF_SOFTWARE(, "ipv6")}
		},
	},
	std::pair<ap_uint<7>, ap_uint<7>>{96,16},
	(false),
	IF_SOFTWARE("ethernet_t",)
	(false),
};

#define VLAN_HEADER_SIZE 4

#define VLAN_HEADER_SIZE_BITS bytes2Bits(VLAN_HEADER_SIZE)

#define VLAN_NUM_FIELDS 4

const HeaderFormat<4, 4, ap_uint<16>, 2, MAX_SUPP_HEADERS> vlan_t
{
	(ap_uint<32>("4294967295")),
	{
		{
			{0, 3 IF_SOFTWARE(, "PCP")},
			{3, 1 IF_SOFTWARE(, "DEI")},
			{4, 12 IF_SOFTWARE(, "VID")},
			{16, 16 IF_SOFTWARE(, "etherType")}
		},
	},
	{
		{
			{0x0800, 65535, 3 IF_SOFTWARE(, "ipv4_var")},
			{0x86dd, 65535, 4 IF_SOFTWARE(, "ipv6")}
		},
	},
	std::pair<ap_uint<6>, ap_uint<6>>{16,16},
	(false),
	IF_SOFTWARE("vlan_t",)
	(false),
};

#define IPV4_VAR_HEADER_SIZE 60

#define IPV4_VAR_HEADER_SIZE_BITS bytes2Bits(IPV4_VAR_HEADER_SIZE)

#define IPV4_VAR_NUM_FIELDS 13
template<uint16_t N_Size, uint16_t N_Fields, typename T_Key, uint16_t N_Key, uint16_t N_MaxSuppHeaders> 
struct ipv4_varHeaderFormat { 
	ap_uint<bytes2Bits(N_Size)> PHVMask; 
	std::array<FieldFormat<N_Size>, N_Fields> Fields; 
	std::array<KeyFormat<T_Key, N_MaxSuppHeaders>, N_Key> Key; 
	std::pair<ap_uint<numbits(bytes2Bits(N_Size))>, ap_uint<numbits(bytes2Bits(N_Size))>> KeyLocation; 
	bool LastHeader;
	IF_SOFTWARE(std::string HeaderName;)
	bool varSizeHeader;
	std::pair<ap_uint<numbits(bytes2Bits(N_Size))>, ap_uint<numbits(bytes2Bits(N_Size))>> HeaderLengthInd;
	void getHeaderSize(ap_uint<numbits(bytes2Bits(N_Size))>& size, const ap_uint<4>& ihl) {
		size = ((0x4*ihl)*0x8);
	}
};

const ipv4_varHeaderFormat<60, 13, ap_uint<8>, 2, MAX_SUPP_HEADERS> ipv4_var_t
{
	(ap_uint<480>("3121748550315992231381597229793166305748598142664971150859156959625371738819765620120306103063491971159826931121406622895447975679288285306290175")),
	{
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
			{128, 32 IF_SOFTWARE(, "dstAddr")},
			{160, 320 IF_SOFTWARE(, "options")}
		},
	},
	{
		{
			{0x11, 255, 5 IF_SOFTWARE(, "udp")},
			{0x06, 255, 6 IF_SOFTWARE(, "tcp")}
		},
	},
	std::pair<ap_uint<9>, ap_uint<9>>{72,8},
	(false),
	IF_SOFTWARE("ipv4_var_t",)
	(true),
	std::pair<ap_uint<9>, ap_uint<9>>{4,4}
};

#define IPV6_HEADER_SIZE 40

#define IPV6_HEADER_SIZE_BITS bytes2Bits(IPV6_HEADER_SIZE)

#define IPV6_NUM_FIELDS 8

const HeaderFormat<40, 8, ap_uint<8>, 2, MAX_SUPP_HEADERS> ipv6_t
{
	(ap_uint<320>("2135987035920910082395021706169552114602704522356652769947041607822219725780640550022962086936575")),
	{
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
	{
		{
			{0x11, 255, 5 IF_SOFTWARE(, "udp")},
			{0x06, 255, 6 IF_SOFTWARE(, "tcp")}
		},
	},
	std::pair<ap_uint<9>, ap_uint<9>>{48,8},
	(false),
	IF_SOFTWARE("ipv6_t",)
	(false),
};

#define UDP_HEADER_SIZE 8

#define UDP_HEADER_SIZE_BITS bytes2Bits(UDP_HEADER_SIZE)

#define UDP_NUM_FIELDS 4

const HeaderFormat<8, 4, ap_uint<1>, 1, MAX_SUPP_HEADERS> udp_t
{
	(ap_uint<64>("18446744073709551615")),
	{
		{
			{0, 16 IF_SOFTWARE(, "srcPort")},
			{16, 16 IF_SOFTWARE(, "dstPort")},
			{32, 16 IF_SOFTWARE(, "hdrLength")},
			{48, 16 IF_SOFTWARE(, "chksum")}
		},
	},
	{
		{
			{1, 1, 0 IF_SOFTWARE(, "Last Header")}
		},
	},
	std::pair<ap_uint<7>, ap_uint<7>>{0,1},
	(true),
	IF_SOFTWARE("udp_t",)
	(false),
};

#define TCP_HEADER_SIZE 20

#define TCP_HEADER_SIZE_BITS bytes2Bits(TCP_HEADER_SIZE)

#define TCP_NUM_FIELDS 10

const HeaderFormat<20, 10, ap_uint<1>, 1, MAX_SUPP_HEADERS> tcp_t
{
	(ap_uint<160>("1461501637330902918203684832716283019655932542975")),
	{
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
	{
		{
			{1, 1, 0 IF_SOFTWARE(, "Last Header")}
		},
	},
	std::pair<ap_uint<8>, ap_uint<8>>{0,1},
	(true),
	IF_SOFTWARE("tcp_t",)
	(false),
};

#endif //_PARSER_HEADER_TEMP_HPP_
