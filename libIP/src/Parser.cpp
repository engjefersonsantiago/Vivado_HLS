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
#pragma HLS LATENCY min=8 max=8
/*Consider increase the max acceptable latency in case of clock violation*/

	// Wires
	std::array<PacketData<PKT_BUS_SIZE, MAX_SUPP_HEADERS, MAX_PKT_ID_SIZE>, MAX_SUPP_HEADERS + 1> tmpPacketOut;
#pragma HLS ARRAY_PARTITION variable=tmpPacketOut dim=1
	std::array<PacketData<PKT_BUS_SIZE, MAX_SUPP_HEADERS, MAX_PKT_ID_SIZE>, MAX_SUPP_HEADERS + 1> tmpPacketIn;
#pragma HLS ARRAY_PARTITION variable=tmpPacketIn dim=1

	// Stateful objects

#pragma HLS INTERFACE ap_ovld port=ethernet_PHV
	static Header<ETHERNET_HEADER_SIZE, ETHERNET_NUM_FIELDS, ap_uint<16>, 3, ap_uint<PKT_BUS_SIZE>, PKT_BUS_SIZE, MAX_PKT_SIZE, MAX_SUPP_HEADERS, MAX_PKT_ID_SIZE, HeaderFormat<ETHERNET_HEADER_SIZE, ETHERNET_NUM_FIELDS, ap_uint<16>, 3, MAX_SUPP_HEADERS,1, 1>, 1> ethernet(IF_SOFTWARE("ethernet",) 1, ethernet_t);

	PHVData<ETHERNET_HEADER_SIZE, MAX_SUPP_HEADERS, MAX_PKT_ID_SIZE> wire_ethernet_PHV;

	static std::array<PHVData<ETHERNET_HEADER_SIZE, MAX_SUPP_HEADERS, MAX_PKT_ID_SIZE>, 1> tmp_ethernet_PHV;
#pragma HLS ARRAY_PARTITION variable=tmp_ethernet_PHV dim=1
#pragma HLS DEPENDENCE variable=tmp_ethernet_PHV false

#pragma HLS INTERFACE ap_ovld port=vlan_PHV
	static Header<VLAN_HEADER_SIZE, VLAN_NUM_FIELDS, ap_uint<16>, 2, ap_uint<PKT_BUS_SIZE>, PKT_BUS_SIZE, MAX_PKT_SIZE, MAX_SUPP_HEADERS, MAX_PKT_ID_SIZE, HeaderFormat<VLAN_HEADER_SIZE, VLAN_NUM_FIELDS, ap_uint<16>, 2, MAX_SUPP_HEADERS,1, 1>, 1> vlan(IF_SOFTWARE("vlan",) 2, vlan_t);

	PHVData<VLAN_HEADER_SIZE, MAX_SUPP_HEADERS, MAX_PKT_ID_SIZE> wire_vlan_PHV;

	static std::array<PHVData<VLAN_HEADER_SIZE, MAX_SUPP_HEADERS, MAX_PKT_ID_SIZE>, 1> tmp_vlan_PHV;
#pragma HLS ARRAY_PARTITION variable=tmp_vlan_PHV dim=1
#pragma HLS DEPENDENCE variable=tmp_vlan_PHV false

#pragma HLS INTERFACE ap_ovld port=ipv4_var_PHV
	static Header<IPV4_VAR_HEADER_SIZE, IPV4_VAR_NUM_FIELDS, ap_uint<8>, 2, ap_uint<PKT_BUS_SIZE>, PKT_BUS_SIZE, MAX_PKT_SIZE, MAX_SUPP_HEADERS, MAX_PKT_ID_SIZE, ipv4_varHeaderFormat<IPV4_VAR_HEADER_SIZE, IPV4_VAR_NUM_FIELDS, ap_uint<8>, 2, MAX_SUPP_HEADERS,16, 9>, 16> ipv4_var(IF_SOFTWARE("ipv4_var",) 3, ipv4_var_t);

	PHVData<IPV4_VAR_HEADER_SIZE, MAX_SUPP_HEADERS, MAX_PKT_ID_SIZE> wire_ipv4_var_PHV;

	static std::array<PHVData<IPV4_VAR_HEADER_SIZE, MAX_SUPP_HEADERS, MAX_PKT_ID_SIZE>, 1> tmp_ipv4_var_PHV;
#pragma HLS ARRAY_PARTITION variable=tmp_ipv4_var_PHV dim=1
#pragma HLS DEPENDENCE variable=tmp_ipv4_var_PHV false

