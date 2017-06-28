/******************************************************************************
* Packet Parser: header extraction and supported protocols graph              *
* Jeferson Santiago da Silva (eng.jefersonsantiago@gmail.com)                 *
******************************************************************************/

#include "Parser.hpp"

void HeaderAnalysisTop(const PacketData<PKT_BUS_SIZE, MAX_SUPP_HEADERS, MAX_PKT_ID_SIZE>& PacketIn,
	PHVData<ETHERNET_HEADER_SIZE, MAX_SUPP_HEADERS, MAX_PKT_ID_SIZE>& ethernet_PHV,
	PHVData<VLAN_HEADER_SIZE, MAX_SUPP_HEADERS, MAX_PKT_ID_SIZE>& vlan_PHV,
	PHVData<IPV4_VAR_HEADER_SIZE, MAX_SUPP_HEADERS, MAX_PKT_ID_SIZE>& ipv4_var_PHV,
	PHVData<IPV6_HEADER_SIZE, MAX_SUPP_HEADERS, MAX_PKT_ID_SIZE>& ipv6_PHV,
	PHVData<UDP_HEADER_SIZE, MAX_SUPP_HEADERS, MAX_PKT_ID_SIZE>& udp_PHV,
	PHVData<TCP_HEADER_SIZE, MAX_SUPP_HEADERS, MAX_PKT_ID_SIZE>& tcp_PHV,
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
	static Header<ETHERNET_HEADER_SIZE, ETHERNET_NUM_FIELDS, ap_uint<16>, 3, ap_uint<PKT_BUS_SIZE>, PKT_BUS_SIZE, MAX_PKT_SIZE, MAX_SUPP_HEADERS, MAX_PKT_ID_SIZE, HeaderFormat<ETHERNET_HEADER_SIZE, ETHERNET_NUM_FIELDS, ap_uint<16>, 3, MAX_SUPP_HEADERS>> ethernet(IF_SOFTWARE("ethernet",) 1, ethernet_t);

	PHVData<ETHERNET_HEADER_SIZE, MAX_SUPP_HEADERS, MAX_PKT_ID_SIZE> wire_ethernet_PHV;

	static std::array<PHVData<ETHERNET_HEADER_SIZE, MAX_SUPP_HEADERS, MAX_PKT_ID_SIZE>, MAX_SUPP_HEADERS + 1> tmp_ethernet_PHV;
#pragma HLS ARRAY_PARTITION variable=tmp_ethernet_PHV dim=1
#pragma HLS DEPENDENCE variable=tmp_ethernet_PHV false

#pragma HLS INTERFACE ap_ovld port=vlan_PHV
	static Header<VLAN_HEADER_SIZE, VLAN_NUM_FIELDS, ap_uint<16>, 2, ap_uint<PKT_BUS_SIZE>, PKT_BUS_SIZE, MAX_PKT_SIZE, MAX_SUPP_HEADERS, MAX_PKT_ID_SIZE, HeaderFormat<VLAN_HEADER_SIZE, VLAN_NUM_FIELDS, ap_uint<16>, 2, MAX_SUPP_HEADERS>> vlan(IF_SOFTWARE("vlan",) 2, vlan_t);

	PHVData<VLAN_HEADER_SIZE, MAX_SUPP_HEADERS, MAX_PKT_ID_SIZE> wire_vlan_PHV;

	static std::array<PHVData<VLAN_HEADER_SIZE, MAX_SUPP_HEADERS, MAX_PKT_ID_SIZE>, MAX_SUPP_HEADERS + 1> tmp_vlan_PHV;
#pragma HLS ARRAY_PARTITION variable=tmp_vlan_PHV dim=1
#pragma HLS DEPENDENCE variable=tmp_vlan_PHV false

#pragma HLS INTERFACE ap_ovld port=ipv4_var_PHV
	static Header<IPV4_VAR_HEADER_SIZE, IPV4_VAR_NUM_FIELDS, ap_uint<8>, 2, ap_uint<PKT_BUS_SIZE>, PKT_BUS_SIZE, MAX_PKT_SIZE, MAX_SUPP_HEADERS, MAX_PKT_ID_SIZE, ipv4_varHeaderFormat<IPV4_VAR_HEADER_SIZE, IPV4_VAR_NUM_FIELDS, ap_uint<8>, 2, MAX_SUPP_HEADERS>> ipv4_var(IF_SOFTWARE("ipv4_var",) 3, ipv4_var_t);

	PHVData<IPV4_VAR_HEADER_SIZE, MAX_SUPP_HEADERS, MAX_PKT_ID_SIZE> wire_ipv4_var_PHV;

	static std::array<PHVData<IPV4_VAR_HEADER_SIZE, MAX_SUPP_HEADERS, MAX_PKT_ID_SIZE>, MAX_SUPP_HEADERS + 1> tmp_ipv4_var_PHV;
#pragma HLS ARRAY_PARTITION variable=tmp_ipv4_var_PHV dim=1
#pragma HLS DEPENDENCE variable=tmp_ipv4_var_PHV false

#pragma HLS INTERFACE ap_ovld port=ipv6_PHV
	static Header<IPV6_HEADER_SIZE, IPV6_NUM_FIELDS, ap_uint<8>, 2, ap_uint<PKT_BUS_SIZE>, PKT_BUS_SIZE, MAX_PKT_SIZE, MAX_SUPP_HEADERS, MAX_PKT_ID_SIZE, HeaderFormat<IPV6_HEADER_SIZE, IPV6_NUM_FIELDS, ap_uint<8>, 2, MAX_SUPP_HEADERS>> ipv6(IF_SOFTWARE("ipv6",) 4, ipv6_t);

	PHVData<IPV6_HEADER_SIZE, MAX_SUPP_HEADERS, MAX_PKT_ID_SIZE> wire_ipv6_PHV;

	static std::array<PHVData<IPV6_HEADER_SIZE, MAX_SUPP_HEADERS, MAX_PKT_ID_SIZE>, MAX_SUPP_HEADERS + 1> tmp_ipv6_PHV;
#pragma HLS ARRAY_PARTITION variable=tmp_ipv6_PHV dim=1
#pragma HLS DEPENDENCE variable=tmp_ipv6_PHV false

#pragma HLS INTERFACE ap_ovld port=udp_PHV
	static Header<UDP_HEADER_SIZE, UDP_NUM_FIELDS, ap_uint<1>, 1, ap_uint<PKT_BUS_SIZE>, PKT_BUS_SIZE, MAX_PKT_SIZE, MAX_SUPP_HEADERS, MAX_PKT_ID_SIZE, HeaderFormat<UDP_HEADER_SIZE, UDP_NUM_FIELDS, ap_uint<1>, 1, MAX_SUPP_HEADERS>> udp(IF_SOFTWARE("udp",) 5, udp_t);

	PHVData<UDP_HEADER_SIZE, MAX_SUPP_HEADERS, MAX_PKT_ID_SIZE> wire_udp_PHV;

	static std::array<PHVData<UDP_HEADER_SIZE, MAX_SUPP_HEADERS, MAX_PKT_ID_SIZE>, MAX_SUPP_HEADERS + 1> tmp_udp_PHV;
#pragma HLS ARRAY_PARTITION variable=tmp_udp_PHV dim=1
#pragma HLS DEPENDENCE variable=tmp_udp_PHV false

#pragma HLS INTERFACE ap_ovld port=tcp_PHV
	static Header<TCP_HEADER_SIZE, TCP_NUM_FIELDS, ap_uint<1>, 1, ap_uint<PKT_BUS_SIZE>, PKT_BUS_SIZE, MAX_PKT_SIZE, MAX_SUPP_HEADERS, MAX_PKT_ID_SIZE, HeaderFormat<TCP_HEADER_SIZE, TCP_NUM_FIELDS, ap_uint<1>, 1, MAX_SUPP_HEADERS>> tcp(IF_SOFTWARE("tcp",) 6, tcp_t);

	PHVData<TCP_HEADER_SIZE, MAX_SUPP_HEADERS, MAX_PKT_ID_SIZE> wire_tcp_PHV;

	static std::array<PHVData<TCP_HEADER_SIZE, MAX_SUPP_HEADERS, MAX_PKT_ID_SIZE>, MAX_SUPP_HEADERS + 1> tmp_tcp_PHV;
#pragma HLS ARRAY_PARTITION variable=tmp_tcp_PHV dim=1
#pragma HLS DEPENDENCE variable=tmp_tcp_PHV false

	tmpPacketIn[1] = PacketIn;

	ethernet.HeaderAnalysis(tmpPacketIn[1], tmp_ethernet_PHV[1], tmpPacketOut[1]);

	for (auto i = 1; i < MAX_SUPP_HEADERS; ++i)
		tmp_ethernet_PHV[i+1] = tmp_ethernet_PHV[i];

	ethernet_PHV = tmp_ethernet_PHV[MAX_SUPP_HEADERS];

	tmpPacketIn[2] = tmpPacketOut[1];

	vlan.HeaderAnalysis(tmpPacketIn[2], tmp_vlan_PHV[2], tmpPacketOut[2]);

	for (auto i = 2; i < MAX_SUPP_HEADERS; ++i)
		tmp_vlan_PHV[i+1] = tmp_vlan_PHV[i];

	vlan_PHV = tmp_vlan_PHV[MAX_SUPP_HEADERS];

	tmpPacketIn[3] = tmpPacketOut[2];

	ipv4_var.HeaderAnalysis(tmpPacketIn[3], tmp_ipv4_var_PHV[3], tmpPacketOut[3]);

	for (auto i = 3; i < MAX_SUPP_HEADERS; ++i)
		tmp_ipv4_var_PHV[i+1] = tmp_ipv4_var_PHV[i];

	ipv4_var_PHV = tmp_ipv4_var_PHV[MAX_SUPP_HEADERS];

	tmpPacketIn[4] = tmpPacketOut[2];

	ipv6.HeaderAnalysis(tmpPacketIn[4], tmp_ipv6_PHV[4], tmpPacketOut[4]);

	for (auto i = 4; i < MAX_SUPP_HEADERS; ++i)
		tmp_ipv6_PHV[i+1] = tmp_ipv6_PHV[i];

	ipv6_PHV = tmp_ipv6_PHV[MAX_SUPP_HEADERS];

	if (tmp_ipv4_var_PHV[3].Valid)

		tmpPacketIn[5] = tmpPacketOut[3];

	if (tmp_ipv6_PHV[4].Valid)

		tmpPacketIn[5] = tmpPacketOut[4];

	udp.HeaderAnalysis(tmpPacketIn[5], tmp_udp_PHV[5], tmpPacketOut[5]);

	for (auto i = 5; i < MAX_SUPP_HEADERS; ++i)
		tmp_udp_PHV[i+1] = tmp_udp_PHV[i];

	udp_PHV = tmp_udp_PHV[MAX_SUPP_HEADERS];

	if (tmp_ipv4_var_PHV[3].Valid)

		tmpPacketIn[6] = tmpPacketOut[3];

	if (tmp_ipv6_PHV[4].Valid)

		tmpPacketIn[6] = tmpPacketOut[4];

	tcp.HeaderAnalysis(tmpPacketIn[6], tmp_tcp_PHV[6], tmpPacketOut[6]);

	for (auto i = 6; i < MAX_SUPP_HEADERS; ++i)
		tmp_tcp_PHV[i+1] = tmp_tcp_PHV[i];

	tcp_PHV = tmp_tcp_PHV[MAX_SUPP_HEADERS];

	if (tmp_udp_PHV[5].Valid)

		PacketOut = tmpPacketOut[5];

	if (tmp_tcp_PHV[6].Valid)

		PacketOut = tmpPacketOut[6];

}
