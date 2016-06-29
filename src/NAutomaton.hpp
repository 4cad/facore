#include <stdexcept>
#include <limits>
// TODO I would really like to use flat_map for cache efficiency, but it takes too long to compile for some reason
// #include <boost/container/flat_map.hpp>
#include <map>
#include <boost/dynamic_bitset.hpp>

namespace FACore {
    
    template<unsigned int AlphabetSize>
    class NAutomaton {
        public:
            constexpr static unsigned int ALPHABET_SIZE = AlphabetSize;
            constexpr static unsigned int INVALID_STATE = std::numeric_limits<unsigned int>::max();
            
            // A StateId is really just an index into a vector
            typedef unsigned int StateId;
            
            // Label values must be between 0 and ALPHABET_SIZE
            typedef unsigned int Label;
            
            // <(src,label),dest>
            typedef std::pair<std::tuple<StateId,Label>,StateId> TransitionMapEntry;
            
            typedef std::multimap<std::tuple<StateId,Label>,StateId>::const_iterator TransitionIterator;
            
            class ArcRange {
                public:
                    ArcRange (const std::pair<TransitionIterator,TransitionIterator> &range)
                    {
                        mIterStart = range.first;
                        mIterEnd = range.second;
                    }
                    
                    TransitionIterator begin() const { return mIterStart; }
                    TransitionIterator end() const { return mIterEnd; }
                        
                private:
                    TransitionIterator mIterStart;
                    TransitionIterator mIterEnd;
            };
            
            StateId AddState(bool isFinal) {
                if(mFinalStates.size() == INVALID_STATE) {
                    // WARNING this is outside of testing code coverage
                    throw std::out_of_range("Invalid label"); 
                }
                StateId result = mFinalStates.size();
                mFinalStates.push_back(isFinal);
                return result;
            }
            
            void AddArc(StateId src, Label label, StateId dest) {
                if(!IsValidLabel(label)) {
                    throw std::out_of_range("Invalid label");
                }
                if(!IsValidState(src)) {
                    throw std::out_of_range("Invalid source state.");
                }
                if(!IsValidState(dest)) {
                    throw std::out_of_range("Invalid dest state");
                }
                TransitionMapEntry newTransition {std::make_tuple(src,label),dest};
                mTransitions.insert(newTransition);
            }
            
            bool IsValidState(StateId state) const {
                return state < mFinalStates.size();
            }
            
            bool IsValidLabel(Label label) const {
                return label < ALPHABET_SIZE;
            }
            
            ArcRange GetNext(StateId src, Label label) const {
                std::tuple<StateId,Label> key(src,label);
                return ArcRange( mTransitions.equal_range(key) );
            }
            
            bool IsFinal(StateId state) const {
                if(!IsValidState(state)) {
                    return false;
                }
                return mFinalStates[state];
            }
        
        private:
            std::multimap<std::tuple<StateId,Label>,StateId> mTransitions;
            boost::dynamic_bitset<> mFinalStates;
    };
    
    typedef NAutomaton<2> NFA;
    
    
    template<typename StateId, typename Label>
    StateId ArcSource( const std::pair<const std::tuple<StateId,Label>,StateId> &entry ) {
        return std::get<0>(entry->first);
    }
    
    template<typename StateId, typename Label>
    StateId ArcLabel( const std::pair<const std::tuple<StateId,Label>,StateId> &entry ) {
        return std::get<1>(entry->first);
    }
    
    template<typename StateId, typename Label>
    StateId ArcDestination( const std::pair<const std::tuple<StateId,Label>,StateId> &entry ) {
        return entry.second; // std::get<0>(entry->first);
    }
}