#ifndef MYGLWIDGET_H
#define MYGLWIDGET_H

#include <QtOpenGL/QGLFunctions>
#include <QtWidgets/QOpenGLWidget>
#include <QtGui/QOpenGLVertexArrayObject>
#include <QtGui/QOpenGLBuffer>
#include <QtGui/QOpenGLShaderProgram>
#include <QMatrix4x4>
#include <QVector3D>
#include <QMouseEvent>
#include <qt5/QtOpenGL/QGLShaderProgram>
#include <QCoreApplication>
#include <QMessageBox>
#include <QTimer>
#include <QQuaternion>
#include "plane.h"
#include "mesh.h"
#include "point.h"
#include "utils.h"

#define DX 0.01
#define DY 0.01
#define DZ 0.01

QT_FORWARD_DECLARE_CLASS(QOpenGLShaderProgram)

struct Keyframe{
    double timestamp;
    QVector3D position;
    QQuaternion orientation;
    Keyframe(){}
    Keyframe(double t, QVector3D pos, QQuaternion q):
        timestamp(t), position(pos), orientation(q){}
};

class Camera{
    float m_fx, m_fy;
    float m_cx, m_cy;
    float m_k1, m_k2, m_p1, m_p2;
    float m_near, m_far;

public:
    Camera(){
        m_fx = m_fy = 0;
        m_k1 = m_k2 = m_p1 = m_p2 = 0;
        m_cx = m_cy = 0;
        m_near = 0.01f;
        m_far = 10000.0f;
    }
    Camera(float fx, float fy, float cx, float cy):m_fx(fx), m_fy(fy), m_cx(cx), m_cy(cy), m_near(0.001), m_far(10000.0){}
    void set_distortion(float k1, float k2, float p1, float p2){
        m_k1 = k1; m_k2 = k2;
        m_p1 = p1; m_p2 = p2;
    }
    cv::Mat get_cam_parameter();
    cv::Mat get_cam_distortion();
    QMatrix4x4 getProjectionTransform(int w, int h);
};

class MyGLWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    MyGLWidget(QWidget *parent = 0);
    ~MyGLWidget();

    QSize minimumSizeHint() const Q_DECL_OVERRIDE;
    QSize sizeHint() const Q_DECL_OVERRIDE;

    // Images input at start
    void fill_image_data(std::string dir, std::string csv, std::string settings);

    // Input Events
    void mousePress(QMouseEvent *event);
    void mouseMove(QMouseEvent *event, bool select_mode, std::string insert_mode);
    void keyPress(QKeyEvent *event);

    // Comfirm Click
    void add_plane();
    void remove_plane();
    void adjust_planes();
    void adjust_mesh();

    // Playback Click
    void playback();

    // Add Mesh click
    void input_mesh(std::string f);


public slots:
    void setXRotation(int angle);
    void setYRotation(int angle);
    void setZRotation(int angle);
    void cleanup();

signals:
    void xRotationChanged(int angle);
    void yRotationChanged(int angle);
    void zRotationChanged(int angle);

protected:
    void initializeGL() Q_DECL_OVERRIDE;
    void paintGL() Q_DECL_OVERRIDE;
    void resizeGL(int width, int height) Q_DECL_OVERRIDE;

private:
    void read_points();
    int get_keyframe_index(int start, double val);
    int get_image_index(int start, double val);
    void init_background();

    void draw_scene();
    void draw_mesh();
    void draw_planes();
    void draw_background();

    void adjustWorldRotationTransform();
    void adjustWorldTranslationTransform();
    void adjustCameraTransform();
    void adjustProjectionTransform(int w, int h);


    float m_xPos, m_yPos, m_zPos;
    int m_xRot, m_yRot, m_zRot;
    int m_mode;

    int m_i, m_ij;
    int m_mvMatrixLoc, m_vColor, m_vPosition, m_uIs_tp, m_vTexCoord;
    GLuint bg_tex;

    int m_selected_plane_for_removal, m_curr_image_index, m_curr_keyframe_index;
    int m_plane_1, m_plane_2;
    int m_snap_plane;
    bool m_mesh_point_selected;

    QPoint m_lastPos;

    Camera *m_cam;

    QOpenGLVertexArrayObject m_vao;
    QOpenGLBuffer m_scene_vbo, m_bg_vbo;

    QVector<Plane> m_planes;
    Mesh m_mesh;

    QVector<Point> m_scene_points, m_bg_points;
    QOpenGLShaderProgram *m_program;

    QMatrix4x4 m_proj;
    QMatrix4x4 m_worldTranslation;
    QMatrix4x4 m_worldRotation;
    QMatrix4x4 m_camera;

    std::vector<std::pair<std::string, double> > m_image_data;
    std::vector<QMatrix4x4> m_image_rt;
    std::vector<Keyframe> m_keyframes;
    std::string m_image_dir;
    QTimer *m_timer;
    int m_simulation_time_ms;
};

#endif // MYGLWIDGET_H

