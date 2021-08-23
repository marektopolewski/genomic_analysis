#include "../analyser.hpp"

#include <cassert>
#include <iostream>

namespace test
{

class Fixture : public Analyser
{
public:
    Fixture(size_t regionStart = 0, size_t regionEnd = INT_MAX)
        : Analyser("", "", 2, regionStart, regionEnd) {
    }

    bool openVcfFile(const std::string & path) {
        m_vcfFiles.emplace_back(path);
        return m_vcfFiles.back().is_open();
    }
    void write(const VariantEntry & entry, size_t count) override {
        auto str = std::to_string(entry.pos) + " " + entry.variant + " " + std::to_string(count);
        variants.push_back(str);
    }

    std::vector<std::string> variants;
};

void sameFileTwice()
{
    Fixture fixture;

    assert(fixture.openVcfFile("tests/test.1.vcf"));
    assert(fixture.openVcfFile("tests/test.1.vcf"));

    fixture.start();

    assert(fixture.variants.size() == 10);
    for (auto variant : fixture.variants)
        assert(variant[variant.length() - 1] == '2');
}

void samePositionDifferentMutation()
{
    Fixture fixture;

    assert(fixture.openVcfFile("tests/test.1.vcf"));
    assert(fixture.openVcfFile("tests/test.2.vcf"));

    fixture.start();

    assert(fixture.variants.size() == 11);
    assert(fixture.variants[0] == "280 C\tTT 1");
    assert(fixture.variants[1] == "280 G\tT 1");
}

void multipleOccurrences()
{
    Fixture fixture;

    assert(fixture.openVcfFile("tests/test.1.vcf"));
    assert(fixture.openVcfFile("tests/test.2.vcf"));
    assert(fixture.openVcfFile("tests/test.3.vcf"));

    fixture.start();

    assert(fixture.variants.size() == 14);
    assert(fixture.variants[2] == "280 T\tA 1");
    assert(fixture.variants[3] == "280 T\tC 1");
    assert(fixture.variants[4] == "280 T\tG 1");
}

void regionsOfInterestAreRespected()
{
    Fixture fixture(331, 354);

    assert(fixture.openVcfFile("tests/test.1.vcf"));

    fixture.start();

    assert(fixture.variants.size() == 2);
    assert(fixture.variants[0] == "331 T\tA 1");
    assert(fixture.variants[1] == "345 C\tA 1");
}

} // namespace test

int main()
{
    std::cout << "\n[TEST] Analyser\n";
    test::sameFileTwice();
    test::samePositionDifferentMutation();
    test::multipleOccurrences();
    test::regionsOfInterestAreRespected();
    std::cout << "[TEST] OK\n";
}
