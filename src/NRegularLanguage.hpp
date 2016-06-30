#pragma once

#include "utils/DigitIterator.hpp"
#include "NAutomaton.hpp"

#include <memory>
#include <set>

namespace FACore {
    template<unsigned int ALPHABET_SIZE>
    class NRegularLanguage {
        
        public:
            typedef NAutomaton<ALPHABET_SIZE> Machine;
            
            typedef typename Machine::Label Character;
            typedef typename Machine::StateId StateId;
            
            constexpr static unsigned int AlphabetSize = ALPHABET_SIZE;
        
        private:
            typedef DigitIterator<ALPHABET_SIZE, Character> Digitizer;
        
        public:
            NRegularLanguage(std::shared_ptr<const Machine> automaton, std::set<StateId> initialStates) : mAutomaton(automaton), mInitialStates(initialStates)
            {}
            
            NRegularLanguage(Machine* automaton, StateId initialState) : NRegularLanguage(std::shared_ptr<const Machine>(automaton), {initialState})
            {}
            
            // These objects are lightweight and dynamic allocating should be avoided.
            NRegularLanguage(NRegularLanguage&& other) = default;
            
            template<typename IterType>
            bool contains(IterType begin, IterType end) {
                std::set<StateId> currentStates = mInitialStates;
                std::set<StateId> nextStates;
                
                for(auto iter = begin; iter != end && currentStates.size() > 0; ++iter) {
                    Character c = *iter;
                    for(StateId src : currentStates) {
                        for(auto &arc : mAutomaton->GetNext(src, c)) {
                            nextStates.insert(ArcDestination(arc));
                        }
                    }
                    currentStates.swap(nextStates);
                    nextStates.clear();
                }
                
                for(StateId state : currentStates) {
                    if(mAutomaton->IsFinal(state)) {
                        return true;
                    }
                }
                return false;
            }
            
            virtual bool contains(unsigned int number) {
                return contains(Digitizer(number), Digitizer::end());
            }
        
        private:
            std::shared_ptr<const Machine> mAutomaton;
            std::set<StateId> mInitialStates;
    };
}