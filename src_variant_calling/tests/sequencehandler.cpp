#include "../sequencehandler.hpp"

#include <cassert>
#include <iostream>

namespace test
{

void parseSam()
{
    SequenceHandler fixture("tests/test.sam");
    assert(fixture.valid());

    bool hasRead = fixture.next();
    assert(hasRead);

    assert(fixture.getPosition() == 1);
    assert(fixture.getMapQuality() == 45);
    assert(fixture.getCigar() == "20M");
    assert(fixture.getSequence() == std::string(20, 'A'));
}

void parseSamMultipleLines()
{
    SequenceHandler fixture("tests/test.sam");
    assert(fixture.valid());

    auto hasRead = fixture.next(); // Read 1
    assert(hasRead);

    hasRead = fixture.next(); // Read 2
    assert(hasRead);

    assert(fixture.getPosition() == 10);
    assert(fixture.getMapQuality() == 69);
    assert(fixture.getCigar() == "10S10M");
    assert(fixture.getSequence() == (std::string(10, 'T') + std::string(10, 'A')));

    hasRead = fixture.next(); // Read 3
    assert(hasRead);

    assert(fixture.getPosition() == 123);
    assert(fixture.getMapQuality() == 30);
    assert(fixture.getCigar() == "5M5I1D10M");
    assert(fixture.getSequence() == (std::string(20, 'T')));

    hasRead = fixture.next(); // Read 4 -> returns false to mark no lines read
    assert(!hasRead);
}

} // namespace test

int main()
{
    std::cout << "\n[TEST] SequenceHandler\n";
    test::parseSam();
    test::parseSamMultipleLines();
    std::cout << "[TEST] OK\n";
}
