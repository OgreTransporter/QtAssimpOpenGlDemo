#ifndef QTKSHADERS_H
#define QTKSHADERS_H
#include "qtkapi.h"
#include <QString>
#include <QFile>

namespace qtk
{
    class QTKAPI Shaders
    {
        friend class MeshRenderer;
        friend class ModelMesh;
        friend class Skybox;
    public:
        Shaders(QString vertexShader, QString fragmentShader);
        Shaders(QFile vertexShader, QFile fragmentShader);

    private:
        QString mVertexShaderText;
        QString mFragmentShaderText;
    };
}
#endif // QTKSHADERS_H
