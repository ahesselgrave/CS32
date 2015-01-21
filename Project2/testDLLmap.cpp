#include "Map.h"
#include <string>
#include <iostream>
#include <cassert>
#include <type_traits>
using namespace std;

#define CHECKTYPE(f, t) { auto p = (t)(f); (void)p; }

static_assert(std::is_default_constructible<Map>::value,
        "Map must be default-constructible.");
static_assert(std::is_copy_constructible<Map>::value,
        "Map must be copy-constructible.");

void ThisFunctionWillNeverBeCalled()
{
    CHECKTYPE(&Map::operator=,      Map& (Map::*)(const Map&));
    CHECKTYPE(&Map::empty,          bool (Map::*)() const);
    CHECKTYPE(&Map::size,           int  (Map::*)() const);
    CHECKTYPE(&Map::insert,         bool (Map::*)(const KeyType&, const ValueType&));
    CHECKTYPE(&Map::update,         bool (Map::*)(const KeyType&, const ValueType&));
    CHECKTYPE(&Map::insertOrUpdate, bool (Map::*)(const KeyType&, const ValueType&));
    CHECKTYPE(&Map::erase,          bool (Map::*)(const KeyType&));
    CHECKTYPE(&Map::contains,       bool (Map::*)(const KeyType&) const);
    CHECKTYPE(&Map::get,            bool (Map::*)(const KeyType&, ValueType&) const);
    CHECKTYPE(&Map::get,            bool (Map::*)(int, KeyType&, ValueType&) const);
    CHECKTYPE(&Map::swap,           void (Map::*)(Map&));
    
    CHECKTYPE(combine,  bool (*)(const Map&, const Map&, Map&));
    CHECKTYPE(subtract, void (*)(const Map&, const Map&, Map&));
}

void test()
{
    Map m;
    Map ggggg = m;
    assert(m.empty() && ggggg.empty());
    assert(!m.erase("") && !ggggg.erase(""));
    assert(m.insert("Fred", 123));
    assert(m.insert("Ethel", 456));
    assert(m.insert("Lucy", 789));
    assert(m.insert("", 0));
    ggggg = m;
    assert(m.size() == ggggg.size() && ggggg.size() == 4);

    double d, q;
    string s, t;
    assert(m.get("Lucy", d) == ggggg.get("Lucy", q));
    assert(!m.get("Pingguo", d));
    assert(m.get(1, s, d) && ggggg.get(1, t, q) && s == t && d == q);
    assert(!m.update("Shepard", 5) && m.insertOrUpdate("", 20) && m.get("", d) && d == 20);
    assert(m.erase("") && !ggggg.erase(" "));
    m.swap(ggggg);
    assert(m.get(s, d) && !ggggg.get("", q));

    //Test nonmember algorithms
    Map m1;
    m1.insert("Fred", 123);
    m1.insert("Ethel", 456);
    m1.insert("Lucy", 789);
    Map m2;
    m2.insert("Lucy", 789);
    m2.insert("Ricky", 321);
    Map result;
    assert(combine(m1, m2, result));
    result.dump();
    assert(m2.update("Lucy", 654));
    assert(!combine(m1, m2, result));
    result.dump();
    cout << endl << endl; 

    Map m3;
    m3.insert("Fred", 123);
    m3.insert("Ethel", 456);
    m3.insert("Lucy", 789);
    Map m4;
    m4.insert("Lucy", 789);
    m4.insert("Ricky", 321);
    m4.insert("Ethel", 654);
    Map result2;
    subtract(m3, m4, result2);
    result2.dump();

}

int main()
{
    test();
    cout << "Passed all tests" << endl;
}

