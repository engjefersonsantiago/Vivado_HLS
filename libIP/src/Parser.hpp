/******************************************************************************
* Packet Parser: header extraction and support protocols graph generation     *
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

// Number of supported headers
#define HEADER_NUM 3

// Parser header size
#define ETH_HEADER_SIZE 14
#define IP_HEADER_SIZE 20
#define UDP_HEADER_SIZE 8

// 14: ethernet, 20: IP, 8: UDP. Greater() among them
#define MAX_HEADER_SIZE 20
#define MAX_HEADER_SIZE_BITS bytes2Bits(MAX_HEADER_SIZE)

//void HeaderAnalysisTop(PacketData<PKT_BUS_SIZE> PacketIn, std::array<PHVData<MAX_HEADER_SIZE>, HEADER_NUM>* PHV, PacketData<PKT_BUS_SIZE>* PacketOut);
void HeaderAnalysisTop(PacketData<PKT_BUS_SIZE> PacketIn, PHVData<ETH_HEADER_SIZE>* Ethernet_PHV, PHVData<IP_HEADER_SIZE>* IP_PHV, PHVData<UDP_HEADER_SIZE>* UDP_PHV, PacketData<PKT_BUS_SIZE>* PacketOut);

#endif //_TST_HEADER_HPP_
