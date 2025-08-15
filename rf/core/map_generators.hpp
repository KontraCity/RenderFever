#pragma once

#include <array>

#include <frozen/unordered_map.h>

namespace rf {

template <typename Array, typename ValueExtractor>
constexpr bool HasDuplicates(const Array& array, ValueExtractor extractor) {
    for (size_t i = 0; i < array.size(); ++i)
        for (size_t j = i + 1; j < array.size(); ++j)
            if (extractor(array[i]) == extractor(array[j]))
                return true;
    return false;
}

template <auto& records, typename Value, typename Entry, typename ValueExtractor>
constexpr auto GenerateValueToEntryMap(ValueExtractor extractor) {
    static_assert(
        !HasDuplicates(records, extractor),
        "Record arrays can only contain unique values!"
    );

    std::array<std::pair<Value, Entry>, records.size()> array = {};
    for (size_t index = 0; index < records.size(); ++index)
        array[index] = { extractor(records[index]), records[index] };
    return frozen::make_unordered_map(array);
}

} // namespace rf
