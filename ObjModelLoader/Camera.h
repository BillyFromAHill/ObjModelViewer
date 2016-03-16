#pragma once
#include "glm.hpp"

// ������, ����������� ������, � ������� �������
// �������������� �������� ����������� ������ �� �����.
class Camera
{
public:
    Camera();
    Camera(const Camera& camera);

    ~Camera();

    // ���������� View-������� ������.
    const glm::mat4x4& GetViewMatrix();

    // ���������� ������� ������������� �������� ������.
    const glm::mat4x4& GetProjectionMatrix();

    void operator=(const Camera& camera);

    // ������������ ������ �� ���� ������ �������.
    void rotate(double radians, glm::vec3 vector);

    // ��������� ������ �� ������.
    void translate(glm::vec3 vector);

    // ������������� ��������� ����� ������.
    void setFocalPoint(glm::vec3 focalPoint);

    // ���������� ��������� ����� ������.
    const glm::vec3& getFocalPoint();

    // ������������� ������� ����������� ������.
    void setViewUp(glm::vec3 viewUp);

    // ���������� ������� ����������� ������.
    const glm::vec3& getViewUp();

    // ������������� ��������� ������.
    void setPosition(glm::vec3 position);

    // ���������� ��������� ������.
    const glm::vec3& getPosition();

    // ������������� ��������� ������ � ������ ��������.
    void setScreenAspect(double aspect);

    // ������������� ���� ������ ������ ����� ������ �����, ������������ �� �������.
    void setViewHeight(double height);

    // ���������� ������ �����, ������������ �� �������.
    double getViewHeight() const;
private:

    void updateMatrices();

    double m_viewHeight;
    double m_screenAspect;
    double m_nearPlane;
    double m_farPlane;

    glm::vec3 m_position;
    glm::vec3 m_viewup;
    glm::vec3 m_focalPoint;

    glm::mat4x4 m_viewMatrix;
    glm::mat4x4 m_projectionMatrix;


};

