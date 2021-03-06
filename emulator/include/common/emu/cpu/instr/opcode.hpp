#pragma once

#include <string>
#include "emu/types.hpp"
#include "primitives.hpp"

namespace emu::cpu::instr {
    /// Indicated by the W-bit of opcode.
    enum DataSize { WORD_DATA_SIZE, BYTE_DATA_SIZE };

    /// Indicated by the D-bit of opcode.
    enum RegDirection { REG_IS_SOURCE, REG_IS_DESTINATION };

    /**
     * Holder for an instruction opcode value. Has methods for fetching the direction and data size of the opcode based
     * on the opcode's D-bit and W-bit respectively.
     */
    class Opcode {
    public:
        Opcode(u8 opcodeValue);

        /**
         * Produces a string useful for debugging purposes (shows the opcode value in binary as well as the state of
         * its direction and word bits).
         */
        std::string toString() const;

        /**
         * Fetch the 6 unique bits of this opcode (removes the word and direction bits).
         */
        u8 getUniqueValue() const;

        /**
         * Indicates whether the data size of this opcode is a word (when the w-bit is 1/true) or a byte (when the w-bit
         * is 0/false). This bit is the least significant bit of the opcode.
         */
        bool getWordBit() const;
        DataSize getDataSize() const;

        /**
         * Will return the length in bytes that any immediate instruction component should be. This is based of the data
         * size of the instruction such that this function will return 2 or a word data size and 1 for a byte data size.
         *
         * Note that this method will not return the appropriate value in order to read a displacement component from
         * memory as whether a displacement component is one or two bytes is determined by the MOD-REG-R/M addressing
         * mode and not the opcode data size.
         */
        AbsAddr getImmediateReadLength() const;

        /**
         * Indicates whether the REG component of a MOD-REG-R/M byte is the source or destination for data handled by
         * the instruction. This bit is the second-to-least significant bit of the opcode.
         */
        bool getDirectionBit() const;
        RegDirection getDirection() const;

        const u8 value;
    };
}