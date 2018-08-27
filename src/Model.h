#include <assimp/Importer.hpp>
#include <iostream>
#include <memory>
#include "Shader.h"
#include "Mesh.h"
#include <map>
#include <assimp/scene.h>
class Model {
public:
    /* Functions */
    Model() = delete;
    Model(char const *path)
    {
        importer.reset(new Assimp::Importer());
        loadModel(path);
    }
    void Draw(Shader shader);
private:
    /* Model Data */
    std::map<std::string, Texture> textures_loaded;
    std::vector<Mesh> meshes;
    std::string directory;
    /*Singletons*/
    std::shared_ptr<Assimp::Importer> importer;
    /* Functions */
    void loadModel(std::string path);
    void processNode(aiNode *node, const aiScene *scene);
    Mesh processMesh(aiMesh *mesh, const aiScene *scene);
    std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);
};
