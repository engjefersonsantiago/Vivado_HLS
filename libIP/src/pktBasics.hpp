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
#ifndef __SYNTHESIS__
	std::string FieldName;
#endif
};

// Parser match key structrure
template<typename T_Key>
struct KeyFormat {
	T_Key	KeyVal;
	T_Key	KeyMask;
	uint_16	NextHeader;
#ifndef __SYNTHESIS__
	std::string NextHeaderName;
#endif
};

// Basic Header Format structure
template<uint_16 N_Fields, typename T_Key, uint_16 N_Key>
class HeaderFormat {
	public:
	std::array<FieldFormat, N_Fields> Fields;
	std::array<KeyFormat<T_Key>, N_Key> Key;
	std::pair<uint_16, uint_16> KeyLocation;	// First: Offset. Second: Lenght

#ifndef __SYNTHESIS__
	std::string HeaderName;
#endif

};

// Ethernet frame layout
const HeaderFormat<3, uint_16, 7> EthernetLayout
{
	{
		// Header layout
		{
#ifndef __SYNTHESIS__
			{0, 48, "DST_MAC"},
			{48, 48, "SRC_MAC"},
			{96, 16, "EtherType"}
#else
			{0, 48},
			{48, 48},
			{96, 16}
#endif
		},
	},
	{
		// Keys
		{
#ifndef __SYNTHESIS__
			{0x0800, 0xFFFF, 11, "IPV4"},
			{0x86DD, 0xFFFF, 12, "IPV6"},
			{0x0806, 0xFFFF, 13, "ARP"},
			{0x8847, 0xFFFF, 14, "MPLS_unicast"},
			{0x8848, 0xFFFF, 15, "MPLS_multicast"},
			{0x8100, 0xFFFF, 16, "VLAN_tagged_single"},
			{0x9100, 0xFFFF, 17, "VLAN_tagged_double"}
#else
			{0x0800, 0xFFFF, 11},
			{0x86DD, 0xFFFF, 12},
			{0x0806, 0xFFFF, 13},
			{0x8847, 0xFFFF, 14},
			{0x8848, 0xFFFF, 15},
			{0x8100, 0xFFFF, 16},
			{0x9100, 0xFFFF, 17}
#endif
		},
	},
#ifndef __SYNTHESIS__
	// Key position
	std::pair<uint_16, uint_16>{96, 16},		// EtherType byte 12..14
	// Header Layout name
	"Ethernet_Layout"
#else
	std::pair<uint_16, uint_16>{96, 16}			// EtherType byte 12..14
#endif
};

#endif //_PKT_BASICS_HPP_
