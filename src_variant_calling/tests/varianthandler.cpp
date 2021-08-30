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
    void write(const VariantEntry & entry) override {
        variants.push_back(std::to_string(entry.pos) + " " + entry.variant);
    }
    void flush(size_t pos) { VariantHandler::flush(pos); }
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
    fixture.forceFlush();

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
    fixture.forceFlush();

    assert(fixture.variants[0] == "143 T,A");
    assert(fixture.variants[1] == "144 T,A");
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
    fixture.forceFlush();

    assert(fixture.variants[0] == "143 T,TGT");
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
    fixture.forceFlush();

    assert(fixture.variants[0] == "143 TTTT,T");
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
    fixture.forceFlush();

    assert(fixture.variants[0] == "123 A,ATCG");
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
    fixture.forceFlush();

    assert(fixture.variants[0] == "123 ATCG,A");
}

void variantsNotFlushedTooEarly()
{
    Fixture fixture;

    auto prefix = "A";
    auto refSeq = std::string(82, 'T');
    auto altSeq = std::string(20, 'T') + "A" + std::string(61, 'T');
    Cigar cigar("82M");

    fixture.call(200, prefix, refSeq, altSeq, cigar.getEntries());
    fixture.call(300, prefix, refSeq, altSeq, cigar.getEntries());

    fixture.flush(220);
    assert(fixture.variants.empty());

    fixture.flush(220 + 82);
    assert(fixture.variants.empty());

    fixture.flush(220 + 99);
    assert(fixture.variants.size() == 1);
    assert(fixture.variants[0] == "220 T,A");

    fixture.flush(420 + 83);
    assert(fixture.variants[0] == "220 T,A");
    assert(fixture.variants[1] == "320 T,A");
}

void variantsNotDuplicated()
{
    Fixture fixture;

    auto prefix = "A";
    auto refSeq = std::string(82, 'T');
    auto altSeq1 = std::string(20, 'T') + "A" + std::string(61, 'T');
    auto altSeq2 = "A" + std::string(81, 'T');
    Cigar cigar("82M");

    fixture.call(200, prefix, refSeq, altSeq1, cigar.getEntries());
    fixture.call(220, prefix, refSeq, altSeq2, cigar.getEntries());

    fixture.flush(200 + 82);
    assert(fixture.variants.empty());

    fixture.flush(220 + 83);
    assert(fixture.variants.size() == 1);
    assert(fixture.variants[0] == "220 T,A");
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
    test::variantsNotFlushedTooEarly();
    test::variantsNotDuplicated();
    std::cout << "[TEST] OK\n";
}

