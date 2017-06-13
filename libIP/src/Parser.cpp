/******************************************************************************
* Packet Parser: header extraction and support protocols graph generation     *
* Jeferson Santiago da Silva (eng.jefersonsantiago@gmail.com)                 *
******************************************************************************/

#include "Parser.hpp"

//void HeaderAnalysisTop(PacketData<PKT_BUS_SIZE> PacketIn, std::array<PHVData<MAX_HEADER_SIZE>, HEADER_NUM>* PHV, PacketData<PKT_BUS_SIZE>* PacketOut)
void HeaderAnalysisTop(PacketData<PKT_BUS_SIZE> PacketIn, PHVData<ETH_HEADER_SIZE>* Ethernet_PHV, PHVData<IP_HEADER_SIZE>* IP_PHV, PHVData<UDP_HEADER_SIZE>* UDP_PHV, PacketData<PKT_BUS_SIZE>* PacketOut)
{
#pragma HLS INTERFACE ap_ctrl_none port=return
#pragma HLS PIPELINE II=1

	// Wires
	std::array<PHVData<MAX_HEADER_SIZE>, HEADER_NUM> tmpPHV;
#pragma HLS ARRAY_PARTITION variable=tmpPHV dim=1
	std::array<PacketData<PKT_BUS_SIZE>, HEADER_NUM> tmpPacketOut;
#pragma HLS ARRAY_PARTITION variable=tmpPacketOut dim=1
	//PHVData<ETH_HEADER_SIZE> Ethernet_PHV;
	//PHVData<IP_HEADER_SIZE> IP_PHV;
	//PHVData<UDP_HEADER_SIZE> UDP_PHV;

	// Stateful objects
	static Header<ETH_HEADER_SIZE, 3, uint_16, 7, ap_uint<PKT_BUS_SIZE>, PKT_BUS_SIZE, MAX_PKT_SIZE>
		Ethernet(IF_SOFTWARE("Ethernet",) 0, EthernetLayout);
	static Header<IP_HEADER_SIZE, 13, uint_8, 2, ap_uint<PKT_BUS_SIZE>, PKT_BUS_SIZE, MAX_PKT_SIZE>
		IPv4(IF_SOFTWARE("IPv4",) 11, IPv4Layout);
	static Header<UDP_HEADER_SIZE, 4, uint_8, 1, ap_uint<PKT_BUS_SIZE>, PKT_BUS_SIZE, MAX_PKT_SIZE>
		UDP(IF_SOFTWARE("UDP",) 22, UDPLayout);

	//Ethernet.HeaderAnalysis(PacketIn, &Ethernet_PHV, &tmpPacketOut[0]);
	//IPv4.HeaderAnalysis(tmpPacketOut[0], &IP_PHV, &tmpPacketOut[1]);
	//UDP.HeaderAnalysis(tmpPacketOut[1], &UDP_PHV, &tmpPacketOut[2]);

	Ethernet.HeaderAnalysis(PacketIn, Ethernet_PHV, &tmpPacketOut[0]);
	IPv4.HeaderAnalysis(tmpPacketOut[0], IP_PHV, &tmpPacketOut[1]);
	UDP.HeaderAnalysis(tmpPacketOut[1], UDP_PHV, &tmpPacketOut[2]);

	//tmpPHV[0].Data= ap_uint<MAX_HEADER_SIZE_BITS>(Ethernet_PHV.Data);
	//tmpPHV[1].Data= ap_uint<MAX_HEADER_SIZE_BITS>(IP_PHV.Data);
	//tmpPHV[2].Data= ap_uint<MAX_HEADER_SIZE_BITS>(UDP_PHV.Data);
	//tmpPHV[0].Valid = Ethernet_PHV.Valid;
	//tmpPHV[1].Valid = IP_PHV.Valid;
	//tmpPHV[2].Valid = UDP_PHV.Valid;
	//tmpPHV[0].ID = Ethernet_PHV.ID;
	//tmpPHV[1].ID = IP_PHV.ID;
	//tmpPHV[2].ID = UDP_PHV.ID;

	//*PHV = tmpPHV;
	*PacketOut = tmpPacketOut[2];

}
