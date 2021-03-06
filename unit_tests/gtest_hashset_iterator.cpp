#include "gtest_hashset.h"

namespace {

class HashSetIteratorTest : public ::testing::Test
{
  public:
	HashSetIteratorTest()
	    : hashset_(Capacity) {}

  protected:
	void SetUp() override { initHashSet(hashset_); }

	HashSetTestType hashset_;
};

TEST_F(HashSetIteratorTest, ForLoopIteration)
{
	int n = 0;

	printf("Iterating through elements with for loop:\n");
	for (HashSetTestType::ConstIterator i = hashset_.begin(); i != hashset_.end(); ++i)
	{
		printf(" [%d] hash: %u, key: %d\n", n, i.hash(), i.key());
		ASSERT_EQ(i.key(), n);
		n++;
	}
	printf("\n");
}

TEST_F(HashSetIteratorTest, ForLoopEmptyIteration)
{
	HashSetTestType newHashset(Capacity);

	printf("Iterating over an empty hashset with for loop:\n");
	for (HashSetTestType::ConstIterator i = newHashset.begin(); i != newHashset.end(); ++i)
		ASSERT_TRUE(false); // should never reach this point
	printf("\n");
}

TEST_F(HashSetIteratorTest, ReverseForLoopIteration)
{
	int n = Size - 1;

	printf("Reverse iterating through elements with for loop:\n");
	for (HashSetTestType::ConstReverseIterator r = hashset_.rBegin(); r != hashset_.rEnd(); ++r)
	{
		printf(" [%d] hash: %u, key: %d\n", n, r.base().hash(), r.base().key());
		ASSERT_EQ(r.base().key(), n);
		n--;
	}
	printf("\n");
}

TEST_F(HashSetIteratorTest, ReverseForLoopEmptyIteration)
{
	HashSetTestType newHashset(Capacity);

	printf("Reverse iterating over an empty hashset with for loop:\n");
	for (HashSetTestType::ConstReverseIterator r = newHashset.rBegin(); r != newHashset.rEnd(); ++r)
		ASSERT_TRUE(false); // should never reach this point
	printf("\n");
}

TEST_F(HashSetIteratorTest, WhileLoopIteration)
{
	int n = 0;

	printf("Iterating through elements with while loop:\n");
	HashSetTestType::ConstIterator i = hashset_.begin();
	while (i != hashset_.end())
	{
		printf(" [%d] hash: %u, key: %d\n", n, i.hash(), i.key());
		ASSERT_EQ(i.key(), n);
		++i;
		++n;
	}
	printf("\n");
}

TEST_F(HashSetIteratorTest, WhileLoopEmptyIteration)
{
	HashSetTestType newHashset(Capacity);

	printf("Iterating over an empty hashset with while loop:\n");
	HashSetTestType::ConstIterator i = newHashset.begin();
	while (i != newHashset.end())
	{
		ASSERT_TRUE(false); // should never reach this point
		++i;
	}
	printf("\n");
}

TEST_F(HashSetIteratorTest, ReverseWhileLoopIteration)
{
	int n = Size - 1;

	printf("Reverse iterating through elements with while loop:\n");
	HashSetTestType::ConstReverseIterator r = hashset_.rBegin();
	while (r != hashset_.rEnd())
	{
		printf(" [%d] hash: %u, key: %d\n", n, r.base().hash(), r.base().key());
		ASSERT_EQ(r.base().key(), n);
		++r;
		--n;
	}
	printf("\n");
}

TEST_F(HashSetIteratorTest, ReverseWhileLoopEmptyIteration)
{
	HashSetTestType newHashset(Capacity);

	printf("Reverse iterating over an empty hashset with while loop:\n");
	HashSetTestType::ConstReverseIterator r = newHashset.rBegin();
	while (r != newHashset.rEnd())
	{
		ASSERT_TRUE(false); // should never reach this point
		++r;
	}
	printf("\n");
}

}
