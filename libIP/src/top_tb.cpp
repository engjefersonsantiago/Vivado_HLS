#include "ap_int.h"
#include "tstFIFO.hpp"

int main () {

    std::array<FifoElement<ap_int<64>>, 10> Packet;
    std::array<FifoElement<ap_int<64>>, 10> Packet_out;

    for (int i = 0; i < Packet.size(); ++i) {
        Packet[i].Element = (i + 1) * 10;
        Packet[i].PacketIni = 0;
        Packet[i].PacketEnd = 0;
        if (i == 0) Packet[i].PacketIni = 1;
        if (i == 7) Packet[i].PacketEnd = 1;
    }

	FIFO_access(&Packet[0], true, &Packet_out[0], false);
    for (int i = 1; i < Packet.size(); ++i) {
        FIFO_access(&Packet[i], true, &Packet_out[i - 1], true);
    }

	FIFO_access(&Packet[0], false, &Packet_out[Packet.size() - 1], true);

    for (int i = 0; i < Packet_out.size(); ++i) {
		if (Packet_out[i].Element != Packet[i].Element) {
			std::cout << "Test ERROR!!!" << std::endl;
			return -1;
		}
	}

	std::cout << "Test Passed!!! " << std::endl;
    return 0;

}
