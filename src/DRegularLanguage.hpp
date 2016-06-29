#include <memory>

#include "DAutomaton.hpp"

namespace FACore {
    
    
    template<unsigned int ALPHABET_SIZE>
    class DRegularLanguage {
        
            
        public:
            typedef DAutomaton<ALPHABET_SIZE> Machine;
            
            typedef typename Machine::Label Character;
            typedef typename Machine::StateId StateId;
            
            constexpr static unsigned int AlphabetSize = ALPHABET_SIZE;
            
        private:
        
            class DigitIterator {
                public:
                    static DigitIterator end() {
                        return DigitIterator(0);
                    }
                
                    DigitIterator(unsigned int value) : mValue(value)
                    {}
                    
                    Character operator* () {
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
                    unsigned int mValue;
            };
            
        public:
                
            DRegularLanguage(std::shared_ptr<const Machine> automaton, StateId initialState) : mAutomaton(automaton), mInitialState(initialState)
            {}
            
            DRegularLanguage(Machine* automaton, StateId initialState) : DRegularLanguage(std::shared_ptr<const Machine>(automaton), initialState)
            {}
            
            // These objects are lightweight and dynamic allocating should be avoided.
            DRegularLanguage(DRegularLanguage&& other) = default;
            
            template<typename IterType>
            bool contains(IterType begin, IterType end) {
                StateId currentState = mInitialState;
                for(auto iter = begin; iter != end && currentState != Machine::INVALID_STATE; ++iter) {
                    Character c = *iter;
                    currentState = mAutomaton->GetNext(currentState, c);
                }
                
                return mAutomaton->IsFinal(currentState);
            }
            
            bool contains(unsigned int number) {
                return contains(DigitIterator(number), DigitIterator::end());
            }
        
        private:
            std::shared_ptr<const Machine> mAutomaton;
            StateId mInitialState;
    };
}