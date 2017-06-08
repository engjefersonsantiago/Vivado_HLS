#include "ap_int.h"
#include "tstFIFO.hpp"

struct PacketStruct {
	ap_uint<PKT_BUS_SIZE>  Element;
	bool PacketIni;
	bool PacketEnd;
};

int main () {

    std::array<PacketStruct, 10> Packet;
    std::array<PacketStruct, 10> Packet_out;

    for (int i = 0; i < Packet.size(); ++i) {
        Packet[i].Element = (i + 1) * 10;
        Packet[i].PacketIni = false;
        Packet[i].PacketEnd = false;
        if (i == 0) Packet[i].PacketIni = true;
        if (i == Packet.size() - 1) Packet[i].PacketEnd = true;
    }
#if PKT_BUS_SIZE == 32 
	Packet[0].Element = "0xAABBCCDD";
	Packet[1].Element =	"0xEEFF1122";
	Packet[2].Element = "0x00000000";
	Packet[3].Element = "0x00000800";
#elif PKT_BUS_SIZE == 64 
	Packet[1].Element = "0x0000080000000000";
	Packet[0].Element = "0xAABBCCDDEEFF1122";
#elif PKT_BUS_SIZE == 128
	Packet[0].Element = "0x0000080000000000AABBCCDDEEFF1122";
#else
	Packet[0].Element = "0xaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa0000080000000000AABBCCDDEEFF1122";
#endif
	ap_uint<8*14> PHV;
	bool PHVDone;

	//FIFO_access(&Packet[0], true, &Packet_out[0], false);
    for (int i = 0; i < Packet.size(); ++i) {
        HeaderAnalysisTop(Packet[i].Element, Packet[i].PacketIni, Packet[i].PacketEnd, &PHV, &PHVDone);
    }

	//FIFO_access(&Packet[0], false, &Packet_out[Packet.size() - 1], true);

    //for (int i = 0; i < Packet_out.size(); ++i) {
	//	if (Packet_out[i].Element != Packet[i].Element) {
	//		std::cout << "Test ERROR!!!" << std::endl;
	//		return -1;
	//	}
	//}

	std::cout << "PHV " <<  PHV<< std::endl;
	std::cout << "PHVDone " <<  PHVDone<< std::endl;
	
	//std::cout << "Test Passed!!! " << std::endl;
    return 0;

}
