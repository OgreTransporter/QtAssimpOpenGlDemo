#include "qtkwidget.h"
#include "qtkscene.h"
#include "qtkinput.h"
#include <QKeyEvent>

namespace qtk
{
    OpenGLWidget::OpenGLWidget(QWidget* parent, Qt::WindowFlags f)
        : QOpenGLWidget(parent, f)
        , mScene(nullptr)
#ifdef QTK_DEBUG
        , mDebugLogger(nullptr)
#endif
    {
    }

    void OpenGLWidget::paintGL()
    {
        // Clear buffers
        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

        // Draw the scene first, since it handles drawing our skybox
        if (mScene != nullptr) mScene->privDraw();
    }

    void OpenGLWidget::initializeGL()
    {
        initializeOpenGLFunctions();

        // Connect the frameSwapped signal to call the update() function
        connect(this, SIGNAL(frameSwapped()), this, SLOT(update()));

        // Initialize OpenGL debug context
#ifdef QTK_DEBUG
        mDebugLogger = new QOpenGLDebugLogger(this);
        if (mDebugLogger->initialize())
        {
            qDebug() << "GL_DEBUG Debug Logger" << mDebugLogger << "\n";
            connect(mDebugLogger, SIGNAL(messageLogged(QOpenGLDebugMessage)), this, SLOT(messageLogged(QOpenGLDebugMessage)));
            mDebugLogger->startLogging();
        }
        printContextInformation();
#endif // QTK_DEBUG

        // Initialize opengl settings
        glEnable(GL_MULTISAMPLE);
        glEnable(GL_DEPTH_TEST);
        glDepthMask(GL_TRUE);
        glDepthFunc(GL_LEQUAL);
        glDepthRange(0.1f, 1.0f);
        glClearDepth(1.0f);
        glClearColor(0.0f, 0.25f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void OpenGLWidget::resizeGL(int width, int height)
    {
        Scene::Projection().setToIdentity();
        Scene::Projection().perspective(45.0f, float(width) / float(height), 0.1f, 1000.0f);
    }

    Scene* OpenGLWidget::scene()
    {
        return mScene;
    }

    void OpenGLWidget::setScene(Scene* scenePtr)
    {
        mScene = scenePtr;
    }

    void OpenGLWidget::keyPressEvent(QKeyEvent* event)
    {
        if (event->isAutoRepeat())
        {
            // Do not repeat input while a key is held down
            event->ignore();
        }
        else
        {
            Input::registerKeyPress(event->key());
        }
    }

    void OpenGLWidget::keyReleaseEvent(QKeyEvent* event)
    {
        if (event->isAutoRepeat())
        {
            event->ignore();
        }
        else
        {
            Input::registerKeyRelease(event->key());
        }
    }

    void OpenGLWidget::mousePressEvent(QMouseEvent* event)
    {
        Input::registerMousePress(event->button());
    }

    void OpenGLWidget::mouseReleaseEvent(QMouseEvent* event)
    {
        Input::registerMouseRelease(event->button());
    }

    void OpenGLWidget::initializeWidget()
    {
        QSurfaceFormat format;
        format.setRenderableType(QSurfaceFormat::OpenGL);
        format.setProfile(QSurfaceFormat::CoreProfile);
        format.setVersion(4, 6);
        // Set the number of samples used for glEnable(GL_MULTISAMPLING)
        format.setSamples(4);
        // Set the size of the depth bufer for glEnable(GL_DEPTH_TEST)
        format.setDepthBufferSize(16);
        // If QTK_DEBUG is set, enable debug context
#ifdef QTK_DEBUG
        format.setOption(QSurfaceFormat::DebugContext);
#endif
        setFormat(format);
        setFocusPolicy(Qt::ClickFocus);
    }

#ifdef QTK_DEBUG
    void OpenGLWidget::printContextInformation()
    {
        QString glType;
        QString glVersion;
        QString glProfile;

        QString glVendor;
        QString glRenderer;

        // Get Version Information
        glType = (context()->isOpenGLES()) ? "OpenGL ES" : "OpenGL";
        glVersion = reinterpret_cast<const char*>(glGetString(GL_VERSION));
        glVendor = reinterpret_cast<const char*>(glGetString(GL_VENDOR));
        glRenderer = reinterpret_cast<const char*>(glGetString(GL_RENDERER));

        // Get Profile Information
#define CASE(c) case QSurfaceFormat::c: glProfile = #c; break
        switch (format().profile())
        {
            CASE(NoProfile);
            CASE(CoreProfile);
            CASE(CompatibilityProfile);
        }
#undef CASE

        // qPrintable() will print our QString w/o quotes around it.
        qDebug() << qPrintable(glType) << qPrintable(glVersion) << "("
            << qPrintable(glProfile) << ")"
            << "\nOpenGL Vendor: " << qPrintable(glVendor)
            << "\nRendering Device: " << qPrintable(glRenderer) << "\n";
    }
#endif

    void OpenGLWidget::updateCameraInput()
    {
        Input::update();
        // Camera Transformation
        if (Input::buttonPressed(Qt::RightButton))
        {
            static const float transSpeed = 0.1f;
            static const float rotSpeed = 0.5f;

            // Handle rotations
            Scene::Camera().transform().rotate(-rotSpeed * Input::mouseDelta().x(), Camera3D::LocalUp);
            Scene::Camera().transform().rotate(-rotSpeed * Input::mouseDelta().y(), Scene::Camera().right());

            // Handle translations
            QVector3D translation;
            if (Input::keyPressed(Qt::Key_W))
            {
                translation += Scene::Camera().forward();
            }
            if (Input::keyPressed(Qt::Key_S))
            {
                translation -= Scene::Camera().forward();
            }
            if (Input::keyPressed(Qt::Key_A))
            {
                translation -= Scene::Camera().right();
            }
            if (Input::keyPressed(Qt::Key_D))
            {
                translation += Scene::Camera().right();
            }
            if (Input::keyPressed(Qt::Key_Q))
            {
                translation -= Scene::Camera().up() / 2.0f;
            }
            if (Input::keyPressed(Qt::Key_E))
            {
                translation += Scene::Camera().up() / 2.0f;
            }
            Scene::Camera().transform().translate(transSpeed * translation);
        }
    }

    void OpenGLWidget::update()
    {
        updateCameraInput();
        if(mScene != nullptr) mScene->privUpdate();
        QWidget::update();
    }

#ifdef QTK_DEBUG
    void OpenGLWidget::messageLogged(const QOpenGLDebugMessage& msg)
    {
        QString error;

        // Format based on severity
        switch (msg.severity())
        {
        case QOpenGLDebugMessage::NotificationSeverity:
            error += "--";
            break;
        case QOpenGLDebugMessage::HighSeverity:
            error += "!!";
            break;
        case QOpenGLDebugMessage::MediumSeverity:
            error += "!~";
            break;
        case QOpenGLDebugMessage::LowSeverity:
            error += "~~";
            break;
        }

        error += " (";

        // Format based on source
#define CASE(c) case QOpenGLDebugMessage::c: error += #c; break
        switch (msg.source())
        {
            CASE(APISource);
            CASE(WindowSystemSource);
            CASE(ShaderCompilerSource);
            CASE(ThirdPartySource);
            CASE(ApplicationSource);
            CASE(OtherSource);
            CASE(InvalidSource);
        }
#undef CASE

        error += " : ";

        // Format based on type
#define CASE(c) case QOpenGLDebugMessage::c: error += #c; break
        switch (msg.type())
        {
            CASE(InvalidType);
            CASE(ErrorType);
            CASE(DeprecatedBehaviorType);
            CASE(UndefinedBehaviorType);
            CASE(PortabilityType);
            CASE(PerformanceType);
            CASE(OtherType);
            CASE(MarkerType);
            CASE(GroupPushType);
            CASE(GroupPopType);
        }
#undef CASE

        error += ")";
        qDebug() << qPrintable(error) << "\n" << qPrintable(msg.message()) << "\n";
    }
#endif
}
