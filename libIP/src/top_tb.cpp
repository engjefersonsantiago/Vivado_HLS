#include "ap_int.h"
#include "tstFIFO.hpp"

int main () {

    std::array<PacketData<PKT_BUS_SIZE, 32, 16>, 8> Packet;
    std::array<PacketData<PKT_BUS_SIZE, 32, 16>, 8> Packet_out;
	PHVData<HEADER_SIZE, 32, 16> PHV;
	PHVData<HEADER_SIZE, 32, 16> ExpPHV;
	uint64_t PktID = 0xa5;
	uint16_t HeaderID;

#if HEADER_SIZE == 14
	ExpPHV.Data = "0x001C0F090010001C0F5CA2830800";
	HeaderID = 0;
#elif HEADER_SIZE == 8
	ExpPHV.Data = "0x9F74007B00C80000";
	HeaderID = 22;
#else
	ExpPHV.Data = "0x450000DCD4310000F51117DF592E651FC45F4653";
	HeaderID = 11;
#endif
	bool PHVDone;
	uint16_t NextHeaderOut;
	bool NextHeaderValidOut;

    for (int i = 0; i < Packet.size(); ++i) {
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
#elif PKT_BUS_SIZE == 64
	#if HEADER_SIZE == 14
	Packet[0].Data = "0x001C0F090010001C";
	Packet[1].Data = "0x0F5CA28308004500";
	Packet[2].Data = "0x00DCD4310000F511";
	Packet[3].Data = "0x17DF592E651FC45F";
	Packet[4].Data = "0x46539F74007B00C8";
	Packet[5].Data = "0x00001700032A0000";
	#else
	Packet[0].Data = "0x450000DCD4310000";
	Packet[1].Data = "0xF51117DF592E651F";
	Packet[2].Data = "0xC45F46539F74007B";
	Packet[3].Data = "0x00C800001700032A";
	#endif
#elif PKT_BUS_SIZE == 128
	#if HEADER_SIZE == 14
	Packet[0].Data = "0x001C0F090010001C0F5CA28308004500";
	Packet[1].Data = "0x00DCD4310000F51117DF592E651FC45F";
	Packet[2].Data = "0x46539F74007B00C800001700032A0000";
	#elif HEADER_SIZE == 8
	Packet[0].Data = "0x9F74007B00C800001700032A00000000";
	#else
	Packet[0].Data = "0x450000DCD4310000F51117DF592E651F";
	Packet[1].Data = "0xC45F46539F74007B00C800001700032A";
	#endif
#elif PKT_BUS_SIZE == 256
	Packet[0].Data = "0x001C0F090010001C0F5CA2830800450011223344556677889900112233445566";
#else
	Packet[0].Data = "0x001C0F090010001C0F5CA2830800450011223344556677889900112233445566001C0F090010001C0F5CA2830800450011223344556677889900112233445566";
#endif

	//FIFO_access(&Packet[0], true, &Packet_out[0], false);
    for (int i = 0; i < Packet.size(); ++i) {
		Packet[i].HeaderID = HeaderID;
		auto& Packet_in =Packet[i]; 
		HeaderAnalysisTop(Packet_in, &PHV, &Packet_out[i]);
    }

	Packet[0].Finish = true;
	auto& Packet_in =Packet[0]; 
	HeaderAnalysisTop(Packet_in, &PHV, &Packet_out[0]);

	//FIFO_access(&Packet[0], false, &Packet_out[Packet.size() - 1], true);

    //for (int i = 0; i < Packet_out.size(); ++i) {
	//	std::cout << "Packet In " << std::hex << Packet[i].Element << std::endl;
	//	std::cout << "Packet Out " << std::hex << Packet_out[i].Element << std::endl;
	//}

	std::cout << "PHV: " << std::hex << PHV.Data << std::endl;
	std::cout << "PHV Valid: " << PHV.Valid << std::endl;
	std::cout << "Next Header: " << std::dec << Packet_out[Packet_out.size() - 1].HeaderID << std::endl;
	//std::cout << "NextHeaderValidOut " << NextHeaderValidOut << std::endl;

	if (ExpPHV.Data != PHV.Data) {
		std::cout << "Test ERROR!!!" << std::endl;
		//return -1;
	} else {
		std::cout << "Test Passed!!! " << std::endl;
	}

    return 0;

}