#pragma HLS INTERFACE ap_ovld port=ipv6_PHV
	static Header<IPV6_HEADER_SIZE, IPV6_NUM_FIELDS, ap_uint<8>, 2, ap_uint<PKT_BUS_SIZE>, PKT_BUS_SIZE, MAX_PKT_SIZE, MAX_SUPP_HEADERS, MAX_PKT_ID_SIZE, HeaderFormat<IPV6_HEADER_SIZE, IPV6_NUM_FIELDS, ap_uint<8>, 2, MAX_SUPP_HEADERS,1, 1>, 1> ipv6(IF_SOFTWARE("ipv6",) 4, ipv6_t);

	PHVData<IPV6_HEADER_SIZE, MAX_SUPP_HEADERS, MAX_PKT_ID_SIZE> wire_ipv6_PHV;

	static std::array<PHVData<IPV6_HEADER_SIZE, MAX_SUPP_HEADERS, MAX_PKT_ID_SIZE>, 1> tmp_ipv6_PHV;
#pragma HLS ARRAY_PARTITION variable=tmp_ipv6_PHV dim=1
#pragma HLS DEPENDENCE variable=tmp_ipv6_PHV false

#pragma HLS INTERFACE ap_ovld port=udp_PHV
	static Header<UDP_HEADER_SIZE, UDP_NUM_FIELDS, ap_uint<1>, 1, ap_uint<PKT_BUS_SIZE>, PKT_BUS_SIZE, MAX_PKT_SIZE, MAX_SUPP_HEADERS, MAX_PKT_ID_SIZE, HeaderFormat<UDP_HEADER_SIZE, UDP_NUM_FIELDS, ap_uint<1>, 1, MAX_SUPP_HEADERS,1, 1>, 1> udp(IF_SOFTWARE("udp",) 5, udp_t);

	PHVData<UDP_HEADER_SIZE, MAX_SUPP_HEADERS, MAX_PKT_ID_SIZE> wire_udp_PHV;

	static std::array<PHVData<UDP_HEADER_SIZE, MAX_SUPP_HEADERS, MAX_PKT_ID_SIZE>, 1> tmp_udp_PHV;
#pragma HLS ARRAY_PARTITION variable=tmp_udp_PHV dim=1
#pragma HLS DEPENDENCE variable=tmp_udp_PHV false

#pragma HLS INTERFACE ap_ovld port=tcp_PHV
	static Header<TCP_HEADER_SIZE, TCP_NUM_FIELDS, ap_uint<1>, 1, ap_uint<PKT_BUS_SIZE>, PKT_BUS_SIZE, MAX_PKT_SIZE, MAX_SUPP_HEADERS, MAX_PKT_ID_SIZE, HeaderFormat<TCP_HEADER_SIZE, TCP_NUM_FIELDS, ap_uint<1>, 1, MAX_SUPP_HEADERS,1, 1>, 1> tcp(IF_SOFTWARE("tcp",) 6, tcp_t);

	PHVData<TCP_HEADER_SIZE, MAX_SUPP_HEADERS, MAX_PKT_ID_SIZE> wire_tcp_PHV;

	static std::array<PHVData<TCP_HEADER_SIZE, MAX_SUPP_HEADERS, MAX_PKT_ID_SIZE>, 1> tmp_tcp_PHV;
