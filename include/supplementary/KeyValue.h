#pragma once

#include <utility>
#include <string>

namespace meico {
namespace supplementary {

/**
 * A helper class for key value pairings.
 * Equivalent to Java KeyValue class but simplified using std::pair.
 * @tparam K key type
 * @tparam V value type
 */
template<typename K, typename V>
class KeyValue {
private:
    K key;
    V value;

public:
    /**
     * Constructor
     * @param k the key
     * @param v the value
     */
    KeyValue(const K& k, const V& v) : key(k), value(v) {}

    /**
     * Move constructor
     * @param k the key
     * @param v the value (moved)
     */
    KeyValue(const K& k, V&& v) : key(k), value(std::move(v)) {}

    /**
     * Copy constructor
     */
    KeyValue(const KeyValue& other) = default;

    /**
     * Move constructor
     */
    KeyValue(KeyValue&& other) = default;

    /**
     * Copy assignment operator
     */
    KeyValue& operator=(const KeyValue& other) = default;

    /**
     * Move assignment operator
     */
    KeyValue& operator=(KeyValue&& other) = default;

    /**
     * Get the key
     * @return the key
     */
    const K& getKey() const { return key; }
    K& getKey() { return key; }

    /**
     * Get the value
     * @return the value
     */
    const V& getValue() const { return value; }
    V& getValue() { return value; }

    /**
     * Set the key
     * @param k the new key
     * @return the new key
     */
    K setKey(const K& k) { return key = k; }

    /**
     * Set the value
     * @param v the new value
     * @return the new value
     */
    V setValue(const V& v) { return value = v; }

    /**
     * Convert to std::pair
     * @return std::pair representation
     */
    std::pair<K, V> toPair() const { return std::make_pair(key, value); }
};

// Type aliases for common use cases
using DoubleKeyValue = KeyValue<double, double>;
using StringKeyValue = KeyValue<std::string, std::string>;
using IntKeyValue = KeyValue<int, int>;

} // namespace supplementary
} // namespace meico