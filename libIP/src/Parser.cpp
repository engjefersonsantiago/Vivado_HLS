/******************************************************************************
* Packet Parser: header extraction and supported protocols graph              *
* Jeferson Santiago da Silva (eng.jefersonsantiago@gmail.com)                 *
******************************************************************************/

#include "Parser.hpp"

void HeaderAnalysisTop(const PacketData<PKT_BUS_SIZE, MAX_SUPP_HEADERS, MAX_PKT_ID_SIZE>& PacketIn,
	PHVData<ETHERNET_HEADER_SIZE, MAX_SUPP_HEADERS, MAX_PKT_ID_SIZE>& ethernet_PHV,
	PHVData<OUTER_VLAN_HEADER_SIZE, MAX_SUPP_HEADERS, MAX_PKT_ID_SIZE>& outer_vlan_PHV,
	PHVData<INNER_VLAN_HEADER_SIZE, MAX_SUPP_HEADERS, MAX_PKT_ID_SIZE>& inner_vlan_PHV,
	PHVData<OUTER_MPLS_HEADER_SIZE, MAX_SUPP_HEADERS, MAX_PKT_ID_SIZE>& outer_mpls_PHV,
	PHVData<INNER_MPLS_HEADER_SIZE, MAX_SUPP_HEADERS, MAX_PKT_ID_SIZE>& inner_mpls_PHV,
	PHVData<IPV4_HEADER_SIZE, MAX_SUPP_HEADERS, MAX_PKT_ID_SIZE>& ipv4_PHV,
	PHVData<IPV6_HEADER_SIZE, MAX_SUPP_HEADERS, MAX_PKT_ID_SIZE>& ipv6_PHV,
	PHVData<UDP_HEADER_SIZE, MAX_SUPP_HEADERS, MAX_PKT_ID_SIZE>& udp_PHV,
	PHVData<TCP_HEADER_SIZE, MAX_SUPP_HEADERS, MAX_PKT_ID_SIZE>& tcp_PHV,
	PHVData<ICMP_HEADER_SIZE, MAX_SUPP_HEADERS, MAX_PKT_ID_SIZE>& icmp_PHV,
	PacketData<PKT_BUS_SIZE, MAX_SUPP_HEADERS, MAX_PKT_ID_SIZE>& PacketOut)
{
#pragma HLS INTERFACE ap_ctrl_none port=return
#pragma HLS PIPELINE II=1

	// Wires
	std::array<PacketData<PKT_BUS_SIZE, MAX_SUPP_HEADERS, MAX_PKT_ID_SIZE>, MAX_SUPP_HEADERS + 1> tmpPacketOut;
#pragma HLS ARRAY_PARTITION variable=tmpPacketOut dim=1
	std::array<PacketData<PKT_BUS_SIZE, MAX_SUPP_HEADERS, MAX_PKT_ID_SIZE>, MAX_SUPP_HEADERS + 1> tmpPacketIn;
#pragma HLS ARRAY_PARTITION variable=tmpPacketIn dim=1

	// Stateful objects

#pragma HLS INTERFACE ap_ovld port=ethernet_PHV
	static Header<ETHERNET_HEADER_SIZE, ETHERNET_NUM_FIELDS, ap_uint<16>, 5, ap_uint<PKT_BUS_SIZE>, PKT_BUS_SIZE, MAX_PKT_SIZE, MAX_SUPP_HEADERS, MAX_PKT_ID_SIZE> ethernet(IF_SOFTWARE("ethernet",) 1, ethernet_t);

	PHVData<ETHERNET_HEADER_SIZE, MAX_SUPP_HEADERS, MAX_PKT_ID_SIZE> wire_ethernet_PHV;

	static std::array<PHVData<ETHERNET_HEADER_SIZE, MAX_SUPP_HEADERS, MAX_PKT_ID_SIZE>, MAX_SUPP_HEADERS + 1> tmp_ethernet_PHV;
#pragma HLS ARRAY_PARTITION variable=tmp_ethernet_PHV dim=1
#pragma HLS DEPENDENCE variable=tmp_ethernet_PHV false

#pragma HLS INTERFACE ap_ovld port=outer_vlan_PHV
	static Header<OUTER_VLAN_HEADER_SIZE, OUTER_VLAN_NUM_FIELDS, ap_uint<16>, 1, ap_uint<PKT_BUS_SIZE>, PKT_BUS_SIZE, MAX_PKT_SIZE, MAX_SUPP_HEADERS, MAX_PKT_ID_SIZE> outer_vlan(IF_SOFTWARE("outer_vlan",) 2, vlan_outer_t);

	PHVData<OUTER_VLAN_HEADER_SIZE, MAX_SUPP_HEADERS, MAX_PKT_ID_SIZE> wire_outer_vlan_PHV;

	static std::array<PHVData<OUTER_VLAN_HEADER_SIZE, MAX_SUPP_HEADERS, MAX_PKT_ID_SIZE>, MAX_SUPP_HEADERS + 1> tmp_outer_vlan_PHV;
#pragma HLS ARRAY_PARTITION variable=tmp_outer_vlan_PHV dim=1
#pragma HLS DEPENDENCE variable=tmp_outer_vlan_PHV false

#pragma HLS INTERFACE ap_ovld port=inner_vlan_PHV
	static Header<INNER_VLAN_HEADER_SIZE, INNER_VLAN_NUM_FIELDS, ap_uint<16>, 2, ap_uint<PKT_BUS_SIZE>, PKT_BUS_SIZE, MAX_PKT_SIZE, MAX_SUPP_HEADERS, MAX_PKT_ID_SIZE> inner_vlan(IF_SOFTWARE("inner_vlan",) 3, vlan_inner_t);

	PHVData<INNER_VLAN_HEADER_SIZE, MAX_SUPP_HEADERS, MAX_PKT_ID_SIZE> wire_inner_vlan_PHV;

	static std::array<PHVData<INNER_VLAN_HEADER_SIZE, MAX_SUPP_HEADERS, MAX_PKT_ID_SIZE>, MAX_SUPP_HEADERS + 1> tmp_inner_vlan_PHV;
#pragma HLS ARRAY_PARTITION variable=tmp_inner_vlan_PHV dim=1
#pragma HLS DEPENDENCE variable=tmp_inner_vlan_PHV false

#pragma HLS INTERFACE ap_ovld port=outer_mpls_PHV
	static Header<OUTER_MPLS_HEADER_SIZE, OUTER_MPLS_NUM_FIELDS, ap_uint<3>, 1, ap_uint<PKT_BUS_SIZE>, PKT_BUS_SIZE, MAX_PKT_SIZE, MAX_SUPP_HEADERS, MAX_PKT_ID_SIZE> outer_mpls(IF_SOFTWARE("outer_mpls",) 4, mpls_outer_t);

	PHVData<OUTER_MPLS_HEADER_SIZE, MAX_SUPP_HEADERS, MAX_PKT_ID_SIZE> wire_outer_mpls_PHV;

	static std::array<PHVData<OUTER_MPLS_HEADER_SIZE, MAX_SUPP_HEADERS, MAX_PKT_ID_SIZE>, MAX_SUPP_HEADERS + 1> tmp_outer_mpls_PHV;
#pragma HLS ARRAY_PARTITION variable=tmp_outer_mpls_PHV dim=1
#pragma HLS DEPENDENCE variable=tmp_outer_mpls_PHV false

#pragma HLS INTERFACE ap_ovld port=inner_mpls_PHV
	static Header<INNER_MPLS_HEADER_SIZE, INNER_MPLS_NUM_FIELDS, ap_uint<1>, 1, ap_uint<PKT_BUS_SIZE>, PKT_BUS_SIZE, MAX_PKT_SIZE, MAX_SUPP_HEADERS, MAX_PKT_ID_SIZE> inner_mpls(IF_SOFTWARE("inner_mpls",) 5, mpls_inner_t);

	PHVData<INNER_MPLS_HEADER_SIZE, MAX_SUPP_HEADERS, MAX_PKT_ID_SIZE> wire_inner_mpls_PHV;

	static std::array<PHVData<INNER_MPLS_HEADER_SIZE, MAX_SUPP_HEADERS, MAX_PKT_ID_SIZE>, MAX_SUPP_HEADERS + 1> tmp_inner_mpls_PHV;
#pragma HLS ARRAY_PARTITION variable=tmp_inner_mpls_PHV dim=1
#pragma HLS DEPENDENCE variable=tmp_inner_mpls_PHV false

#pragma HLS INTERFACE ap_ovld port=ipv4_PHV
	static Header<IPV4_HEADER_SIZE, IPV4_NUM_FIELDS, ap_uint<8>, 3, ap_uint<PKT_BUS_SIZE>, PKT_BUS_SIZE, MAX_PKT_SIZE, MAX_SUPP_HEADERS, MAX_PKT_ID_SIZE> ipv4(IF_SOFTWARE("ipv4",) 6, ipv4_t);

	PHVData<IPV4_HEADER_SIZE, MAX_SUPP_HEADERS, MAX_PKT_ID_SIZE> wire_ipv4_PHV;

	static std::array<PHVData<IPV4_HEADER_SIZE, MAX_SUPP_HEADERS, MAX_PKT_ID_SIZE>, MAX_SUPP_HEADERS + 1> tmp_ipv4_PHV;
#pragma HLS ARRAY_PARTITION variable=tmp_ipv4_PHV dim=1
#pragma HLS DEPENDENCE variable=tmp_ipv4_PHV false

#pragma HLS INTERFACE ap_ovld port=ipv6_PHV
	static Header<IPV6_HEADER_SIZE, IPV6_NUM_FIELDS, ap_uint<8>, 3, ap_uint<PKT_BUS_SIZE>, PKT_BUS_SIZE, MAX_PKT_SIZE, MAX_SUPP_HEADERS, MAX_PKT_ID_SIZE> ipv6(IF_SOFTWARE("ipv6",) 7, ipv6_t);

	PHVData<IPV6_HEADER_SIZE, MAX_SUPP_HEADERS, MAX_PKT_ID_SIZE> wire_ipv6_PHV;

	static std::array<PHVData<IPV6_HEADER_SIZE, MAX_SUPP_HEADERS, MAX_PKT_ID_SIZE>, MAX_SUPP_HEADERS + 1> tmp_ipv6_PHV;
#pragma HLS ARRAY_PARTITION variable=tmp_ipv6_PHV dim=1
#pragma HLS DEPENDENCE variable=tmp_ipv6_PHV false

#pragma HLS INTERFACE ap_ovld port=udp_PHV
	static Header<UDP_HEADER_SIZE, UDP_NUM_FIELDS, ap_uint<1>, 1, ap_uint<PKT_BUS_SIZE>, PKT_BUS_SIZE, MAX_PKT_SIZE, MAX_SUPP_HEADERS, MAX_PKT_ID_SIZE> udp(IF_SOFTWARE("udp",) 8, udp_t);

	PHVData<UDP_HEADER_SIZE, MAX_SUPP_HEADERS, MAX_PKT_ID_SIZE> wire_udp_PHV;

	static std::array<PHVData<UDP_HEADER_SIZE, MAX_SUPP_HEADERS, MAX_PKT_ID_SIZE>, MAX_SUPP_HEADERS + 1> tmp_udp_PHV;
#pragma HLS ARRAY_PARTITION variable=tmp_udp_PHV dim=1
#pragma HLS DEPENDENCE variable=tmp_udp_PHV false

#pragma HLS INTERFACE ap_ovld port=tcp_PHV
	static Header<TCP_HEADER_SIZE, TCP_NUM_FIELDS, ap_uint<1>, 1, ap_uint<PKT_BUS_SIZE>, PKT_BUS_SIZE, MAX_PKT_SIZE, MAX_SUPP_HEADERS, MAX_PKT_ID_SIZE> tcp(IF_SOFTWARE("tcp",) 9, tcp_t);

	PHVData<TCP_HEADER_SIZE, MAX_SUPP_HEADERS, MAX_PKT_ID_SIZE> wire_tcp_PHV;

	static std::array<PHVData<TCP_HEADER_SIZE, MAX_SUPP_HEADERS, MAX_PKT_ID_SIZE>, MAX_SUPP_HEADERS + 1> tmp_tcp_PHV;
#pragma HLS ARRAY_PARTITION variable=tmp_tcp_PHV dim=1
#pragma HLS DEPENDENCE variable=tmp_tcp_PHV false

#pragma HLS INTERFACE ap_ovld port=icmp_PHV
	static Header<ICMP_HEADER_SIZE, ICMP_NUM_FIELDS, ap_uint<1>, 1, ap_uint<PKT_BUS_SIZE>, PKT_BUS_SIZE, MAX_PKT_SIZE, MAX_SUPP_HEADERS, MAX_PKT_ID_SIZE> icmp(IF_SOFTWARE("icmp",) 10, icmp_t);

	PHVData<ICMP_HEADER_SIZE, MAX_SUPP_HEADERS, MAX_PKT_ID_SIZE> wire_icmp_PHV;

	static std::array<PHVData<ICMP_HEADER_SIZE, MAX_SUPP_HEADERS, MAX_PKT_ID_SIZE>, MAX_SUPP_HEADERS + 1> tmp_icmp_PHV;
#pragma HLS ARRAY_PARTITION variable=tmp_icmp_PHV dim=1
#pragma HLS DEPENDENCE variable=tmp_icmp_PHV false

	tmpPacketIn[1] = PacketIn;

	ethernet.HeaderAnalysis(tmpPacketIn[1], tmp_ethernet_PHV[1], tmpPacketOut[1]);

	for (auto i = 1; i < MAX_SUPP_HEADERS; ++i)
		tmp_ethernet_PHV[i+1] = tmp_ethernet_PHV[i];

	ethernet_PHV = tmp_ethernet_PHV[MAX_SUPP_HEADERS];

	tmpPacketIn[2] = tmpPacketOut[1];

	outer_vlan.HeaderAnalysis(tmpPacketIn[2], tmp_outer_vlan_PHV[2], tmpPacketOut[2]);

	for (auto i = 2; i < MAX_SUPP_HEADERS; ++i)
		tmp_outer_vlan_PHV[i+1] = tmp_outer_vlan_PHV[i];

	outer_vlan_PHV = tmp_outer_vlan_PHV[MAX_SUPP_HEADERS];

	tmpPacketIn[3] = tmpPacketOut[2];

	inner_vlan.HeaderAnalysis(tmpPacketIn[3], tmp_inner_vlan_PHV[3], tmpPacketOut[3]);

	for (auto i = 3; i < MAX_SUPP_HEADERS; ++i)
		tmp_inner_vlan_PHV[i+1] = tmp_inner_vlan_PHV[i];

	inner_vlan_PHV = tmp_inner_vlan_PHV[MAX_SUPP_HEADERS];

	tmpPacketIn[4] = tmpPacketOut[1];

	outer_mpls.HeaderAnalysis(tmpPacketIn[4], tmp_outer_mpls_PHV[4], tmpPacketOut[4]);

	for (auto i = 4; i < MAX_SUPP_HEADERS; ++i)
		tmp_outer_mpls_PHV[i+1] = tmp_outer_mpls_PHV[i];

	outer_mpls_PHV = tmp_outer_mpls_PHV[MAX_SUPP_HEADERS];

	tmpPacketIn[5] = tmpPacketOut[4];

	inner_mpls.HeaderAnalysis(tmpPacketIn[5], tmp_inner_mpls_PHV[5], tmpPacketOut[5]);

	for (auto i = 5; i < MAX_SUPP_HEADERS; ++i)
		tmp_inner_mpls_PHV[i+1] = tmp_inner_mpls_PHV[i];

	inner_mpls_PHV = tmp_inner_mpls_PHV[MAX_SUPP_HEADERS];

	tmpPacketIn[6] = tmpPacketOut[3];

	ipv4.HeaderAnalysis(tmpPacketIn[6], tmp_ipv4_PHV[6], tmpPacketOut[6]);

	for (auto i = 6; i < MAX_SUPP_HEADERS; ++i)
		tmp_ipv4_PHV[i+1] = tmp_ipv4_PHV[i];

	ipv4_PHV = tmp_ipv4_PHV[MAX_SUPP_HEADERS];

	tmpPacketIn[7] = tmpPacketOut[3];

	ipv6.HeaderAnalysis(tmpPacketIn[7], tmp_ipv6_PHV[7], tmpPacketOut[7]);

	for (auto i = 7; i < MAX_SUPP_HEADERS; ++i)
		tmp_ipv6_PHV[i+1] = tmp_ipv6_PHV[i];

	ipv6_PHV = tmp_ipv6_PHV[MAX_SUPP_HEADERS];

	if (tmp_ipv4_PHV[6].Valid)

		tmpPacketIn[8] = tmpPacketOut[6];

	if (tmp_ipv6_PHV[7].Valid)

		tmpPacketIn[8] = tmpPacketOut[7];

	udp.HeaderAnalysis(tmpPacketIn[8], tmp_udp_PHV[8], tmpPacketOut[8]);

	for (auto i = 8; i < MAX_SUPP_HEADERS; ++i)
		tmp_udp_PHV[i+1] = tmp_udp_PHV[i];

	udp_PHV = tmp_udp_PHV[MAX_SUPP_HEADERS];

	if (tmp_ipv4_PHV[6].Valid)

		tmpPacketIn[9] = tmpPacketOut[6];

	if (tmp_ipv6_PHV[7].Valid)

		tmpPacketIn[9] = tmpPacketOut[7];

	tcp.HeaderAnalysis(tmpPacketIn[9], tmp_tcp_PHV[9], tmpPacketOut[9]);

	for (auto i = 9; i < MAX_SUPP_HEADERS; ++i)
		tmp_tcp_PHV[i+1] = tmp_tcp_PHV[i];

	tcp_PHV = tmp_tcp_PHV[MAX_SUPP_HEADERS];

	if (tmp_ipv4_PHV[6].Valid)

		tmpPacketIn[10] = tmpPacketOut[6];

	if (tmp_ipv6_PHV[7].Valid)

		tmpPacketIn[10] = tmpPacketOut[7];

	icmp.HeaderAnalysis(tmpPacketIn[10], tmp_icmp_PHV[10], tmpPacketOut[10]);

	for (auto i = 10; i < MAX_SUPP_HEADERS; ++i)
		tmp_icmp_PHV[i+1] = tmp_icmp_PHV[i];

	icmp_PHV = tmp_icmp_PHV[MAX_SUPP_HEADERS];

	if (tmp_inner_mpls_PHV[5].Valid)

		PacketOut = tmpPacketOut[5];

	if (tmp_udp_PHV[8].Valid)

		PacketOut = tmpPacketOut[8];

	if (tmp_tcp_PHV[9].Valid)

		PacketOut = tmpPacketOut[9];

	if (tmp_icmp_PHV[10].Valid)

		PacketOut = tmpPacketOut[10];

}
