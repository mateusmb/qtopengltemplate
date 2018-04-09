#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QWindow>
#include <QtGui/QOpenGLFunctions>

class QPainter;
class QOpenGLContext;
class QOpenGLPaintDevice;

class MainWindow : public QWindow, protected QOpenGLFunctions
{
	Q_OBJECT
public:
	explicit MainWindow(QWindow *parent = 0);
	~MainWindow();

	virtual void render(QPainter *painter);
	virtual void render();

	virtual void initialize();

	void setAnimating(bool animating);

public slots:
	void renderLater();
	void renderNow();

protected:
	bool event(QEvent *event) override;

	void exposeEvent(QExposeEvent *event) override;

private:
	bool animating;

	QOpenGLContext *context;
	QOpenGLPaintDevice *device;

};

#endif // MAINWINDOW_H
