#ifndef POOL_H
#define POOL_H

#include <base/CCVector.h>

#include <algorithm>

template<typename T>
class Pool {
public:
    T *fetch() {
        auto found = std::find_if(_objects.begin(), _objects.end(),
                                  [&](T *p) { return p->getParent() == nullptr; });

        if (found == _objects.end()) {
            T *newObject = T::create();
            _objects.pushBack(newObject);

            return newObject;
        } else {
            return *found;
        }
    }

    void reserve(unsigned int amount) {
        for (int i = 0; i < amount; i++)
            _objects.pushBack(T::create());
    }

    void clear() {
        _objects.clear();
    }

    ssize_t getSize() const {
        return _objects.size();
    }

private:
    cocos2d::Vector<T *> _objects;
};

#endif