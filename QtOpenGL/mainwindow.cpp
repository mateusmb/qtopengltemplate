#include "mainwindow.h"

#include <QtCore/QCoreApplication>
#include <QtGui/QOpenGLContext>
#include <QtGui/QOpenGLPaintDevice>
#include <QtGui/QPainter>

MainWindow::MainWindow(QWindow *parent) :
    QWindow(parent),
	animating(false),
    context(0),
    device(0)
{
	setSurfaceType(QWindow::OpenGLSurface);
}

MainWindow::~MainWindow()
{
	delete device;
}

void MainWindow::render(QPainter *painter)
{
	Q_UNUSED(painter);
}

void MainWindow::initialize()
{
}

void MainWindow::render()
{
	if(!this->device) {
		this->device = new QOpenGLPaintDevice;
	}

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	this->device->setSize(size());

	QPainter painter(this->device);
	render(&painter);
}

void MainWindow::renderLater()
{
	requestUpdate();
}

bool MainWindow::event(QEvent *event)
{
	switch(event->type()) {
		case QEvent::UpdateRequest:
			this->renderNow();
			return true;
		default:
			return QWindow::event(event);
	}
}

void MainWindow::exposeEvent(QExposeEvent *event)
{
	Q_UNUSED(event);
	if(isExposed()) {
		this->renderNow();
	}
}

void MainWindow::renderNow()
{
	if(!isExposed()) {
		return;
	}
	bool needsInitialize = false;

	if(!this->context) {
		this->context = new QOpenGLContext(this);
		this->context->setFormat(requestedFormat());
		this->context->create();

		needsInitialize = true;
	}

	this->context->makeCurrent(this);

	if(needsInitialize) {
		initializeOpenGLFunctions();
		this->initialize();
	}

	this->render();

	this->context->swapBuffers(this);

	if(this->animating) {
		this->renderLater();
	}
}

void MainWindow::setAnimating(bool animating)
{
	this->animating = animating;
	if(animating) {
		this->renderLater();
	}
}
