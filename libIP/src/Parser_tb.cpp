/******************************************************************************
* Packet Parser TB: Testbench for the packet parser                           *
* Jeferson Santiago da Silva (eng.jefersonsantiago@gmail.com)                 *
******************************************************************************/

#include "Parser.hpp"

int main () {

	std::array<PacketData<PKT_BUS_SIZE, 32, 16>, 16> Packet;
    std::array<PacketData<PKT_BUS_SIZE, 32, 16>, 16> Packet_out;
	#ifdef ARRAY_OF_POINTERS_TO_PHV
	std::array<PHVData<MAX_HEADER_SIZE, 32, 16>*, HEADER_NUM> PHV;  for (auto & it: PHV) it = new PHVData<MAX_HEADER_SIZE>;
	#else
	std::array<PHVData<MAX_HEADER_SIZE, 32, 16>, HEADER_NUM> PHV;
	#endif

	std::array<PHVData<MAX_HEADER_SIZE, 32, 16>, HEADER_NUM> ExpPHV;
	uint64_t PktID = 69;
	uint16_t HeaderID;
	PHVData<ETH_HEADER_SIZE, 32, 16> Ethernet_PHV;
	PHVData<IP_HEADER_SIZE, 32, 16> IP_PHV;
	PHVData<UDP_HEADER_SIZE, 32, 16> UDP_PHV;
	PHVData<ETH_HEADER_SIZE, 32, 16> ExpEthernet_PHV;
	PHVData<IP_HEADER_SIZE, 32, 16> ExpIP_PHV;
	PHVData<UDP_HEADER_SIZE, 32, 16> ExpUDP_PHV;
	ExpPHV[0].Data = ap_uint<MAX_HEADER_SIZE_BITS>("0x001C0F090010001C0F5CA2830800");
	ExpPHV[1].Data = ap_uint<MAX_HEADER_SIZE_BITS>("0x450000DCD4310000F51117DF592E651FC45F4653");
	ExpPHV[2].Data = ap_uint<MAX_HEADER_SIZE_BITS>("0x9F74007B00C80000");
	ExpEthernet_PHV.Data = "0x001C0F090010001C0F5CA2830800";
	ExpIP_PHV.Data = "0x450000DCD4310000F51117DF592E651FC45F4653";
	ExpUDP_PHV.Data = "0x9F74007B00C80000";
	HeaderID = 0;

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
	Packet[0].Data = "0x001C0F090010001C0F5CA28308004500";
	Packet[1].Data = "0x00DCD4310000F51117DF592E651FC45F";
	Packet[2].Data = "0x46539F74007B00C800001700032A0000";
#elif PKT_BUS_SIZE == 256
	Packet[0].Data = "0x001C0F090010001C0F5CA2830800450000DCD4310000F51117DF592E651FC45F";
	Packet[1].Data = "0x46539F74007B00C800001700032A0000aabbccddeeff00112233445566778899";
#elif PKT_BUS_SIZE == 320
	Packet[0].Data = "0x001C0F090010001C0F5CA2830800450000DCD4310000F51117DF592E651FC45F46539F74007B00C8";
	Packet[1].Data = "0x00001700032A0000aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";
#elif PKT_BUS_SIZE == 512
	Packet[0].Data = "0x001C0F090010001C0F5CA2830800450000DCD4310000F51117DF592E651FC45F46539F74007B00C800001700032A0000aabbccddeeff00112233445566778899";
#elif PKT_BUS_SIZE == 1024
	Packet[0].Data = "0x001C0F090010001C0F5CA2830800450000DCD4310000F51117DF592E651FC45F46539F74007B00C800001700032A0000aabbccddeeff00112233445566778899001C0F090010001C0F5CA2830800450000DCD4310000F51117DF592E651FC45F46539F74007B00C800001700032A0000aabbccddeeff00112233445566778899";
#endif

    for (auto i = 0; i < Packet.size(); ++i) {
		Packet[i].HeaderID = HeaderID;
		Packet[i].ID = PktID;
		PacketData<PKT_BUS_SIZE, 32, 16>& PacketIn = Packet[i];
#ifdef ARRAY_FOR_PHV
	#ifdef ARRAY_OF_POINTERS_TO_PHV
		HeaderAnalysisTop(PacketIn, PHV, &Packet_out[i]);
	#else
		HeaderAnalysisTop(PacketIn, &PHV, &Packet_out[i]);
	#endif
#else
		HeaderAnalysisTop(PacketIn, &Ethernet_PHV, &IP_PHV, &UDP_PHV, &Packet_out[i]);
#endif
	}

	Packet[0].Finish = true;
#ifdef ARRAY_FOR_PHV
	#ifdef ARRAY_OF_POINTERS_TO_PHV
	HeaderAnalysisTop(Packet[0], PHV, &Packet_out[0]);
	#else
	HeaderAnalysisTop(Packet[0], &PHV, &Packet_out[0]);
	#endif
#else
	HeaderAnalysisTop(Packet[0], &Ethernet_PHV, &IP_PHV, &UDP_PHV, &Packet_out[0]);
#endif

#ifdef ARRAY_FOR_PHV
	#ifdef ARRAY_OF_POINTERS_TO_PHV
    for (auto i = 0; i < HEADER_NUM; ++i) {
		std::cout << /*PHV[i].Name <<*/ " PHV: " << std::hex << PHV[i]->Data << std::endl;
		std::cout << /*PHV[i].Name <<*/ " PHV Valid: " << std::dec << PHV[i]->Valid << std::endl;
		std::cout << /*PHV[i].Name <<*/ " PHV ID: " << std::dec << PHV[i]->ID << std::endl;
		std::cout << /*PHV[i].Name <<*/ " PHV Packet ID: " << std::dec << PHV[i]->PktID << std::endl;
		if (ExpPHV[i].Data != PHV[i]->Data || !PHV[i]->Valid) {
			std::cout << "Test ERROR!!!" << std::endl;
			return -1;
		}
	}
	#else
	for (auto i = 0; i < HEADER_NUM; ++i) {
		std::cout << /*PHV[i].Name <<*/ " PHV: " << std::hex << PHV[i].Data << std::endl;
		std::cout << /*PHV[i].Name <<*/ " PHV Valid: " << std::dec << PHV[i].Valid << std::endl;
		std::cout << /*PHV[i].Name <<*/ " PHV ID: " << std::dec << PHV[i].ID << std::endl;
		std::cout << /*PHV[i].Name <<*/ " PHV Packet ID: " << std::dec << PHV[i].PktID << std::endl;
		if (ExpPHV[i].Data != PHV[i].Data || !PHV[i].Valid) {
			std::cout << "Test ERROR!!!" << std::endl;
			//return -1;
		}
	}
	#endif
#else
	std::cout << Ethernet_PHV.Name << " PHV: " << std::hex << Ethernet_PHV.Data << std::endl;
	std::cout << Ethernet_PHV.Name << " PHV Valid: " << std::dec << Ethernet_PHV.Valid << std::endl;
	std::cout << Ethernet_PHV.Name << " PHV ID: " << std::dec << Ethernet_PHV.ID << std::endl;
	std::cout << Ethernet_PHV.Name << " PHV Packet ID: " << std::dec << Ethernet_PHV.PktID << std::endl;
	if (ExpEthernet_PHV.Data != Ethernet_PHV.Data || !Ethernet_PHV.Valid) {
		std::cout << "Ethernet Test ERROR!!!" << std::endl;
		return -1;
	}

	std::cout << IP_PHV.Name << " PHV: " << std::hex << IP_PHV.Data << std::endl;
	std::cout << IP_PHV.Name << " PHV Valid: " << std::dec << IP_PHV.Valid << std::endl;
	std::cout << IP_PHV.Name << " PHV ID: " << std::dec << IP_PHV.ID << std::endl;
	std::cout << IP_PHV.Name << " PHV Packet ID: " << std::dec << IP_PHV.PktID << std::endl;
	if (ExpIP_PHV.Data != IP_PHV.Data || !IP_PHV.Valid) {
		std::cout << "IP Test ERROR!!!" << std::endl;
		return -1;
	}

	std::cout << UDP_PHV.Name << " PHV: " << std::hex << UDP_PHV.Data << std::endl;
	std::cout << UDP_PHV.Name << " PHV Valid: " << std::dec << UDP_PHV.Valid << std::endl;
	std::cout << UDP_PHV.Name << " PHV ID: " << std::dec << UDP_PHV.ID << std::endl;
	std::cout << UDP_PHV.Name << " PHV Packet ID: " << std::dec << UDP_PHV.PktID << std::endl;
	if (ExpUDP_PHV.Data != UDP_PHV.Data || !UDP_PHV.Valid) {
		std::cout << "UDP Test ERROR!!!" << std::endl;
		return -1;
	}
#endif

	std::cout << "Test Passed!!! " << std::endl;
	return 0;
}
