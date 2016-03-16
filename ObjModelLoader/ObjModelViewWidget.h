#pragma once
#include <memory>

#include <QOpenGLWidget>
#include <QOpenGLShaderProgram>
#include <QMouseEvent>
#include <QOpenGLBuffer>
#include <QOpenGLTexture>

#include "ObjModel.h"
#include "Camera.h"

// Вьюер obj модели.
class ObjModelViewWidget :
    public QOpenGLWidget
{
    Q_OBJECT

public:
    ObjModelViewWidget();
    ~ObjModelViewWidget();

    // Устанавливает текущую отображаемую модель во вьюер.
    void setCurrentModel(std::shared_ptr<ObjModel> model);

    // Возвращает активную камеру вьюера.
    const Camera& getActiveCamera() const;

public slots:
    void centerModel();

protected:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int width, int height) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent * event) override;
private:
    Camera m_activeCamera;

    std::shared_ptr<QOpenGLShaderProgram> m_shaderProgram;
    std::shared_ptr<ObjModel> m_currentModel;
    std::shared_ptr<QOpenGLBuffer> m_vertexBuffer;
    std::shared_ptr<QOpenGLTexture> m_openGLTexture;

    glm::vec3 m_initialFocalPoint;
    glm::vec3 m_initialPositin;
    QPoint m_lastPos;
    bool m_isModelInvalidated;
};


