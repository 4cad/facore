#pragma once

namespace FACore {
    template<
        unsigned int ALPHABET_SIZE,
        typename DigitType = unsigned int,
        typename NumberType = unsigned int
        >
    class DigitIterator {
        public:
            static DigitIterator end() {
                return DigitIterator(0);
            }
        
            DigitIterator(NumberType value) : mValue(value)
            {}
            
            DigitType operator* () {
                return mValue % ALPHABET_SIZE;
            }
            
            DigitIterator& operator++() {
                mValue = mValue / ALPHABET_SIZE;
                return *this;
            }
            
            bool operator==(const DigitIterator &other) {
                return mValue == other.mValue;
            }
            
            bool operator!=(const DigitIterator &other) {
                return !operator==(other);
            }
        
        private:
            NumberType mValue;
    };
}