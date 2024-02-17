/*
 * Copyright 2024 Želimir Marojević
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt
 * or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * For more information, see http://www.boost.org
 */

#include <boost/filesystem/operations.hpp>
#include <boost/system/detail/error_code.hpp>
#include <catch2/catch_message.hpp>
#include <catch2/catch_session.hpp>
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_all.hpp>

#include <boost/config.hpp>
#include <boost/filesystem/convenience.hpp>

#include <CXodrRoadGraph.hpp>
#include <catch2/matchers/catch_matchers_string.hpp>

static boost::filesystem::path g_base_dir;
static boost::filesystem::path g_build_dir;

TEST_CASE("1: road_to_road_linkage", "[road][lane][sections]")
{
    boost::system::error_code ec;

    //  road 1  road 2
    // |-------|-------|
    // | +1    | +1    |
    // |------>|------>|
    // | -1    | -1    |
    // |-------|-------|
    SECTION("end_to_start, (S=+1, S=+1)")
    {
        boost::filesystem::path filepath = g_base_dir / "end_to_start.xodr";
        boost::filesystem::path filepath_dot = g_build_dir / "end_to_start.dot";

        REQUIRE(boost::filesystem::is_regular_file(filepath, ec));
        REQUIRE_FALSE(ec.failed());
        INFO(ec.to_string());

        std::stringstream os;
        CXodrRoadGraph oTest;
        oTest.init(filepath.string());
        oTest.to_graphviz(filepath_dot.string());
        oTest.to_txt(os);
        auto result = os.str();

        CHECK_THAT(result, Catch::Matchers::ContainsSubstring("0 1  (road, lane, sct) := (1, 1, 0)"));
        CHECK_THAT(result, Catch::Matchers::ContainsSubstring("1 4  (road, lane, sct) := (2, 1, 0)"));
        CHECK_THAT(result, Catch::Matchers::ContainsSubstring("2 3  (road, lane, sct) := (1, -1, 0)"));
        CHECK_THAT(result, Catch::Matchers::ContainsSubstring("3 5  (road, lane, sct) := (2, -1, 0)"));
    }

    //  road 1  road 2
    // |-------|-------|
    // | +1    | -1    |
    // |------>|<------|
    // | -1    | +1    |
    // |-------|-------|
    // (0) --> (1) <-- (5)
    // (2) --> (3) <-- (4)
    SECTION("end_to_end, (S=+1, S=-1)")
    {
        boost::filesystem::path filepath = g_base_dir / "end_to_end.xodr";
        boost::filesystem::path filepath_dot = g_build_dir / "end_to_end.dot";

        REQUIRE(boost::filesystem::is_regular_file(filepath, ec));
        REQUIRE_FALSE(ec.failed());
        INFO(ec.to_string());

        std::stringstream os;
        CXodrRoadGraph oTest;
        oTest.init(filepath.string());
        oTest.to_graphviz(filepath_dot.string());
        oTest.to_txt(os);
        auto result = os.str();

        CHECK_THAT(result, Catch::Matchers::ContainsSubstring("0 1  (road, lane, sct) := (1, 1, 0)"));
        CHECK_THAT(result, Catch::Matchers::ContainsSubstring("5 1  (road, lane, sct) := (2, -1, 0)"));
        CHECK_THAT(result, Catch::Matchers::ContainsSubstring("2 3  (road, lane, sct) := (1, -1, 0)"));
        CHECK_THAT(result, Catch::Matchers::ContainsSubstring("4 3  (road, lane, sct) := (2, 1, 0)"));
    }

    //  road 1  road 2
    // |-------|-------|
    // | -1    | +1    |
    // |<------|------>|
    // | +1    | -1    |
    // |-------|-------|
    // (3) <-- (2) --> (4)
    // (1) <-- (0) --> (5)

    SECTION("start_to_start, (S=-1, S=+1)")
    {
        boost::filesystem::path filepath = g_base_dir / "start_to_start.xodr";
        boost::filesystem::path filepath_dot = g_build_dir / "start_to_start.dot";

        REQUIRE(boost::filesystem::is_regular_file(filepath, ec));
        REQUIRE_FALSE(ec.failed());
        INFO(ec.to_string());

        std::stringstream os;
        CXodrRoadGraph oTest;
        oTest.init(filepath.string());
        oTest.to_graphviz(filepath_dot.string());
        oTest.to_txt(os);
        auto result = os.str();

        CHECK_THAT(result, Catch::Matchers::ContainsSubstring("0 1  (road, lane, sct) := (1, 1, 0)"));
        CHECK_THAT(result, Catch::Matchers::ContainsSubstring("2 3  (road, lane, sct) := (1, -1, 0)"));
        CHECK_THAT(result, Catch::Matchers::ContainsSubstring("2 4  (road, lane, sct) := (2, 1, 0)"));
        CHECK_THAT(result, Catch::Matchers::ContainsSubstring("0 5  (road, lane, sct) := (2, -1, 0)"));
    }
}

