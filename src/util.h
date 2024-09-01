#pragma once 
#include <unordered_map>
#include <iostream>

typedef uint32_t FreqType;
typedef std::unordered_map<char, FreqType> CharFreqMap;
/// <summary>
/// Get frequency of every char in istream <para/>
/// Returns pair with string instead of char, because more convinient to use in buildTree functon
/// </summary>
/// <returns>frequency map of every char.</returns>
CharFreqMap getFrequency(std::istream& is);