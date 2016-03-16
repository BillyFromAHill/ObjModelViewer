#pragma once
#include "glm.hpp"

// Объект, описывающий камеру, с помощью которой
// осуществляется просмотр загруженной модели на сцене.
class Camera
{
public:
    Camera();
    Camera(const Camera& camera);

    ~Camera();

    // Возвращает View-матрицу камеры.
    const glm::mat4x4& GetViewMatrix();

    // Возвращает матрицу перспективной проекции камеры.
    const glm::mat4x4& GetProjectionMatrix();

    void operator=(const Camera& camera);

    // Поворачивает камеру на угол вокруг вектора.
    void rotate(double radians, glm::vec3 vector);

    // Переносит камеру на вектор.
    void translate(glm::vec3 vector);

    // Устанавливает фокальную точку камеры.
    void setFocalPoint(glm::vec3 focalPoint);

    // Возвращает фокальную точку камеры.
    const glm::vec3& getFocalPoint();

    // Устанавливает верхнее направление камеры.
    void setViewUp(glm::vec3 viewUp);

    // Возвращает верхнее направление камеры.
    const glm::vec3& getViewUp();

    // Устанавливает положение камеры.
    void setPosition(glm::vec3 position);

    // Возвращает положение камеры.
    const glm::vec3& getPosition();

    // Устанавливает отношение ширины к высоте вьюпорта.
    void setScreenAspect(double aspect);

    // Устанавливает угол обзора камеры через высоту сцены, укладываемую во вьюпорт.
    void setViewHeight(double height);

    // Возвращает высоту сцены, укладываемую во вьюпорт.
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

