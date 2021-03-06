#pragma once

#include <optional>
#include "emu/cpu/instr/instruction.hpp"
#include "emu/cpu/instr/modregrm.hpp"

namespace emu::cpu::instr {
    /**
     * A 'complex' instruction is one which has a MOD-REG-R/M byte and may also have a displacement and/or immediate
     * value encoded as part of the instruction.
     */
    class ComplexInstruction : public Instruction {
    public:
        ComplexInstruction(std::string instrIdentifier, Opcode instrOpcode, ModRegRm instrModRegRm,
                           std::optional<Displacement> displacement = {}, std::optional<Immediate> immediate = {});

        OffsetAddr execute(Intel8086& cpu, Mem& memory) override final;

        std::string toAssembly(const Intel8086& cpu, const assembly::Style& style) const override final;

        std::vector<u8> getRawData() const override final;

    protected:
        /**
         * Pure virtual method that converts instruction arguments into assembly when the MOD-REG-R/M component
         * indicates no displacement addressing mode.
         */
        virtual std::string argumentsToAssemblyNoDisplacement(const Intel8086& cpu,
                                                              const assembly::Style& style) const = 0;

        /**
         * Pure virtual method that converts instruction arguments into assembly when the MOD-REG-R/M component
         * indicates either byte or word displacement addressing mode.
         */
        virtual std::string argumentsToAssemblyDisplacement(const Intel8086& cpu,
                                                            const assembly::Style& style) const = 0;

        /**
         * Pure virtual method that converts instruction arguments into assembly when the MOD-REG-R/M component
         * indicates register addressing mode.
         */
        virtual std::string argumentsToAssemblyRegisterAddressingMode(const Intel8086& cpu,
                                                                      const assembly::Style& style) const = 0;

        /**
         * Arrange two assembly instruction arguments based on the register direction value given.
         *
         * @param reg The assembly argument derrived from the REG component of the MOD-REG-R/M byte.
         * @param rm The assembly instruction argument derrived from the R/M component.
         * @param direction The direction of the instruction (usually indicated by the opcode).
         */
        std::string argumentsToAssemblyOpcodeDirection(std::string reg, std::string rm, const assembly::Style& style,
                                                       RegDirection direction) const;

        virtual void executeNoDisplacement(Intel8086& cpu, Mem& memory) = 0;
        virtual void executeByteDisplacement(Intel8086& cpu, Mem& memory) = 0;
        virtual void executeWordDisplacement(Intel8086& cpu, Mem& memory) = 0;
        virtual void executeRegisterAddressingMode(Intel8086& cpu, Mem& memory) = 0;

        const ModRegRm modRegRm;
        const std::optional<Displacement> displacementValue;
        const std::optional<Immediate> immediateValue;
    };

    /**
     * An instruction with a MOD-REG-R/M byte taking two arguments (one of argument being either a general-purpose
     * register or a memory address and the other argument being a general-purpose register).
     *
     * Examples:
     * - ADD (0x00, 0x01, 0x02, 0x03)
     * - XOR (0x30, 0x31, 0x32, 0x33)
     */
    class ComplexInstructionEG : public ComplexInstruction {
    public:
        using ComplexInstruction::ComplexInstruction;

    protected:
        std::string argumentsToAssemblyNoDisplacement(const Intel8086& cpu,
                                                      const assembly::Style& style) const override final;
        
        std::string argumentsToAssemblyDisplacement(const Intel8086& cpu,
                                                    const assembly::Style& style) const override final;
        
        std::string argumentsToAssemblyRegisterAddressingMode(const Intel8086& cpu,
                                                              const assembly::Style& style) const override final;

        std::string argumentsToAssemblySpecifiedDisplacement(const Intel8086& cpu, const assembly::Style& style,
                                                             const Displacement& specifiedDisplacement) const;

        void executeNoDisplacement(Intel8086&, Mem&) override final {};
        void executeByteDisplacement(Intel8086&, Mem&) override final {};
        void executeWordDisplacement(Intel8086&, Mem&) override final {};
        void executeRegisterAddressingMode(Intel8086& cpu, Mem&) override final;

        virtual u16 performOperation(u16 dest, u16 src) = 0;
    };

    /**
     * An instruction with a MOD-REG-R/M byte that takes a register argument and an immediate value.
     */
    class ComplexInstructionRegI : public ComplexInstruction {
    public:
        using ComplexInstruction::ComplexInstruction;

    protected:
    };
}