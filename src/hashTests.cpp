#include <iterator>             // std::begin(), std::end()
#include <functional>           // std::function
#include <algorithm>            // std::min_element
#include <array>
#include <map>

#include "gtest/gtest.h"        // gtest lib
#include "../include/hashtbl.h"   // header file for tested functions
#include "../include/account.h"  // To get the account class

// ============================================================================
// Test Fxture
// ============================================================================

class HTTest : public ::testing::Test {

    public:
    // Infrastructure to help the tests.
    std::array<Account, 8> m_accounts; //!< our fixed account data base
    Account target; //!< Target account to search for.

    /// This is the hash table we use in the tests.
    ac::HashTbl< Account::AcctKey, Account, KeyHash, KeyEqual > ht_accounts{ 4 };

    protected:
    void SetUp() override
    {
        // Set up our accounts.
        m_accounts[0] = {"Alex Bastos", 1, 1668, 54321, 1500.f};
        m_accounts[1] = {"Aline Souza", 1, 1668, 45794, 530.f};
        m_accounts[2] = {"Cristiano Ronaldo", 13, 557, 87629, 150000.f};
        m_accounts[3] = {"Jose Lima", 18, 331, 1231, 850.f};
        m_accounts[4] = {"Saulo Cunha", 116, 666, 1, 5490.f};
        m_accounts[5] = {"Lima Junior", 12, 123, 5671, 150.f};
        m_accounts[6] = {"Carlito Pardo", 28, 506, 9816, 50.f};
        m_accounts[7] = {"Januario Medeiros", 17, 324, 7777, 4850.f};

        target = m_accounts[0];
    }

  void TearDown() override {/* empty */}

  void insert_accounts();
};

void HTTest::insert_accounts( void )
{
    // Inserindo as contas na tabela hash.
    for( auto & e : m_accounts )
        ht_accounts.insert( e.getKey(), e );
}


// ============================================================================
// TESTING HASH TABLE
// ============================================================================

TEST_F(HTTest, InitialState)
{
    ASSERT_TRUE( ht_accounts.empty() );
    ASSERT_EQ( ht_accounts.size(), 0 );
}

TEST_F(HTTest, InsertingData)
{
    Account temp;
    size_t i(0);
    // Inserindo as contas na tabela hash.
    for( auto & e : m_accounts )
    {
        ht_accounts.insert( e.getKey(), e );
        ASSERT_EQ( ++i, ht_accounts.size() );
        //std::cout << ">>> Inserindo \"" << e.m_name << "\"\n";
        //std::cout << ">>> Tabela Hash de Contas depois da insercao: \n" << ht_accounts << std::endl;
        // Unit test for insertion
        ht_accounts.retrieve( e.getKey(), temp );
        ASSERT_EQ( temp, e );
    }
}

TEST_F(HTTest, OperatorSquareBraketsRHS)
{
    insert_accounts();

    // Retrieve each element
    for( auto & e : m_accounts )
        ASSERT_EQ( ht_accounts[e.getKey()], e );
}

TEST_F(HTTest, OperatorSquareBraketsLHS)
{
    insert_accounts();

    auto curr_size = ht_accounts.size();
    // Change the data in the table.
    auto i{10};
    for( auto & e : m_accounts )
    {
        auto x = ht_accounts[e.getKey()] ;
        x.m_balance = 100.+i;
        i+= 10;
        ht_accounts[e.getKey()] = x;
    }
    // The table size should be the same.
    ASSERT_EQ( curr_size, ht_accounts.size() );

    i=10;
    // Check the table if the changes took place.
    for( auto & e : m_accounts )
    {
        auto x = ht_accounts[e.getKey()] ;
        ASSERT_EQ ( x.m_balance, 100.+i );
        i+= 10;
    }
}

TEST_F(HTTest, OperatorSquareBraketsLHS2)
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

TEST_F(HTTest, AtRHS)
{
    insert_accounts();

    // Retrieve each element
    for( auto & e : m_accounts )
        ASSERT_EQ( ht_accounts.at(e.getKey()), e );
}

TEST_F(HTTest, AtLHS)
{
    insert_accounts();

    auto curr_size = ht_accounts.size();
    // Change the data in the table.
    auto i{10};
    for( auto & e : m_accounts )
    {
        auto x = ht_accounts.at(e.getKey()) ;
        x.m_balance = 100.+i;
        i+= 10;
        ht_accounts.at(e.getKey()) = x;
    }
    ASSERT_EQ( curr_size, ht_accounts.size() );

    i=10;
    // Check the table if the changes took place.
    for( auto & e : m_accounts )
    {
        auto x = ht_accounts.at(e.getKey()) ;
        ASSERT_EQ ( x.m_balance, 100.+i );
        i+= 10;
    }
}

TEST_F(HTTest, AtLHS2)
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

TEST_F(HTTest, AtException)
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

TEST_F(HTTest, CopyConstructor)
{
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
    ac::HashTbl<std::string, size_t>  copy( word_map );

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

TEST_F(HTTest, ConstructorInitializer)
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

TEST_F(HTTest, AssignmentOperator)
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

TEST_F(HTTest, AssignmentInitializer)
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

TEST_F(HTTest, Insert)
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

TEST_F(HTTest, InsertExisting)
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

TEST_F(HTTest, Retrieve)
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

TEST_F(HTTest, EraseExisting)
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

TEST_F(HTTest, EraseNonExisting)
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

TEST_F(HTTest, Clear)
{
    ac::HashTbl<char, int> htable {{'x', 2}, {'y', 1}, {'w', 4}, {'a', 5}, {'b', 8}, {'c', 7}};

    auto curr_size = htable.size();
    ASSERT_FALSE( htable.empty() );

    htable.clear();

    ASSERT_NE( curr_size, htable.size() );
    ASSERT_TRUE( htable.empty() );
    ASSERT_EQ( htable.size(), 0 );
}

TEST_F(HTTest, Rehash)
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


TEST_F(HTTest, Count)
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
    //std::cout << "The table: \n" << htable << std::endl;
}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
