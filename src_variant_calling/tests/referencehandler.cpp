#include "../referencehandler.hpp"

#include <cassert>
#include <iostream>

namespace test
{

void readFirstSequence()
{
    ReferenceHandler fixture("tests/test.fasta");
    assert(fixture.valid());

    auto expSeq = std::string(20, 'A') + std::string(20, 'T') +
        std::string(20, 'C') + std::string(22, 'G');

    assert(fixture.getPrefix() == "N");
    assert(fixture.getSequence() == expSeq);
}

void readOverlappingSequence()
{
    ReferenceHandler fixture("tests/test.fasta");
    assert(fixture.valid());

    fixture.seek(21);

    auto expSeq =  std::string(20, 'T') + std::string(20, 'C')
        + std::string(29, 'G') + "X" + std::string(10, 'G') + std::string(2, 'C');

    assert(fixture.getPrefix() == "A");
    assert(fixture.getSequence() == expSeq);
}

void readTheSameSequence()
{
    ReferenceHandler fixture("tests/test.fasta");
    assert(fixture.valid());

    fixture.seek(3);
    fixture.seek(3);

    auto expSeq = std::string(18, 'A') + std::string(20, 'T') +
        std::string(20, 'C') + std::string(24, 'G');

    assert(fixture.getPrefix() == "A");
    assert(fixture.getSequence() == expSeq);
}

void readNonOverlappingSequence()
{
    ReferenceHandler fixture("tests/test.fasta");
    assert(fixture.valid());

    fixture.seek(91);

    auto expSeq = std::string(10, 'G') + std::string(20, 'C') +
        std::string(20, 'T') + std::string(32, 'A');

    assert(fixture.getPrefix() == "X");
    assert(fixture.getSequence() == expSeq);
}

} // namespace test

int main()
{
    std::cout << "\n[TEST] ReferenceHandler\n";
    test::readFirstSequence();
    test::readOverlappingSequence();
    test::readTheSameSequence();
    test::readNonOverlappingSequence();
    std::cout << "[TEST] OK\n";
}
