/******************************************************************************
* Generic types and constants                                                 *
* Jeferson Santiago da Silva (eng.jefersonsantiago@gmail.com)                 *
******************************************************************************/
#include <features.h>
#include <cstdint>
#include "ap_int.h"

#ifndef _DEF_TYPES_H_
#define _DEF_TYPES_H_

// Macro to remove non-synthesizable constructs
// Code from Francois-Raymond Boyer (francois-r.boyer@polymtl.ca)
#ifdef __SYNTHESIS__
	#define IF_SOFTWARE(...)
#else
	#define IF_SOFTWARE(...) __VA_ARGS__
#endif

//template<uint16_t Size>
//using n_uint = ap_uint<Size>;

//FIFO parameters
constexpr uint16_t MAX_PKT_SIZE = 1518;
constexpr uint16_t MIN_PKT_SIZE = 64;
constexpr uint16_t FIFO_ELEMENT_SIZE = 64;
constexpr uint16_t FIFO_SIZE = 4;     // Number of maximum sized packets

// FIFO basic element
template<typename T>
struct FifoElement
{
    T Element;
    bool PacketIni;
    bool PacketEnd;
    FifoElement() : PacketIni{false}, PacketEnd{false}, Element{} {}
};

// Workaround non constexpr log and ceil
// Reference: https://hbfs.wordpress.com/2016/03/22/log2-with-c-metaprogramming/
template<typename T>
constexpr T numbits(T n) {
	return ((n<2) ? 1 : 1+numbits(n>>1));
}

template<typename T_a, typename T_b>
constexpr T_a mod(T_a a, T_b b) {
	return (a<b) ? a : mod(a-b, b);
}

template<typename T_a, typename T_b>
constexpr T_a DivAndCeil(T_a a, T_b b) {
	return (a%b == 0 ? a/b : (a/b + 1));
}

template<typename T>
constexpr T bytes2Bits (T a) {
	return a<<3;
}

template<typename T>
constexpr T bits2Bytes (T a) {
	return a>>3;
}

#endif //_DEF_TYPES_H_
