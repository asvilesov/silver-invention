#pragma once
#include <string>
#include <utility>
#include <functional>
#include <iostream>

template <typename ValueType, typename HashFuncType = std::hash<std::string>>
class HashTable
{
public:
	// Constructs the hash table with the given capacity (size of underlying array)
	HashTable(size_t capacity)
		:mTableData(new std::pair<std::string, ValueType>[capacity])
		,mCapacity(capacity)
		,mSize(0)
	{ }

	HashTable(const HashTable& p2)
		:mTableData(new std::pair<std::string, ValueType>[p2.Capacity()])
		,mCapacity(p2.Capacity())
		,mSize(p2.Size())
	{
		for (auto i = 0; i < mCapacity; ++i) {
			mTableData[i].first = p2.mTableData[i].first;
			mTableData[i].second = p2.mTableData[i].second;
		}
	}

	HashTable& operator = (const HashTable& rhs) {
		if (&rhs != this) {
			mCapacity = rhs.Capacity();
			mSize = rhs.Size();
			delete[]mTableData;

			mTableData = new std::pair<std::string, ValueType>[rhs.Capacity()];
			for (auto i = 0; i < mCapacity; ++i) {
				mTableData[i].first = rhs.mTableData[i].first;
				mTableData[i].second = rhs.mTableData[i].second;
			}
		}
		return *this;
	}

	HashTable(HashTable&& rhs)
		:mCapacity(std::move(rhs.mCapacity))
		, mSize(std::move(rhs.mSize))
		, mTableData(std::move(rhs.mTableData))
	{
		rhs.mTableData = nullptr;
		rhs.mCapacity = 0;
		rhs.mSize = 0;
	}

	//Move Assignement Operator
	HashTable& operator = (HashTable&& rhs) {
		if (&rhs != this) {
			delete[]mTableData;

			mCapacity = std::move(rhs.mCapacity);
			mSize = std::move(rhs.mSize);
			mTableData = std::move(rhs.mTableData);

			rhs.mTableData = nullptr;
			rhs.mCapacity = 0;
			rhs.mSize = 0;
		}
		return *this;
	}

	~HashTable() {
		delete []mTableData;
		mCapacity = 0;
		mSize = 0;
		mTableData = nullptr;
	}

	// Returns overall capacity of hash table (size of underlying array)
	size_t Capacity() const
	{
		return mCapacity;
	}

	// Returns the number of elements currently inserted in the hash table
	size_t Size() const
	{
		return mSize;
	}

	// Tries to insert (key, value) into hash table
	// Returns true if insert succeeds, false if not
	bool Insert(const std::string& key, const ValueType& value)
	{
		if (mSize == mCapacity) {
			return false;
		}
		size_t index = mHashFunc(key) % mCapacity;
		for (auto i = 0; i < mCapacity; ++i) {
			if (mTableData[(index+i)%mCapacity].first.empty()) {
				mTableData[(index + i) % mCapacity].first = key;
				mTableData[(index + i) % mCapacity].second = value;
				break;
			}
		}
		++mSize;
		return true;
	}

	//overload insert if adding value not needed at first
	bool Insert(const std::string& key)
	{
		if (mSize == mCapacity) {
			return false;
		}
		size_t index = mHashFunc(key) % mCapacity;
		for (auto i = 0; i < mCapacity; ++i) {
			if (mTableData[(index + i) % mCapacity].first.empty()) {
				mTableData[(index + i) % mCapacity].first = key;
				break;
			}
		}
		++mSize;
		return true;
	}

	// Returns pointer to value for given key, or nullptr
	// if not in hash table
	ValueType* Find(const std::string& key) const
	{
		size_t index = mHashFunc(key) % mCapacity;
		for (auto i = 0; i < mCapacity; ++i) {
			if (mTableData[(index + i) % mCapacity].first == key) {
				return &mTableData[(index + i) % mCapacity].second;
			}
			if (mTableData[(index + i) % mCapacity].first.empty()) {
				return nullptr;
			}
		}
		return nullptr;
	}

	// Executes provided lambda expression on each element inserted into the table
	void ForEach(std::function<void(const std::string& key, ValueType& value)> func)
	{
		for (auto i = 0; i < mCapacity; ++i) {
			if (!mTableData[(i) % mCapacity].first.empty()) {
				func(mTableData[(i) % mCapacity].first, mTableData[(i) % mCapacity].second);
			}
		}
	}

	// Gets a const pointer to underlying data (for testing)
	const std::pair<std::string, ValueType>* Data() const
	{
		return mTableData;
	}
	
private:
	// Underlying hash table array
	std::pair<std::string, ValueType>* mTableData;
	// Allocated capacity of hash table array
	size_t mCapacity;
	// Number of elements in HashTable
	size_t mSize;
	// Hash function
	HashFuncType mHashFunc;
};
