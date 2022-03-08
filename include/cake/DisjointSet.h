#pragma once

#include <limits>
#include <unordered_map>
#include <vector>

namespace cake {
/**
 * Disjoint Set data structure. Maintains a collection of disjoint sets, and
 * provides methods for querying ownership of elements to the sets, and
 * merging them.
 */
template <typename TElement> class DisjointSet {
  public:
    using SetId = size_t;

    using element_type = TElement;

    static const size_t NoSet = std::numeric_limits<size_t>::max();

  public:
    SetId add(const element_type &element);

    SetId find(const element_type &element) const;

    bool join(const element_type &element1, const element_type &element2);

    bool join(SetId set1, SetId set2);

  private:
    std::vector<size_t> m_sizes;
    std::vector<SetId> m_elements;
    std::unordered_map<element_type, size_t> m_index;
};

/**
 * Creates a new disjoint set with a single element. If the element
 * is already present in any of the sets, no new set is created.
 *
 * @param element The element of the new set.
 * @return The id of the set containing the element.
 */
template <typename TElement>
typename DisjointSet<TElement>::SetId DisjointSet<TElement>::add(const element_type &element) {
    SetId id = find(element);

    if (id)
        while (id != m_elements[id]) {
            m_elements[id] = m_elements[m_elements[id]];
            id = m_elements[id];
        }

    if (id != NoSet)
        return id;

    id = m_elements.size();
    m_index.insert({element, id});
    m_sizes.push_back(1);
    m_elements.push_back(id);

    return id;
}

/**
 * Finds the ownership a an element.  and path
 * halving
 *
 * @param element The element.
 * @return If the element is present in some set, it is the id of such set,
 *         otherwise it is NoSet.
 */
template <typename TElement>
typename DisjointSet<TElement>::SetId
DisjointSet<TElement>::find(const element_type &element) const {
    const auto it = m_index.find(element);

    if (it == m_index.end())
        return NoSet;

    return it->second;
}

/**
 * Merges two sets. The sets are identified by two distinct elements.
 * If the to elements belong to different sets, those sets are merged.
 * Otherwise no merge operation takes place.
 * If any of the elements belongs to no set yet, the element is added
 * to a newly created set.
 *
 * @param element1 The first element.
 * @param element2. The second element.
 * @return true, if a merged operation took place; false, otherwise.
 */
template <typename TElement>
bool DisjointSet<TElement>::join(const element_type &element1, const element_type &element2) {
    const auto set1 = add(element1);
    const auto set2 = add(element2);

    return join(set1, set2);
}

/**
 * Merges two sets. The sets are identified by two distinct ids.
 * If the ids identify two distinct disjoint sets, then a merge operation
 * take place. Otherwise, no merge operation takes place.
 * The implementation is using the weighted union heuristic.
 *
 * @param element1 The first element.
 * @param element2. The second element.
 * @return true, if a merged operation took place; false, otherwise.
 */
template <typename TElement> bool DisjointSet<TElement>::join(SetId set1, SetId set2) {
    if (set1 == NoSet || set2 == NoSet || set1 == set2 || set1 > m_elements.size() ||
        set2 > m_elements.size() || m_sizes[set1] == 0 || m_sizes[set2] == 0)
        return false;

    const SetId smallSet = m_sizes[set1] < m_sizes[set2] ? set1 : set2;
    const SetId largeSet = smallSet == set1 ? set2 : set1;

    m_elements[smallSet] = largeSet;
    m_sizes[largeSet] += m_sizes[smallSet];
    m_sizes[smallSet] = 0;

    return true;
}
} // namespace cake