#pragma once

#include <memory>

#include "DigitIterator.hpp"
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
            typedef DigitIterator<ALPHABET_SIZE, Character> Digitizer;
        
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
            
            virtual bool contains(unsigned int number) {
                return contains(Digitizer(number), Digitizer::end());
            }
        
        private:
            std::shared_ptr<const Machine> mAutomaton;
            StateId mInitialState;
    };
}