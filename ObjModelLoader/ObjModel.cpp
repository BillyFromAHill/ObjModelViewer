#include "ObjModel.h"
#include <fstream>
#include <sstream>
#include <QString>
#include <limits>
using namespace std;

ObjModel::ObjModel() : m_modelMatrix(1.0)
{
}

void ObjModel::loadFromFile(wstring fileName, wstring textureName)
{
    m_bounds[0] = m_bounds[2] = m_bounds[4] = numeric_limits<double>::max();
    m_bounds[1] = m_bounds[3] = m_bounds[5] = numeric_limits<double>::min();

    ifstream in(fileName, ios::in);
    if (!in)
    {
        throw wstring(L"Can't open file ") + fileName;
    }

    if (!textureName.empty())
    {
        m_texture = make_shared<QImage>(QString::fromStdWString(textureName));
    }

    if (m_texture->isNull())
    {
        m_texture = make_shared<QImage>(200, 200, QImage::Format_RGBA8888);
        m_texture->fill(Qt::gray);
    }

    std::vector<glm::vec4> vertices;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> textureCoords;
    std::vector<glm::u32vec3> elements;

    m_vertexSize = 9;

    string line;
    while (getline(in, line))
    {
        if (line.substr(0, 2) == "v ")
        {
            istringstream s(line.substr(2));

            glm::vec4 v;
            s >> v.x;
            s >> v.y;
            s >> v.z;
            v.w = 1.0f;

            if (v.x < m_bounds[0]) m_bounds[0] = v.x;
            if (v.x > m_bounds[1]) m_bounds[1] = v.x;

            if (v.y < m_bounds[2]) m_bounds[2] = v.y;
            if (v.y > m_bounds[3]) m_bounds[3] = v.y;

            if (v.z < m_bounds[4]) m_bounds[4] = v.z;
            if (v.z > m_bounds[5]) m_bounds[5] = v.z;
            vertices.push_back(v);
        }
        else if (line.substr(0, 2) == "vn")
        {
            istringstream s(line.substr(2));

            glm::vec3 v;
            s >> v.x;
            s >> v.y;
            s >> v.z;

            normals.push_back(v);
        }
        else if (line.substr(0, 2) == "vt")
        {
            istringstream s(line.substr(2));

            glm::vec2 v;
            s >> v.x;
            s >> v.y;

            textureCoords.push_back(v);
        }
        else if (line.substr(0, 2) == "f ")
        {
            istringstream s(line.substr(2));
            char divisor;
            while (!s.eof())
            {
                glm::u32vec3 vertParam;

                s >> vertParam.x;
                s >> divisor;
                s >> vertParam.y;
                s >> divisor;
                s >> vertParam.z;

                vertParam--;
                elements.push_back(vertParam);
            }
        }
    }

    m_objBuffer.clear();
    m_objBuffer.reserve(elements.size() * m_vertexSize);

    for (glm::u32vec3& verticeIndex : elements)
    {
        glm::vec4& vertice = vertices[verticeIndex.x];
        m_objBuffer.push_back(vertice.x);
        m_objBuffer.push_back(vertice.y);
        m_objBuffer.push_back(vertice.z);
        m_objBuffer.push_back(vertice.w);

        glm::vec3& normal = normals[verticeIndex.z];
        m_objBuffer.push_back(normal.x);
        m_objBuffer.push_back(normal.y);
        m_objBuffer.push_back(normal.z);

        glm::vec2& textureCoord = textureCoords[verticeIndex.y];
        m_objBuffer.push_back(textureCoord.x);
        m_objBuffer.push_back(textureCoord.y);
    }
}

const shared_ptr<QImage> ObjModel::getTextureImage() const
{
    return m_texture;
}

const double* ObjModel::getBounds() const
{
    return m_bounds;
}

int ObjModel::getVertexSize() const
{
    return m_vertexSize;
}

const glm::mat4x4& ObjModel::getModelMatrix() const
{
    return m_modelMatrix;
}

const std::vector<float>& ObjModel::getObjectBuffer() const
{
    return m_objBuffer;
}

ObjModel::~ObjModel()
{
}
