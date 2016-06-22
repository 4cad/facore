#include <vector>
#include <stdexcept>
#include <climits>
#include <array>
#include <limits>
#include <boost/dynamic_bitset.hpp>

namespace FACore {
    
    template<unsigned int ALPHABET_SIZE>
    class DAutomaton {
        public:
            // A StateId is really just an index into a vector
            typedef unsigned int StateId;
            
            // Label values must be between 0 and ALPHABET_SIZE
            typedef unsigned int Label;
            
            // These arrays will be indexed by Label
            typedef std::array<StateId, ALPHABET_SIZE> TransitionArr;
            
            static inline StateId INVALID_STATE() {
                return std::numeric_limits<unsigned int>::max();
            }
            
            StateId AddState(bool isFinal) {
                if(mFinalStates.size() == INVALID_STATE()) {
                    // WARNING this is outside of testing code coverage
                    throw std::out_of_range("Invalid label"); 
                }
                StateId result = mTransitions.size();
                mTransitions.emplace_back();
                mTransitions[result].fill(INVALID_STATE());
                mFinalStates.push_back(isFinal);
                return result;
            }
            
            void SetArc(StateId src, Label label, StateId dest) {
                if(ALPHABET_SIZE <= label) {
                    throw std::out_of_range("Invalid label");
                }
                if(!IsValid(src)) {
                    throw std::out_of_range("Invalid source state.");
                }
                if(!IsValid(dest)) {
                    throw std::out_of_range("Invalid dest state");
                }
                mTransitions[src][label] = dest;
            }
            
            bool IsValid(StateId state) const {
                return state < mTransitions.size();
            }
            
            StateId GetNext(StateId src, Label label) const {
                if(!IsValid(src) || label ) {
                    return INVALID_STATE();
                }
                return mTransitions[src][label];
            }
            
            bool IsFinal(StateId state) const {
                if(!IsValid(state)) {
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