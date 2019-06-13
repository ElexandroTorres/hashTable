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

TEST(Hash, AssignmentOperator)
{
    ac::HashTbl<char, int> htable {{'a', 27}, {'b', 3}, {'c', 1}};
    ac::HashTbl<char, int> htable_copy;
    std::map<char, int> expected {{'a', 27}, {'b', 3}, {'c', 1}};

    // Make sure they are different
    for( const auto &e : expected )
    {
        int data;
        auto result = htable_copy.retrieve( e.first, data );
        ASSERT_FALSE( result );
    }
    // Testing assignment.
    htable_copy = htable;
    // Destroy orginal table, to prove they dont share memory
    htable.clear();

    // Make sure they have the same elements with the same information.
    for( const auto &e : expected )
    {
        int data;
        auto result = htable_copy.retrieve( e.first, data );
        ASSERT_TRUE( result );
        ASSERT_EQ( e.second, data );
    }

    ASSERT_EQ( htable_copy.size(), expected.size() );
}

TEST(Hash, Insert)
{
    ac::HashTbl<char, int> htable( 3 );
    std::map<char, int> expected {{'x', 27}, {'y', 3}, {'w', 1}, {'a', 21}, {'b', 6}, {'c', 11}};

    ASSERT_TRUE( htable.empty() );

    // Test insertion
    for( const auto &e : expected )
    {
        auto result = htable.insert( e.first, e.second );
        ASSERT_TRUE( result );
    }

    // Make sure they have the same elements with the same information.
    for( const auto &e : expected )
    {
        int data;
        auto result = htable.retrieve( e.first, data );
        ASSERT_TRUE( result );
        ASSERT_EQ( e.second, data );
    }
}

TEST(Hash, InsertExisting)
{
    ac::HashTbl<char, int> htable {{'x', 2}, {'y', 1}, {'w', 4}, {'a', 5}, {'b', 8}, {'c', 7}};
    std::map<char, int> expected {{'x', 27}, {'y', 3}, {'w', 1}, {'a', 21}, {'b', 6}, {'c', 11}};

    // Make sure the two hash tables store different values.
    for( const auto &e : expected )
    {
        int data;
        auto result = htable.retrieve( e.first, data );
        ASSERT_TRUE( result ); // We must found the key
        ASSERT_NE( e.second, data ); // BUT, the data must differ
    }
    
    // Let us use the insert() to change the data stored in the table.
    for( const auto &e : expected )
    {
        int data;
        auto result = htable.retrieve( e.first, data );
        ASSERT_TRUE( result );
        result = htable.insert( e.first, e.second );
        ASSERT_FALSE( result ); // False in case the element is already in the table.
    }
	
	
    // Finally, Make sure that now they have the same elements with the same information.
    for( const auto &e : expected )
    {
        int data;
        auto result = htable.retrieve( e.first, data );
        ASSERT_TRUE( result );
        ASSERT_EQ( e.second, data );
    }
    
}

TEST(Hash, Retrieve)
{
    ac::HashTbl<char, int> htable{{'x', 27}, {'y', 3}, {'w', 1}, {'a', 21}, {'b', 6}, {'c', 11}};
    std::map<char, int> expected {{'x', 27}, {'y', 3}, {'w', 1}, {'a', 21}, {'b', 6}, {'c', 11}};
    std::map<char, int> unexpected {{'s', 27}, {'e', 3}, {'g', 1}, {'q', 21}, {'i', 6}, {'j', 11}};

    // Make sure they have the same elements with the same information.
    for( const auto &e : expected )
    {
        int data;
        auto result = htable.retrieve( e.first, data );
        ASSERT_TRUE( result );
        ASSERT_EQ( e.second, data );
    }
    // Make sure none of the elements in the unexpected map are found.
    for( const auto &e : unexpected )
    {
        int data;
        auto result = htable.retrieve( e.first, data );
        ASSERT_FALSE( result );
    }
}

