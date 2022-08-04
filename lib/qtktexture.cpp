#include "qtktexture.h"
#include <QImageReader>

namespace qtk
{
    Texture::Texture()
    {
    }

    Texture::~Texture()
    {
    }

    QImage* Texture::initImage(QString image, bool flipX, bool flipY)
    {
        // Qt6 limits loaded images to 256MB by default
        QImageReader::setAllocationLimit(512);
        auto loadedImage = new QImage(QImage(image).mirrored(flipX, flipY));
        if (loadedImage->isNull())
        {
            qDebug() << "Error loading image: " << image << "\n";
            qDebug() << QImageReader::supportedImageFormats();
            return Q_NULLPTR;
        }
        return loadedImage;
    }

    QOpenGLTexture* Texture::initTexture2D(QString texture, bool flipX, bool flipY)
    {
        QImage* image = initImage(texture, flipX, flipY);
        auto newTexture = new QOpenGLTexture(QOpenGLTexture::Target2D);
        newTexture->setData(*image);
        newTexture->setWrapMode(QOpenGLTexture::Repeat);
        newTexture->setMinMagFilters(QOpenGLTexture::LinearMipMapLinear, QOpenGLTexture::Linear);
        delete image;
        return newTexture;
    }

    QOpenGLTexture* Texture::initCubeMap(QImage right, QImage top, QImage front, QImage left, QImage bottom, QImage back)
    {
        auto texture = new QOpenGLTexture(QOpenGLTexture::TargetCubeMap);
        std::vector<QImage> faceTextures = { right, top, front, left, bottom, back };

        // Initialize skybox cubemap texture
        texture->create();
        texture->bind();

        // For each cube map face
        std::vector<QOpenGLTexture::CubeMapFace> faces = {
            QOpenGLTexture::CubeMapPositiveX, QOpenGLTexture::CubeMapPositiveY,
            QOpenGLTexture::CubeMapPositiveZ, QOpenGLTexture::CubeMapNegativeX,
            QOpenGLTexture::CubeMapNegativeY, QOpenGLTexture::CubeMapNegativeZ
        };
        int i = 0;
        for (const auto& face : faces)
        {
            QImage faceImage(faceTextures[i]);
            if (faceImage.isNull())
            {
                qDebug() << "Error loading cube map image\n";
            }
            faceImage = faceImage.convertToFormat(QImage::Format_RGBA8888);

            // On the first iteration, set format and allocate texture storage
            if (face == QOpenGLTexture::CubeMapPositiveX)
            {
                // This also needs to happen on the first iteration, anyways
                texture->setSize(faceImage.width(), faceImage.height(), faceImage.depth());
                texture->setFormat(QOpenGLTexture::RGBA8_UNorm);
                texture->allocateStorage();
            }

            texture->setData(0, 0, face, QOpenGLTexture::RGBA, QOpenGLTexture::UInt8, faceImage.constBits());
            i++;
        }

        texture->setWrapMode(QOpenGLTexture::ClampToEdge);
        texture->generateMipMaps();
        texture->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
        texture->setMagnificationFilter(QOpenGLTexture::Linear);
        texture->release();
        return texture;
    }

    QOpenGLTexture* Texture::initCubeMap(QString tile)
    {
        return initCubeMap(QImage(tile), QImage(tile), QImage(tile), QImage(tile), QImage(tile), QImage(tile));
    }

    QOpenGLTexture* Texture::initCubeMap(QString right, QString top, QString front, QString left, QString bottom, QString back)
    {
        return initCubeMap(QImage(right), QImage(top), QImage(front), QImage(left), QImage(bottom), QImage(back));
    }
}
