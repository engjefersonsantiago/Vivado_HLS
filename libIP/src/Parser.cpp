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
	PHVData<IPV6_EXT_1_HEADER_SIZE, MAX_SUPP_HEADERS, MAX_PKT_ID_SIZE>& ipv6_ext_1_PHV,
	PHVData<IPV6_EXT_2_HEADER_SIZE, MAX_SUPP_HEADERS, MAX_PKT_ID_SIZE>& ipv6_ext_2_PHV,
	PHVData<UDP_HEADER_SIZE, MAX_SUPP_HEADERS, MAX_PKT_ID_SIZE>& udp_PHV,
	PHVData<TCP_HEADER_SIZE, MAX_SUPP_HEADERS, MAX_PKT_ID_SIZE>& tcp_PHV,
	PHVData<ICMP_HEADER_SIZE, MAX_SUPP_HEADERS, MAX_PKT_ID_SIZE>& icmp_PHV,
	PacketData<PKT_BUS_SIZE, MAX_SUPP_HEADERS, MAX_PKT_ID_SIZE>& PacketOut)
{
#pragma HLS INTERFACE ap_ctrl_none port=return
#pragma HLS PIPELINE II=1
#pragma HLS LATENCY min=7 max=10
/*Consider increase the max acceptable latency in case of clock violation*/

	// Wires
	std::array<PacketData<PKT_BUS_SIZE, MAX_SUPP_HEADERS, MAX_PKT_ID_SIZE>, MAX_SUPP_HEADERS + 1> tmpPacketOut;
#pragma HLS ARRAY_PARTITION variable=tmpPacketOut dim=1
	std::array<PacketData<PKT_BUS_SIZE, MAX_SUPP_HEADERS, MAX_PKT_ID_SIZE>, MAX_SUPP_HEADERS + 1> tmpPacketIn;
#pragma HLS ARRAY_PARTITION variable=tmpPacketIn dim=1

	// Stateful objects

#pragma HLS INTERFACE ap_ovld port=ethernet_PHV
	static Header<ETHERNET_HEADER_SIZE, ETHERNET_NUM_FIELDS, ap_uint<16>, 5, ap_uint<PKT_BUS_SIZE>, PKT_BUS_SIZE, MAX_PKT_SIZE, MAX_SUPP_HEADERS, MAX_PKT_ID_SIZE, HeaderFormat<ETHERNET_HEADER_SIZE, ETHERNET_NUM_FIELDS, ap_uint<16>, 5, MAX_SUPP_HEADERS,1, 1>, 1> ethernet(IF_SOFTWARE("ethernet",) 1, ethernet_t);

	PHVData<ETHERNET_HEADER_SIZE, MAX_SUPP_HEADERS, MAX_PKT_ID_SIZE> wire_ethernet_PHV;

	static std::array<PHVData<ETHERNET_HEADER_SIZE, MAX_SUPP_HEADERS, MAX_PKT_ID_SIZE>, 1> tmp_ethernet_PHV;
#pragma HLS ARRAY_PARTITION variable=tmp_ethernet_PHV dim=1
#pragma HLS DEPENDENCE variable=tmp_ethernet_PHV false

#pragma HLS INTERFACE ap_ovld port=outer_vlan_PHV
	static Header<OUTER_VLAN_HEADER_SIZE, OUTER_VLAN_NUM_FIELDS, ap_uint<16>, 1, ap_uint<PKT_BUS_SIZE>, PKT_BUS_SIZE, MAX_PKT_SIZE, MAX_SUPP_HEADERS, MAX_PKT_ID_SIZE, HeaderFormat<OUTER_VLAN_HEADER_SIZE, OUTER_VLAN_NUM_FIELDS, ap_uint<16>, 1, MAX_SUPP_HEADERS,1, 1>, 1> outer_vlan(IF_SOFTWARE("outer_vlan",) 2, vlan_outer_t);

	PHVData<OUTER_VLAN_HEADER_SIZE, MAX_SUPP_HEADERS, MAX_PKT_ID_SIZE> wire_outer_vlan_PHV;

	static std::array<PHVData<OUTER_VLAN_HEADER_SIZE, MAX_SUPP_HEADERS, MAX_PKT_ID_SIZE>, 1> tmp_outer_vlan_PHV;
#pragma HLS ARRAY_PARTITION variable=tmp_outer_vlan_PHV dim=1
#pragma HLS DEPENDENCE variable=tmp_outer_vlan_PHV false

#pragma HLS INTERFACE ap_ovld port=inner_vlan_PHV
	static Header<INNER_VLAN_HEADER_SIZE, INNER_VLAN_NUM_FIELDS, ap_uint<16>, 2, ap_uint<PKT_BUS_SIZE>, PKT_BUS_SIZE, MAX_PKT_SIZE, MAX_SUPP_HEADERS, MAX_PKT_ID_SIZE, HeaderFormat<INNER_VLAN_HEADER_SIZE, INNER_VLAN_NUM_FIELDS, ap_uint<16>, 2, MAX_SUPP_HEADERS,1, 1>, 1> inner_vlan(IF_SOFTWARE("inner_vlan",) 3, vlan_inner_t);

	PHVData<INNER_VLAN_HEADER_SIZE, MAX_SUPP_HEADERS, MAX_PKT_ID_SIZE> wire_inner_vlan_PHV;

	static std::array<PHVData<INNER_VLAN_HEADER_SIZE, MAX_SUPP_HEADERS, MAX_PKT_ID_SIZE>, 1> tmp_inner_vlan_PHV;
#pragma HLS ARRAY_PARTITION variable=tmp_inner_vlan_PHV dim=1
#pragma HLS DEPENDENCE variable=tmp_inner_vlan_PHV false

#pragma HLS INTERFACE ap_ovld port=outer_mpls_PHV
	static Header<OUTER_MPLS_HEADER_SIZE, OUTER_MPLS_NUM_FIELDS, ap_uint<3>, 1, ap_uint<PKT_BUS_SIZE>, PKT_BUS_SIZE, MAX_PKT_SIZE, MAX_SUPP_HEADERS, MAX_PKT_ID_SIZE, HeaderFormat<OUTER_MPLS_HEADER_SIZE, OUTER_MPLS_NUM_FIELDS, ap_uint<3>, 1, MAX_SUPP_HEADERS,1, 1>, 1> outer_mpls(IF_SOFTWARE("outer_mpls",) 4, mpls_outer_t);

	PHVData<OUTER_MPLS_HEADER_SIZE, MAX_SUPP_HEADERS, MAX_PKT_ID_SIZE> wire_outer_mpls_PHV;

	static std::array<PHVData<OUTER_MPLS_HEADER_SIZE, MAX_SUPP_HEADERS, MAX_PKT_ID_SIZE>, 1> tmp_outer_mpls_PHV;
#pragma HLS ARRAY_PARTITION variable=tmp_outer_mpls_PHV dim=1
#pragma HLS DEPENDENCE variable=tmp_outer_mpls_PHV false

#pragma HLS INTERFACE ap_ovld port=inner_mpls_PHV
	static Header<INNER_MPLS_HEADER_SIZE, INNER_MPLS_NUM_FIELDS, ap_uint<1>, 1, ap_uint<PKT_BUS_SIZE>, PKT_BUS_SIZE, MAX_PKT_SIZE, MAX_SUPP_HEADERS, MAX_PKT_ID_SIZE, HeaderFormat<INNER_MPLS_HEADER_SIZE, INNER_MPLS_NUM_FIELDS, ap_uint<1>, 1, MAX_SUPP_HEADERS,1, 1>, 1> inner_mpls(IF_SOFTWARE("inner_mpls",) 5, mpls_inner_t);

	PHVData<INNER_MPLS_HEADER_SIZE, MAX_SUPP_HEADERS, MAX_PKT_ID_SIZE> wire_inner_mpls_PHV;

	static std::array<PHVData<INNER_MPLS_HEADER_SIZE, MAX_SUPP_HEADERS, MAX_PKT_ID_SIZE>, 1> tmp_inner_mpls_PHV;
#pragma HLS ARRAY_PARTITION variable=tmp_inner_mpls_PHV dim=1
#pragma HLS DEPENDENCE variable=tmp_inner_mpls_PHV false

#pragma HLS INTERFACE ap_ovld port=ipv4_PHV
	static Header<IPV4_HEADER_SIZE, IPV4_NUM_FIELDS, ap_uint<8>, 3, ap_uint<PKT_BUS_SIZE>, PKT_BUS_SIZE, MAX_PKT_SIZE, MAX_SUPP_HEADERS, MAX_PKT_ID_SIZE, HeaderFormat<IPV4_HEADER_SIZE, IPV4_NUM_FIELDS, ap_uint<8>, 3, MAX_SUPP_HEADERS,1, 1>, 1> ipv4(IF_SOFTWARE("ipv4",) 6, ipv4_t);

	PHVData<IPV4_HEADER_SIZE, MAX_SUPP_HEADERS, MAX_PKT_ID_SIZE> wire_ipv4_PHV;

	static std::array<PHVData<IPV4_HEADER_SIZE, MAX_SUPP_HEADERS, MAX_PKT_ID_SIZE>, 1> tmp_ipv4_PHV;
#pragma HLS ARRAY_PARTITION variable=tmp_ipv4_PHV dim=1
#pragma HLS DEPENDENCE variable=tmp_ipv4_PHV false

#pragma HLS INTERFACE ap_ovld port=ipv6_PHV
	static Header<IPV6_HEADER_SIZE, IPV6_NUM_FIELDS, ap_uint<8>, 4, ap_uint<PKT_BUS_SIZE>, PKT_BUS_SIZE, MAX_PKT_SIZE, MAX_SUPP_HEADERS, MAX_PKT_ID_SIZE, HeaderFormat<IPV6_HEADER_SIZE, IPV6_NUM_FIELDS, ap_uint<8>, 4, MAX_SUPP_HEADERS,1, 1>, 1> ipv6(IF_SOFTWARE("ipv6",) 7, ipv6_t);

	PHVData<IPV6_HEADER_SIZE, MAX_SUPP_HEADERS, MAX_PKT_ID_SIZE> wire_ipv6_PHV;

	static std::array<PHVData<IPV6_HEADER_SIZE, MAX_SUPP_HEADERS, MAX_PKT_ID_SIZE>, 1> tmp_ipv6_PHV;
#pragma HLS ARRAY_PARTITION variable=tmp_ipv6_PHV dim=1
#pragma HLS DEPENDENCE variable=tmp_ipv6_PHV false

#pragma HLS INTERFACE ap_ovld port=ipv6_ext_1_PHV
	static Header<IPV6_EXT_1_HEADER_SIZE, IPV6_EXT_1_NUM_FIELDS, ap_uint<8>, 4, ap_uint<PKT_BUS_SIZE>, PKT_BUS_SIZE, MAX_PKT_SIZE, MAX_SUPP_HEADERS, MAX_PKT_ID_SIZE, ipv6_ext_1HeaderFormat<IPV6_EXT_1_HEADER_SIZE, IPV6_EXT_1_NUM_FIELDS, ap_uint<8>, 4, MAX_SUPP_HEADERS,4, 9>, 4> ipv6_ext_1(IF_SOFTWARE("ipv6_ext_1",) 10, ipv6_ext_1_t);

	PHVData<IPV6_EXT_1_HEADER_SIZE, MAX_SUPP_HEADERS, MAX_PKT_ID_SIZE> wire_ipv6_ext_1_PHV;

	static std::array<PHVData<IPV6_EXT_1_HEADER_SIZE, MAX_SUPP_HEADERS, MAX_PKT_ID_SIZE>, 1> tmp_ipv6_ext_1_PHV;
#pragma HLS ARRAY_PARTITION variable=tmp_ipv6_ext_1_PHV dim=1
#pragma HLS DEPENDENCE variable=tmp_ipv6_ext_1_PHV false

#pragma HLS INTERFACE ap_ovld port=ipv6_ext_2_PHV
	static Header<IPV6_EXT_2_HEADER_SIZE, IPV6_EXT_2_NUM_FIELDS, ap_uint<8>, 3, ap_uint<PKT_BUS_SIZE>, PKT_BUS_SIZE, MAX_PKT_SIZE, MAX_SUPP_HEADERS, MAX_PKT_ID_SIZE, ipv6_ext_2HeaderFormat<IPV6_EXT_2_HEADER_SIZE, IPV6_EXT_2_NUM_FIELDS, ap_uint<8>, 3, MAX_SUPP_HEADERS,4, 9>, 4> ipv6_ext_2(IF_SOFTWARE("ipv6_ext_2",) 11, ipv6_ext_2_t);

	PHVData<IPV6_EXT_2_HEADER_SIZE, MAX_SUPP_HEADERS, MAX_PKT_ID_SIZE> wire_ipv6_ext_2_PHV;

	static std::array<PHVData<IPV6_EXT_2_HEADER_SIZE, MAX_SUPP_HEADERS, MAX_PKT_ID_SIZE>, 1> tmp_ipv6_ext_2_PHV;
#pragma HLS ARRAY_PARTITION variable=tmp_ipv6_ext_2_PHV dim=1
#pragma HLS DEPENDENCE variable=tmp_ipv6_ext_2_PHV false

#pragma HLS INTERFACE ap_ovld port=udp_PHV
	static Header<UDP_HEADER_SIZE, UDP_NUM_FIELDS, ap_uint<1>, 1, ap_uint<PKT_BUS_SIZE>, PKT_BUS_SIZE, MAX_PKT_SIZE, MAX_SUPP_HEADERS, MAX_PKT_ID_SIZE, HeaderFormat<UDP_HEADER_SIZE, UDP_NUM_FIELDS, ap_uint<1>, 1, MAX_SUPP_HEADERS,1, 1>, 1> udp(IF_SOFTWARE("udp",) 8, udp_t);

	PHVData<UDP_HEADER_SIZE, MAX_SUPP_HEADERS, MAX_PKT_ID_SIZE> wire_udp_PHV;

	static std::array<PHVData<UDP_HEADER_SIZE, MAX_SUPP_HEADERS, MAX_PKT_ID_SIZE>, 1> tmp_udp_PHV;
#pragma HLS ARRAY_PARTITION variable=tmp_udp_PHV dim=1
#pragma HLS DEPENDENCE variable=tmp_udp_PHV false

#pragma HLS INTERFACE ap_ovld port=tcp_PHV
	static Header<TCP_HEADER_SIZE, TCP_NUM_FIELDS, ap_uint<1>, 1, ap_uint<PKT_BUS_SIZE>, PKT_BUS_SIZE, MAX_PKT_SIZE, MAX_SUPP_HEADERS, MAX_PKT_ID_SIZE, HeaderFormat<TCP_HEADER_SIZE, TCP_NUM_FIELDS, ap_uint<1>, 1, MAX_SUPP_HEADERS,1, 1>, 1> tcp(IF_SOFTWARE("tcp",) 9, tcp_t);

	PHVData<TCP_HEADER_SIZE, MAX_SUPP_HEADERS, MAX_PKT_ID_SIZE> wire_tcp_PHV;

	static std::array<PHVData<TCP_HEADER_SIZE, MAX_SUPP_HEADERS, MAX_PKT_ID_SIZE>, 1> tmp_tcp_PHV;
#pragma HLS ARRAY_PARTITION variable=tmp_tcp_PHV dim=1
#pragma HLS DEPENDENCE variable=tmp_tcp_PHV false

#pragma HLS INTERFACE ap_ovld port=icmp_PHV
	static Header<ICMP_HEADER_SIZE, ICMP_NUM_FIELDS, ap_uint<1>, 1, ap_uint<PKT_BUS_SIZE>, PKT_BUS_SIZE, MAX_PKT_SIZE, MAX_SUPP_HEADERS, MAX_PKT_ID_SIZE, HeaderFormat<ICMP_HEADER_SIZE, ICMP_NUM_FIELDS, ap_uint<1>, 1, MAX_SUPP_HEADERS,1, 1>, 1> icmp(IF_SOFTWARE("icmp",) 12, icmp_t);

	PHVData<ICMP_HEADER_SIZE, MAX_SUPP_HEADERS, MAX_PKT_ID_SIZE> wire_icmp_PHV;

	static std::array<PHVData<ICMP_HEADER_SIZE, MAX_SUPP_HEADERS, MAX_PKT_ID_SIZE>, 1> tmp_icmp_PHV;
#pragma HLS ARRAY_PARTITION variable=tmp_icmp_PHV dim=1
#pragma HLS DEPENDENCE variable=tmp_icmp_PHV false

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
	//outer_mpls
	//---------------------------------------

	tmpPacketIn[4] = tmpPacketOut[1];

	outer_mpls.HeaderAnalysis(tmpPacketIn[4], tmp_outer_mpls_PHV[0], tmpPacketOut[4]);

	outer_mpls_PHV.Data = (mpls_outer_t.PHVMask == 0) ? ap_uint<bytes2Bits(4)>(0) :  tmp_outer_mpls_PHV[0].Data & mpls_outer_t.PHVMask;
	outer_mpls_PHV.ExtractedBitNum = tmp_outer_mpls_PHV[0].ExtractedBitNum;
	outer_mpls_PHV.Valid = tmp_outer_mpls_PHV[0].Valid;
	outer_mpls_PHV.ValidPulse = tmp_outer_mpls_PHV[0].ValidPulse;
	outer_mpls_PHV.ID = tmp_outer_mpls_PHV[0].ID;
	outer_mpls_PHV.PktID = tmp_outer_mpls_PHV[0].PktID;
	IF_SOFTWARE(outer_mpls_PHV.Name = tmp_outer_mpls_PHV[0].Name;)
	//---------------------------------------
	//outer_vlan
	//---------------------------------------

	tmpPacketIn[2] = tmpPacketOut[1];

	outer_vlan.HeaderAnalysis(tmpPacketIn[2], tmp_outer_vlan_PHV[0], tmpPacketOut[2]);

	outer_vlan_PHV.Data = (vlan_outer_t.PHVMask == 0) ? ap_uint<bytes2Bits(4)>(0) :  tmp_outer_vlan_PHV[0].Data & vlan_outer_t.PHVMask;
	outer_vlan_PHV.ExtractedBitNum = tmp_outer_vlan_PHV[0].ExtractedBitNum;
	outer_vlan_PHV.Valid = tmp_outer_vlan_PHV[0].Valid;
	outer_vlan_PHV.ValidPulse = tmp_outer_vlan_PHV[0].ValidPulse;
	outer_vlan_PHV.ID = tmp_outer_vlan_PHV[0].ID;
	outer_vlan_PHV.PktID = tmp_outer_vlan_PHV[0].PktID;
	IF_SOFTWARE(outer_vlan_PHV.Name = tmp_outer_vlan_PHV[0].Name;)
	//---------------------------------------
	//inner_vlan
	//---------------------------------------

		tmpPacketIn[3] = tmpPacketOut[2];

	inner_vlan.HeaderAnalysis(tmpPacketIn[3], tmp_inner_vlan_PHV[0], tmpPacketOut[3]);

	inner_vlan_PHV.Data = (vlan_inner_t.PHVMask == 0) ? ap_uint<bytes2Bits(4)>(0) :  tmp_inner_vlan_PHV[0].Data & vlan_inner_t.PHVMask;
	inner_vlan_PHV.ExtractedBitNum = tmp_inner_vlan_PHV[0].ExtractedBitNum;
	inner_vlan_PHV.Valid = tmp_inner_vlan_PHV[0].Valid;
	inner_vlan_PHV.ValidPulse = tmp_inner_vlan_PHV[0].ValidPulse;
	inner_vlan_PHV.ID = tmp_inner_vlan_PHV[0].ID;
	inner_vlan_PHV.PktID = tmp_inner_vlan_PHV[0].PktID;
	IF_SOFTWARE(inner_vlan_PHV.Name = tmp_inner_vlan_PHV[0].Name;)
	//---------------------------------------
	//inner_mpls
	//---------------------------------------

	tmpPacketIn[5] = tmpPacketOut[4];

	inner_mpls.HeaderAnalysis(tmpPacketIn[5], tmp_inner_mpls_PHV[0], tmpPacketOut[5]);

	inner_mpls_PHV.Data = (mpls_inner_t.PHVMask == 0) ? ap_uint<bytes2Bits(4)>(0) :  tmp_inner_mpls_PHV[0].Data & mpls_inner_t.PHVMask;
	inner_mpls_PHV.ExtractedBitNum = tmp_inner_mpls_PHV[0].ExtractedBitNum;
	inner_mpls_PHV.Valid = tmp_inner_mpls_PHV[0].Valid;
	inner_mpls_PHV.ValidPulse = tmp_inner_mpls_PHV[0].ValidPulse;
	inner_mpls_PHV.ID = tmp_inner_mpls_PHV[0].ID;
	inner_mpls_PHV.PktID = tmp_inner_mpls_PHV[0].PktID;
	IF_SOFTWARE(inner_mpls_PHV.Name = tmp_inner_mpls_PHV[0].Name;)
	//---------------------------------------
	//ipv4
	//---------------------------------------

		tmpPacketIn[6] = tmpPacketOut[3];

	if (tmp_inner_mpls_PHV[0].Valid)

		tmpPacketIn[6] = tmpPacketOut[5];

	ipv4.HeaderAnalysis(tmpPacketIn[6], tmp_ipv4_PHV[0], tmpPacketOut[6]);

	ipv4_PHV.Data = (ipv4_t.PHVMask == 0) ? ap_uint<bytes2Bits(20)>(0) :  tmp_ipv4_PHV[0].Data & ipv4_t.PHVMask;
	ipv4_PHV.ExtractedBitNum = tmp_ipv4_PHV[0].ExtractedBitNum;
	ipv4_PHV.Valid = tmp_ipv4_PHV[0].Valid;
	ipv4_PHV.ValidPulse = tmp_ipv4_PHV[0].ValidPulse;
	ipv4_PHV.ID = tmp_ipv4_PHV[0].ID;
	ipv4_PHV.PktID = tmp_ipv4_PHV[0].PktID;
	IF_SOFTWARE(ipv4_PHV.Name = tmp_ipv4_PHV[0].Name;)
	//---------------------------------------
	//ipv6
	//---------------------------------------

		tmpPacketIn[7] = tmpPacketOut[3];

	if (tmp_inner_mpls_PHV[0].Valid)

		tmpPacketIn[7] = tmpPacketOut[5];

	ipv6.HeaderAnalysis(tmpPacketIn[7], tmp_ipv6_PHV[0], tmpPacketOut[7]);

	ipv6_PHV.Data = (ipv6_t.PHVMask == 0) ? ap_uint<bytes2Bits(40)>(0) :  tmp_ipv6_PHV[0].Data & ipv6_t.PHVMask;
	ipv6_PHV.ExtractedBitNum = tmp_ipv6_PHV[0].ExtractedBitNum;
	ipv6_PHV.Valid = tmp_ipv6_PHV[0].Valid;
	ipv6_PHV.ValidPulse = tmp_ipv6_PHV[0].ValidPulse;
	ipv6_PHV.ID = tmp_ipv6_PHV[0].ID;
	ipv6_PHV.PktID = tmp_ipv6_PHV[0].PktID;
	IF_SOFTWARE(ipv6_PHV.Name = tmp_ipv6_PHV[0].Name;)
	//---------------------------------------
	//ipv6_ext_1
	//---------------------------------------

		tmpPacketIn[10] = tmpPacketOut[7];

	if (tmp_ipv4_PHV[0].Valid)

		tmpPacketIn[10] = tmpPacketOut[6];

	ipv6_ext_1.HeaderAnalysis(tmpPacketIn[10], tmp_ipv6_ext_1_PHV[0], tmpPacketOut[10]);

	ipv6_ext_1_PHV.Data = (ipv6_ext_1_t.PHVMask == 0) ? ap_uint<bytes2Bits(32)>(0) :  tmp_ipv6_ext_1_PHV[0].Data & ipv6_ext_1_t.PHVMask;
	ipv6_ext_1_PHV.ExtractedBitNum = tmp_ipv6_ext_1_PHV[0].ExtractedBitNum;
	ipv6_ext_1_PHV.Valid = tmp_ipv6_ext_1_PHV[0].Valid;
	ipv6_ext_1_PHV.ValidPulse = tmp_ipv6_ext_1_PHV[0].ValidPulse;
	ipv6_ext_1_PHV.ID = tmp_ipv6_ext_1_PHV[0].ID;
	ipv6_ext_1_PHV.PktID = tmp_ipv6_ext_1_PHV[0].PktID;
	IF_SOFTWARE(ipv6_ext_1_PHV.Name = tmp_ipv6_ext_1_PHV[0].Name;)
	//---------------------------------------
	//ipv6_ext_2
	//---------------------------------------

	tmpPacketIn[11] = tmpPacketOut[10];

	ipv6_ext_2.HeaderAnalysis(tmpPacketIn[11], tmp_ipv6_ext_2_PHV[0], tmpPacketOut[11]);

	ipv6_ext_2_PHV.Data = (ipv6_ext_2_t.PHVMask == 0) ? ap_uint<bytes2Bits(32)>(0) :  tmp_ipv6_ext_2_PHV[0].Data & ipv6_ext_2_t.PHVMask;
	ipv6_ext_2_PHV.ExtractedBitNum = tmp_ipv6_ext_2_PHV[0].ExtractedBitNum;
	ipv6_ext_2_PHV.Valid = tmp_ipv6_ext_2_PHV[0].Valid;
	ipv6_ext_2_PHV.ValidPulse = tmp_ipv6_ext_2_PHV[0].ValidPulse;
	ipv6_ext_2_PHV.ID = tmp_ipv6_ext_2_PHV[0].ID;
	ipv6_ext_2_PHV.PktID = tmp_ipv6_ext_2_PHV[0].PktID;
	IF_SOFTWARE(ipv6_ext_2_PHV.Name = tmp_ipv6_ext_2_PHV[0].Name;)
	//---------------------------------------
	//udp
	//---------------------------------------

		tmpPacketIn[8] = tmpPacketOut[11];

	udp.HeaderAnalysis(tmpPacketIn[8], tmp_udp_PHV[0], tmpPacketOut[8]);

	udp_PHV.Data = (udp_t.PHVMask == 0) ? ap_uint<bytes2Bits(8)>(0) :  tmp_udp_PHV[0].Data & udp_t.PHVMask;
	udp_PHV.ExtractedBitNum = tmp_udp_PHV[0].ExtractedBitNum;
	udp_PHV.Valid = tmp_udp_PHV[0].Valid;
	udp_PHV.ValidPulse = tmp_udp_PHV[0].ValidPulse;
	udp_PHV.ID = tmp_udp_PHV[0].ID;
	udp_PHV.PktID = tmp_udp_PHV[0].PktID;
	IF_SOFTWARE(udp_PHV.Name = tmp_udp_PHV[0].Name;)
	//---------------------------------------
	//icmp
	//---------------------------------------

		tmpPacketIn[12] = tmpPacketOut[11];

	icmp.HeaderAnalysis(tmpPacketIn[12], tmp_icmp_PHV[0], tmpPacketOut[12]);

	icmp_PHV.Data = (icmp_t.PHVMask == 0) ? ap_uint<bytes2Bits(6)>(0) :  tmp_icmp_PHV[0].Data & icmp_t.PHVMask;
	icmp_PHV.ExtractedBitNum = tmp_icmp_PHV[0].ExtractedBitNum;
	icmp_PHV.Valid = tmp_icmp_PHV[0].Valid;
	icmp_PHV.ValidPulse = tmp_icmp_PHV[0].ValidPulse;
	icmp_PHV.ID = tmp_icmp_PHV[0].ID;
	icmp_PHV.PktID = tmp_icmp_PHV[0].PktID;
	IF_SOFTWARE(icmp_PHV.Name = tmp_icmp_PHV[0].Name;)
	//---------------------------------------
	//tcp
	//---------------------------------------

		tmpPacketIn[9] = tmpPacketOut[11];

	tcp.HeaderAnalysis(tmpPacketIn[9], tmp_tcp_PHV[0], tmpPacketOut[9]);

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

		PacketOut = tmpPacketOut[8];

	else if (tmp_tcp_PHV[0].Valid)

		PacketOut = tmpPacketOut[9];

	else

		PacketOut = tmpPacketOut[12];

}
