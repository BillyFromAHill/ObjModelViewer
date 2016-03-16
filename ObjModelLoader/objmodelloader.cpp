#include "objmodelloader.h"
#include "ObjModel.h"
#include <QString>
#include <QFileDialog>
using namespace std;

ObjModelLoader::ObjModelLoader(QWidget *parent)
: QMainWindow(parent),
m_verticalLayout(new QVBoxLayout),
m_openGLWidget(new ObjModelViewWidget),
m_buttonsLayout(new QHBoxLayout),
m_centerButton(new QPushButton),
m_objPath(new QLineEdit),
m_openObjFindDialog(new QPushButton),
m_texturePath(new QLineEdit),
m_openTextureDialog(new QPushButton),
m_loadModelButton(new QPushButton)
{
    ui.setupUi(this);
    m_verticalLayout->addWidget(m_openGLWidget.get());
    m_verticalLayout->addLayout(m_buttonsLayout.get());
    m_verticalLayout->setMargin(1);

    m_buttonsLayout->addWidget(m_centerButton.get());
    m_buttonsLayout->addWidget(m_objPath.get());
    m_buttonsLayout->addWidget(m_openObjFindDialog.get());
    m_buttonsLayout->addWidget(m_texturePath.get());
    m_buttonsLayout->addWidget(m_openTextureDialog.get());
    m_buttonsLayout->addWidget(m_loadModelButton.get());

    ui.centralWidget->setLayout(m_verticalLayout.get());

    m_objPath->setText("crown_victoria.obj");
    m_texturePath->setText("crown_victoria.bmp");

    m_openTextureDialog->setText("...");
    connect(
        m_openTextureDialog.get(),
        &QPushButton::clicked,
        [&]()->void
    {
        m_texturePath->setText(QFileDialog::getOpenFileName());
    });

    m_openObjFindDialog->setText("...");
    connect(
        m_openObjFindDialog.get(),
        &QPushButton::clicked,
        [&]()->void
    {
        m_objPath->setText(QFileDialog::getOpenFileName());
    });

    m_loadModelButton->setText("Load");
    connect(
        m_loadModelButton.get(),
        &QPushButton::clicked,
        [&]()->void 
    {
        shared_ptr<ObjModel> model = make_shared<ObjModel>();
        model->loadFromFile(m_objPath->text().toStdWString(), m_texturePath->text().toStdWString());
        m_openGLWidget->setCurrentModel(model);
        m_openGLWidget->centerModel();
    });

    m_centerButton->setText(QString::fromWCharArray(L"Fit"));
    connect(m_centerButton.get(), SIGNAL(clicked()), m_openGLWidget.get(), SLOT(CenterModel()));
}

ObjModelLoader::~ObjModelLoader()
{

}
