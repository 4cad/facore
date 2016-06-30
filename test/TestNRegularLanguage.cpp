#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#include "NRegularLanguage.hpp"
#include <memory>

using namespace FACore;
using namespace std;

template<unsigned int ALPHABET_SIZE>
NRegularLanguage<ALPHABET_SIZE> EmptyLanguage() {
    typedef NRegularLanguage<ALPHABET_SIZE> Language;
    
    // By default, it will be the implementation of the empty language
    return Language(new typename Language::Machine(), 0);
}

template<unsigned int ALPHABET_SIZE>
NRegularLanguage<ALPHABET_SIZE> FullLanguage() {
    typedef NRegularLanguage<ALPHABET_SIZE> Language;
    typedef typename Language::Machine Machine;
    
    Machine* machine = new Machine();
    
    // By default, it will be the implementation of the empty language
    auto state = machine->AddState(true);
    
    for(unsigned int c = 0; c < Language::AlphabetSize; c++) {
        machine->AddArc(state, c, state);
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

BOOST_AUTO_TEST_SUITE( TestNRegularLanguage );

BOOST_AUTO_TEST_CASE( empty_language_alphabet2 )
{
    NRegularLanguage<2> emptyLanguage = EmptyLanguage<2>(); // This is the default
    
    BOOST_CHECK( NotInLanguage(emptyLanguage, {}) );
    BOOST_CHECK( NotInLanguage(emptyLanguage, {0}) );
    BOOST_CHECK( NotInLanguage(emptyLanguage, {1}) );
    BOOST_CHECK( NotInLanguage(emptyLanguage, {1,0,1}) );
}


BOOST_AUTO_TEST_CASE( empty_language_alphabet3 )
{
    NRegularLanguage<3> emptyLanguage = EmptyLanguage<3>(); // This is the default
    
    BOOST_CHECK( NotInLanguage(emptyLanguage, {}) );
    BOOST_CHECK( NotInLanguage(emptyLanguage, {0}) );
    BOOST_CHECK( NotInLanguage(emptyLanguage, {1}) );
    BOOST_CHECK( NotInLanguage(emptyLanguage, {1,0,2}) );
}

BOOST_AUTO_TEST_CASE( full_language )
{
    NRegularLanguage<2> emptyLanguage = FullLanguage<2>(); // This is the default
    
    BOOST_CHECK( InLanguage(emptyLanguage, {}) );
    BOOST_CHECK( InLanguage(emptyLanguage, {0}) );
    BOOST_CHECK( InLanguage(emptyLanguage, {1}) );
    BOOST_CHECK( InLanguage(emptyLanguage, {1,0,1}) );
}

BOOST_AUTO_TEST_CASE( thue_morse )
{
    // The thue morse machine accepts a bit string iff it has an odd number of ones
    NAutomaton<2> *thueMorseMachine = new NAutomaton<2>();
    auto evenState = thueMorseMachine->AddState(false);
    auto oddState = thueMorseMachine->AddState(true);
    
    thueMorseMachine->AddArc(evenState, 0, evenState);
    thueMorseMachine->AddArc(evenState, 1, oddState);
    thueMorseMachine->AddArc(oddState, 0, oddState);
    thueMorseMachine->AddArc(oddState, 1, evenState);
    
    NRegularLanguage<2> thueMorseSequence(thueMorseMachine, evenState);
    
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

BOOST_AUTO_TEST_SUITE_END();