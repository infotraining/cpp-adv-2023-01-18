
#include "paragraph.hpp"
#include <iostream>
#include <memory>
#include <catch2/catch_test_macros.hpp>

using namespace std;

TEST_CASE("Moving paragraph")
{
    LegacyCode::Paragraph p("***");
    LegacyCode::Paragraph mp = move(p);

    REQUIRE(mp.get_paragraph() == string("***"));
    REQUIRE(p.get_paragraph() == nullptr);
}

TEST_CASE("Moving text shape")
{
    Text txt{10, 20, "text"};
    Text mtxt = move(txt);

    REQUIRE(mtxt.text() == string("text"));
    REQUIRE(txt.text() == string());
}

TEST_CASE("ShapeGroup")
{      
    ShapeGroup sg;
    //sg.add(std::make_unique<Text>(10, 20, "text")); // uncomment this line

    REQUIRE(sg.shapes.size() == 1);

    Text& t = dynamic_cast<Text&>(*sg.shapes[0]);
    REQUIRE(t.text() == "text"s);
}