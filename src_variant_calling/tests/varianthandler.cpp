#include "../varianthandler.hpp"

#include <cassert>
#include <iostream>
#include <vector>

namespace test
{

class Fixture : public VariantHandler
{
public:
    Fixture() : VariantHandler("") {}
    void save(size_t pos, const std::string & ref, const std::string & alt) override {
        variants.push_back(std::to_string(pos) + " " + ref + " " + alt);
    }
    std::vector<std::string> variants;
};

void noVariant()
{
    Fixture fixture;

    auto pos = 123;
    auto prefix = "A";
    auto refSeq = std::string(82, 'T');
    auto altSeq = std::string(82, 'T');
    Cigar cigar("82M");
    fixture.call(pos, prefix, refSeq, altSeq, cigar.getEntries());

    assert(fixture.variants.empty());
}

void matchVariant()
{
    Fixture fixture;

    auto pos = 123;
    auto prefix = "A";
    auto refSeq = std::string(82, 'T');
    auto altSeq = std::string(20, 'T') + "AA" + std::string(60, 'T');
    Cigar cigar("82M");
    fixture.call(pos, prefix, refSeq, altSeq, cigar.getEntries());

    assert(fixture.variants[0] == "143 T A");
    assert(fixture.variants[1] == "144 T A");
}

void insertVariant()
{
    Fixture fixture;

    auto pos = 123;
    auto prefix = "A";
    auto refSeq = std::string(82, 'T');
    auto altSeq = std::string(20, 'T') + "GT" + std::string(60, 'T');
    Cigar cigar("20M2I60M");
    fixture.call(pos, prefix, refSeq, altSeq, cigar.getEntries());

    assert(fixture.variants[0] == "143 T TGT");
}

void deleteVariant()
{
    Fixture fixture;

    auto pos = 123;
    auto prefix = "A";
    auto refSeq = std::string(82, 'T');
    auto altSeq = std::string(82, 'T');
    Cigar cigar("20M3D62M");
    fixture.call(pos, prefix, refSeq, altSeq, cigar.getEntries());

    assert(fixture.variants[0] == "143 TTTT T");
}

void startsWithInsertVariant()
{
    Fixture fixture;

    auto pos = 123;
    auto prefix = "A";
    auto refSeq = std::string(82, 'T');
    auto altSeq = "TCG" + std::string(79, 'T');
    Cigar cigar("3I79M");
    fixture.call(pos, prefix, refSeq, altSeq, cigar.getEntries());

    assert(fixture.variants[0] == "123 A ATCG");
}

void startsWithDeleteVariant()
{
    Fixture fixture;

    auto pos = 123;
    auto prefix = "A";
    auto refSeq = "TCG" + std::string(82, 'T');
    auto altSeq = std::string(82, 'T');
    Cigar cigar("3D79M");
    fixture.call(pos, prefix, refSeq, altSeq, cigar.getEntries());

    assert(fixture.variants[0] == "123 ATCG A");
}

} // namespace test

int main()
{
    std::cout << "\n[TEST] VariantHandler\n";
    test::noVariant();
    test::matchVariant();
    test::insertVariant();
    test::deleteVariant();
    test::startsWithInsertVariant();
    test::startsWithDeleteVariant();
    std::cout << "[TEST] OK\n";
}

