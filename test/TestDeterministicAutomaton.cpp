#define BOOST_TEST_MODULE UnitTest DeterministicAutomaton
#include <boost/test/included/unit_test.hpp>

#include "DAutomaton.hpp"

using namespace FACore;

BOOST_AUTO_TEST_CASE( simple_pass )
{
    BOOST_CHECK( true );
}

BOOST_AUTO_TEST_CASE( add_two_states )
{
    DFA dfa;
    DFA::StateId state1 = dfa.AddState(true);
    BOOST_CHECK( dfa.IsFinal(state1));
    
    DFA::StateId state2 = dfa.AddState(false);
    BOOST_CHECK( !dfa.IsFinal(state2));
}


BOOST_AUTO_TEST_CASE( set_transition )
{
    DFA dfa;
    DFA::StateId state1 = dfa.AddState(true);
    dfa.SetArc(state1, 0, state1);
    
    BOOST_CHECK( dfa.GetNext(state1,0) == state1 );
    BOOST_CHECK( dfa.GetNext(state1,1) == DFA::INVALID_STATE() );
}


BOOST_AUTO_TEST_CASE( default_state_isfinal )
{
    DFA dfa;
    BOOST_CHECK(!dfa.IsFinal(0));
    BOOST_CHECK(!dfa.IsFinal(1));
}

BOOST_AUTO_TEST_CASE( default_arc_toinvalid )
{
    DFA dfa;
    // Invalid state, valid label
    BOOST_CHECK(dfa.GetNext(0,0) == DFA::INVALID_STATE());
    
    // Valid state, invalid label
    DFA::StateId state = dfa.AddState(true);
    BOOST_CHECK(dfa.GetNext(0,3) == DFA::INVALID_STATE());
}


BOOST_AUTO_TEST_CASE( invalid_state_behavoir )
{
    DFA dfa;
    DFA::StateId invalidState = DFA::INVALID_STATE();
    BOOST_CHECK(!dfa.IsFinal(invalidState));
    BOOST_CHECK(dfa.GetNext(invalidState,0) == invalidState);
}

BOOST_AUTO_TEST_CASE( invalid_arc )
{
    DFA dfa;
    DFA::StateId state1 = dfa.AddState(true);
    
    // invalid source state
    BOOST_CHECK_THROW( dfa.SetArc(state1+1,0,state1), std::out_of_range);
    
    // invalid dest state
    BOOST_CHECK_THROW( dfa.SetArc(state1,0,state1+1), std::out_of_range);
    
    // invalid label state
    BOOST_CHECK_THROW( dfa.SetArc(state1,2,state1), std::out_of_range);
}