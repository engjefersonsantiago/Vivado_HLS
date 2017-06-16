/******************************************************************************
* Packet Parser: header extraction and supported protocols graph              *
* Jeferson Santiago da Silva (eng.jefersonsantiago@gmail.com)                 *
******************************************************************************/

#include "Parser.hpp"

#ifdef ARRAY_FOR_PHV
void HeaderAnalysisTop(const PacketData<PKT_BUS_SIZE, 32, 16>& PacketIn, std::array<PHVData<MAX_HEADER_SIZE, 32, 16>, HEADER_NUM>& PHV, PacketData<PKT_BUS_SIZE, 32, 16>& PacketOut)
#else
void HeaderAnalysisTop(const PacketData<PKT_BUS_SIZE, 32, 16>& PacketIn, PHVData<ETH_HEADER_SIZE, 32, 16>& Ethernet_PHV, PHVData<IP_HEADER_SIZE, 32, 16>& IP_PHV, PHVData<UDP_HEADER_SIZE, 32, 16>& UDP_PHV, PHVData<TCP_HEADER_SIZE, 32, 16>& TCP_PHV, PacketData<PKT_BUS_SIZE, 32, 16>& PacketOut)
#endif
{
#pragma HLS INTERFACE ap_ctrl_none port=return
#pragma HLS PIPELINE II=1

#ifdef ARRAY_FOR_PHV
#pragma HLS ARRAY_PARTITION variable=PHV dim=1
#else
#pragma HLS INTERFACE ap_ovld port=Ethernet_PHV
#pragma HLS INTERFACE ap_ovld port=IP_PHV
#pragma HLS INTERFACE ap_ovld port=UDP_PHV
#pragma HLS INTERFACE ap_ovld port=TCP_PHV
#endif

	// Wires
	std::array<PacketData<PKT_BUS_SIZE, 32, 16>, HEADER_NUM> tmpPacketOut;
#pragma HLS ARRAY_PARTITION variable=tmpPacketOut dim=1
#ifdef ARRAY_FOR_PHV
	static PHVData<ETH_HEADER_SIZE, 32, 16> Ethernet_PHV;
	static PHVData<IP_HEADER_SIZE, 32, 16> IP_PHV;
	static PHVData<UDP_HEADER_SIZE, 32, 16> UDP_PHV;
	static PHVData<TCP_HEADER_SIZE, 32, 16> TCP_PHV;
#pragma HLS DEPENDENCE variable=Ethernet_PHV false
#pragma HLS DEPENDENCE variable=IP_PHV false
#pragma HLS DEPENDENCE variable=TCP_PHV false
#pragma HLS DEPENDENCE variable=UDP_PHV false
#pragma HLS DEPENDENCE variable=PHV false
#endif

	// Stateful objects
	static Header<ETH_HEADER_SIZE, 3, uint16_t, 7, ap_uint<PKT_BUS_SIZE>, PKT_BUS_SIZE, MAX_PKT_SIZE, 32, 16>
		Ethernet(IF_SOFTWARE("Ethernet",) 0, EthernetLayout);
	static Header<IP_HEADER_SIZE, 13, uint8_t, 2, ap_uint<PKT_BUS_SIZE>, PKT_BUS_SIZE, MAX_PKT_SIZE, 32, 16>
		IPv4(IF_SOFTWARE("IPv4",) 11, IPv4Layout);
	static Header<UDP_HEADER_SIZE, 4, uint8_t, 1, ap_uint<PKT_BUS_SIZE>, PKT_BUS_SIZE, MAX_PKT_SIZE, 32, 16>
		UDP(IF_SOFTWARE("UDP",) 22, UDPLayout);
	static Header<TCP_HEADER_SIZE, 10, uint8_t, 1, ap_uint<PKT_BUS_SIZE>, PKT_BUS_SIZE, MAX_PKT_SIZE, 32, 16>
		TCP(IF_SOFTWARE("TCP",) 21, TCPLayout);

#ifdef ARRAY_FOR_PHV

	Ethernet.HeaderAnalysis(PacketIn, Ethernet_PHV, tmpPacketOut[0]);
	PHV[0] = Ethernet_PHV;

	IPv4.HeaderAnalysis(tmpPacketOut[0], IP_PHV, tmpPacketOut[1]);
	PHV[1] = IP_PHV;

	UDP.HeaderAnalysis(tmpPacketOut[1], UDP_PHV, tmpPacketOut[2]);
	PHV[2] = UDP_PHV;

	TCP.HeaderAnalysis(tmpPacketOut[1], TCP_PHV, tmpPacketOut[3]);
	PHV[3] = TCP_PHV;

#else
	Ethernet.HeaderAnalysis(PacketIn, Ethernet_PHV, tmpPacketOut[0]);
	IPv4.HeaderAnalysis(tmpPacketOut[0], IP_PHV, tmpPacketOut[1]);
	UDP.HeaderAnalysis(tmpPacketOut[1], UDP_PHV, tmpPacketOut[2]);
	TCP.HeaderAnalysis(tmpPacketOut[1], TCP_PHV, tmpPacketOut[3]);
#endif

	PacketOut = tmpPacketOut[3];

}
