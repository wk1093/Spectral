#pragma once

#include <stdlib.h>
#include <stdio.h>

template<typename T=double>
struct Vector2 {
    T x, y;
};
typedef Vector2<float> Vector2f;
typedef Vector2<double> Vector2d;
typedef Vector2<int> Vector2i;
template<typename T=double>
struct Vector3 {
    T x, y, z;
};
typedef Vector3<float> Vector3f;
typedef Vector3<double> Vector3d;
typedef Vector3<int> Vector3i;
template<typename T=double>
struct Vector4 {
    T x, y, z, w;
};
typedef Vector4<float> Vector4f;
typedef Vector4<double> Vector4d;
typedef Vector4<int> Vector4i;
template<typename T=double>
struct Vector {
    T* data;
    int size{};
private:
    int capacity{};

public:
    Vector() {
        capacity = 4;
        size = 0;
        data = (T*)malloc(sizeof(T)*capacity);
    }

    Vector(T* data, int size) {
        this->data = data;
        this->size = size;
        capacity = size;
    }



private:
    void expand() {
        capacity *= 2;
        data = (T*)realloc(data, sizeof(T)*capacity);
    }
public:
    void reserve(int n) {
        if (n > capacity) {
            capacity = n;
            data = (T*)realloc(data, sizeof(T)*capacity);
        }
    }
    void push_back(T value) {
        if (size == capacity) {
            expand();
        }
        data[size] = value;
        size++;
    }
    T pop_back() {
        size--;
        return data[size];
    }
    T& operator[](int i) {
        return data[i];
    }
    T& at(int i) {
        if (i >= size) { // TODO: Error system for engine errors (so later we can have a message box or something)
            fprintf(stderr, "Error: index %d out of bounds for vector of size %d\n", i, size);
            exit(1);
        }
        return data[i];
    }

    void clear() {
        size = 0;
    }

    void free() {
        ::free(data);
    }

    ~Vector() {
        ::free(data);
    }
};