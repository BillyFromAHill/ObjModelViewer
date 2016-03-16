#pragma once
#include <string>
#include <vector>
#include <memory>

#include "glm/glm.hpp"

#include <QImage>

// Оболочка для моделей obj.
class ObjModel
{
public:
    ObjModel();

    // Загружает модель и текстуру из файлов.
    // В случае, если путь к текстуре не указан или
    // отсутствует файл, текстурой будет серый bitmap.
    void loadFromFile(std::wstring fileName, std::wstring textureName = L"");

    // Возвращает буфер, готовый для загрузки в видеопамять,
    // включающий вершины, нормали, координаты текстур.
    const std::vector<float>& getObjectBuffer() const;

    // Возвращает изображение, предназначенное для текстурирования.
    const std::shared_ptr<QImage> getTextureImage() const;

    // Возвращает размер одного вертекса. В вертекс входят
    // координаты вершины, нормаль, координаты текстуры.
    int getVertexSize() const;

    // Возвращает model матрицу текстуры. 
    // В данном случае, она единичная.
    const glm::mat4x4& getModelMatrix() const;

    // Возращает границы модели в координатах модели.
    const double* getBounds() const;
    ~ObjModel();

private:

    std::shared_ptr<QImage> m_texture;
    std::vector<float> m_objBuffer;
    glm::mat4x4 m_modelMatrix;
    double m_bounds[6];
    int m_vertexSize;
};

