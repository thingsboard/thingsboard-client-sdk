/*
  Vector.h - Library API for sending data to the ThingsBoard
  Based on PubSub MQTT library.
  Created by Olender M. Oct 2018.
  Released into the public domain.
*/
#ifndef Vector_h
#define Vector_h

// Local include
#include "Configuration.h"

#if !THINGSBOARD_ENABLE_STL

/// @brief Replacement data container for boards that do not support the C++ STL.
/// @tparam T Type of the underlying data the list should point too.
template <typename T>
class Vector {
  public:
    /// @brief Constructor
    Vector(void) :
        m_elements(nullptr),
        m_capacity(0U),
        m_size(0U)
    {
        // Nothing to do.
    }

    /// @brief Destructor
    ~Vector() {
        delete[] m_elements;
        m_elements = nullptr;
    }

    /// @brief Returns whether there are still any element in the underlying data container
    /// @return Whether the underlying data container is empty or not
    bool empty() const {
        return m_size == 0U;
    }

    /// @brief Gets the current amount of elements in the underlying data container
    /// @return The amount of items currently in the underlying data container
    const size_t& size() const {
        return m_size;
    }

    /// @brief Gets the maximum amount of elements that can currently be stored in the underlying data container
    /// @return The maximum amount of items that can currently be stored in the underlying data container
    const size_t& capacity() const {
        return m_capacity;
    }
    
    /// @brief Returns a pointer to the first element of the vector
    /// @return Pointer to the first element of the vector
    T* begin() {
        return m_elements;
    }

    /// @brief Returns the last element of the vector
    /// @return Reference to the last element of the vector
    T& back() {
        if (m_size == 0U) {
          abort();
        }
        return m_elements[m_size - 1U];
    }

    /// @brief Returns a pointer to one-past-the-end element of the vector
    /// @return Pointer to one-past-the-end element of the vector
    T* end() {
        return m_elements + m_size;
    }

    /// @brief Returns a constant pointer to the first element of the vector
    /// @return Constant pointer to the first element of the vector
    const T* cbegin() const {
        return m_elements;
    }

    /// @brief Returns a constant pointer to one-past-the-end element of the vector
    /// @return Constant pointer to one-past-the-end element of the vector
    const T* cend() const {
        return m_elements + m_size;
    }

    /// @brief Reserves the given capacity for the underlying data container
    /// @param capacity Capacity that should be reserved in the underlying data container
    void reserve(const size_t& capacity) {
        if (capacity > m_capacity) {
            T* newElements = new T[m_capacity];
            if (m_elements != nullptr) {
                memcpy(newElements, m_elements, m_size * sizeof(T));
                delete[] m_elements;
            }
            m_elements = newElements;
        }
    }

    /// @brief Inserts the given element at the end of the underlying data container
    /// @param element Element that should be inserted at the end
    void push_back(T element) {
        if (m_size == m_capacity) {
            m_capacity = (m_capacity == 0) ? 1 : 2 * m_capacity;
            T* newElements = new T[m_capacity];
            if (m_elements != nullptr) {
                memcpy(newElements, m_elements, m_size * sizeof(T));
                delete[] m_elements;
            }
            m_elements = newElements;
        }
        m_elements[m_size] = element;
        m_size++;
    }

    /// @brief Removes the element at the given index, has to move all element one to the left if the index is not at the end of the array
    /// @param index Index the element should be removed at from the underlying data container
    void erase(const size_t& index) {
        if (index < m_size) {
            // Move all elements after the index one position to the left
            for (size_t i = index; i < m_size - 1; i++) {
                m_elements[i] = m_elements[i + 1];
            }
            // Decrease the size of the vector
            m_size--;
        }
    }

    /// @brief Method to access an element at a given index,
    /// ensures the device crashes if we attempted to access in an invalid location
    /// @param index Index we want to get the corresponding element for
    T& at(const size_t& index) {
        if (index >= m_size) {
          abort();
        }
        return m_elements[index];
    }

    /// @brief Bracket operator to access an element at a given index
    /// @param index Index we want to get the corresponding element for
    T& operator[](const size_t& index) {
        return m_elements[index];
    }

    /// @brief Bracket operator to access an element at a given index
    /// @param index Index we want to get the corresponding element for
    const T& operator[](const size_t& index) const {
        return m_elements[index];
    }

    /// @brief Clears the given underlying data container.
    /// Simply sets the underlying size to 0, data will only be cleared in the destructor
    void clear() {
        m_size = 0;
    }

private:
    T* m_elements;
    size_t m_capacity;
    size_t m_size;
};

#endif // !THINGSBOARD_ENABLE_STL

#endif // Vector_h
