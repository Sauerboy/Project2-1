//=================================================================
// Copyright 2022 Georgia Tech.  All rights reserved.
// The materials provided by the instructor in this course are for
// the use of the students currently enrolled in the course.
// Copyrighted course materials may not be further disseminated.
// This file must not be made publicly available anywhere.
//=================================================================

// Inform the compiler that this included module is written in C instead of C++.
extern "C" {
	#include "hash_table.h"
}
#include "gtest/gtest.h"


// Use the TEST macro to define your tests.
//
// TEST has two parameters: the test case name and the test name.
// After using the macro, you should define your test logic between a
// pair of braces.  You can use a bunch of macros to indicate the
// success or failure of a test.  EXPECT_TRUE and EXPECT_EQ are
// examples of such macros.  For a complete list, see gtest.h.
//
// <TechnicalDetails>
//
// In Google Test, tests are grouped into test cases.  This is how we
// keep test code organized.  You should put logically related tests
// into the same test case.
//
// The test case name and the test name should both be valid C++
// identifiers.  And you should not use underscore (_) in the names.
//
// Google Test guarantees that each test you define is run exactly
// once, but it makes no guarantee on the order the tests are
// executed.  Therefore, you should write your tests in such a way
// that their results don't depend on their order.
//
// </TechnicalDetails>

// The #define directive defines a constant value that can be accessed throughout
// your code. Here it defines the default number of buckets in the hash table.
// You can change this number, but make sure to update the hash function with
// the right algorithm to compute the indices for buckets.
// For example, if the BUCKET_NUM is set to 5, the hash function should map a
// positive number to an integer between 0 and 4.
#define BUCKET_NUM  3

// Dummy value to store in hash table entry
// Please beware that any type of data (e.g. int, double, struct and etc.) can
// be stored in hash table for testing your hash table. Only the pointer to
// the data will be stored in the HashTableEntry.
struct HTItem {};

// Helper function for creating a lot of dummy values.
void make_items(HTItem* result[], unsigned n)
{
	// Populate the array with pointers to the dummy values.
	while(n--)
	{
		result[n] = (HTItem*) malloc(sizeof(HTItem));
	}
}

// A simple hash function that maps a positive number to an integer between 0~(BUCKET_NUM-1).
unsigned int hash(unsigned int key) {
	return key%BUCKET_NUM;
}

////////////////////////
// Initialization tests
////////////////////////
TEST(InitTest, CreateDestroyHashTable)
{
	HashTable* ht = createHashTable(hash, BUCKET_NUM);
	destroyHashTable(ht);
}

////////////////
// Access tests
////////////////
TEST(AccessTest, GetKey_TableEmpty)
{
	HashTable* ht = createHashTable(hash, BUCKET_NUM);

	// Test when table is empty.
	EXPECT_EQ(NULL, getItem(ht, 0));
	EXPECT_EQ(NULL, getItem(ht, 1));
	EXPECT_EQ(NULL, getItem(ht, 2));

	// Test with index greater than the number of buckets.
	EXPECT_EQ(NULL, getItem(ht, 10));

	destroyHashTable(ht);
}

TEST(AccessTest, GetKey_InvalidKey)
{
	HashTable* ht = createHashTable(hash, BUCKET_NUM);

	// Create list of items
  	size_t num_items = 3;
  	HTItem* m[num_items];
  	make_items(m, num_items);

  	insertItem(ht, 0, m[0]);
  	insertItem(ht, 1, m[1]);
  	insertItem(ht, 2, m[2]);

	// Test when table is empty.
	EXPECT_EQ(NULL, getItem(ht, 3));
	EXPECT_EQ(NULL, getItem(ht, 4));
	EXPECT_EQ(NULL, getItem(ht, 5));

	destroyHashTable(ht);
}

TEST(AccessTest, GetSingleKey)
{
  HashTable* ht = createHashTable(hash, BUCKET_NUM);

  // Create list of items
  size_t num_items = 1;
  HTItem* m[num_items];
  make_items(m, num_items);

  insertItem(ht, 0, m[0]);
  EXPECT_EQ(m[0], getItem(ht, 0));

  destroyHashTable(ht);    // dummy item is also freed here
}

