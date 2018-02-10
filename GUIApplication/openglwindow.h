#ifndef OPENGLWINDOW_H
#define OPENGLWINDOW_H

#include <QDialog>
#include<QMouseEvent>

namespace Ui {
class OpenGLWindow;
}

class OpenGLWindow : public QDialog
{
    Q_OBJECT

public:
    explicit OpenGLWindow(QWidget *parent = 0);
    ~OpenGLWindow();

protected:
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
private slots:
    void on_pushButton_confirm_clicked();

    void on_pushButton_clicked();

private:
    Ui::OpenGLWindow *ui;
};

#endif // OPENGLWINDOW_H