TEST(Hash, EraseExisting)
{
    ac::HashTbl<char, int> htable {{'x', 2}, {'y', 1}, {'w', 4}, {'a', 5}, {'b', 8}, {'c', 7}};
    std::map<char, int> expected {{'x', 2}, {'y', 1}, {'w', 4}, {'a', 5}, {'b', 8}, {'c', 7}};

    // Make sure the two hash tables store different values.
    for( const auto &e : expected )
    {
        auto result = htable.erase( e.first );
        ASSERT_TRUE( result ); // We must found the key
    }

    // Let us try to locate the erased elements.
    for( const auto &e : expected )
    {
        int data=0;
        auto result = htable.retrieve( e.first, data );
        ASSERT_FALSE( result ); // We must NOT found the key
        ASSERT_EQ( data, 0 );
    }

    ASSERT_TRUE( htable.empty() );
}

TEST(Hash, EraseNonExisting)
{
    ac::HashTbl<char, int> htable {{'x', 2}, {'y', 1}, {'w', 4}, {'a', 5}, {'b', 8}, {'c', 7}};
    std::map<char, int> expected {{'x', 2}, {'y', 1}, {'w', 4}, {'a', 5}, {'b', 8}, {'c', 7}};
    std::map<char, int> unexpected {{'s', 27}, {'e', 3}, {'g', 1}, {'q', 21}, {'i', 6}, {'j', 11}};

    auto curr_size = htable.size();
    
    // Try to erase non-existing keys
    
    for( const auto &e : unexpected )
    {
        auto result = htable.erase( e.first );
        ASSERT_FALSE( result ); // We must NOT found the key
    }
    ASSERT_EQ( curr_size, htable.size() ); // This must've not changed.
    
    // Make sure tha hash table has not changed.
    for( const auto &e : expected )
    {
        int data;
        auto result = htable.retrieve( e.first, data );
        ASSERT_TRUE( result );
        ASSERT_EQ( e.second, data );
    }
    
}


TEST(Hash, Clear)
{
    ac::HashTbl<char, int> htable {{'x', 2}, {'y', 1}, {'w', 4}, {'a', 5}, {'b', 8}, {'c', 7}};

    auto curr_size = htable.size();
    ASSERT_FALSE( htable.empty() );

    htable.clear();

    ASSERT_NE( curr_size, htable.size() );
    ASSERT_TRUE( htable.empty() );
    ASSERT_EQ( htable.size(), 0 );
}

TEST(Hash, Count)
{
    ac::HashTbl<int, std::string> htable (9);
    std::map<int, std::string> set1 {{11, "eleven"}, {2*11, "twenty two"}, {3*11, "thirty three"}, {4*11, "fourty four"} };
    std::map<int, std::string> set2 {{5, "five"}, {2*11+5, "twnety seven"}, {3*11+5, "thirty eight"}};
    std::map<int, std::string> set3 {{2, "seven"}, {2*11+2, "twenty four"}};

    // Insert elements from the first set.
    // This should trigger the rehash()
    for( const auto &e : set1 )
    {
        auto result = htable.insert( e.first, e.second );
        ASSERT_TRUE( result ); // We must NOT found the key
    }
    
    for( const auto &e : set2 )
    {
        auto result = htable.insert( e.first, e.second );
        ASSERT_TRUE( result ); // We must NOT found the key
    }

    for( const auto &e : set3 )
    {
        auto result = htable.insert( e.first, e.second );
        ASSERT_TRUE( result ); // We must NOT found the key
    }

    // Let us compare the number of elements hashed to the same bucket.
    for ( const auto &e: set1 )
        ASSERT_EQ( set1.size(), htable.count(e.first) );
    for ( const auto &e: set2 )
        ASSERT_EQ( set2.size(), htable.count(e.first) );
    for ( const auto &e: set3 )
        ASSERT_EQ( set3.size(), htable.count(e.first) );
    
}

