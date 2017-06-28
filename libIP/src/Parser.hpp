/******************************************************************************
* Packet Parser: header extraction and supported protocols graph              *
* Jeferson Santiago da Silva (eng.jefersonsantiago@gmail.com)                 *
******************************************************************************/

#include "Header.hpp"

#include "parser_header_template.hpp"

#ifndef _PARSER_HEADER_HPP_

#define _PARSER_HEADER_HPP_

#define PKT_BUS_SIZE 128

#define MAX_PKT_ID_SIZE 16

void HeaderAnalysisTop(const PacketData<PKT_BUS_SIZE, MAX_SUPP_HEADERS, MAX_PKT_ID_SIZE>& PacketIn,
	PHVData<ETHERNET_HEADER_SIZE, MAX_SUPP_HEADERS, MAX_PKT_ID_SIZE>& ethernet_PHV,
	PHVData<VLAN_HEADER_SIZE, MAX_SUPP_HEADERS, MAX_PKT_ID_SIZE>& vlan_PHV,
	PHVData<IPV4_VAR_HEADER_SIZE, MAX_SUPP_HEADERS, MAX_PKT_ID_SIZE>& ipv4_var_PHV,
	PHVData<IPV6_HEADER_SIZE, MAX_SUPP_HEADERS, MAX_PKT_ID_SIZE>& ipv6_PHV,
	PHVData<UDP_HEADER_SIZE, MAX_SUPP_HEADERS, MAX_PKT_ID_SIZE>& udp_PHV,
	PHVData<TCP_HEADER_SIZE, MAX_SUPP_HEADERS, MAX_PKT_ID_SIZE>& tcp_PHV,
	PacketData<PKT_BUS_SIZE, MAX_SUPP_HEADERS, MAX_PKT_ID_SIZE>& PacketOut);

#endif //_PARSER_HEADER_HPP_
