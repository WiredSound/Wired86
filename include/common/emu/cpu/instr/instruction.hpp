#pragma once

#include <string>
#include <vector>
#include <optional>
#include "emu/types.hpp"
#include "emu/cpu/instr/opcode.hpp"
#include "emu/cpu/instr/modregrm.hpp"
#include "emu/cpu/instr/immediate.hpp"

namespace emu::cpu::instr {
    class Instruction {
    public:
        /**
         * Create a new instruction representation object.
         *
         * @param instrIdentifier Assembly code identifier for this instruction (e.g. "mov").
         * @param instrOpcode The Opcode object of this instruction.
         */
        Instruction(std::string instrIdentifier, Opcode instrOpcode);

        /**
         * Execute this instruction using the given CPU internal values.
         *
         * Is pure virtual and must therefore be overriden by subclasses.
         *
         * It is the role of this method to return the appropriate instruction pointer value so that the correct next
         * instruction is executed. Unless a jumping/calling instruction, in most instances it is best to return the
         * current instruction pointer plus the value returned by Instruction::getRawSize.
         *
         * @param ip The current value of the instruction pointer.
         * @param memory Reference to the memory managed by the CPU.
         * @param generalRegisters Reference to general-purpose registers of the CPU this instruction is executed on.
         * @param indexRegisters Reference to the CPU's indexing registers.
         * @param segmentRegisters Reference to the CPU's segment registers.
         * @param flags Reference to the CPU's status flags.
         * @return The new instruction pointer value after completing execution.
         */
        virtual OffsetAddr execute(OffsetAddr ip, Mem& memory, GeneralRegs& generalRegisters,
                                   SegmentRegs& segmentRegisters, Flags& flags) = 0;

        /**
         * Disassemble this instruction into Intel-syntax assembly code. Returns this as a string.
         *
         * Is pure virtual and must therefore be overriden by subclasses.
         */
        virtual std::string toAssembly() const = 0;

        /**
         * Fetch the raw 8-bit values that make this instruction include the opcode value.
         *
         * Is pure virtual and must therefore be overriden by subclasses.
         */
        virtual std::vector<u8> getRawData() const = 0;

        /**
         * Fetch the raw 8-bit values that make up this instruction expressed as a string.
         *
         * @param separator The separating string placed between each binary value in the string representation.
         */
        std::string getRawDataString(std::string separator = ", ") const;

        /**
         * Returns the number of bytes that make up this instruction.
         */
        OffsetAddr getRawSize() const;

        const std::string identifier;
        const Opcode opcode;
    };

    /**
     * Used to represent instructions that take a general-purpose register as a first argument and either a register or
     * a memory offset as a second.
     */
    class InstructionWithModRegRm : public Instruction {
    public:
        InstructionWithModRegRm(std::string instrIdentifier, Opcode instrOpcode, ModRegRm modRegRm);

        OffsetAddr execute(OffsetAddr ip, Mem& memory, GeneralRegs& generalRegisters, SegmentRegs&,
                           Flags&) override final;

        std::vector<u8> getRawData() const override final;

        virtual void handleExecution(GeneralRegister destination, GeneralRegister source,
                                     Mem& memory, GeneralRegs& registers) = 0;
        
        virtual void handleExecution(GeneralRegister destination, AbsAddr source,
                                     Mem& memory, GeneralRegs& registers) = 0;
       
        virtual void handleExecution(AbsAddr destination, GeneralRegister source,
                                     Mem& memory, GeneralRegs& registers) = 0;

    protected:
        ModRegRm modRegRmByte;
        std::optional<Displacement> displacement;

    private:
        AbsAddr resolveDisplacementToAddress(u16 displacementValue, GeneralRegs& registers) const;

        /**
         * Calls a InstructionWithModRegRm::handleExecution method with arguments given in ordering based upon the
         * direction specified by the opcode.
         */
        template <typename T>
        void callHandlerWithCorrectOrdering(GeneralRegister reg, T other, Mem& memory, GeneralRegs& registers) {
            switch(opcode.getDirection()) {
            case REG_IS_SOURCE:
                    handleExecution(other, reg, memory, registers); break;
            
            case REG_IS_DESTINATION:
                handleExecution(reg, other, memory, registers); break;
            }
        }
    };
}