#include "tstFIFO.hpp"
#include "Header.hpp"

//bool FIFO_access (FIFOElementType *wrData, bool wrEn,
//					FIFOElementType *rdData, bool rdEn) {
bool HeaderAnalysisTop (ap_uint<PKT_BUS_SIZE> DataIn, bool HeaderStart, bool HeaderFinish, ap_uint<8*14>* PHV, bool* PHVDone)
{
#pragma HLS INTERFACE ap_ctrl_none port=return
#pragma HLS PIPELINE II=1

#ifndef __SYNTHESIS__
	static PktFIFO<FIFO_SIZE, FIFO_ELEMENT_SIZE, MAX_PKT_SIZE, MIN_PKT_SIZE> myFIFO ("my_fifo", 0);
	static Header<14, 3, uint_16, 7, ap_uint<PKT_BUS_SIZE>, PKT_BUS_SIZE, MAX_PKT_SIZE> Ethernet ("Ethernet", 0, EthernetLayout);
#else
	static PktFIFO<FIFO_SIZE, FIFO_ELEMENT_SIZE, MAX_PKT_SIZE, MIN_PKT_SIZE> myFIFO (0);
	static Header<14, 3, uint_16, 7, ap_uint<PKT_BUS_SIZE>, PKT_BUS_SIZE, MAX_PKT_SIZE> Ethernet (77, EthernetLayout);
#endif

	Ethernet.HeaderAnalysis(DataIn, HeaderStart, HeaderFinish, PHV, PHVDone);
   // myFIFO.access(wrData, wrEn, rdData, rdEn);
    return true;
}
