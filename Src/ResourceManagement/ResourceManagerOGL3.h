#ifndef PGEINTERNAL_RESOURCEMANAGER_OGL3
#define PGEINTERNAL_RESOURCEMANAGER_OGL3

#include <PGE/ResourceManagement/ResourceManager.h>

namespace PGE {

class Graphics;
class GraphicsOGL3;

class ResourceManagerOGL3 : public ResourceManager {
    private:
        GraphicsOGL3* graphics;

    public:
        ResourceManagerOGL3(Graphics* gfx);
        ~ResourceManagerOGL3();
};

}

#endif // PGEINTERNAL_RESOURCEMANAGER_OGL3
