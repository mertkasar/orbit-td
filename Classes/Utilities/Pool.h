#ifndef POOL_H
#define POOL_H

#include <base/CCVector.h>

#include <algorithm>

template<typename T>
class Pool {
private:
    cocos2d::Vector<T *> mObjects;

public:
    T *fetch() {
        auto found = std::find_if(mObjects.begin(), mObjects.end(),
                                  [&](T *p) { return p->getParent() == nullptr; });

        if (found == mObjects.end()) {
            T *newObject = T::create();
            mObjects.pushBack(newObject);

            return newObject;
        } else {
            return *found;
        }
    }

    void reserve(unsigned int pAmount) {
        for (int i = 0; i < pAmount; i++)
            mObjects.pushBack(T::create());
    }

    void clear() {
        mObjects.clear();
    }

    ssize_t getSize() const {
        return mObjects.size();
    }
};

#endif