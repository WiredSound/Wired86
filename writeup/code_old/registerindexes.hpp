#pragma once

#include <string>

namespace emu::cpu {
    enum RegisterPart { FULL_WORD, LOW_BYTE, HIGH_BYTE };

    /**
     * Class used to indicate which register is required within a collect of registers. Should be used somewhat like a
     * Java-style enum by extending this call and then having each enum item as a static const instance of that same
     * class.
     */
    class RegisterIndex {
    protected:
        RegisterIndex(std::string indexName, std::string indexDescription = "");

    public:
        /**
         * Converts this register index to its name in x86 assembly. Since Intel syntax is used the short-hand name
         * is simply returned (e.g. 'ax').
         *
         * @return Assembly representation of this register index.
         */
        std::string toAssembly() const;

        /**
         * Builds a string displaying information about this register index.
         *
         * @return Informational string.
         */
        std::string getInfo() const;

    private:
        const std::string name, description;
    };

    /**
     * Used in a fashion similar to RegisterIndex but with the added option to have separate names depending on whether
     * the entire register is accessed or just its high or low bytes.
     */
    class RegisterIndexLowHigh : public RegisterIndex {
    protected:
        RegisterIndexLowHigh(std::string name, std::string description = "");
        RegisterIndexLowHigh(std::string name, std::string low, std::string high, std::string description = "");

    public:
        std::string toAssembly(RegisterPart part = FULL_WORD) const;

    private:
        const std::string lowName, highName;
    };

    /**
      * Register indexes for the general-purpose registers (e.g. AX, BX, etc).
      */
    class GeneralIndex : public RegisterIndexLowHigh {
    public:     static const GeneralIndex AX, BX, CX, DX;
    private:    using RegisterIndexLowHigh::RegisterIndexLowHigh;
    };

    /**
      * Register indexes for the segment registers (e.g. CS, DS, etc).
      */
    class SegmentIndex : public RegisterIndex {
    public:     static const SegmentIndex CODE, DATA, EXTRA, STACK;
    private:    using RegisterIndex::RegisterIndex;
    };
}
