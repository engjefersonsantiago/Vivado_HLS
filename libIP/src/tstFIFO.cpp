#include "tstFIFO.hpp"

bool FIFO_access (FIFOElementType *wrData, bool wrEn,
					FIFOElementType *rdData, bool rdEn) {
#pragma HLS PIPELINE II=1

#ifndef __SYNTHESIS__
	static PktFIFO<FIFO_SIZE, FIFO_ELEMENT_SIZE, MAX_PKT_SIZE, MIN_PKT_SIZE> myFIFO ("my_fifo", 0);
#else
	static PktFIFO<FIFO_SIZE, FIFO_ELEMENT_SIZE, MAX_PKT_SIZE, MIN_PKT_SIZE> myFIFO (0);
#endif

#ifndef __SYNTHESIS__
	std::cout << "Instance "<< myFIFO.getInstName() << std::endl;
	std::cout << "Instance ID "<< myFIFO.getInstId() << std::endl;
	std::cout << "Empty " << myFIFO.getFIFOSts().first << std::endl;
    std::cout << "Full " << myFIFO.getFIFOSts().second << std::endl;
    std::cout << "Packets Buff " << myFIFO.getBuffPkt() << std::endl;
#endif

    myFIFO.access(wrData, wrEn, rdData, rdEn);
    return true;
}
