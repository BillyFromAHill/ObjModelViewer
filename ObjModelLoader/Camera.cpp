#include "Camera.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
Camera::Camera() :
m_viewHeight(1.0),
m_screenAspect(1.0),
m_nearPlane(1.0),
m_farPlane(10),
m_position(500, 0, 0),
m_focalPoint(0, 0, 0),
m_viewup(0,1,0)
{
    updateMatrices();
}

Camera::Camera(const Camera& camera)
{
    *this = camera;
}

void Camera::operator=(const Camera & camera)
{
    m_viewHeight = camera.m_viewHeight;
    m_viewMatrix = camera.m_viewMatrix;
    m_screenAspect = camera.m_screenAspect;
    m_nearPlane = camera.m_nearPlane;
    m_farPlane = camera.m_farPlane;
    m_projectionMatrix = camera.m_projectionMatrix;
}

void Camera::setFocalPoint(glm::vec3 focalPoint)
{
    m_focalPoint = focalPoint;
    updateMatrices();
}

const glm::vec3& Camera::getFocalPoint()
{
    return m_focalPoint;
}

void Camera::setViewUp(glm::vec3 viewUp)
{
    m_viewup = viewUp;
    updateMatrices();
}

const glm::vec3& Camera::getViewUp()
{
    return m_viewup;
}

void Camera::setPosition(glm::vec3 position)
{
    m_position = position;
    updateMatrices();
}

const glm::vec3& Camera::getPosition()
{
    return m_position;
}

void Camera::setScreenAspect(double aspect)
{
    m_screenAspect = aspect;
    updateMatrices();
}

const glm::mat4x4& Camera::GetViewMatrix()
{
    return m_viewMatrix;
}

const glm::mat4x4& Camera::GetProjectionMatrix()
{
    return m_projectionMatrix;
}

void Camera::rotate(double radians, glm::vec3 vector)
{
    m_viewMatrix = glm::rotate(m_viewMatrix, (float)radians, vector);

    glm::mat4x4 inversed = glm::inverse(m_viewMatrix);

    m_viewup = glm::vec3(inversed[1]);
    m_position = glm::vec3(inversed[3]);
    m_focalPoint = m_position - glm::vec3(inversed[2]) * glm::length(m_position);
}

void Camera::translate(glm::vec3 vector)
{
    m_position += vector;
    m_focalPoint += vector;
    updateMatrices();
}

void Camera::setViewHeight(double height)
{
    m_viewHeight = height;
    updateMatrices();
}

void  Camera::updateMatrices()
{
    double fov = 2 * atan(m_viewHeight / glm::distance(m_focalPoint, m_position) / 2);

    m_projectionMatrix = glm::perspective(fov, m_screenAspect, m_nearPlane, m_farPlane);
    m_viewMatrix = glm::lookAt(m_position, m_focalPoint, m_viewup);
}

double Camera::getViewHeight() const
{
    return m_viewHeight;
}

Camera::~Camera()
{
}
