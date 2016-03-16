#pragma once
#include <memory>

#include <QtWidgets/QMainWindow>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLineEdit>

#include "ui_objmodelloader.h"
#include "ObjModelViewWidget.h"

// Главное окно приложения.
class ObjModelLoader : public QMainWindow
{
    Q_OBJECT

public:
    ObjModelLoader(QWidget *parent = 0);
    ~ObjModelLoader();

private:
    Ui::ObjModelLoaderClass ui;


    std::shared_ptr<QPushButton> m_centerButton;
    std::shared_ptr<QVBoxLayout> m_verticalLayout;
    std::shared_ptr<QHBoxLayout> m_buttonsLayout;
    std::shared_ptr<QLineEdit> m_objPath;
    std::shared_ptr<QPushButton> m_openObjFindDialog;
    std::shared_ptr<QLineEdit> m_texturePath;
    std::shared_ptr<QPushButton> m_openTextureDialog;
    std::shared_ptr<QPushButton> m_loadModelButton;

    std::shared_ptr<ObjModelViewWidget> m_openGLWidget;

};
