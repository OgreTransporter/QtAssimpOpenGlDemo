#include "qtkshaders.h"

namespace qtk
{
	Shaders::Shaders(QString vertexShader, QString fragmentShader)
		: mVertexShaderText(vertexShader)
		, mFragmentShaderText(fragmentShader)
	{
		if (vertexShader.left(2) == ":/")
		{
			QFile vertFile(vertexShader);
			if (vertFile.open(QIODevice::ReadOnly))
			{
				mVertexShaderText = vertFile.readAll();
				vertFile.close();
			}
		}
		if (fragmentShader.left(2) == ":/")
		{
			QFile fragFile(fragmentShader);
			if (fragFile.open(QIODevice::ReadOnly))
			{
				mFragmentShaderText = fragFile.readAll();
				fragFile.close();
			}
		}
	}

	Shaders::Shaders(QFile vertexShader, QFile fragmentShader)
		: mVertexShaderText()
		, mFragmentShaderText()
	{
		if (vertexShader.open(QIODevice::ReadOnly))
		{
			mVertexShaderText = vertexShader.readAll();
			vertexShader.close();
		}
		if (fragmentShader.open(QIODevice::ReadOnly))
		{
			mFragmentShaderText = fragmentShader.readAll();
			fragmentShader.close();
		}
	}
}
