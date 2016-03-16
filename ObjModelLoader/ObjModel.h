#pragma once
#include <string>
#include <vector>
#include <memory>

#include "glm/glm.hpp"

#include <QImage>

// �������� ��� ������� obj.
class ObjModel
{
public:
    ObjModel();

    // ��������� ������ � �������� �� ������.
    // � ������, ���� ���� � �������� �� ������ ���
    // ����������� ����, ��������� ����� ����� bitmap.
    void loadFromFile(std::wstring fileName, std::wstring textureName = L"");

    // ���������� �����, ������� ��� �������� � �����������,
    // ���������� �������, �������, ���������� �������.
    const std::vector<float>& getObjectBuffer() const;

    // ���������� �����������, ��������������� ��� ���������������.
    const std::shared_ptr<QImage> getTextureImage() const;

    // ���������� ������ ������ ��������. � ������� ������
    // ���������� �������, �������, ���������� ��������.
    int getVertexSize() const;

    // ���������� model ������� ��������. 
    // � ������ ������, ��� ���������.
    const glm::mat4x4& getModelMatrix() const;

    // ��������� ������� ������ � ����������� ������.
    const double* getBounds() const;
    ~ObjModel();

private:

    std::shared_ptr<QImage> m_texture;
    std::vector<float> m_objBuffer;
    glm::mat4x4 m_modelMatrix;
    double m_bounds[6];
    int m_vertexSize;
};

