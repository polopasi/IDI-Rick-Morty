#include "LL2GLWidget.h"
#include "model.h"
#define GLM_FORCE_RADIANS
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLWidget>
#include <QOpenGLShader>
#include <QOpenGLShaderProgram>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QTimer>
#include <glm/gtc/type_ptr.hpp>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <cmath>
#include <utility>
#include <vector>

class MyGLWidget : public LL2GLWidget {
  Q_OBJECT

  public:
    MyGLWidget(QWidget *parent=0) : LL2GLWidget(parent) {}
    ~MyGLWidget();

  protected:

    virtual void iniCamera();

    virtual void initializeGL ();

    virtual void RickTransform ();

    virtual void VideoCameraTransform ();

    virtual void viewTransform ();

    virtual void projectTransform ();

    virtual void viewTransform2 ();

    virtual void projectTransform2 ();

    virtual void MortyTransform ();

    virtual void keyPressEvent(QKeyEvent* event);

    virtual void mousePressEvent (QMouseEvent *e);

    virtual void mouseReleaseEvent( QMouseEvent *);

    virtual void mouseMoveEvent(QMouseEvent *e);

    virtual void carregaShaders();

    virtual void paintGL ();

    virtual void creaBuffersTerra ();

    virtual void creaBuffersModels ();

  private:

    int x_ant, y_ant;

    bool cameraOn = false;
    bool pintarVerd = false;
    bool rickCamuflado = false;
    float cameraGirX, cameraGirY;

    float angleCamara = 0;
    /*
     * mortyDirecciones son las 8 posibles direcciones en las que puede caminar Morty (eje x y z):
     * Como gira de 45 en 45 grados, las posibles direcciones son en línea recta o cuatro diagonales.
    */
    std::vector <std::pair<float, float>> mortyDirecciones = {{0,1}, {1,1}, {1,0}, {1,-1}, {0,-1}, {-1,-1}, {-1,0}, {-1,1}};
    int dirMorty = 0;
  
    int printOglError(const char file[], int line, const char func[]);

    //Camera vars
    float radiEscena2;
    float fov2, ra2, znear2, zfar2;
    glm::vec3 centreEscena2, obs2, vrp2, up2;

    // attribute locations
    GLuint vertexLoc2, colorLoc2;

    // uniform locations
    GLuint transLoc2, viewLoc2, projLoc2, greenLoc, rickVerdLoc;

    glm::mat4 camaraPos;
};
