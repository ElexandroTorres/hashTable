#include <iterator>             // std::begin(), std::end()
#include <functional>           // std::function
#include <algorithm>            // std::min_element
#include <array>
#include <map>

#include "gtest/gtest.h"        // gtest lib
#include "../include/hashtbl.h"   // header file for tested functions
//#include "../include/account.h"  // To get the account class

TEST(Hash, CopyConstrutor) {
	std::map<std::string, size_t> expected;
    for (const auto &w : { "this", "sentence", "is", "not", "a", "sentence",
                           "this", "sentence", "is", "a", "hoax"})
    {
        ++expected[w];
    }
    // Make this hash have the same elements as the map.
    ac::HashTbl<std::string, size_t>  word_map;
    for( const auto &e : expected )
        word_map.insert( e.first, e.second );

    // Create a copy
    ac::HashTbl<std::string, size_t> copy( word_map );

    // Make sure they have the same elements with the same information.
    for( const auto &e : expected )
    {
        size_t data;
        auto result = copy.retrieve( e.first, data );
        ASSERT_TRUE( result );
        ASSERT_EQ( e.second, data );
    }

    ASSERT_EQ( copy.size(), word_map.size() );
    ASSERT_EQ( expected.size(), copy.size() );
}

TEST(Hash, ConstructorInitializer)
{
    ac::HashTbl<char, int> htables {{'a', 27}, {'b', 3}, {'c', 1}};
    std::map<char, int> expected {{'a', 27}, {'b', 3}, {'c', 1}};

    // Make sure they have the same elements with the same information.
    for( const auto &e : expected )
    {
        int data;
        auto result = htables.retrieve( e.first, data );
        ASSERT_TRUE( result );
        ASSERT_EQ( e.second, data );
    }

    ASSERT_EQ( htables.size(), expected.size() );
}

int main(int argc, char** argv) {

    ::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}