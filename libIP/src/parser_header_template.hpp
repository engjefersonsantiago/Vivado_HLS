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

// Ethernet frame layout
const HeaderFormat<14, 3, uint16_t, 7, 32> EthernetLayout
{
	// PHV mask
	//(0xFFFFFFFFFFFFFFFFFFFFFFFFFFFF),
	//(ap_uint<112>(0)),	// None
	(ap_uint<112>(~0)),		// All
	{
		// Header layout
		{
			{0, 48 IF_SOFTWARE(, "Destination MAC")},
			{48, 48 IF_SOFTWARE(, "Source MAC")},
			{96, 16 IF_SOFTWARE(, "EtherType")}
		},
	},
	{
		// Keys
		{
			{0x0800, 0xFFFF, 11 IF_SOFTWARE(, "IPv4")},
			{0x86DD, 0xFFFF, 12 IF_SOFTWARE(, "IPv6")},
			{0x0806, 0xFFFF, 13 IF_SOFTWARE(, "ARP")},
			{0x8847, 0xFFFF, 14 IF_SOFTWARE(, "MPLS unicast")},
			{0x8848, 0xFFFF, 15 IF_SOFTWARE(, "MPLS multicast")},
			{0x8100, 0xFFFF, 16 IF_SOFTWARE(, "VLAN tagged single")},
			{0x9100, 0xFFFF, 17 IF_SOFTWARE(, "VLAN tagged double")}
		},
	},
	// Key position
	std::pair<ap_uint<7>, ap_uint<7>>{96, 16},	// EtherType byte 12..14
	// Last header
	(false)
	// Header Layout name
	IF_SOFTWARE(, "Ethernet Layout")
};

// IPv4 frame layout
const HeaderFormat<20, 13, uint8_t, 2, 32> IPv4Layout
{
	// PHV mask
	//("0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF"),
	//("0x00000000000000FF00000000FFFFFFFFFFFFFFFF"),	// Protocol and addresses
	(ap_uint<160>(~0)),	// All
	{
		// Header layout
		{
			{0, 3 IF_SOFTWARE(, "Version")},
			{4, 7 IF_SOFTWARE(, "IHL")},
			{8, 13 IF_SOFTWARE(, "DSCP")},
			{14, 15 IF_SOFTWARE(, "ECN")},
			{16, 31 IF_SOFTWARE(, "Total Length")},
			{32, 47 IF_SOFTWARE(, "Identification")},
			{48, 51 IF_SOFTWARE(, "Flags")},
			{52, 63 IF_SOFTWARE(, "Fragment Offset")},
			{64, 71 IF_SOFTWARE(, "TTL")},
			{72, 79 IF_SOFTWARE(, "Protocol")},
			{80, 95 IF_SOFTWARE(, "Header Checksum")},
			{96, 127 IF_SOFTWARE(, "Source Address")},
			{128, 159 IF_SOFTWARE(, "Destination Address")}
		},
	},
	{
		// Keys
		{
			{0x06, 0xFF, 21 IF_SOFTWARE(, "TCP")},
			{0x11, 0xFF, 22 IF_SOFTWARE(, "UDP")}
		},
	},
	// Key position
	std::pair<ap_uint<8>, ap_uint<8>>{72, 8},		// Protocol
	// Last header
	(false)
	// Header Layout name
	IF_SOFTWARE(, "IPv4 Layout")
};

// UDP frame layout
const HeaderFormat<8, 4, uint8_t, 1, 32> UDPLayout
{
	// PHV mask
	//(0xFFFFFFFFFFFFFFFF),
	//("0xFFFFFFFF00000000"),	// Ports
	(ap_uint<64>(~0)),	// All
	{
		// Header layout
		{
			{0, 15 IF_SOFTWARE(, "Source Port")},
			{16, 31 IF_SOFTWARE(, "Destination Port")},
			{32, 47 IF_SOFTWARE(, "Length")},
			{48, 63 IF_SOFTWARE(, "Checksum")}
		},
	},
	{
		// Keys
		{
			{0xFF, 0xFF, 0xFFFF IF_SOFTWARE(, "Last Header")},
		},
	},
	// Key position
	std::pair<ap_uint<6>, ap_uint<6>>{0, 8},		// Invalid
	// Last header
	(true)
	// Header Layout name
	IF_SOFTWARE(, "UDP Layout")
};

// TCP frame layout
const HeaderFormat<20, 10, uint8_t, 1, 32> TCPLayout
{
	// PHV mask
	//("0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF"),
	//("0xFFFFFFFF00000000000000000000000000000000"),	// Ports
	(ap_uint<160>(~0)),	// All
	{
		// Header layout
		{
			{0, 15 IF_SOFTWARE(, "Source Port")},
			{16, 31 IF_SOFTWARE(, "Destination Port")},
			{32, 64 IF_SOFTWARE(, "Sequence Number")},
			{64, 95 IF_SOFTWARE(, "ACK number")},
			{96, 99 IF_SOFTWARE(, "Data Offset")},
			{100, 105 IF_SOFTWARE(, "Unused")},
			{106, 111 IF_SOFTWARE(, "Flags")},
			{112, 127 IF_SOFTWARE(, "Window")},
			{128, 143 IF_SOFTWARE(, "Checksum")},
			{144, 159 IF_SOFTWARE(, "Urgent Pointer")}
		},
	},
	{
		// Keys
		{
			{0xFF, 0xFF, 0xFFFF IF_SOFTWARE(, "Last Header")},
		},
	},
	// Key position
	std::pair<ap_uint<6>, ap_uint<6>>{0, 8},		// Invalid
	// Last header
	(true)
	// Header Layout name
	IF_SOFTWARE(, "TCP Layout")
};

#endif //_PARSER_HEADER_TEMP_HPP_
