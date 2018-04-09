#include "mainwindow.h"

#include <QtGui/QGuiApplication>
#include <QtGui/QMatrix4x4>
#include <QtGui/QOpenGLShaderProgram>
#include <QtGui/QScreen>

#include <QtCore/qmath.h>

class TriangleWindow : public MainWindow
{
public:
	TriangleWindow();

	void initialize() override;
	void render() override;

private:
	GLuint posAttr;
	GLuint colAttr;
	GLuint matrixUniform;

	QOpenGLShaderProgram *program;
	int frame;
};

TriangleWindow::TriangleWindow() :
    program(0),
    frame(0)
{
}

static const char *vertexShaderSource =
        "attribute highp vec4 posAttr;\n"
        "attribute lowp vec4 colAttr;\n"
        "varying lowp vec4 col;\n"
        "uniform highp mat4 matrix;\n"
		"void main() {\n"
        "    col = colAttr;\n"
        "    gl_Position = matrix * posAttr;\n"
        "}\n";

static const char *fragmentShaderSource =
        "varying lowp vec4 col;\n"
        "void main() {\n"
        "    gl_FragColor = col;\n"
        "}\n";

void TriangleWindow::initialize()
{
	this->program = new QOpenGLShaderProgram(this);
	this->program->addShaderFromSourceCode(QOpenGLShader::Vertex, vertexShaderSource);
	this->program->addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShaderSource);
	this->program->link();
	this->posAttr = this->program->attributeLocation("posAttr");
	this->colAttr = this->program->attributeLocation("colAttr");
	this->matrixUniform = this->program->uniformLocation("matrix");
}

void TriangleWindow::render()
{
	const qreal retina_scale = devicePixelRatio();
	glViewport(0, 0, width() * retina_scale, height() * retina_scale);

	glClear(GL_COLOR_BUFFER_BIT);

	this->program->bind();

	QMatrix4x4 matrix;
	matrix.perspective(60.0f, 4.0f/3.0f, 0.1f, 100.0f);
	matrix.translate(0, 0, -2);
	matrix.rotate(100.0f * this->frame / screen()->refreshRate(), 0, 1, 0);

	this->program->setUniformValue(this->matrixUniform, matrix);

	GLfloat vertices[] = {
		0.0f, 0.707f,
	    -0.5f,-0.5f,
	    0.5f,-0.5f
	};

	GLfloat colors[] = {
		1.0f, 0.0f, 0.0f,
	    0.0f, 1.0f, 0.0f,
	    0.0f, 0.0f, 1.0f
	};

	glVertexAttribPointer(this->posAttr, 2, GL_FLOAT, GL_FALSE, 0, vertices);
	glVertexAttribPointer(this->colAttr, 3, GL_FLOAT, GL_FALSE, 0, colors);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glDrawArrays(GL_TRIANGLES, 0, 3);

	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);

	this->program->release();

	++this->frame;
}

int main(int argc, char *argv[])
{
	QGuiApplication app(argc, argv);
	QSurfaceFormat format;
	format.setSamples(16);

	TriangleWindow window;
	window.setFormat(format);
	window.resize(800, 600);
	window.show();

	window.setAnimating(true);

	return app.exec();
}