TEST_CASE("2: lanesection_to_lanesection_linkage", "[road][lane][sections]")
{
    boost::system::error_code ec;

    //  lanesct 0  lanesct 1  lanesct 2
    // |--------->|--------->|--------->|
    // | -1       | -1       | -1       |
    // |--------->|--------->|--------->|
    // | -2       | -2       | -2       |
    // |--------->|--------->|--------->|
    // | -3       | -3       |
    // |--------->|--------->|
    // (0) --> (1) --> (6) --> (9)
    // (2) --> (3) --> (7) --> (10)
    // (4) --> (5) --> (8)
    SECTION("70_100_200_3_3_2")
    {
        boost::filesystem::path filepath = g_base_dir / "70_100_200_3_3_2.xodr";
        boost::filesystem::path filepath_dot = g_build_dir / "70_100_200_3_3_2.dot";

        REQUIRE(boost::filesystem::is_regular_file(filepath, ec));
        REQUIRE_FALSE(ec.failed());
        INFO(ec.to_string());

        std::stringstream os;
        CXodrRoadGraph oTest;
        oTest.init(filepath.string());
        oTest.to_graphviz(filepath_dot.string());
        oTest.to_txt(os);
        auto result = os.str();

        CHECK_THAT(result, Catch::Matchers::ContainsSubstring("0 1  (road, lane, sct) := (1, -1, 0)"));
        CHECK_THAT(result, Catch::Matchers::ContainsSubstring("1 6  (road, lane, sct) := (1, -1, 1)"));
        CHECK_THAT(result, Catch::Matchers::ContainsSubstring("6 9  (road, lane, sct) := (1, -1, 2)"));
        CHECK_THAT(result, Catch::Matchers::ContainsSubstring("2 3  (road, lane, sct) := (1, -2, 0)"));
        CHECK_THAT(result, Catch::Matchers::ContainsSubstring("3 7  (road, lane, sct) := (1, -2, 1)"));
        CHECK_THAT(result, Catch::Matchers::ContainsSubstring("7 10  (road, lane, sct) := (1, -2, 2)"));
        CHECK_THAT(result, Catch::Matchers::ContainsSubstring("4 5  (road, lane, sct) := (1, -3, 0)"));
        CHECK_THAT(result, Catch::Matchers::ContainsSubstring("5 8  (road, lane, sct) := (1, -3, 1)"));
    }

    //  lanesct 0  lanesct 1  lanesct 2
    // |--------->|--------->|
    // | -1       | -1       |
    // |--------->|--------->|--------->|
    // | -2       | -2       | -1       |
    // |--------->|--------->|--------->|
    // | -3       | -3       | -2       |
    // |--------->|--------->|--------->|
    // (0) --> (1) --> (6)
    // (2) --> (3) --> (7) --> (9)
    // (4) --> (5) --> (8) --> (10)
    SECTION("70_100_200_3_3_2_jump")
    {
        boost::filesystem::path filepath = g_base_dir / "70_100_200_3_3_2_jump.xodr";
        boost::filesystem::path filepath_dot = g_build_dir / "70_100_200_3_3_2_jump.dot";

        REQUIRE(boost::filesystem::is_regular_file(filepath, ec));
        REQUIRE_FALSE(ec.failed());
        INFO(ec.to_string());

        std::stringstream os;
        CXodrRoadGraph oTest;
        oTest.init(filepath.string());
        oTest.to_graphviz(filepath_dot.string());
        oTest.to_txt(os);
        auto result = os.str();

        CHECK_THAT(result, Catch::Matchers::ContainsSubstring("0 1  (road, lane, sct) := (1, -1, 0)"));
        CHECK_THAT(result, Catch::Matchers::ContainsSubstring("1 6  (road, lane, sct) := (1, -1, 1)"));
        CHECK_THAT(result, Catch::Matchers::ContainsSubstring("2 3  (road, lane, sct) := (1, -2, 0)"));
        CHECK_THAT(result, Catch::Matchers::ContainsSubstring("3 7  (road, lane, sct) := (1, -2, 1)"));
        CHECK_THAT(result, Catch::Matchers::ContainsSubstring("7 9  (road, lane, sct) := (1, -1, 2)"));
        CHECK_THAT(result, Catch::Matchers::ContainsSubstring("4 5  (road, lane, sct) := (1, -3, 0)"));
        CHECK_THAT(result, Catch::Matchers::ContainsSubstring("5 8  (road, lane, sct) := (1, -3, 1)"));
        CHECK_THAT(result, Catch::Matchers::ContainsSubstring("8 10  (road, lane, sct) := (1, -2, 2)"));
    }

    //  lanesct 0  lanesct 1  lanesct 2
    // |--------->|--------->|--------->|
    // | -1       | -1       | -1       |
    // |--------->|--------->|--------->|
    // | -2       | -2       | -1       |
    // |--------->|--------->|--------->|
    // | -3       |          | -3       |
    // |--------->|          |--------->|
    // (0) --> (1) --> (6) --> (8)
    // (2) --> (3) --> (7) --> (9)
    // (4) --> (5)     (10) --> (11)
    SECTION("70_100_200_3_2_3")
    {
        boost::filesystem::path filepath = g_base_dir / "70_100_200_3_2_3.xodr";
        boost::filesystem::path filepath_dot = g_build_dir / "70_100_200_3_2_3.dot";

        REQUIRE(boost::filesystem::is_regular_file(filepath, ec));
        REQUIRE_FALSE(ec.failed());
        INFO(ec.to_string());

        std::stringstream os;
        CXodrRoadGraph oTest;
        oTest.init(filepath.string());
        oTest.to_graphviz(filepath_dot.string());
        oTest.to_txt(os);
        auto result = os.str();

        CHECK_THAT(result, Catch::Matchers::ContainsSubstring("0 1  (road, lane, sct) := (1, -1, 0)"));
        CHECK_THAT(result, Catch::Matchers::ContainsSubstring("2 3  (road, lane, sct) := (1, -2, 0)"));
        CHECK_THAT(result, Catch::Matchers::ContainsSubstring("4 5  (road, lane, sct) := (1, -3, 0)"));
        CHECK_THAT(result, Catch::Matchers::ContainsSubstring("1 6  (road, lane, sct) := (1, -1, 1)"));
        CHECK_THAT(result, Catch::Matchers::ContainsSubstring("3 7  (road, lane, sct) := (1, -2, 1)"));
        CHECK_THAT(result, Catch::Matchers::ContainsSubstring("6 8  (road, lane, sct) := (1, -1, 2)"));
        CHECK_THAT(result, Catch::Matchers::ContainsSubstring("7 9  (road, lane, sct) := (1, -2, 2)"));
        CHECK_THAT(result, Catch::Matchers::ContainsSubstring("10 11  (road, lane, sct) := (1, -3, 2)"));
    }

    //  lanesct 0  lanesct 1  lanesct 2
    // |--------->|          |--------->|
    // | -1       |          | -1       |
    // |--------->|--------->|--------->|
    // | -2       | -1       | -2       |
    // |--------->|--------->|--------->|
    // | -3       | -2       | -3       |
    // |--------->|--------->|--------->|
    // (0) --> (1)     (8) --> (9)
    // (2) --> (3) --> (6) --> (10)
    // (4) --> (5) --> (7) --> (11)
    SECTION("70_100_200_3_2_3_jump")
    {
        boost::filesystem::path filepath = g_base_dir / "70_100_200_3_2_3_jump.xodr";
        boost::filesystem::path filepath_dot = g_build_dir / "70_100_200_3_2_3_jump.dot";

        REQUIRE(boost::filesystem::is_regular_file(filepath, ec));
        REQUIRE_FALSE(ec.failed());
        INFO(ec.to_string());

        std::stringstream os;
        CXodrRoadGraph oTest;
        oTest.init(filepath.string());
        oTest.to_graphviz(filepath_dot.string());
        oTest.to_txt(os);
        auto result = os.str();

        CHECK_THAT(result, Catch::Matchers::ContainsSubstring("0 1  (road, lane, sct) := (1, -1, 0)"));
        CHECK_THAT(result, Catch::Matchers::ContainsSubstring("2 3  (road, lane, sct) := (1, -2, 0)"));
        CHECK_THAT(result, Catch::Matchers::ContainsSubstring("4 5  (road, lane, sct) := (1, -3, 0)"));
        CHECK_THAT(result, Catch::Matchers::ContainsSubstring("3 6  (road, lane, sct) := (1, -1, 1)"));
        CHECK_THAT(result, Catch::Matchers::ContainsSubstring("5 7  (road, lane, sct) := (1, -2, 1)"));
        CHECK_THAT(result, Catch::Matchers::ContainsSubstring("8 9  (road, lane, sct) := (1, -1, 2)"));
        CHECK_THAT(result, Catch::Matchers::ContainsSubstring("6 10  (road, lane, sct) := (1, -2, 2)"));
        CHECK_THAT(result, Catch::Matchers::ContainsSubstring("7 11  (road, lane, sct) := (1, -3, 2)"));
    }
}

