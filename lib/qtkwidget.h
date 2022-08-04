#ifndef QTKWIDGET_H
#define QTKWIDGET_H
#include "qtkapi.h"
#include <QOpenGLFunctions>
#include <QOpenGLWidget>
#ifdef QTK_DEBUG
#include <QOpenGLDebugLogger>
#endif

namespace qtk
{
    class Scene;

    class QTKAPI OpenGLWidget : public QOpenGLWidget, protected QOpenGLFunctions
    {
        Q_OBJECT
    public:
        explicit OpenGLWidget(QWidget* parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());

        // Inherited virtual Members
        void paintGL() override;
        void initializeGL() override;
        void resizeGL(int width, int height) override;

        Scene* scene();
        void setScene(Scene* scenePtr);

    protected:
        // Protected Helpers
        void keyPressEvent(QKeyEvent* event);
        void keyReleaseEvent(QKeyEvent* event);
        void mousePressEvent(QMouseEvent* event);
        void mouseReleaseEvent(QMouseEvent* event);

    private:
        // Private helpers
        void initializeWidget();
#ifdef QTK_DEBUG
        void printContextInformation();
#endif
        void updateCameraInput();

    protected slots:
        void update();
#ifdef QTK_DEBUG
        void messageLogged(const QOpenGLDebugMessage& msg);
#endif

    private:
        Scene* mScene;
#ifdef QTK_DEBUG
        QOpenGLDebugLogger* mDebugLogger;
#endif
    };
}
#endif // QTKWIDGET_H
