/******************************************************************************
* Packet Parser TB: Testbench for the packet parser                           *
* Jeferson Santiago da Silva (eng.jefersonsantiago@gmail.com)                 *
******************************************************************************/

#include "Parser.hpp"

int main () {

	std::array<PacketData<PKT_BUS_SIZE, MAX_SUPP_HEADERS, MAX_PKT_ID_SIZE>, 5> Packet;
    std::array<PacketData<PKT_BUS_SIZE, MAX_SUPP_HEADERS, MAX_PKT_ID_SIZE>, 5> Packet_out;
	std::array<PHVData<MAX_HEADER_SIZE, MAX_SUPP_HEADERS, MAX_PKT_ID_SIZE>, HEADER_NUM> PHV;
	std::array<PHVData<MAX_HEADER_SIZE, MAX_SUPP_HEADERS, MAX_PKT_ID_SIZE>, HEADER_NUM> ExpPHV;
	uint64_t PktID = 69;
	uint16_t HeaderID;
	
	std::array<PHVData<ETHERNET_HEADER_SIZE, MAX_SUPP_HEADERS, MAX_PKT_ID_SIZE>, 2> Ethernet_PHV;
	std::array<PHVData<IPV4_HEADER_SIZE, MAX_SUPP_HEADERS, MAX_PKT_ID_SIZE>, 2> IP_PHV;
	std::array<PHVData<IPV6_HEADER_SIZE, MAX_SUPP_HEADERS, MAX_PKT_ID_SIZE>, 2> IPV6_PHV;
	std::array<PHVData<VLAN_HEADER_SIZE, MAX_SUPP_HEADERS, MAX_PKT_ID_SIZE>, 2> VLAN_PHV;
	std::array<PHVData<UDP_HEADER_SIZE, MAX_SUPP_HEADERS, MAX_PKT_ID_SIZE>, 2> UDP_PHV;
	std::array<PHVData<TCP_HEADER_SIZE, MAX_SUPP_HEADERS, MAX_PKT_ID_SIZE>, 2> TCP_PHV;

	PHVData<ETHERNET_HEADER_SIZE, MAX_SUPP_HEADERS, MAX_PKT_ID_SIZE> ExpEthernet_PHV;
	PHVData<VLAN_HEADER_SIZE, MAX_SUPP_HEADERS, MAX_PKT_ID_SIZE> ExpVLAN_PHV;
	PHVData<IPV4_HEADER_SIZE, MAX_SUPP_HEADERS, MAX_PKT_ID_SIZE> ExpIP_PHV;
	PHVData<IPV6_HEADER_SIZE, MAX_SUPP_HEADERS, MAX_PKT_ID_SIZE> ExpIPV6_PHV;
	PHVData<UDP_HEADER_SIZE, MAX_SUPP_HEADERS, MAX_PKT_ID_SIZE> ExpUDP_PHV;
	PHVData<TCP_HEADER_SIZE, MAX_SUPP_HEADERS, MAX_PKT_ID_SIZE> ExpTCP_PHV;
	////ExpPHV[0].Data = ap_uint<MAX_HEADER_SIZE_BITS>("0x001C0F090010001C0F5CA2830800");
	//////ExpPHV[1].Data = ap_uint<MAX_HEADER_SIZE_BITS>("0x450000DCD4310000F51117DF592E651FC45F4653");
	////ExpPHV[1].Data = ap_uint<MAX_HEADER_SIZE_BITS>("0x45000034c0b90000f606ba2384cf0306c0a80169");
	////ExpPHV[2].Data = ap_uint<MAX_HEADER_SIZE_BITS>("0x9F74007B00C80000");
	////ExpPHV[3].Data = ap_uint<MAX_HEADER_SIZE_BITS>("0x01bbe96863826efbd36e81f9801080000c920000");
	ExpEthernet_PHV.Data = "0x001C0F090010001C0F5CA2830800";
	//ExpIP_PHV.Data = "0x450000DCD4310000F51117DF592E651FC45F4653";
	ExpVLAN_PHV.Data = "0x00000800";
	ExpIP_PHV.Data = "0x45000034c0b90000f606ba2384cf0306c0a80169";
	ExpIPV6_PHV.Data = "0x001C0F090010001C0F5CA2830800450000DCD4310000F51117DF592E651FC45F46539F74007B00C8";
	ExpTCP_PHV.Data ="0x01bbe96863826efbd36e81f9801080000c920000";
	ExpUDP_PHV.Data = "0x9F74007B00C80000";
	HeaderID = 1;

	bool PHVDone;
	uint16_t NextHeaderOut;
	bool NextHeaderValidOut;

    for (auto i = 0; i < Packet.size(); ++i) {
        Packet[i].Data = (i + 1) * 10;
        Packet[i].Start = false;
        Packet[i].Finish = false;
        if (i == 0) Packet[i].Start = true;
    }

// MSB (Eth -> IP -> UDP -> NTP)
//001C0F090010001C
//0F5CA28308004500
//00DCD4310000F511
//17DF592E651FC45F
//46539F74007B00C8
//00001700032A0000

// MSB (ETH -> IP -> TCP)
//001C0F090010001C
//0F5CA28308004500
//0034c0b90000f606
//ba2384cf0306c0a8
//016901bbe9686382
//6efbd36e81f98010
//80000c9200000101
//080a5c36e4f102b1
//4961000000000000

#if PKT_BUS_SIZE == 32
	Packet[0].Data = "0x001C0F09";
	Packet[1].Data = "0x0010001C";
	Packet[2].Data = "0x0F5CA283";
	Packet[3].Data = "0x08004500";
	Packet[4].Data = "0x00DCD431";
	Packet[5].Data = "0x0000F511";
	Packet[6].Data = "0x17DF592E";
	Packet[7].Data = "0x651FC45F";
	Packet[8].Data = "0x46539F74";
	Packet[9].Data = "0x007B00C8";
	Packet[10].Data ="0x00001700";
	Packet[11].Data ="0x032A0000";
#elif PKT_BUS_SIZE == 64
	Packet[0].Data = "0x001C0F090010001C";
	Packet[1].Data = "0x0F5CA28308004500";
	Packet[2].Data = "0x00DCD4310000F511";
	Packet[3].Data = "0x17DF592E651FC45F";
	Packet[4].Data = "0x46539F74007B00C8";
	Packet[5].Data = "0x00001700032A0000";
#elif PKT_BUS_SIZE == 128
	//Packet[0].Data = "0x001C0F090010001C0F5CA28308004500";
	//Packet[1].Data = "0x00DCD4310000F51117DF592E651FC45F";
	//Packet[2].Data = "0x46539F74007B00C800001700032A0000";

	Packet[0].Data = "0x001C0F090010001C0F5CA28308004500";
	Packet[1].Data = "0x0034c0b90000f606ba2384cf0306c0a8";
	Packet[2].Data = "0x016901bbe96863826efbd36e81f98010";
	Packet[3].Data = "0x80000c9200000101080a5c36e4f102b1";
	Packet[4].Data = "0x49610000000000000000000000000000";
#elif PKT_BUS_SIZE == 256
	Packet[0].Data = "0x001C0F090010001C0F5CA2830800450000DCD4310000F51117DF592E651FC45F";
	Packet[1].Data = "0x46539F74007B00C800001700032A0000aabbccddeeff00112233445566778899";
#elif PKT_BUS_SIZE == 320
	//Packet[0].Data = "0x001C0F090010001C0F5CA2830800450000DCD4310000F51117DF592E651FC45F46539F74007B00C8";
	//Packet[1].Data = "0x00001700032A0000aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";

	Packet[0].Data = "0x001C0F090010001C0F5CA283080045000034c0b90000f606ba2384cf0306c0a8016901bbe9686382";
	Packet[1].Data = "0x6efbd36e81f9801080000c9200000101080a5c36e4f102b149610000000000000000000000000000";

#elif PKT_BUS_SIZE == 512
	Packet[0].Data = "0x001C0F090010001C0F5CA2830800450000DCD4310000F51117DF592E651FC45F46539F74007B00C800001700032A0000aabbccddeeff00112233445566778899";
#elif PKT_BUS_SIZE == 1024
	Packet[0].Data = "0x001C0F090010001C0F5CA2830800450000DCD4310000F51117DF592E651FC45F46539F74007B00C800001700032A0000aabbccddeeff00112233445566778899001C0F090010001C0F5CA2830800450000DCD4310000F51117DF592E651FC45F46539F74007B00C800001700032A0000aabbccddeeff00112233445566778899";
#endif

	for (auto j = 0; j < 2; ++j) {
	    for (auto i = 0; i < Packet.size(); ++i) {
			Packet[i].HeaderID = HeaderID;
			Packet[i].ID = PktID + j;
	#ifdef ARRAY_FOR_PHV
			HeaderAnalysisTop(Packet[i], PHV, Packet_out[i]);
	#else
			HeaderAnalysisTop(Packet[i], Ethernet_PHV[j], VLAN_PHV[j], IP_PHV[j], IPV6_PHV[j], UDP_PHV[j], TCP_PHV[j], Packet_out[i]);
	#endif
		}
	
		Packet[0].Finish = true;
		Packet[0].Start = false;
	#ifdef ARRAY_FOR_PHV
		HeaderAnalysisTop(Packet[0], PHV, Packet_out[0]);
	#else
		HeaderAnalysisTop(Packet[0], Ethernet_PHV[j], VLAN_PHV[j], IP_PHV[j], IPV6_PHV[j], UDP_PHV[j], TCP_PHV[j], Packet_out[0]);
	#endif
		Packet[0].Finish = false;
		Packet[0].Start = true;
//	}
//
//	// Results check
	std::cout << "----------------------------------------------" << std::endl;
//#ifdef ARRAY_FOR_PHV
//	for (auto i = 0; i < HEADER_NUM; ++i) {
//		if (PHV[i].Valid) {
//			std::cout << PHV[i].Name << " PHV: " << std::hex << PHV[i].Data << std::endl;
//			std::cout << PHV[i].Name << " PHV Valid: " << std::dec << PHV[i].Valid << std::endl;
//			std::cout << PHV[i].Name << " PHV ID: " << std::dec << PHV[i].ID << std::endl;
//			std::cout << PHV[i].Name << " PHV Packet ID: " << std::dec << PHV[i].PktID << std::endl;
//			if (ExpPHV[i].Data != PHV[i].Data || !PHV[i].Valid) {
//				std::cout << "Test ERROR!!!" << std::endl;
//				return -1;
//			}
//		}
//	}
//#else
//	for (auto j = 0; j < 2; ++j) {

		if(IF_SOFTWARE(Ethernet_PHV[j].Valid or) Ethernet_PHV[j].ValidPulse) {
			std::cout << Ethernet_PHV[j].Name << " PHV: " << std::hex << Ethernet_PHV[j].Data << std::endl;
			std::cout << Ethernet_PHV[j].Name << " PHV Valid: " << std::dec << Ethernet_PHV[j].Valid << std::endl;
			std::cout << Ethernet_PHV[j].Name << " PHV ValidPulse: " << std::dec << Ethernet_PHV[j].ValidPulse << std::endl;
			std::cout << Ethernet_PHV[j].Name << " PHV ID: " << std::dec << Ethernet_PHV[j].ID << std::endl;
			std::cout << Ethernet_PHV[j].Name << " PHV Packet ID: " << std::dec << Ethernet_PHV[j].PktID << std::endl;
			if (ExpEthernet_PHV.Data != Ethernet_PHV[j].Data) {
				std::cout << "Ethernet Test ERROR!!!" << std::endl;
				return -1;
			}
		}

		if(IF_SOFTWARE(VLAN_PHV[j].Valid or) VLAN_PHV[j].ValidPulse) {
			std::cout << VLAN_PHV[j].Name << " PHV: " << std::hex << VLAN_PHV[j].Data << std::endl;
			std::cout << VLAN_PHV[j].Name << " PHV Valid: " << std::dec << VLAN_PHV[j].Valid << std::endl;
			std::cout << VLAN_PHV[j].Name << " PHV ValidPulse: " << std::dec << VLAN_PHV[j].ValidPulse << std::endl;
			std::cout << VLAN_PHV[j].Name << " PHV ID: " << std::dec << VLAN_PHV[j].ID << std::endl;
			std::cout << VLAN_PHV[j].Name << " PHV Packet ID: " << std::dec << VLAN_PHV[j].PktID << std::endl;
			if (ExpVLAN_PHV.Data != VLAN_PHV[j].Data) {
				std::cout << "VLAN Test ERROR!!!" << std::endl;
				return -1;
			}
		}

		if(IF_SOFTWARE(IP_PHV[j].Valid or) IP_PHV[j].ValidPulse) {
			std::cout << IP_PHV[j].Name << " PHV: " << std::hex << IP_PHV[j].Data << std::endl;
			std::cout << IP_PHV[j].Name << " PHV Valid: " << std::dec << IP_PHV[j].Valid << std::endl;
			std::cout << IP_PHV[j].Name << " PHV ValidPulse: " << std::dec << IP_PHV[j].ValidPulse << std::endl;
			std::cout << IP_PHV[j].Name << " PHV ID: " << std::dec << IP_PHV[j].ID << std::endl;
			std::cout << IP_PHV[j].Name << " PHV Packet ID: " << std::dec << IP_PHV[j].PktID << std::endl;
			if (ExpIP_PHV.Data != IP_PHV[j].Data) {
				std::cout << "IP Test ERROR!!!" << std::endl;
				return -1;
			}
		}

		if(IF_SOFTWARE(IPV6_PHV[j].Valid or) IP_PHV[j].ValidPulse) {
			std::cout << IPV6_PHV[j].Name << " PHV: " << std::hex << IPV6_PHV[j].Data << std::endl;
			std::cout << IPV6_PHV[j].Name << " PHV Valid: " << std::dec << IPV6_PHV[j].Valid << std::endl;
			std::cout << IPV6_PHV[j].Name << " PHV ValidPulse: " << std::dec << IPV6_PHV[j].ValidPulse << std::endl;
			std::cout << IPV6_PHV[j].Name << " PHV ID: " << std::dec << IPV6_PHV[j].ID << std::endl;
			std::cout << IPV6_PHV[j].Name << " PHV Packet ID: " << std::dec << IPV6_PHV[j].PktID << std::endl;
			if (ExpIPV6_PHV.Data != IPV6_PHV[j].Data) {
				std::cout << "IP Test ERROR!!!" << std::endl;
				return -1;
			}
		}

		if(IF_SOFTWARE(UDP_PHV[j].Valid or) IP_PHV[j].ValidPulse) {
			std::cout << UDP_PHV[j].Name << " PHV: " << std::hex << UDP_PHV[j].Data << std::endl;
			std::cout << UDP_PHV[j].Name << " PHV Valid: " << std::dec << UDP_PHV[j].Valid << std::endl;
			std::cout << UDP_PHV[j].Name << " PHV ValidPulse: " << std::dec << UDP_PHV[j].ValidPulse << std::endl;
			std::cout << UDP_PHV[j].Name << " PHV ID: " << std::dec << UDP_PHV[j].ID << std::endl;
			std::cout << UDP_PHV[j].Name << " PHV Packet ID: " << std::dec << UDP_PHV[j].PktID << std::endl;
			if (ExpUDP_PHV.Data != UDP_PHV[j].Data) {
				std::cout << "UDP Test ERROR!!!" << std::endl;
				return -1;
			}
		}

		if(IF_SOFTWARE(TCP_PHV[j].Valid or) TCP_PHV[j].ValidPulse) {
			std::cout << TCP_PHV[j].Name << " PHV: " << std::hex << TCP_PHV[j].Data << std::endl;
			std::cout << TCP_PHV[j].Name << " PHV Valid: " << std::dec << TCP_PHV[j].Valid << std::endl;
			std::cout << TCP_PHV[j].Name << " PHV ValidPulse: " << std::dec << TCP_PHV[j].ValidPulse << std::endl;
			std::cout << TCP_PHV[j].Name << " PHV ID: " << std::dec << TCP_PHV[j].ID << std::endl;
			std::cout << TCP_PHV[j].Name << " PHV Packet ID: " << std::dec << TCP_PHV[j].PktID << std::endl;
			if (ExpTCP_PHV.Data != TCP_PHV[j].Data) {
				std::cout << "TCP Test ERROR!!!" << std::endl;
				return -1;
			}
		}
		std::cout << "----------------------------------------------" << std::endl;
	}
//#endif

	std::cout << "Test Passed!!! " << std::endl;
	return 0;
}
