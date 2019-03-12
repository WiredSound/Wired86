#pragma once

#include <memory>
#include "primitives.hpp"
#include "emu/memory.hpp"
#include "emu/cpu/registers.hpp"
#include "emu/cpu/instr/instruction.hpp"

namespace emu {
    /// Absolute address on the 8086 are 20-bit however no 20-bit unsigned integer type exists in C++ so a 32-bit
    /// unsigned integer is used instead.
    using AbsAddr = u32;

    /// Values stored in memory are 8-bit wide.
    using MemValue = u8;

    /// Memory type for memory storing 8-bit byte values ad taking 32-bit addresses.
    using Mem = Memory<MemValue, AbsAddr>;

    /// Offset addresses within a given segment are 16-bit wide.
    using OffsetAddr = u16;

    /// Type of values stored by standard registers.
    using RegSize = u16;

    /// Unique pointer to an cpu::instr::Instruction object.
    using InstructionPtr = std::unique_ptr<cpu::instr::Instruction>;

    /// cpu::RegistersLowHigh template using cpu::GeneralRegister as indexes.
    using GeneralRegs = cpu::RegistersLowHigh<cpu::GeneralRegister>;

    /// cpu::Registers template using cpu::IndexRegister and RegSize as arguments.
    using IndexRegs = cpu::Registers<cpu::IndexRegister, RegSize>;

    /// cpu::Registers template using cpu::SegmentRegister and RegSize as arguments.
    using SegmentRegs = cpu::Registers<cpu::SegmentRegister, RegSize>;

    /// cpu::Registers template using cpu::Flag and boolean as type arguments.
    using Flags = cpu::Registers<cpu::Flag, bool>;
}