TEST(AccessTest, InsertLargeKey)
{
  HashTable* ht = createHashTable(hash, BUCKET_NUM);

  // Create list of items
  size_t num_items = 1;
  HTItem* m[num_items];
  make_items(m, num_items);

  insertItem(ht, 13543, m[0]);
  EXPECT_EQ(m[0], getItem(ht, 13543));

  destroyHashTable(ht);    // dummy item is also freed here
}

TEST(AccessTest, InsertNegativeKey)
{
  HashTable* ht = createHashTable(hash, BUCKET_NUM);

  // Create list of items
  size_t num_items = 1;
  HTItem* m[num_items];
  make_items(m, num_items);

  insertItem(ht, -1, m[0]);
  EXPECT_EQ(m[0], getItem(ht, -1));

  destroyHashTable(ht);    // dummy item is also freed here
}

TEST(AccessTest, GetMultiKey)
{
  HashTable* ht = createHashTable(hash, BUCKET_NUM);

  // Create list of items
  size_t num_items = 3;
  HTItem* m[num_items];
  make_items(m, num_items);

  insertItem(ht, 0, m[0]);
  insertItem(ht, 1, m[1]);
  insertItem(ht, 2, m[2]);
  EXPECT_EQ(m[0], getItem(ht, 0));
  EXPECT_EQ(m[1], getItem(ht, 1));
  EXPECT_EQ(m[2], getItem(ht, 2));

  destroyHashTable(ht);    // dummy items are also freed here
}

TEST(AccessTest, GetMultiSameKey)
{
  HashTable* ht = createHashTable(hash, BUCKET_NUM);

  // Create list of items
  size_t num_items = 4;
  HTItem* m[num_items];
  make_items(m, num_items);

  insertItem(ht, 0, m[0]);
  insertItem(ht, 1, m[1]);
  insertItem(ht, 2, m[2]);
  free(insertItem(ht, 2, m[3])); //must free the value insert returns
  EXPECT_EQ(m[0], getItem(ht, 0));
  EXPECT_EQ(m[1], getItem(ht, 1));
  EXPECT_EQ(m[3], getItem(ht, 2));

  destroyHashTable(ht);    // dummy items are also freed here
}

TEST(AccessTest, GetKey_KeyNotPresent)
{
	HashTable* ht = createHashTable(hash, BUCKET_NUM);

	// Create a list of items to add to hash table.
	size_t num_items = 1;
	HTItem* m[num_items];
	make_items(m, num_items);

	// Insert one item into the hash table.
	insertItem(ht, 0, m[0]);

	// Test if the hash table returns NULL when the key is not found.
	EXPECT_EQ(NULL, getItem(ht, 1));

	// Destroy the hash table togeter with the inserted values
	destroyHashTable(ht);
}

////////////////////////////
// Removal and delete tests
////////////////////////////
TEST(RemoveTest, SingleValidRemove)
{
	HashTable* ht = createHashTable(hash, BUCKET_NUM);

	// Create a list of items to add to hash table.
	size_t num_items = 1;
	HTItem* m[num_items];
	make_items(m, num_items);

	// Insert one item into the hash table.
	insertItem(ht, 0, m[0]);

	// After removing an item with a specific key, the data stored in the
	// corresponding entry should be returned. If the key is not present in the
	// hash table, then NULL should be returned.
	void* data = removeItem(ht, 0);

	// Since the key we want to remove is present in the hash table, the correct
	// data should be returned.
	EXPECT_EQ(m[0], data);

	// Free the data
	free(data);

	destroyHashTable(ht);
}

TEST(RemoveTest, MultiRemoveDiffBuckets)
{
	HashTable* ht = createHashTable(hash, BUCKET_NUM);

	// Create a list of items to add to hash table.
	size_t num_items = 4;
	HTItem* m[num_items];
	make_items(m, num_items);

	// Insert one item into the hash table.
	insertItem(ht, 0, m[0]);
	insertItem(ht, 1, m[1]);
	free(insertItem(ht, 1, m[2]));
	insertItem(ht, 2, m[3]);

	// After removing an item with a specific key, the data stored in the
	// corresponding entry should be returned. If the key is not present in the
	// hash table, then NULL should be returned.
	void* data[3];
	data[0] = removeItem(ht, 0);
	data[1] = removeItem(ht, 1);
	data[2] = removeItem(ht, 2);

	// Since the key we want to remove is present in the hash table, the correct
	// data should be returned.
	EXPECT_EQ(m[0], data[0]);
	EXPECT_EQ(m[2], data[1]);
	EXPECT_EQ(m[3], data[2]);

	// Free the data
	free(data[0]);
	free(data[1]);
	free(data[2]);

	destroyHashTable(ht);
}

