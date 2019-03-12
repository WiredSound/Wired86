#pragma once

#include <string>
#include <sstream>
#include <bitset>
#include "primitives.hpp"

namespace convert {
    /**
     * Returns the most significant byte of the given value.
     *
     * @param value Value to fetch high byte of.
     * @return Most significant byte of value.
     */
    u8 getMostSigByte(u16 value);

    /**
     * Returns the least significant byte of the given value.
     *
     * @param value Value to fetch the low byte of.
     * @return Least significant byte of value.
     */
    u8 getLeastSigByte(u16 value);

    /**
     * Create a 16-bit word from a low and a high byte.
     *
     * @param low The least significant byte.
     * @param high The most significant byte.
     * @return 16-bit word constructed from the low/high bytes.
     */
    u16 createWordFromBytes(u8 low, u8 high);

    /**
     * Convert a numerical value to a string representation in binary format. This is done using a `std::bitset`.
     *
     * @tparam T Type of numerical value to convert to binary string.
     * @tparam bitCount The number of bits of the given value to display.
     * @param value Value to convert to binary string.
     * @param prefix Prefix string value to prefix (defaults to '0b').
     * @param suffix Suffix string value to append (defaults to an empty string).
     * @return The binary string representation of the given value.
     */
    template <typename T, std::size_t bitCount = 16>
    std::string toBinaryString(T value, std::string prefix = "0b", std::string suffix = "") {
        std::bitset<bitCount> bits(value);
        
        std::stringstream stream;
        stream << prefix << bits << suffix;

        return stream.str();
    }

    /**
     * Convert a numerical value to a string representation in hexadecimal format. This is done using the `std::hex`
     * into a string stream.
     *
     * @tparam T Type of numerical value to convert to hexadecimal string.
     * @param value Value convert to hexadecimal string.
     * @param prefix Prefix string value to prefix (defaults to '0x').
     * @param suffix Suffix string value to append (defaults to an empty string).
     * @return The hexadecimal string representation of the given value.
     */
    template <typename T>
    std::string toHexString(T value, std::string prefix = "0x", std::string suffix = "") {
        std::stringstream stream;
        
        stream << std::hex << std::uppercase << std::noshowbase
               << prefix
               << +value // The '+' prefix ensures 'char' types are interpreted as numerical rather than as characters.
               << suffix;

        return stream.str();
    }

    /**
     * Fetch a specific bit of a numerical value (expressed as a boolean).
     *
     * @tparam T Numerical type to fetch bit from.
     * @param value Value to fetch bit of.
     * @param index Specify which bit to fetch. Indexing begins from 0 with the least significant bit up to the most
     *              significant.
     * @return The fetched bit expressed as a boolean value.
     */
    template <typename T>
    bool getBitFrom(T value, unsigned int index) {
        return (value >> index) & 1;
    }

    /**
     * Fetch multiple bits from a numerical value.
     *
     * @tparam T Numerical type to fetch bits from.
     * @param value Value to fetch bits from.
     * @param index Specify the beginning of the sequence of bits to fetch (indexing beginning from 0).
     * @param count The number of bits to fetch starting from the specified index.
     * @return The fetched sequence of bits.
     */
    template <typename T>
    T getBitsFrom(T value, unsigned int index, unsigned int count) {
        T mask = (1 << count) - 1; // (2 ^ count) - 1
        return (value >> index) & mask;
    }
}