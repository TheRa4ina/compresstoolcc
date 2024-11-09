#pragma once 
#include <unordered_map>
#include <iostream>
#include <array>
#include <cstdint>
typedef uint8_t Byte;
typedef uint32_t FreqType;

// TODO change unordered map to array
typedef std::array<FreqType,256> CharFreqMap;
/// <summary>
/// Get frequency of every char in istream <para/>
/// Returns pair with string instead of char, because more convinient to use in buildTree functon
/// </summary>
/// <returns>frequency map of every char.</returns>
CharFreqMap getFrequency(std::istream& is);