TEST(RemoveTest, RemoveLastValue)
{
	HashTable* ht = createHashTable(hash, BUCKET_NUM);

	// Create a list of items to add to hash table.
	size_t num_items = 3;
	HTItem* m[num_items];
	make_items(m, num_items);

	// Insert one item into the hash table.
	insertItem(ht, 0, m[0]);
	insertItem(ht, 3, m[1]);
	insertItem(ht, 6, m[2]);
	// After removing an item with a specific key, the data stored in the
	// corresponding entry should be returned. If the key is not present in the
	// hash table, then NULL should be returned.
	void* data;
	data = removeItem(ht, 0);

	// Since the key we want to remove is present in the hash table, the correct
	// data should be returned.
	EXPECT_EQ(m[0], data);

	// Free the data
	free(data);
	destroyHashTable(ht);
}

TEST(RemoveTest, RemoveFirstValue)
{
	HashTable* ht = createHashTable(hash, BUCKET_NUM);

	// Create a list of items to add to hash table.
	size_t num_items = 3;
	HTItem* m[num_items];
	make_items(m, num_items);

	// Insert one item into the hash table.
	insertItem(ht, 0, m[0]);
	insertItem(ht, 3, m[1]);
	insertItem(ht, 6, m[2]);
	// After removing an item with a specific key, the data stored in the
	// corresponding entry should be returned. If the key is not present in the
	// hash table, then NULL should be returned.
	void* data;
	data = removeItem(ht, 6);

	// Since the key we want to remove is present in the hash table, the correct
	// data should be returned.
	EXPECT_EQ(m[2], data);

	// Free the data
	free(data);
	destroyHashTable(ht);
}

TEST(RemoveTest, RemoveMiddleValue)
{
	HashTable* ht = createHashTable(hash, BUCKET_NUM);

	// Create a list of items to add to hash table.
	size_t num_items = 3;
	HTItem* m[num_items];
	make_items(m, num_items);

	// Insert one item into the hash table.
	insertItem(ht, 0, m[0]);
	insertItem(ht, 3, m[1]);
	insertItem(ht, 6, m[2]);
	// After removing an item with a specific key, the data stored in the
	// corresponding entry should be returned. If the key is not present in the
	// hash table, then NULL should be returned.
	void* data;
	data = removeItem(ht, 3);

	// Since the key we want to remove is present in the hash table, the correct
	// data should be returned.
	EXPECT_EQ(m[1], data);

	// Free the data
	free(data);
	destroyHashTable(ht);
}

TEST(RemoveTest, RemoveInvalidKey)
{
	HashTable* ht = createHashTable(hash, BUCKET_NUM);

	// Create a list of items to add to hash table.
	size_t num_items = 3;
	HTItem* m[num_items];
	make_items(m, num_items);

	// Insert one item into the hash table.
	insertItem(ht, 0, m[0]);
	insertItem(ht, 3, m[1]);
	insertItem(ht, 6, m[2]);
	// After removing an item with a specific key, the data stored in the
	// corresponding entry should be returned. If the key is not present in the
	// hash table, then NULL should be returned.
	void* data;
	data = removeItem(ht, 9);

	// Since the key we want to remove is not present in the hash table, NULL
	// data should be returned.
	EXPECT_EQ(NULL, data);

	// Free the data
	free(data);
	destroyHashTable(ht);
}

TEST(DeleteTest, DeleteInvalidKey)
{
	HashTable* ht = createHashTable(hash, BUCKET_NUM);

	// Create a list of items to add to hash table.
	size_t num_items = 3;
	HTItem* m[num_items];
	make_items(m, num_items);

	// Insert one item into the hash table.
	insertItem(ht, 0, m[0]);
	insertItem(ht, 3, m[1]);
	insertItem(ht, 6, m[2]);
	// After removing an item with a specific key, the data stored in the
	// corresponding entry should be returned. If the key is not present in the
	// hash table, then NULL should be returned.
	deleteItem(ht, 9);

	EXPECT_EQ(m[0], getItem(ht, 0));
	EXPECT_EQ(m[1], getItem(ht, 3));
	EXPECT_EQ(m[2], getItem(ht, 6));
	
	destroyHashTable(ht);
}

