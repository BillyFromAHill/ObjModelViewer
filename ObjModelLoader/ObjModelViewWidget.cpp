#include "ObjModelViewWidget.h"
#include <qtransform.h>
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
using namespace std;

ObjModelViewWidget::ObjModelViewWidget() :
QOpenGLWidget()
{
    m_shaderProgram = make_shared<QOpenGLShaderProgram>();
    m_vertexBuffer = make_shared<QOpenGLBuffer>();
}

void ObjModelViewWidget::initializeGL()
{
    QOpenGLWidget::initializeGL();

    QOpenGLShader *vshader = new QOpenGLShader(QOpenGLShader::Vertex, this);
    if (!vshader->compileSourceFile("VertexShader.txt"))
    {
        throw L"Can't compile vertex shader.";
    }

    QOpenGLShader *fshader = new QOpenGLShader(QOpenGLShader::Fragment, this);
    if (!fshader->compileSourceFile("PixelShader.txt"))
    {
        throw L"Can't compile pixel shader.";
    }

    m_vertexBuffer->create();

    m_shaderProgram->addShader(vshader);
    m_shaderProgram->addShader(fshader);
    m_shaderProgram->bindAttributeLocation("vertex", 0);
    m_shaderProgram->bindAttributeLocation("normal", 1);
    m_shaderProgram->bindAttributeLocation("texCoord", 2);


    if (!m_shaderProgram->link())
    {
        throw "Can't link shader program.";
    }

    if (!m_shaderProgram->bind())
    {
        throw "Can't bind shader program.";
    }

    m_shaderProgram->setUniformValue("tex", 0);
}

void ObjModelViewWidget::centerModel()
{
    if (m_currentModel == nullptr)
    {
        return;
    }

    glm::mat4 sceneVectors = glm::inverse(m_activeCamera.GetViewMatrix());

    glm::vec3 baseX(1, 0, 0);
    glm::vec3 baseY(0, 1, 0);
    glm::vec3 baseZ(0, 0, 1);

    glm::vec3 ortX = glm::normalize(glm::vec3(sceneVectors[0]));
    glm::vec3 ortY = glm::normalize(glm::vec3(sceneVectors[1]));

    const double * bounds = m_currentModel->getBounds();

    double xSize = bounds[1] - bounds[0];
    double ySize = bounds[3] - bounds[2];
    double zSize = bounds[5] - bounds[4];

    double currentXSize =
        abs(glm::dot(ortX, baseX)) * xSize +
        abs(glm::dot(ortX, baseY)) * ySize +
        abs(glm::dot(ortX, baseZ)) * zSize;

    double currentYSize =
        abs(glm::dot(ortY, baseX)) * xSize +
        abs(glm::dot(ortY, baseY)) * ySize +
        abs(glm::dot(ortY, baseZ)) * zSize;

    double modelAspect = currentXSize / currentYSize;
    double screenAspect = (double)width() / height();

    if (modelAspect > screenAspect)
    {
        m_activeCamera.setViewHeight(currentXSize);
    }
    else
    {
        m_activeCamera.setViewHeight(currentYSize);
    }

    glm::vec3 modelCenter((bounds[0] + bounds[1]) / 2, (bounds[2] + bounds[3]) / 2, (bounds[4] + bounds[5]) / 2);

    m_activeCamera.translate(modelCenter - m_activeCamera.getFocalPoint());
    update();
}

const Camera& ObjModelViewWidget::getActiveCamera() const
{
    return m_activeCamera;
}

void ObjModelViewWidget::setCurrentModel(shared_ptr<ObjModel> model)
{
    if (m_currentModel != model)
    {
        m_isModelInvalidated = true;
    }

    m_currentModel = model;

    update();
}

