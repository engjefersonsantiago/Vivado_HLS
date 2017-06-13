/******************************************************************************
* Packet Parser: header extraction and support protocols graph generation     *
* Jeferson Santiago da Silva (eng.jefersonsantiago@gmail.com)                 *
******************************************************************************/

#include "Parser.hpp"

#ifdef ARRAY_FOR_PHV
	#ifdef ARRAY_OF_POINTERS_TO_PHV
void HeaderAnalysisTop(PacketData<PKT_BUS_SIZE> PacketIn, std::array<PHVData<MAX_HEADER_SIZE>*, HEADER_NUM> PHV, PacketData<PKT_BUS_SIZE>* PacketOut)
	#else
void HeaderAnalysisTop(PacketData<PKT_BUS_SIZE> PacketIn, std::array<PHVData<MAX_HEADER_SIZE>, HEADER_NUM>* PHV, PacketData<PKT_BUS_SIZE>* PacketOut)
	#endif
#else
void HeaderAnalysisTop(PacketData<PKT_BUS_SIZE> PacketIn, PHVData<ETH_HEADER_SIZE>* Ethernet_PHV, PHVData<IP_HEADER_SIZE>* IP_PHV, PHVData<UDP_HEADER_SIZE>* UDP_PHV, PacketData<PKT_BUS_SIZE>* PacketOut)
#endif
{
#pragma HLS INTERFACE ap_ctrl_none port=return
#pragma HLS PIPELINE II=1

#ifdef ARRAY_FOR_PHV
#pragma HLS INTERFACE ap_ovld port=PHV
#pragma HLS ARRAY_PARTITION variable=PHV dim=1
#endif

	// Wires
	std::array<PHVData<MAX_HEADER_SIZE>, HEADER_NUM> tmpPHV;
#pragma HLS ARRAY_PARTITION variable=tmpPHV dim=1
#pragma HLS DEPENDENCE variable=tmpPHV false
	std::array<PacketData<PKT_BUS_SIZE>, HEADER_NUM> tmpPacketOut;
#pragma HLS ARRAY_PARTITION variable=tmpPacketOut dim=1
#ifdef ARRAY_FOR_PHV
	static PHVData<ETH_HEADER_SIZE> Ethernet_PHV;
	static PHVData<IP_HEADER_SIZE> IP_PHV;
	static PHVData<UDP_HEADER_SIZE> UDP_PHV;
#pragma HLS DEPENDENCE variable=Ethernet_PHV false
#pragma HLS DEPENDENCE variable=IP_PHV false
#pragma HLS DEPENDENCE variable=UDP_PHV false
#pragma HLS DEPENDENCE variable=PHV false
#endif

	// Stateful objects
	static Header<ETH_HEADER_SIZE, 3, uint_16, 7, ap_uint<PKT_BUS_SIZE>, PKT_BUS_SIZE, MAX_PKT_SIZE>
		Ethernet(IF_SOFTWARE("Ethernet",) 0, EthernetLayout);
	static Header<IP_HEADER_SIZE, 13, uint_8, 2, ap_uint<PKT_BUS_SIZE>, PKT_BUS_SIZE, MAX_PKT_SIZE>
		IPv4(IF_SOFTWARE("IPv4",) 11, IPv4Layout);
	static Header<UDP_HEADER_SIZE, 4, uint_8, 1, ap_uint<PKT_BUS_SIZE>, PKT_BUS_SIZE, MAX_PKT_SIZE>
		UDP(IF_SOFTWARE("UDP",) 22, UDPLayout);

#ifdef ARRAY_FOR_PHV
	Ethernet.HeaderAnalysis(PacketIn, &Ethernet_PHV /* &PHVData<ETH_HEADER_SIZE>((*PHV)[0]) */, &tmpPacketOut[0]); // Ça c'est que je veux
	IPv4.HeaderAnalysis(tmpPacketOut[0], &IP_PHV, &tmpPacketOut[1]);
	UDP.HeaderAnalysis(tmpPacketOut[1], &UDP_PHV, &tmpPacketOut[2]);

	#ifdef ARRAY_OF_POINTERS_TO_PHV
	PHV[0]->Data= ap_uint<MAX_HEADER_SIZE_BITS>(Ethernet_PHV.Data);
	PHV[0]->Valid = Ethernet_PHV.Valid;
	PHV[0]->ID = Ethernet_PHV.ID;
	PHV[0]->PktID = Ethernet_PHV.PktID;
	IF_SOFTWARE(PHV[0]->Name = Ethernet_PHV.Name);		// Problem with Name
	PHV[1]->Data= ap_uint<MAX_HEADER_SIZE_BITS>(IP_PHV.Data);
	PHV[1]->Valid = IP_PHV.Valid;
	PHV[1]->ID = IP_PHV.ID;
	PHV[1]->PktID = IP_PHV.PktID;
	IF_SOFTWARE(PHV[1]->Name = IP_PHV.Name);
	PHV[2]->Data= ap_uint<MAX_HEADER_SIZE_BITS>(UDP_PHV.Data);
	PHV[2]->Valid = UDP_PHV.Valid;
	PHV[2]->ID = UDP_PHV.ID;
	PHV[2]->PktID = UDP_PHV.PktID;
	IF_SOFTWARE(PHV[2]->Name = UDP_PHV.Name);

	#else
	(*PHV)[0].Data= ap_uint<MAX_HEADER_SIZE_BITS>(Ethernet_PHV.Data);
	(*PHV)[0].Valid = Ethernet_PHV.Valid;
	(*PHV)[0].ID = Ethernet_PHV.ID;
	(*PHV)[0].PktID = Ethernet_PHV.PktID;
	IF_SOFTWARE((*PHV)[0].Name = Ethernet_PHV.Name);	// Problem with Name
	(*PHV)[1].Data= ap_uint<MAX_HEADER_SIZE_BITS>(IP_PHV.Data);
	(*PHV)[1].Valid = IP_PHV.Valid;
	(*PHV)[1].ID = IP_PHV.ID;
	(*PHV)[1].PktID = IP_PHV.PktID;
	IF_SOFTWARE((*PHV)[1].Name = IP_PHV.Name);
	(*PHV)[2].Data= ap_uint<MAX_HEADER_SIZE_BITS>(UDP_PHV.Data);
	(*PHV)[2].Valid = UDP_PHV.Valid;
	(*PHV)[2].ID = UDP_PHV.ID;
	(*PHV)[2].PktID = UDP_PHV.PktID;
	IF_SOFTWARE((*PHV)[2].Name = UDP_PHV.Name);

	#endif
#else
	Ethernet.HeaderAnalysis(PacketIn, Ethernet_PHV, &tmpPacketOut[0]);
	IPv4.HeaderAnalysis(tmpPacketOut[0], IP_PHV, &tmpPacketOut[1]);
	UDP.HeaderAnalysis(tmpPacketOut[1], UDP_PHV, &tmpPacketOut[2]);
#endif

	*PacketOut = tmpPacketOut[2];

}
