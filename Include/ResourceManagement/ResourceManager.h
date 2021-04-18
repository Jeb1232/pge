#ifndef PGE_RESOURCEMANAGER_H_INCLUDED
#define PGE_RESOURCEMANAGER_H_INCLUDED

#include <vector>

#include "Resource.h"

namespace PGE {

class ResourceBase;
class ResourceManager {
    private:
        std::vector<ResourceBase*> resources;

#ifdef DEBUG
        // We need this, because the C++ specification states that reserve on a vector might increase the capacity beyond the specified size.
        int size;
#endif

    public:
        ResourceManager(int sz);
        virtual ~ResourceManager();

        template <class T, class... Args>
        ResourceReference<decltype(T::resource)> addNewResource(Args... args) {
#ifdef DEBUG
            __ASSERT(size > resources.size(), "Tried to add resource to full ResourceManager");
#endif
            T* res = new T(args...);
            resources.push_back(res);
            return ResourceReference<decltype(T::resource)>(*res);
        }

        template <class T>
        void deleteResource(T internalResource) {
            // Static assertion is likely not possible here due to ResourceReference's template.
            for (auto it = resources.end(); it > resources.begin();) {
                it--;
                Resource<T>* specifiedResource = dynamic_cast<Resource<T>*>(*it);
                if (specifiedResource != nullptr && (*specifiedResource)() == internalResource) {
                    delete specifiedResource;
                    resources.erase(it);
                    return;
                }
            }
        }

        template <class T>
        void deleteResourcefromReference(ResourceReference<T> reference) {
            if (!reference.isHoldingResource()) {
                return;
            }

            deleteResource(reference());
        }

        void increaseSize(int count);
};

}

#endif // PGE_RESOURCEMANAGER_H_INCLUDED