void ObjModelViewWidget::paintGL()
{
    glClearColor(0.2, 0, 0.2, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    if (m_currentModel == nullptr)
    {
        return;
    }

    if (m_isModelInvalidated)
    {
        if (!m_vertexBuffer->bind())
        {
            throw "Can't bind vertex buffer.";
        }

        int vertexSize = m_currentModel->getVertexSize();

        m_shaderProgram->enableAttributeArray(0);
        m_shaderProgram->enableAttributeArray(1);
        m_shaderProgram->enableAttributeArray(2);
        m_shaderProgram->setAttributeBuffer(0, GL_FLOAT, 0, 4, vertexSize * sizeof(float));
        m_shaderProgram->setAttributeBuffer(1, GL_FLOAT, 4 * sizeof(float), 3, vertexSize * sizeof(float));
        m_shaderProgram->setAttributeBuffer(2, GL_FLOAT, 7 * sizeof(float), 2, vertexSize * sizeof(float));

        m_vertexBuffer->allocate(
            m_currentModel->getObjectBuffer().data(),
            m_currentModel->getObjectBuffer().size() * sizeof(float));

        m_vertexBuffer->release();

        m_openGLTexture = make_shared<QOpenGLTexture>(m_currentModel->getTextureImage()->mirrored());
        m_openGLTexture->setMinMagFilters(QOpenGLTexture::Linear, QOpenGLTexture::Linear);

        m_isModelInvalidated = false;
    }

    if (!m_vertexBuffer->bind())
    {
        throw "Can't bind vertex buffer.";
    }

    if (!m_shaderProgram->bind())
    {
        throw "Can't bind shader program.";
    }

    m_shaderProgram->setUniformValue("modelMatrix", QMatrix4x4(glm::value_ptr(m_currentModel->getModelMatrix())));
    m_shaderProgram->setUniformValue("projectionMatrix", QMatrix4x4(glm::value_ptr(m_activeCamera.GetProjectionMatrix())));
    m_shaderProgram->setUniformValue("viewMatrix", QMatrix4x4(glm::value_ptr(glm::transpose(m_activeCamera.GetViewMatrix()))));
    m_shaderProgram->setUniformValue(
        "lightPos",
        QVector3D(m_activeCamera.getPosition().x, m_activeCamera.getPosition().y, m_activeCamera.getPosition().z));

    m_openGLTexture->bind();

    glDrawArrays(GL_TRIANGLES, 0, m_currentModel->getObjectBuffer().size());
    m_shaderProgram->release();

}
void ObjModelViewWidget::resizeGL(int width, int height)
{
    int side = qMin(width, height);
    glViewport((width - side) / 2, (height - side) / 2, side, side);
    m_activeCamera.setScreenAspect((double)width / height);
}

void ObjModelViewWidget::mousePressEvent(QMouseEvent *event)
{
    m_initialPositin = m_activeCamera.getPosition();
    m_initialFocalPoint = m_activeCamera.getFocalPoint();
    m_lastPos = event->pos();
}

void ObjModelViewWidget::wheelEvent(QWheelEvent *event)
{
    double delta = 0.1 * event->delta() / abs(event->delta());
    double currentHeight = m_activeCamera.getViewHeight();
    m_activeCamera.setViewHeight(currentHeight + currentHeight * delta);
    update();
}

void ObjModelViewWidget::mouseMoveEvent(QMouseEvent *event)
{
    int dx = event->x() - m_lastPos.x();
    int dy = event->y() - m_lastPos.y();

    if (event->buttons() & Qt::LeftButton)
    {
        glm::vec2 mouseVector(dx, dy);
        double mouseLength = glm::length(mouseVector);
        mouseVector = glm::normalize(mouseVector);

        const glm::mat4x4& viewMatrix = m_activeCamera.GetViewMatrix();
        glm::mat4 movingConversion = glm::inverse(viewMatrix);

        glm::vec4 ortX = movingConversion[0];

        glm::vec4 ortY = movingConversion[1];

        double xMove = mouseVector[0] * mouseLength;
        double yMove = mouseVector[1] * mouseLength;

        m_activeCamera.rotate((float)glm::radians(360 * (float)xMove /
            width()),
            glm::vec3(ortY));

        m_activeCamera.rotate(
            (float)glm::radians(360 * (float)yMove /
            height()),
            glm::vec3(ortX));

        m_lastPos = event->pos();
    }
    else if (event->buttons() & Qt::MiddleButton)
    {
        double hworld = m_activeCamera.getViewHeight();

        double windowAspect = (double)width() / height();

        glm::mat4 movingConversion = glm::inverse(m_activeCamera.GetViewMatrix());
        movingConversion[0] *= windowAspect * hworld ;
        movingConversion[1] *= hworld;

        glm::vec4 normalizedDispEnd((event->x() / ((float)width() / 2) - 1),
            (-event->y() / ((float)height() / 2) + 1),
            0,
            1);

        glm::vec4 normalizedDispStart((m_lastPos.x() / ((float)width() / 2) - 1),
            (-m_lastPos.y() / ((float)height() / 2) + 1),
            0,
            1);

        normalizedDispStart = movingConversion * normalizedDispStart;
        normalizedDispEnd = movingConversion * normalizedDispEnd;

        glm::vec3 worldMove(normalizedDispEnd - normalizedDispStart);

        m_activeCamera.setPosition(m_initialPositin - worldMove);
        m_activeCamera.setFocalPoint(m_initialFocalPoint - worldMove);
    }

    update();
}



ObjModelViewWidget::~ObjModelViewWidget()
{
}
