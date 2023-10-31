#ifndef Array_h
#define Array_h

// Local include.
#include "Helper.h"

// Library includes.
#include <assert.h>


/// @brief Replacement data container for boards that do not support the C++ STL and therefore do not have the std::array class.
/// @tparam T Type of the underlying data the list should point too.
/// @tparam Capacity Amount of elements that can be saved into the underlying data structure allows to wrap a simple c-array and allocate it on the stack.
template <typename T, size_t Capacity>
class Array {
  public:
    /// @brief Constructor
    inline Array(void) :
        m_elements(),
        m_size(0U)
    {
        // Nothing to do
    }

    /// @brief Constructor that allows compatibility with std::vector, simply forwards call to internal insert method
    /// @param first_itr Beginning of the elements we want to copy into our underlying data container
    /// @param last_itr One past the end of the elements we want to copy into our underlying data container
    inline Array(const T* first_itr, const T* last_itr) :
        m_elements(),
        m_size(0U)
    {
        insert(nullptr, first_itr, last_itr);
    }

    /// @brief Method that allows compatibility with std::vector, simply forwards call to internal insert method
    /// @param first_itr Beginning of the elements we want to copy into our underlying data container
    /// @param last_itr One past the end of the elements we want to copy into our underlying data container
    inline void assign(const T* first_itr, const T* last_itr) {
        insert(nullptr, first_itr, last_itr);
    }

    /// @brief Returns whether there are still any element in the underlying data container
    /// @return Whether the underlying data container is empty or not
    inline bool empty() const {
        return m_size == 0U;
    }

    /// @brief Gets the current amount of elements in the underlying data container
    /// @return The amount of items currently in the underlying data container
    inline const size_t& size() const {
        return m_size;
    }

    /// @brief Gets the maximum amount of elements that can currently be stored in the underlying data container
    /// @return The maximum amount of items that can currently be stored in the underlying data container
    inline size_t capacity() const {
        return Capacity;
    }
    
    /// @brief Returns a constant pointer to the first element of the vector
    /// @return Constant pointer to the first element of the vector
    inline const T* begin() const {
        return m_elements;
    }

    /// @brief Returns the last element of the vector
    /// @return Reference to the last element of the vector
    inline T& back() {
        assert(m_size != 0U);
        return m_elements[m_size - 1U];
    }

    /// @brief Returns a constant pointer to one-past-the-end element of the vector
    /// @return Constant pointer to one-past-the-end element of the vector
    inline const T* end() const {
        return m_elements + m_size;
    }

    /// @brief Inserts the given element at the end of the underlying data container,
    /// If the interal data structure is full already then this method will assert and stop the application.
    /// Because if we do not we could cause an out of bounds write, which could possibly overwrite other memory.
    /// Causing hard to debug issues, therefore this behaviour is not allowed in the first place
    /// @param element Element that should be inserted at the end
    inline void push_back(const T& element) {
        assert(m_size < Capacity);
        m_elements[m_size] = element;
        m_size++;
    }

    /// @brief Inserts all element from the given start to the given end iterator into the underlying data container.
    /// Simply calls push_back on each element, meaning if the initally allocated size if not big enough to hold all elements,
    /// then this method will simply not insert those elements instead
    /// @param position_itr Attribute is not used and can be left as nullptr, simply there to keep compatibility with std::vector insert method
    /// @param first_itr Beginning of the elements we want to copy into our underlying data container
    /// @param last_itr One past the end of the elements we want to copy into our underlying data container
    inline void insert(const T* position_itr, const T* first_itr, const T* last_itr) {
        while (first_itr < last_itr) {
            push_back(*first_itr);
            first_itr++;
        }
    }

    /// @brief Removes the element at the given iterator, has to move all element one to the left if the index is not at the end of the array
    /// @param iterator Iterator the element should be removed at from the underlying data container
    inline void erase(const T* iterator) {
        const size_t index = Helper::distance(begin(), iterator);
        erase(index);
    }

    /// @brief Removes the element at the given index, has to move all element one to the left if the index is not at the end of the array
    /// @param index Index the element should be removed at from the underlying data container
    inline void erase(const size_t& index) {
        // Check if the given index is bigger or equal than the actual amount of elements if it is we can not erase that element because it does not exist
        if (index < m_size) {
            // Move all elements after the index one position to the left
            for (size_t i = index; i < m_size - 1; i++) {
                m_elements[i] = m_elements[i + 1];
            }
            // Decrease the size of the vector to remove the last element, because either it was moved one index to the left or was the element we wanted to delete
            m_size--;
        }
    }

    /// @brief Method to access an element at a given index,
    /// ensures the device crashes if we attempted to access in an invalid location
    /// @param index Index we want to get the corresponding element for
    inline T& at(const size_t& index) {
        assert(index < m_size);
        return m_elements[index];
    }

    /// @brief Bracket operator to access an element at a given index.
    /// Does not do any bounds checks, meaning the access is more efficient but it is possible to read out of bounds data
    /// @param index Index we want to get the corresponding element for
    inline T& operator[](const size_t& index) {
        return m_elements[index];
    }

    /// @brief Bracket operator to access an element at a given index.
    /// Does not do any bounds checks, meaning the access is more efficient but it is possible to read out of bounds data
    /// @param index Index we want to get the corresponding element for
    inline const T& operator[](const size_t& index) const {
        return m_elements[index];
    }

    /// @brief Clears the given underlying data container.
    /// Simply sets the underlying size to 0, data will only be cleared in the destructor
    inline void clear() {
        m_size = 0;
    }

  private:
    T m_elements[Capacity]; // Underlying c-array holding our data
    size_t m_size;          // Used size that shows how many elements we entered
};

#endif // Array_h
