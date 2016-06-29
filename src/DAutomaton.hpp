#include <vector>
#include <stdexcept>
#include <climits>
#include <array>
#include <limits>
#include <boost/dynamic_bitset.hpp>

namespace FACore {
    
    template<unsigned int AlphabetSize>
    class DAutomaton {
        public:
            constexpr static unsigned int ALPHABET_SIZE = AlphabetSize;
            constexpr static unsigned int INVALID_STATE = std::numeric_limits<unsigned int>::max();
            
            // A StateId is really just an index into a vector
            typedef unsigned int StateId;
            
            // Label values must be between 0 and ALPHABET_SIZE
            typedef unsigned int Label;
            
            // These arrays will be indexed by Label
            typedef std::array<StateId, ALPHABET_SIZE> TransitionArr;
            
            StateId AddState(bool isFinal) {
                if(mFinalStates.size() == INVALID_STATE) {
                    // WARNING this is outside of testing code coverage
                    throw std::out_of_range("Invalid label"); 
                }
                StateId result = mTransitions.size();
                mTransitions.emplace_back();
                mTransitions[result].fill((unsigned int)INVALID_STATE);
                mFinalStates.push_back(isFinal);
                return result;
            }
            
            void SetArc(StateId src, Label label, StateId dest) {
                if(ALPHABET_SIZE <= label) {
                    throw std::out_of_range("Invalid label");
                }
                if(!IsValidState(src)) {
                    throw std::out_of_range("Invalid source state.");
                }
                if(!IsValidState(dest)) {
                    throw std::out_of_range("Invalid dest state");
                }
                mTransitions[src][label] = dest;
            }
            
            bool IsValidState(StateId state) const {
                return state < mTransitions.size();
            }
            
            bool IsValidLabel(Label label) const {
                return label < ALPHABET_SIZE;
            }
            
            StateId GetNext(StateId src, Label label) const {
                if(IsValidState(src) && IsValidLabel(label) ) {
                    return mTransitions[src][label];
                } else {
                    return INVALID_STATE;
                }
            }
            
            bool IsFinal(StateId state) const {
                if(!IsValidState(state)) {
                    return false;
                }
                return mFinalStates[state];
            }
        
        private:
            std::vector<TransitionArr> mTransitions;
            boost::dynamic_bitset<> mFinalStates;
    };
    
    typedef DAutomaton<2> DFA;
}