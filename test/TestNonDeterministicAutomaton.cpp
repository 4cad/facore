#define BOOST_TEST_MODULE UnitTest NonDeterministicAutomaton
#include <boost/test/included/unit_test.hpp>

#include "NAutomaton.hpp"

using namespace FACore;

bool IsEmpty(const NFA::ArcRange &range) {
    return range.begin() == range.end();
};

#include <iostream>
using namespace std;

void AssertEquals(const NFA::ArcRange &range, std::set<NFA::StateId> expected) {
    int count = 0;
    for(auto &arc : range) {
        BOOST_CHECK(expected.count(ArcDestination(arc)) == 1);
        count++;
    }
    BOOST_CHECK(count == expected.size());
};

BOOST_AUTO_TEST_CASE( simple_pass )
{
    BOOST_CHECK( true );
}

BOOST_AUTO_TEST_CASE( add_two_states )
{
    NFA nfa;
    NFA::StateId state1 = nfa.AddState(true);
    BOOST_CHECK( nfa.IsFinal(state1));
    
    NFA::StateId state2 = nfa.AddState(false);
    BOOST_CHECK( !nfa.IsFinal(state2));
}

BOOST_AUTO_TEST_CASE( set_transition )
{
    NFA nfa;
    NFA::StateId state1 = nfa.AddState(true);
    nfa.AddArc(state1, 0, state1);
    
    AssertEquals( nfa.GetNext(state1,1), {} );
    AssertEquals( nfa.GetNext(state1,0), {state1} );
}


BOOST_AUTO_TEST_CASE( default_state_isfinal )
{
    NFA nfa;
    BOOST_CHECK(!nfa.IsFinal(0));
    BOOST_CHECK(!nfa.IsFinal(1));
}

BOOST_AUTO_TEST_CASE( default_arc_toinvalid )
{
    NFA nfa;
    // Invalid state, valid label
    AssertEquals(nfa.GetNext(0,0), {} );
    
    // Valid state, invalid label
    NFA::StateId state = nfa.AddState(true);
    AssertEquals(nfa.GetNext(0,3), {});
    AssertEquals(nfa.GetNext(0,3), {});
}


BOOST_AUTO_TEST_CASE( invalid_state_behavoir )
{
    NFA nfa;
    NFA::StateId invalidState = NFA::INVALID_STATE();
    BOOST_CHECK(!nfa.IsFinal(invalidState));
    AssertEquals(nfa.GetNext(invalidState,0), {});
}

BOOST_AUTO_TEST_CASE( invalid_arc )
{
    NFA nfa;
    NFA::StateId state1 = nfa.AddState(true);
    
    // invalid source state
    BOOST_CHECK_THROW( nfa.AddArc(state1+1,0,state1), std::out_of_range);
    
    // invalid dest state
    BOOST_CHECK_THROW( nfa.AddArc(state1,0,state1+1), std::out_of_range);
    
    // invalid label state
    BOOST_CHECK_THROW( nfa.AddArc(state1,2,state1), std::out_of_range);
}

BOOST_AUTO_TEST_CASE( add_parallel_states )
{
    NFA nfa;
    NFA::StateId state1 = nfa.AddState(true);
    NFA::StateId state2 = nfa.AddState(true);
    
    nfa.AddArc(state1,0,state1);
    nfa.AddArc(state1,0,state2);
    nfa.AddArc(state2,1,state2);
    
    auto arcRange = nfa.GetNext(state1,0);

    BOOST_CHECK( std::distance(arcRange.begin(), arcRange.end()) == 2 );
    BOOST_CHECK( std::distance(arcRange.begin(), arcRange.end()) == 2 );
}