TEST_CASE("3: road_to_road_linkage_with_lanesections", "[road][lane][lanesection]")
{
    boost::system::error_code ec;

    //  road 1                             road 2 
    //  lanesct 0  lanesct 1  lanesct 2    lanesct 0  lanesct 1  lanesct 2
    // |--------->|--------->|--------->| |--------->|          |--------->|
    // | -1       | -1       | -1       | | -1       |          | -1       |
    // |--------->|--------->|--------->| |--------->|--------->|--------->|
    // | -2       | -2       | -1       | | -2       | -1       | -2       |
    // |--------->|--------->|--------->| |--------->|--------->|--------->|
    // | -3       |          | -3       | | -3       | -2       | -3       |
    // |--------->|          |--------->| |--------->|--------->|--------->|
    // (0) --> (1) --> (6) --> (8)        (8) --> (12)     (17) --> (18)
    // (2) --> (3) --> (7) --> (9)        (9) --> (13) --> (15) --> (19)
    // (4) --> (5)     (10) --> (11)      (11) --> (14) --> (16) --> (20)
    SECTION("3_2_3_3_2_3_jump")
    {
        boost::filesystem::path filepath = g_base_dir / "3_2_3_3_2_3.xodr";
        boost::filesystem::path filepath_dot = g_build_dir / "3_2_3_3_2_3.dot";

        REQUIRE(boost::filesystem::is_regular_file(filepath, ec));
        REQUIRE_FALSE(ec.failed());
        INFO(ec.to_string());

        std::stringstream os;
        CXodrRoadGraph oTest;
        oTest.init(filepath.string());
        oTest.to_graphviz(filepath_dot.string());
        oTest.to_txt(os);
        auto result = os.str();

        CHECK_THAT(result, Catch::Matchers::ContainsSubstring("0 1  (road, lane, sct) := (1, -1, 0)"));
        CHECK_THAT(result, Catch::Matchers::ContainsSubstring("2 3  (road, lane, sct) := (1, -2, 0)"));
        CHECK_THAT(result, Catch::Matchers::ContainsSubstring("4 5  (road, lane, sct) := (1, -3, 0)"));
        CHECK_THAT(result, Catch::Matchers::ContainsSubstring("1 6  (road, lane, sct) := (1, -1, 1)"));
        CHECK_THAT(result, Catch::Matchers::ContainsSubstring("3 7  (road, lane, sct) := (1, -2, 1)"));
        CHECK_THAT(result, Catch::Matchers::ContainsSubstring("6 8  (road, lane, sct) := (1, -1, 2)"));
        CHECK_THAT(result, Catch::Matchers::ContainsSubstring("7 9  (road, lane, sct) := (1, -2, 2)"));
        CHECK_THAT(result, Catch::Matchers::ContainsSubstring("10 11  (road, lane, sct) := (1, -3, 2)"));
        CHECK_THAT(result, Catch::Matchers::ContainsSubstring("8 12  (road, lane, sct) := (2, -1, 0)"));
        CHECK_THAT(result, Catch::Matchers::ContainsSubstring("9 13  (road, lane, sct) := (2, -2, 0)"));
        CHECK_THAT(result, Catch::Matchers::ContainsSubstring("11 14  (road, lane, sct) := (2, -3, 0)"));
        CHECK_THAT(result, Catch::Matchers::ContainsSubstring("13 15  (road, lane, sct) := (2, -1, 1)"));
        CHECK_THAT(result, Catch::Matchers::ContainsSubstring("14 16  (road, lane, sct) := (2, -2, 1)"));
        CHECK_THAT(result, Catch::Matchers::ContainsSubstring("17 18  (road, lane, sct) := (2, -1, 2)"));
        CHECK_THAT(result, Catch::Matchers::ContainsSubstring("15 19  (road, lane, sct) := (2, -2, 2)"));
        CHECK_THAT(result, Catch::Matchers::ContainsSubstring("16 20  (road, lane, sct) := (2, -3, 2)"));
    }

    //  road 1                  road 2 
    //  lanesct 0  lanesct 1    lanesct 0  lanesct 1  
    // |--------->|--------->| |--------->|--------->|
    // | -1       | -1       | | -1       | -1       |
    // |--------->|--------->| |--------->|--------->|
    // | -2       | -2       | | -2       | -2       |
    // |--------->|--------->| |--------->|--------->|
    // | -3       |                       | -3       |
    // |--------->|                       |--------->|
    // (0) --> (1) --> (6)     (6) --> (8)  --> (10)
    // (2) --> (3) --> (7)     (7) --> (9)  --> (11)
    // (4) --> (5)                     (12) --> (13)
    SECTION("3_2_x_2_3")
    {
        boost::filesystem::path filepath = g_base_dir / "3_2_x_2_3.xodr";
        boost::filesystem::path filepath_dot = g_build_dir / "3_2_x_2_3.dot";

        REQUIRE(boost::filesystem::is_regular_file(filepath, ec));
        REQUIRE_FALSE(ec.failed());
        INFO(ec.to_string());

        std::stringstream os;
        CXodrRoadGraph oTest;
        oTest.init(filepath.string());
        oTest.to_graphviz(filepath_dot.string());
        oTest.to_txt(os);
        auto result = os.str();

        CHECK_THAT(result, Catch::Matchers::ContainsSubstring("0 1  (road, lane, sct) := (1, -1, 0)"));
        CHECK_THAT(result, Catch::Matchers::ContainsSubstring("2 3  (road, lane, sct) := (1, -2, 0)"));
        CHECK_THAT(result, Catch::Matchers::ContainsSubstring("4 5  (road, lane, sct) := (1, -3, 0)"));
        CHECK_THAT(result, Catch::Matchers::ContainsSubstring("1 6  (road, lane, sct) := (1, -1, 1)"));
        CHECK_THAT(result, Catch::Matchers::ContainsSubstring("3 7  (road, lane, sct) := (1, -2, 1)"));
        CHECK_THAT(result, Catch::Matchers::ContainsSubstring("6 8  (road, lane, sct) := (2, -1, 0)"));
        CHECK_THAT(result, Catch::Matchers::ContainsSubstring("7 9  (road, lane, sct) := (2, -2, 0)"));
        CHECK_THAT(result, Catch::Matchers::ContainsSubstring("8 10  (road, lane, sct) := (2, -1, 1)"));
        CHECK_THAT(result, Catch::Matchers::ContainsSubstring("9 11  (road, lane, sct) := (2, -2, 1)"));
        CHECK_THAT(result, Catch::Matchers::ContainsSubstring("12 13  (road, lane, sct) := (2, -3, 1)"));
    }
}