#pragma HLS ARRAY_PARTITION variable=tmp_tcp_PHV dim=1
#pragma HLS DEPENDENCE variable=tmp_tcp_PHV false

	//---------------------------------------
	//ethernet
	//---------------------------------------

	tmpPacketIn[1] = PacketIn;

	ethernet.HeaderAnalysis(tmpPacketIn[1], tmp_ethernet_PHV[0], tmpPacketOut[1]);

	ethernet_PHV.Data = (ethernet_t.PHVMask == 0) ? ap_uint<bytes2Bits(14)>(0) :  tmp_ethernet_PHV[0].Data & ethernet_t.PHVMask;
	ethernet_PHV.ExtractedBitNum = tmp_ethernet_PHV[0].ExtractedBitNum;
	ethernet_PHV.Valid = tmp_ethernet_PHV[0].Valid;
	ethernet_PHV.ValidPulse = tmp_ethernet_PHV[0].ValidPulse;
	ethernet_PHV.ID = tmp_ethernet_PHV[0].ID;
	ethernet_PHV.PktID = tmp_ethernet_PHV[0].PktID;
	IF_SOFTWARE(ethernet_PHV.Name = tmp_ethernet_PHV[0].Name;)
	//---------------------------------------
	//vlan
	//---------------------------------------

	tmpPacketIn[2] = tmpPacketOut[1];

	vlan.HeaderAnalysis(tmpPacketIn[2], tmp_vlan_PHV[0], tmpPacketOut[2]);

	vlan_PHV.Data = (vlan_t.PHVMask == 0) ? ap_uint<bytes2Bits(4)>(0) :  tmp_vlan_PHV[0].Data & vlan_t.PHVMask;
	vlan_PHV.ExtractedBitNum = tmp_vlan_PHV[0].ExtractedBitNum;
	vlan_PHV.Valid = tmp_vlan_PHV[0].Valid;
	vlan_PHV.ValidPulse = tmp_vlan_PHV[0].ValidPulse;
	vlan_PHV.ID = tmp_vlan_PHV[0].ID;
	vlan_PHV.PktID = tmp_vlan_PHV[0].PktID;
	IF_SOFTWARE(vlan_PHV.Name = tmp_vlan_PHV[0].Name;)
	//---------------------------------------
	//ipv6
	//---------------------------------------

		tmpPacketIn[4] = tmpPacketOut[2];

	ipv6.HeaderAnalysis(tmpPacketIn[4], tmp_ipv6_PHV[0], tmpPacketOut[4]);

	ipv6_PHV.Data = (ipv6_t.PHVMask == 0) ? ap_uint<bytes2Bits(40)>(0) :  tmp_ipv6_PHV[0].Data & ipv6_t.PHVMask;
	ipv6_PHV.ExtractedBitNum = tmp_ipv6_PHV[0].ExtractedBitNum;
	ipv6_PHV.Valid = tmp_ipv6_PHV[0].Valid;
	ipv6_PHV.ValidPulse = tmp_ipv6_PHV[0].ValidPulse;
	ipv6_PHV.ID = tmp_ipv6_PHV[0].ID;
	ipv6_PHV.PktID = tmp_ipv6_PHV[0].PktID;
	IF_SOFTWARE(ipv6_PHV.Name = tmp_ipv6_PHV[0].Name;)
	//---------------------------------------
	//ipv4_var
	//---------------------------------------

		tmpPacketIn[3] = tmpPacketOut[2];

	ipv4_var.HeaderAnalysis(tmpPacketIn[3], tmp_ipv4_var_PHV[0], tmpPacketOut[3]);

	ipv4_var_PHV.Data = (ipv4_var_t.PHVMask == 0) ? ap_uint<bytes2Bits(60)>(0) :  tmp_ipv4_var_PHV[0].Data & ipv4_var_t.PHVMask;
	ipv4_var_PHV.ExtractedBitNum = tmp_ipv4_var_PHV[0].ExtractedBitNum;
	ipv4_var_PHV.Valid = tmp_ipv4_var_PHV[0].Valid;
	ipv4_var_PHV.ValidPulse = tmp_ipv4_var_PHV[0].ValidPulse;
	ipv4_var_PHV.ID = tmp_ipv4_var_PHV[0].ID;
	ipv4_var_PHV.PktID = tmp_ipv4_var_PHV[0].PktID;
	IF_SOFTWARE(ipv4_var_PHV.Name = tmp_ipv4_var_PHV[0].Name;)
	//---------------------------------------
	//udp
	//---------------------------------------

	if (tmp_ipv4_var_PHV[0].Valid)

		tmpPacketIn[5] = tmpPacketOut[3];

	if (tmp_ipv6_PHV[0].Valid)

		tmpPacketIn[5] = tmpPacketOut[4];

	udp.HeaderAnalysis(tmpPacketIn[5], tmp_udp_PHV[0], tmpPacketOut[5]);

	udp_PHV.Data = (udp_t.PHVMask == 0) ? ap_uint<bytes2Bits(8)>(0) :  tmp_udp_PHV[0].Data & udp_t.PHVMask;
	udp_PHV.ExtractedBitNum = tmp_udp_PHV[0].ExtractedBitNum;
	udp_PHV.Valid = tmp_udp_PHV[0].Valid;
	udp_PHV.ValidPulse = tmp_udp_PHV[0].ValidPulse;
	udp_PHV.ID = tmp_udp_PHV[0].ID;
	udp_PHV.PktID = tmp_udp_PHV[0].PktID;
	IF_SOFTWARE(udp_PHV.Name = tmp_udp_PHV[0].Name;)
	//---------------------------------------
	//tcp
	//---------------------------------------

	if (tmp_ipv4_var_PHV[0].Valid)

		tmpPacketIn[6] = tmpPacketOut[3];

	if (tmp_ipv6_PHV[0].Valid)

		tmpPacketIn[6] = tmpPacketOut[4];

	tcp.HeaderAnalysis(tmpPacketIn[6], tmp_tcp_PHV[0], tmpPacketOut[6]);

	tcp_PHV.Data = (tcp_t.PHVMask == 0) ? ap_uint<bytes2Bits(20)>(0) :  tmp_tcp_PHV[0].Data & tcp_t.PHVMask;
	tcp_PHV.ExtractedBitNum = tmp_tcp_PHV[0].ExtractedBitNum;
	tcp_PHV.Valid = tmp_tcp_PHV[0].Valid;
	tcp_PHV.ValidPulse = tmp_tcp_PHV[0].ValidPulse;
	tcp_PHV.ID = tmp_tcp_PHV[0].ID;
	tcp_PHV.PktID = tmp_tcp_PHV[0].PktID;
	IF_SOFTWARE(tcp_PHV.Name = tmp_tcp_PHV[0].Name;)
	//---------------------------------------
	//Ouput data bus
	//---------------------------------------

	if (tmp_udp_PHV[0].Valid)

		PacketOut = tmpPacketOut[5];

	else

		PacketOut = tmpPacketOut[6];

}
