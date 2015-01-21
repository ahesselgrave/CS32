#ifndef MAP_LINKEDLIST
#define MAP_LINKEDLIST

#include <string>

typedef std::string KeyType;
typedef double ValueType;

class Map
{
	public:
		Map();
		~Map();
		Map(const Map& src);
		Map& operator=(const Map& src);

		//Accessor functions
		bool empty() const;
		int size() const;
		bool contains(const KeyType& key) const;
		bool get(const KeyType& key, ValueType& value) const;
		bool get(int i, KeyType& key, ValueType& value) const;
		void dump() const;

		//Mutator functions		
		bool insert(const KeyType& key, const ValueType& value);
		bool update(const KeyType& key, const ValueType& value);
		bool insertOrUpdate(const KeyType& key, const ValueType& value);
		bool erase(const KeyType& key);
		void swap(Map& other);

	private:
		int m_size;
		struct Node
		{
			Node* prev;
			Node* next;
			KeyType key;
			ValueType value;
		};
		Node* head;
		Node* tail;
};

bool combine(const Map& m1, const Map& m2, Map& result);
void subtract(const Map& m1, const Map& m2, Map& result);

#endif