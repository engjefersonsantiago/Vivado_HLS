#include "ap_int.h"
#include "tstFIFO.hpp"

struct PacketStruct {
	ap_uint<PKT_BUS_SIZE>  Element;
	bool PacketIni;
	bool PacketEnd;
};

int main () {

    std::array<PacketStruct, 8> Packet;
    std::array<PacketStruct, 8> Packet_out;
	ap_uint<8*HEADER_SIZE> PHV;
	ap_uint<8*HEADER_SIZE> ExpPHV;// = "0x001C0F090010001C0F5CA2830800";

#if HEADER_SIZE == 14
	ExpPHV = "0x001C0F090010001C0F5CA2830800";
#elif HEADER_SIZE == 8
	ExpPHV = "0x9F74007B00C80000";
#else
	ExpPHV = "0x450000DCD4310000F51117DF592E651FC45F4653";
#endif
	bool PHVDone;
	uint_16 NextHeaderOut;
	bool NextHeaderValidOut;

    for (int i = 0; i < Packet.size(); ++i) {
        Packet[i].Element = (i + 1) * 10;
        Packet[i].PacketIni = false;
        Packet[i].PacketEnd = false;
        if (i == 0) Packet[i].PacketIni = true;
        if (i == Packet.size() - 1) Packet[i].PacketEnd = true;
    }

// MSB (Eth -> IP -> UDP -> NTP)
//001C0F090010001C
//0F5CA28308004500
//00DCD4310000F511
//17DF592E651FC45F
//46539F74007B00C8
//00001700032A0000

#if PKT_BUS_SIZE == 32
	Packet[0].Element = "0x001C0F09";
	Packet[1].Element =	"0x0010001C";
	Packet[2].Element = "0x0F5CA283";
	Packet[3].Element = "0x08004500";
#elif PKT_BUS_SIZE == 64
	#if HEADER_SIZE == 14
	Packet[0].Element = "0x001C0F090010001C";
	Packet[1].Element = "0x0F5CA28308004500";
	Packet[2].Element = "0x00DCD4310000F511";
	Packet[3].Element = "0x17DF592E651FC45F";
	Packet[4].Element = "0x46539F74007B00C8";
	Packet[5].Element = "0x00001700032A0000";
	#else
	Packet[0].Element = "0x450000DCD4310000";
	Packet[1].Element = "0xF51117DF592E651F";
	Packet[2].Element = "0xC45F46539F74007B";
	Packet[3].Element = "0x00C800001700032A";
	#endif
#elif PKT_BUS_SIZE == 128
	#if HEADER_SIZE == 14
	Packet[0].Element = "0x001C0F090010001C0F5CA28308004500";
	#elif HEADER_SIZE == 8
	Packet[0].Element = "0x9F74007B00C800001700032A00000000";
	#else
	Packet[0].Element = "0x450000DCD4310000F51117DF592E651F";
	Packet[1].Element = "0xC45F46539F74007B00C800001700032A";
	#endif
#else
	Packet[0].Element = "0x001C0F090010001C0F5CA2830800450011223344556677889900112233445566";
#endif

	//FIFO_access(&Packet[0], true, &Packet_out[0], false);
    for (int i = 0; i < Packet.size(); ++i) {
        HeaderAnalysisTop(Packet[i].Element, Packet[i].PacketIni, Packet[i].PacketEnd, &PHV, &PHVDone, &Packet_out[i].Element, &NextHeaderOut, &NextHeaderValidOut);
    }

	//FIFO_access(&Packet[0], false, &Packet_out[Packet.size() - 1], true);

    //for (int i = 0; i < Packet_out.size(); ++i) {
	//	std::cout << "Packet In " << std::hex << Packet[i].Element << std::endl;
	//	std::cout << "Packet Out " << std::hex << Packet_out[i].Element << std::endl;
	//}

	std::cout << "PHV " << std::hex << PHV << std::endl;
	std::cout << "PHVDone " << PHVDone << std::endl;
	std::cout << "NextHeaderOut " << std::dec << NextHeaderOut << std::endl;
	std::cout << "NextHeaderValidOut " << NextHeaderValidOut << std::endl;

	if (ExpPHV != PHV) {
		std::cout << "Test ERROR!!!" << std::endl;
		//return -1;
	} else {
		std::cout << "Test Passed!!! " << std::endl;
	}

    return 0;

}
