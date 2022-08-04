#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "examplescene.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , scene(nullptr)
{
    ui->setupUi(this);

    scene = new ExampleScene();
    ui->openGLWidget->setScene(scene);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete scene;
}

