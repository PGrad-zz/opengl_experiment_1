#include <assimp/postprocess.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "Model.h"
#include <math.h>
unsigned int TextureFromFile(const char *path, const std::string &directory);
void Model::Draw(Shader shader) {
  for(unsigned int i = 0; i < meshes.size(); ++i)
    meshes[i].Draw(shader);
}

void Model::loadModel(std::string path) {
  aiScene const * scene = importer.get()->ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals);
  if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    throw(std::string("ERROR::ASSIMP::") + importer.get()->GetErrorString() + "\n");
  directory = path.substr(0, path.find_last_of('/'));
  processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode *node, const aiScene *scene) {
    // process all the node's meshes (if any)
    for(unsigned int i = 0; i < node->mNumMeshes; ++i)
    {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]]; 
        meshes.push_back(processMesh(mesh, scene));
    }
    // then do the same for each of its children
    for(unsigned int i = 0; i < node->mNumChildren; ++i)
        processNode(node->mChildren[i], scene);
}

#define GETCOORD(x) (mesh->mVertices[i].x)
#define GETNORM(x) (mesh->mNormals[i].x)
#define GETTEX(x) (mesh->mTextureCoords[0][i].x)
void loadVerticesWTex(aiMesh * mesh, std::vector<Vertex> & vertices) {
  Vertex vertex;
  for(unsigned int i = 0; i < mesh->mNumVertices; ++i) {
    vertex.Position = glm::vec3(GETCOORD(x), GETCOORD(y), GETCOORD(z));
    vertex.Normal = glm::vec3(GETNORM(x), GETNORM(y), GETNORM(z));
    vertex.TexCoords = glm::vec2(GETTEX(x), GETTEX(y));
    vertices.push_back(vertex);
  }
}

void loadVerticesWoTex(aiMesh * mesh, std::vector<Vertex> & vertices) {
  Vertex vertex;
  for(unsigned int i = 0; i < mesh->mNumVertices; ++i) {
    vertex.Position = glm::vec3(GETCOORD(x), GETCOORD(y), GETCOORD(z));
    vertex.Normal = glm::vec3(GETNORM(x), GETNORM(y), GETNORM(z));
    vertex.TexCoords = glm::vec2(0);
    vertices.push_back(vertex);
  }
}

Mesh Model::processMesh(aiMesh *mesh, const aiScene *scene) {
  std::vector<Vertex> vertices;
  std::vector<unsigned int> indices;
  std::vector<Texture> textures;

  if(mesh->mTextureCoords[0])
    loadVerticesWTex(mesh, vertices);
  else
    loadVerticesWoTex(mesh, vertices);
  aiFace face;
  for(unsigned int i = 0; i < mesh->mNumFaces; ++i) {
    face = mesh->mFaces[i];
    for(unsigned int j = 0; j < face.mNumIndices; ++j)
      indices.push_back(face.mIndices[j]);
  }
  if(mesh->mMaterialIndex >= 0) {
    aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
    std::vector<Texture> diffuseMaps = loadMaterialTextures(material, 
                                        aiTextureType_DIFFUSE, "texture_diffuse");
    textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
    std::vector<Texture> specularMaps = loadMaterialTextures(material, 
                                        aiTextureType_SPECULAR, "texture_specular");
    textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
  }  

  return Mesh(vertices, indices, textures);
}

std::vector<Texture> Model::loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName)
{
    std::vector<Texture> textures;
    for(unsigned int i = 0; i < mat->GetTextureCount(type); i++)
    {
        aiString str;
        std::string path;
        mat->GetTexture(type, i, &str);
        path = std::string(str.C_Str());
        if(textures_loaded.count(path) != 0) {
          textures.push_back(textures_loaded[path]);
          continue;
        }
        Texture texture;
        texture.id = TextureFromFile(str.C_Str(), directory);
        texture.type = typeName;
        texture.path = path;
        textures_loaded[texture.path] = texture;
        textures.push_back(texture);
    }
    return textures;
}

bool powerOfTwo(int num) {
  //Avoid rounding error for numbers greater than 2^23 (what can fit in a mantissa)
  return floor(log2((double) num)) == ceil(log2((double) num));
}

unsigned int TextureFromFile(const char *path, const std::string &directory) {
  std::string filename = directory + '/' + std::string(path);

  unsigned int textureID;
  glGenTextures(1, &textureID);
  
  int width, height, nrComponents;
  unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
  if(data) {
    GLenum format;
    switch(nrComponents) {
      case 1:
        format = GL_RED;
        break;
      case 3:
        format = GL_RGB;
        break;
      case 4:
        format = GL_RGBA;
        break;
    }
    
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);

    if(powerOfTwo(width) && powerOfTwo(height)) {
      glGenerateMipmap(GL_TEXTURE_2D);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    } else {
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    }

    stbi_image_free(data);
  }
  return textureID;
}