TEST(Hash, AssignmentInitializer)
{
    ac::HashTbl<char, int> htable {{'x', 27}, {'y', 3}, {'w', 1}};
    std::map<char, int> expected {{'a', 27}, {'b', 3}, {'c', 1}};

    // Make sure they are different
    for( const auto &e : expected )
    {
        int data;
        auto result = htable.retrieve( e.first, data );
        ASSERT_FALSE( result );
    }
    // Testing initializer assignment
    htable = {{'a', 27}, {'b', 3}, {'c', 1}};

    // Make sure they have the same elements with the same information.
    for( const auto &e : expected )
    {
        int data;
        auto result = htable.retrieve( e.first, data );
        ASSERT_TRUE( result );
        ASSERT_EQ( e.second, data );
    }
}

TEST(Hash, Rehash)
{
    ac::HashTbl<char, int> htable (2);
    std::map<char, int> set1 {{'x', 2}, {'y', 1}, {'w', 4}, {'a', 5}, {'b', 8}, {'c', 7}};
    std::map<char, int> set2 {{'s', 27}, {'e', 3}, {'g', 1}, {'q', 21}, {'i', 6}, {'j', 11}};

    // Insert elements from the first set.
    // This should trigger the rehash()
    for( const auto &e : set1 )
    {
        auto result = htable.insert( e.first, e.second );
        ASSERT_TRUE( result ); // We must NOT found the key
    }
    ASSERT_EQ( set1.size(), htable.size() ); // This must've not changed.
    
    // Insert more elements to trigger another rehash
    for( const auto &e : set2 )
    {
        auto result = htable.insert( e.first, e.second );
        ASSERT_TRUE( result );
    }
    
    // Check whether the hash now have the same elemens as set1 and set2 combined.
    for( const auto &e : set1 )
    {
        int data;
        auto result = htable.retrieve( e.first, data );
        ASSERT_TRUE( result );
        ASSERT_EQ( e.second, data );
    }

    for( const auto &e : set2 )
    {
        int data;
        auto result = htable.retrieve( e.first, data );
        ASSERT_TRUE( result );
        ASSERT_EQ( e.second, data );
    }
    
}

TEST(Hash, AtException)
{
    // count the number of occurrences of each word
    // (the first call to operator[] initialized the counter with zero)
    ac::HashTbl<std::string, size_t>  word_map;
    for (const auto &w : { "this", "sentence", "is", "not", "a", "sentence",
                           "this", "sentence", "is", "a", "hoax"})
    {
        bool worked{ false };
        try {
            // This should fail, because there is no key in the hash.
            ++word_map.at(w);
        }
        catch( const std::out_of_range& e )
        {
            worked = true;
        }
        ASSERT_TRUE( worked );
    }
}

TEST(Hash, AtLHS2)
{
    // count the number of occurrences of each word
    // (the first call to operator[] initialized the counter with zero)
    std::map<std::string, size_t> expected;
    ac::HashTbl<std::string, size_t>  word_map;
    for (const auto &w : { "this", "sentence", "is", "not", "a", "sentence",
                           "this", "sentence", "is", "a", "hoax"})
    {
        ++word_map[w];
        ++expected[w];
    }
    
    // Compare the two dictionaries
    bool entered{false};
    for (const auto &pair : expected )
    {
        entered = true;
        // Testing the at() as the RHS term in an assignment.
        word_map.at(pair.first) *= 10;
    }
    ASSERT_TRUE( entered );
    
    entered = false;
    for (const auto &pair : expected )
    {
        entered = true;
        ASSERT_EQ( pair.second*10, word_map.at(pair.first) );
    }
    ASSERT_TRUE( entered );
    
}

TEST(Hash, OperatorSquareBraketsLHS2)
{
    // count the number of occurrences of each word
    // (the first call to operator[] initialized the counter with zero)
    std::map<std::string, size_t> expected;
    ac::HashTbl<std::string, size_t>  word_map;
    for (const auto &w : { "this", "sentence", "is", "not", "a", "sentence",
                           "this", "sentence", "is", "a", "hoax"})
    {
        ++word_map[w];
        ++expected[w];
    }

    ASSERT_EQ( expected.size(), word_map.size() );

    // Compare the two dictionaries
    bool entered{false};
    for (const auto &pair : expected )
    {
        entered = true;
        ASSERT_EQ( pair.second, word_map[pair.first] );
    }
    ASSERT_TRUE( entered );
}

int main(int argc, char** argv) {

    ::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}