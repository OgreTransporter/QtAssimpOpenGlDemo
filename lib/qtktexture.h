#ifndef QTKTEXTURE_H
#define QTKTEXTURE_H
#include "qtkapi.h"
#include <QOpenGLTexture>

namespace qtk
{
    class QTKAPI Texture
    {
    private:
        Texture();

    public:
        ~Texture();

        // QImage
        static QImage* initImage(QString image, bool flipX = false, bool flipY = false);

        // 2D Texture
        static QOpenGLTexture* initTexture2D(QString texture, bool flipX = false, bool flipY = false);

        // Cube maps
        static QOpenGLTexture* initCubeMap(QImage right, QImage top, QImage front, QImage left, QImage bottom, QImage back);

        // Overloads for cube map initialization
        static QOpenGLTexture* initCubeMap(QString tile);
        static QOpenGLTexture* initCubeMap(QString right, QString top, QString front, QString left, QString bottom, QString back);
    };
}
#endif // QTKTEXTURE_H