TEST_CASE("4: junction_linkage", "[road][lane][junctions]")
{
    boost::system::error_code ec;

    //  road 0     road 100   road 1        road 0     road 101   road 2         road 1     road 102   road 2
    // |--------->|--------->|<---------|  |--------->|--------->|<---------|   |--------->|--------->|<---------|
    // | +1       | +1       | -1       |  | +1       | +1       | -1       |   | +1       | +1       | -1       |
    // |=========>|=========>|<=========|  |=========>|=========>|<=========|   |=========>|=========>|<=========|
    // | -1       | -1       | +1       |  | -1       | -1       | +1       |   | -1       | -1       | +1       |
    // |--------->|--------->|<---------|  |--------->|--------->|<---------|   |--------->|--------->|<---------|
    //
    // (0) --> (1) --> (7) <-- (6)         (0) --> (1) --> (11) <-- (10)         (4) --> (5) --> (11) <-- (10)
    // (2) --> (3) --> (5) <-- (4)         (2) --> (3) --> (9)  <-- (8)          (6) --> (7) --> (9)  <-- (8)
    SECTION("3_arm_junction")
    {
        boost::filesystem::path filepath = g_base_dir / "3_arm_junction.xodr";
        boost::filesystem::path filepath_dot = g_build_dir / "3_arm_junction.dot";

        REQUIRE(boost::filesystem::is_regular_file(filepath, ec));
        REQUIRE_FALSE(ec.failed());
        INFO(ec.to_string());

        std::stringstream os;
        CXodrRoadGraph oTest;
        oTest.init(filepath.string());
        oTest.to_graphviz(filepath_dot.string());
        oTest.to_txt(os);
        auto result = os.str();

        CHECK_THAT(result, Catch::Matchers::ContainsSubstring("0 1  (road, lane, sct) := (0, 1, 0)"));
        CHECK_THAT(result, Catch::Matchers::ContainsSubstring("2 3  (road, lane, sct) := (0, -1, 0)"));
        CHECK_THAT(result, Catch::Matchers::ContainsSubstring("4 5  (road, lane, sct) := (1, 1, 0)"));
        CHECK_THAT(result, Catch::Matchers::ContainsSubstring("6 7  (road, lane, sct) := (1, -1, 0)"));
        CHECK_THAT(result, Catch::Matchers::ContainsSubstring("8 9  (road, lane, sct) := (2, 1, 0)"));
        CHECK_THAT(result, Catch::Matchers::ContainsSubstring("10 11  (road, lane, sct) := (2, -1, 0)"));
        CHECK_THAT(result, Catch::Matchers::ContainsSubstring("1 7  (road, lane, sct) := (100, 1, 0)"));
        CHECK_THAT(result, Catch::Matchers::ContainsSubstring("3 5  (road, lane, sct) := (100, -1, 0)"));
        CHECK_THAT(result, Catch::Matchers::ContainsSubstring("1 11  (road, lane, sct) := (101, 1, 0)"));
        CHECK_THAT(result, Catch::Matchers::ContainsSubstring("3 9  (road, lane, sct) := (101, -1, 0)"));
        CHECK_THAT(result, Catch::Matchers::ContainsSubstring("5 11  (road, lane, sct) := (102, 1, 0)"));
        CHECK_THAT(result, Catch::Matchers::ContainsSubstring("7 9  (road, lane, sct) := (102, -1, 0)"));
    }
}

int main(int argc, char* argv[])
{
    using namespace Catch::Clara;

    std::string base_dir, build_dir;

    Catch::Session session;
    auto cli = session.cli();
    cli |= Opt(base_dir, "base_dir")["--base_dir"]("Base directory of xodr test maps");
    cli |= Opt(build_dir, "build_dir")["--build_dir"]("CMake build directory");
    session.cli(cli);

    int returnCode = session.applyCommandLine(argc, argv);
    if (returnCode != 0)
        return returnCode;

    g_base_dir = base_dir;
    g_build_dir = build_dir;

    return session.run();
}
