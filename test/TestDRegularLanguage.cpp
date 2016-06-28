#define BOOST_TEST_MODULE UnitTest NonDeterministicAutomaton
#include <boost/test/included/unit_test.hpp>

#include <memory>
#include "DRegularLanguage.hpp"

using namespace FACore;
using namespace std;

template<unsigned int ALPHABET_SIZE>
DRegularLanguage<ALPHABET_SIZE> EmptyLanguage() {
    typedef DRegularLanguage<ALPHABET_SIZE> Language;
    typedef typename Language::Machine Machine;
    
    shared_ptr<const Machine> automaton( new Machine() );
    // By default, it will be the implementation of the empty language
    
    return Language(automaton, 0);
}

template<unsigned int ALPHABET_SIZE>
DRegularLanguage<ALPHABET_SIZE> FullLanguage() {
    typedef DRegularLanguage<ALPHABET_SIZE> Language;
    typedef typename Language::Machine Machine;
    
    Machine* machine = new Machine();
    
    // By default, it will be the implementation of the empty language
    auto state = machine->AddState(true);
    
    for(unsigned int c = 0; c < Language::AlphabetSize; c++) {
        machine->SetArc(state, c, state);
    }
    
    return Language(machine, state);
}

template<class LanguageType>
bool InLanguage(LanguageType &language, std::vector<typename LanguageType::StateId> word) {
    return language.contains(word.begin(), word.end());
}

template<class LanguageType>
bool NotInLanguage(LanguageType &language, std::vector<typename LanguageType::StateId> word) {
    return !language.contains(word.begin(), word.end());
}

BOOST_AUTO_TEST_CASE( empty_language_alphabet2 )
{
    DRegularLanguage<2> emptyLanguage = EmptyLanguage<2>(); // This is the default
    
    BOOST_CHECK( NotInLanguage(emptyLanguage, {}) );
    BOOST_CHECK( NotInLanguage(emptyLanguage, {0}) );
    BOOST_CHECK( NotInLanguage(emptyLanguage, {1}) );
    BOOST_CHECK( NotInLanguage(emptyLanguage, {1,0,1}) );
}


BOOST_AUTO_TEST_CASE( empty_language_alphabet3 )
{
    DRegularLanguage<3> emptyLanguage = EmptyLanguage<3>(); // This is the default
    
    BOOST_CHECK( NotInLanguage(emptyLanguage, {}) );
    BOOST_CHECK( NotInLanguage(emptyLanguage, {0}) );
    BOOST_CHECK( NotInLanguage(emptyLanguage, {1}) );
    BOOST_CHECK( NotInLanguage(emptyLanguage, {1,0,2}) );
}

BOOST_AUTO_TEST_CASE( full_language )
{
    DRegularLanguage<2> emptyLanguage = FullLanguage<2>(); // This is the default
    
    BOOST_CHECK( InLanguage(emptyLanguage, {}) );
    BOOST_CHECK( InLanguage(emptyLanguage, {0}) );
    BOOST_CHECK( InLanguage(emptyLanguage, {1}) );
    BOOST_CHECK( InLanguage(emptyLanguage, {1,0,1}) );
}

BOOST_AUTO_TEST_CASE( thue_morse )
{
    // The thue morse machine accepts a bit string iff it has an odd number of ones
    DAutomaton<2> *thueMorseMachine = new DAutomaton<2>();
    auto evenState = thueMorseMachine->AddState(false);
    auto oddState = thueMorseMachine->AddState(true);
    
    thueMorseMachine->SetArc(evenState, 0, evenState);
    thueMorseMachine->SetArc(evenState, 1, oddState);
    thueMorseMachine->SetArc(oddState, 0, oddState);
    thueMorseMachine->SetArc(oddState, 1, evenState);
    
    DRegularLanguage<2> thueMorseSequence(thueMorseMachine, evenState);
    
    // Test using the normal as well as integer "contains" function
    BOOST_CHECK( !thueMorseSequence.contains(0) );
    BOOST_CHECK( NotInLanguage(thueMorseSequence, {} ));
    BOOST_CHECK( NotInLanguage(thueMorseSequence, {0} ));
    BOOST_CHECK( NotInLanguage(thueMorseSequence, {0,0} ));
    
    BOOST_CHECK( !thueMorseSequence.contains(3) );
    BOOST_CHECK( NotInLanguage(thueMorseSequence, {1,1} ));
    BOOST_CHECK( NotInLanguage(thueMorseSequence, {0,1,1} ));
    
    BOOST_CHECK( !thueMorseSequence.contains(5) );
    BOOST_CHECK( NotInLanguage(thueMorseSequence, {1,0,1} ));
    
    
    BOOST_CHECK( thueMorseSequence.contains(1) );
    BOOST_CHECK( InLanguage(thueMorseSequence, {1}) );
    BOOST_CHECK( InLanguage(thueMorseSequence, {0,1}) );
    
    BOOST_CHECK( thueMorseSequence.contains(2) );
    BOOST_CHECK( InLanguage(thueMorseSequence, {1,0}) );
    BOOST_CHECK( InLanguage(thueMorseSequence, {0,1,0}) );
    
    BOOST_CHECK( thueMorseSequence.contains(4) );
    BOOST_CHECK( InLanguage(thueMorseSequence, {1,0,0}) );
}