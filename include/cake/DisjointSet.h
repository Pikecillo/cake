#pragma once

#include <iostream>

#include <limits>
#include <optional>
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
    using element_type = TElement;

    using SetHandle = size_t;

    /**
     * Creates a new disjoint set with a single element. If the element
     * is already present in any of the sets, no new set is created.
     *
     * @param element The element of the new set.
     * @return The handle of the set containing the element.
     */
    SetHandle add(const element_type &element);

    /**
     * Finds the ownership a an element.
     *
     * @param element The element.
     * @return If the element is present in some set, it is the id of such set,
     *         otherwise it is NoSet.
     */
    std::optional<SetHandle> find(const element_type &element) const;

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
    bool join(const element_type &element1, const element_type &element2);

    /**
     * Merges two sets. The sets are identified by two distinct ids.
     * If the ids identify two distinct disjoint sets, then a merge operation
     * take place. Otherwise, no merge operation takes place.
     * The implementation is using the weighted union heuristic, and path halving
     *
     * @param element1 The first element.
     * @param element2. The second element.
     * @return true, if a merged operation took place; false, otherwise.
     */
    bool join(const SetHandle &handle1, const SetHandle &handle2);

  private:
    std::vector<size_t> m_setSizes;
    mutable std::vector<SetHandle> m_ownerSetHandles;
    std::unordered_map<element_type, size_t> m_elementToIdx; /// Element to idx
};

template <typename TElement>
typename DisjointSet<TElement>::SetHandle DisjointSet<TElement>::add(const element_type &element) {
    const auto setHandle = find(element);

    if (setHandle)
        return setHandle.value();

    size_t idx = m_ownerSetHandles.size();
    m_elementToIdx.insert({element, idx});
    m_setSizes.push_back(1);

    SetHandle newSetHandle(idx);
    m_ownerSetHandles.push_back(newSetHandle);

    return newSetHandle;
}

template <typename TElement>
std::optional<typename DisjointSet<TElement>::SetHandle>
DisjointSet<TElement>::find(const element_type &element) const {
    const auto it = m_elementToIdx.find(element);

    if (it == m_elementToIdx.end())
        return {};

    size_t idx = it->second;
    while (idx != m_ownerSetHandles[idx]) {
        m_ownerSetHandles[idx] = m_ownerSetHandles[m_ownerSetHandles[idx]];
        idx = m_ownerSetHandles[idx];
    }

    return idx;
}

template <typename TElement>
bool DisjointSet<TElement>::join(const element_type &element1, const element_type &element2) {
    const auto handle1 = add(element1);
    const auto handle2 = add(element2);

    return join(handle1, handle2);
}

template <typename TElement>
bool DisjointSet<TElement>::join(const SetHandle &handle1, const SetHandle &handle2) {
    if (handle1 == handle2 || handle1 >= m_ownerSetHandles.size() ||
        handle2 >= m_ownerSetHandles.size() || m_setSizes[handle1] == 0 || m_setSizes[handle2] == 0)
        return false;

    const auto smallHandle = (m_setSizes[handle1] < m_setSizes[handle2] ? handle1 : handle2);
    const auto largeHandle = (smallHandle == handle1 ? handle2 : handle1);

    m_ownerSetHandles[smallHandle] = largeHandle;
    m_setSizes[largeHandle] += m_setSizes[smallHandle];
    m_setSizes[smallHandle] = 0;

    return true;
}
} // namespace cake