TEST(DeleteTest, DeleteValidKey)
{
	HashTable* ht = createHashTable(hash, BUCKET_NUM);

	// Create a list of items to add to hash table.
	size_t num_items = 3;
	HTItem* m[num_items];
	make_items(m, num_items);

	// Insert items into the hash table.
	insertItem(ht, 0, m[0]);
	insertItem(ht, 3, m[1]);
	insertItem(ht, 6, m[2]);
	// Delete the item
	deleteItem(ht, 3);

	EXPECT_EQ(m[0], getItem(ht, 0));
	EXPECT_EQ(NULL, getItem(ht, 3));
	EXPECT_EQ(m[2], getItem(ht, 6));
	
	destroyHashTable(ht);
}

TEST(DeleteTest, DeleteNullValue)
{
	HashTable* ht = createHashTable(hash, BUCKET_NUM);

	// Insert items into the hash table.
	insertItem(ht, 0, NULL);
	// Delete the NULL item
	deleteItem(ht, 0);
	// Should not be able to find item and return NULL
	EXPECT_EQ(NULL, getItem(ht, 0));
	
	destroyHashTable(ht);
}

TEST(RemoveTest, SingleInvalidRemove)
{
	HashTable* ht = createHashTable(hash, BUCKET_NUM);

	// When the hash table is empty, the remove funtion should still work.
	EXPECT_EQ(NULL, removeItem(ht, 1));

	destroyHashTable(ht);
}

///////////////////
// Insersion tests
///////////////////
TEST(InsertTest, InsertAsOverwrite)
{
	HashTable* ht = createHashTable(hash, BUCKET_NUM);

	// Create list of items to be added to the hash table.
	size_t num_items = 2;
	HTItem* m[num_items];
	make_items(m, num_items);

	// Only insert one item with key=0 into the hash table.
	insertItem(ht, 0, m[0]);

	// When we are inserting a different value with the same key=0, the hash table
	// entry should hold the new value instead. In the test case, the hash table entry
	// corresponding to key=0 will hold m[1] and return m[0] as the return value.
	EXPECT_EQ(m[0], insertItem(ht, 0, m[1]));

	// Now check if the new value m[1] has indeed been stored in hash table with
	// key=0.
	EXPECT_EQ(m[1], getItem(ht,0));

	destroyHashTable(ht);
	free(m[0]);    // don't forget to free item 0
}

TEST(InsertTest, InsertNullAsOverwrite)
{
	HashTable* ht = createHashTable(hash, BUCKET_NUM);

	// Create list of items to be added to the hash table.
	size_t num_items = 1;
	HTItem* m[num_items];
	make_items(m, num_items);

	// Only insert one item with key=0 into the hash table.
	insertItem(ht, 0, m[0]);

	// When we are inserting a different value with the same key=0, the hash table
	// entry should hold the new value instead. In the test case, the hash table entry
	// corresponding to key=0 will hold NULL and return m[0] as the return value.
	EXPECT_EQ(m[0], insertItem(ht, 0, NULL));

	// Now check if the NULL has indeed been stored in hash table with
	// key=0.
	EXPECT_EQ(NULL, getItem(ht,0));

	destroyHashTable(ht);
	free(m[0]);    // don't forget to free item 0
}

TEST(InsertTest, InsertAsNew)
{
	HashTable* ht = createHashTable(hash, BUCKET_NUM);

	// Create list of items to be added to the hash table.
	size_t num_items = 2;
	HTItem* m[num_items];
	make_items(m, num_items);

	// Only insert one item with key=0 into the hash table.
	insertItem(ht, 0, m[0]);

	// When we are inserting a different value with different key=1, the hash table
	// entry should hold this value. In the test case, the hash table entry
	// corresponding to key=1 will hold m[1] as the return value.
	EXPECT_EQ(NULL, insertItem(ht, 1, m[1]));

	// Now check if the new value m[1] has indeed been stored in hash table with
	// key=1.
	EXPECT_EQ(m[1], getItem(ht,1));

	destroyHashTable(ht);
}
