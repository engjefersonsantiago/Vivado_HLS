/******************************************************************************
* Packet Parser: header extraction and supported protocols graph              *
* Jeferson Santiago da Silva (eng.jefersonsantiago@gmail.com)                 *
******************************************************************************/

#include "Header.hpp"
#include "parser_header_template.hpp"

#ifndef _TST_HEADER_HPP_
#define _TST_HEADER_HPP_

// MAC bus size
//#define PKT_BUS_SIZE 32
//#define PKT_BUS_SIZE 64
#define PKT_BUS_SIZE 128
//#define PKT_BUS_SIZE 256
//#define PKT_BUS_SIZE 320
//#define PKT_BUS_SIZE 512
//#define PKT_BUS_SIZE 1024

// Number of supported headers
#define HEADER_NUM 4

// Parser header size
#define ETH_HEADER_SIZE 14
#define IP_HEADER_SIZE 20
#define UDP_HEADER_SIZE 8
#define TCP_HEADER_SIZE 20

// 14: ethernet, 20: IP, 8: UDP. Greater() among them
#define MAX_HEADER_SIZE 20
#define MAX_HEADER_SIZE_BITS bytes2Bits(MAX_HEADER_SIZE)

//#define ARRAY_FOR_PHV
//#define ARRAY_OF_POINTERS_TO_PHV

#ifdef ARRAY_FOR_PHV
	#ifdef ARRAY_OF_POINTERS_TO_PHV
void HeaderAnalysisTop(const PacketData<PKT_BUS_SIZE, 32, 16>& PacketIn, std::array<PHVData<MAX_HEADER_SIZE>*, HEADER_NUM> PHV, PacketData<PKT_BUS_SIZE, 32, 16>* PacketOut);
	#else
void HeaderAnalysisTop(const PacketData<PKT_BUS_SIZE, 32, 16>& PacketIn, std::array<PHVData<MAX_HEADER_SIZE, 32, 16>, HEADER_NUM>* PHV, PacketData<PKT_BUS_SIZE, 32, 16>* PacketOut);
	#endif
#else
void HeaderAnalysisTop(const PacketData<PKT_BUS_SIZE, 32, 16>& PacketIn, PHVData<ETH_HEADER_SIZE, 32, 16>* Ethernet_PHV, PHVData<IP_HEADER_SIZE, 32, 16>* IP_PHV, PHVData<UDP_HEADER_SIZE, 32, 16>* UDP_PHV, PacketData<PKT_BUS_SIZE, 32, 16>* PacketOut);
#endif

#endif //_TST_HEADER_HPP_
