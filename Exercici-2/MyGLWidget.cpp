#include "MyGLWidget.h"
#include <iostream>
#include <stdio.h>


#define printOpenGLError() printOglError(__FILE__, __LINE__)
#define CHECK() printOglError(__FILE__, __LINE__,__FUNCTION__)
#define DEBUG() std::cout << __FILE__ << " " << __LINE__ << " " << __FUNCTION__ << std::endl;

int MyGLWidget::printOglError(const char file[], int line, const char func[]) 
{
    GLenum glErr;
    int    retCode = 0;

    glErr = glGetError();
    const char * error = 0;
    switch (glErr)
    {
        case 0x0500:
            error = "GL_INVALID_ENUM";
            break;
        case 0x501:
            error = "GL_INVALID_VALUE";
            break;
        case 0x502: 
            error = "GL_INVALID_OPERATION";
            break;
        case 0x503:
            error = "GL_STACK_OVERFLOW";
            break;
        case 0x504:
            error = "GL_STACK_UNDERFLOW";
            break;
        case 0x505:
            error = "GL_OUT_OF_MEMORY";
            break;
        default:
            error = "unknown error!";
    }
    if (glErr != GL_NO_ERROR)
    {
        printf("glError in file %s @ line %d: %s function: %s\n",
                             file, line, error, func);
        retCode = 1;
    }
    return retCode;
}

MyGLWidget::~MyGLWidget()
{

}

void MyGLWidget::initializeGL ()
{
  // Cal inicialitzar l'ús de les funcions d'OpenGL
  initializeOpenGLFunctions();  
  glEnable(GL_DEPTH_TEST);
  
  glClearColor(0.5, 0.7, 1.0, 1.0); // defineix color de fons (d'esborrat)
  carregaShaders();
  creaBuffersModels();
  creaBuffersTerra();
  iniEscena();
  iniCamera();
}

void MyGLWidget::iniCamera(){

  obs = glm::vec3(-4,3,-4);
  vrp = glm::vec3(0, 1, 0);
  up = glm::vec3(0, 1, 0);
  fov = float(M_PI)/4.0f;
  znear =  1;
  zfar  = 30;
  factorAngleX = 0.0;
  factorAngleY = 0.0;

  obs2 = glm::vec3(-4,3,-4);
  vrp2 = glm::vec3(0, 1, 0);
  up2 = glm::vec3(0, 1, 0);
  fov2 = float(M_PI)/6.0f;
  znear2 =  0.25;
  zfar2  = 30;
  viewTransform();
}

void MyGLWidget::RickTransform () 
  {
    glm::mat4 TG(1.0f);  
    TG = glm::translate(TG, glm::vec3(-2, 0, -2));
    float rotation = 1.0f * M_PI;
    TG = glm::rotate(TG, rotation, glm::vec3(0.0, 1.0, 0.0));
    TG = glm::scale(TG, glm::vec3(escalaModels[RICK]));
    TG = glm::translate(TG, -centreBaseModels[RICK]);
    glUniformMatrix4fv(transLoc, 1, GL_FALSE, &TG[0][0]);
  }   

void MyGLWidget::VideoCameraTransform ()
{
  glm::mat4 TG(1.0f);
  TG = glm::translate(TG, glm::vec3(-4,3,-4));
  float rotation = 0.25f * M_PI;
  float angleCamaraAux = angleCamara*M_PI/180;
  TG = glm::rotate(TG, angleCamaraAux, glm::vec3(0.0, 1.0, 0.0));
  TG = glm::rotate(TG, rotation, glm::vec3(1.0, 0.0, 0.0));
  TG = glm::rotate(TG, rotation, glm::vec3(0.0, 1.0, 0.0));
  
  TG = glm::scale(TG, glm::vec3(escalaModels[VIDEOCAMERA]));
  TG = glm::translate(TG, -centreBaseModels[VIDEOCAMERA]);
  camaraPos = TG;
  glUniformMatrix4fv(transLoc, 1, GL_FALSE, &TG[0][0]);
}


void MyGLWidget::MortyTransform ()
{
  glm::mat4 TG(1.0f);

  TG = glm::translate(TG, posMorty);
  TG = glm::rotate(TG, angleMorty, glm::vec3(0.0, 1.0, 0.0));
  TG = glm::scale(TG, glm::vec3(escalaModels[MORTY]));
  TG = glm::translate(TG, -centreBaseModels[MORTY]);
  
  glUniformMatrix4fv(transLoc, 1, GL_FALSE, &TG[0][0]);
}


void MyGLWidget::projectTransform ()
{  

  glm::mat4 Proj(1.0f);
  znear = 0.01;
  fov = float (M_PI/4.0);
  if (pintarVerd) {
    fov = float (M_PI/3.0);
    znear2 = 0.25;
  }
  Proj = glm::perspective (fov, ra, znear, zfar);
  glUniformMatrix4fv (projLoc, 1, GL_FALSE, &Proj[0][0]);
}

void MyGLWidget::projectTransform2 ()
{  
  glm::mat4 Proj(1.0f);
  znear2 = 0.25;
  fov2 = float (M_PI/3.0);
  Proj = glm::perspective (fov2, ra2, znear2, zfar2);
  glUniformMatrix4fv (projLoc2, 1, GL_FALSE, &Proj[0][0]);
}

void MyGLWidget::viewTransform ()
{
  // Matriu de posició i orientació de l'observador
  glm::mat4 View(1.0f);
  /*
   * 11 = 11/tg(45), donde 11 es la anchura del suelo + 3 [3/tg 45 (y 3 es la altura de la pared)].
  */

  obs = glm::vec3(0, 11, 10);
  View = glm::lookAt (obs, vrp, up);
  View = glm::rotate(View, -factorAngleY, glm::vec3(1., 0., 0.));
  View = glm::rotate(View, -factorAngleX, glm::vec3(0., 1., 0.));
  glUniformMatrix4fv (viewLoc, 1, GL_FALSE, &View[0][0]);
}


void MyGLWidget::viewTransform2 ()
{
  glm::mat4 View2(1.0f);
  znear2 = 0.25;
  fov2 = float (M_PI/3.0);
  obs2 = glm::vec3(-4, 3, -4);

  float angleGir = angleCamara*M_PI/180;
  float x = sqrt(32)*sin(angleGir);
  float z = -sqrt(32)*sin(angleGir);

  vrp2 = glm::vec3(x,1,z);
  View2 = glm::lookAt (obs2, vrp2, up2);
  glUniformMatrix4fv (viewLoc2, 1, GL_FALSE, &View2[0][0]);
}

void MyGLWidget::keyPressEvent(QKeyEvent* event) 
{
  makeCurrent();
  switch (event->key()) {
    case Qt::Key_Up: { 
      //mortyDirecciones son las 8 posibles direcciones que puede caminar Morty.
      glm::vec3 x = glm::vec3(mortyDirecciones[dirMorty].first, 0, mortyDirecciones[dirMorty].second);
    	posMorty = posMorty + x*glm::vec3(0.1);    	
      break;
    }
    case Qt::Key_Down: {
      //mortyDirecciones son las 8 posibles direcciones que puede caminar Morty.
      glm::vec3 x = glm::vec3(mortyDirecciones[dirMorty].first, 0, mortyDirecciones[dirMorty].second);
	    posMorty = posMorty - x*glm::vec3(0.1);
      break;
    }
    case Qt::Key_Q: {
      if (angleCamara < 45) angleCamara += 1;
      VideoCameraTransform();
    	viewTransform2();
      break;
    }
    case Qt::Key_E: 
      if (angleCamara > -45) angleCamara -= 1;
      VideoCameraTransform();
    	viewTransform2();
    	break;
    case Qt::Key_C:
      cameraOn = !cameraOn;
      paintGL();
    	break;
    case Qt::Key_G:
      rickCamuflado = !rickCamuflado;
      paintGL();
    	break;
    case Qt::Key_R:
      cameraOn = false;
      rickCamuflado = false;
      angleCamara = 0;
      posMorty = glm::vec3(1, 0, 0);
      dirMorty = 0;
      angleMorty = 0;
      factorAngleX = factorAngleY = 0;
      paintGL();
    	break;
    
    case Qt::Key_Left:
      angleMorty += 0.25*M_PI;
      ++dirMorty;
      if (dirMorty > 7) dirMorty = 0;
      MortyTransform();
      break;
    case Qt::Key_Right: 
      angleMorty -= 0.25*M_PI;
      --dirMorty;
      if (dirMorty < 0) dirMorty = 7;
      MortyTransform();
    	break;  
    default: event->ignore(); break;
  }
  update();
}


void MyGLWidget::mousePressEvent (QMouseEvent *e)
{
  xClick = e->x();
  yClick = e->y();

  if (e->button() & Qt::LeftButton &&
      ! (e->modifiers() & (Qt::ShiftModifier|Qt::AltModifier|Qt::ControlModifier)))
  {
    DoingInteractive = ROTATE;
  }
}

void MyGLWidget::mouseReleaseEvent( QMouseEvent *)
{
  DoingInteractive = NONE;
}

void MyGLWidget::mouseMoveEvent(QMouseEvent *e) {
	makeCurrent();
	
	if(e->x() > x_ant) factorAngleX -= 0.02;
	else if(e->x() < x_ant) factorAngleX += 0.02;
	
	if(e->y() > y_ant) factorAngleY -= 0.02;
	else if(e->y() < y_ant) factorAngleY += 0.02;
	
	x_ant = e->x();
	y_ant = e->y();
	viewTransform();
	update();
}



void MyGLWidget::paintGL ()
{
  // descomentar per canviar paràmetres
  glViewport (0, 0, ample, alt);
  
  // Esborrem el frame-buffer
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glUniform1f (greenLoc, 0.0);

  viewTransform();

  // Rick
  if (rickCamuflado) glUniform1f (rickVerdLoc, 1.0);
  else  glUniform1f (rickVerdLoc, 0.0);
  RickTransform();
  glBindVertexArray (VAO_models[RICK]);
  glDrawArrays(GL_TRIANGLES, 0, models[RICK].faces().size()*3);
  glUniform1f (rickVerdLoc, 0.0);

  // Morty
  glBindVertexArray (VAO_models[MORTY]);
  MortyTransform();
  glDrawArrays(GL_TRIANGLES, 0, models[MORTY].faces().size()*3);
  
  // VideoCamera
  glBindVertexArray (VAO_models[VIDEOCAMERA]);
  VideoCameraTransform();
  glDrawArrays(GL_TRIANGLES, 0, models[VIDEOCAMERA].faces().size()*3);  
  
  // Terra
  glBindVertexArray (VAO_Terra);
  identTransform();
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

  // Paret
  ParetTransform();
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

  //REPETICIO SEGONA CAMERA: _______________________________
  if (cameraOn) {
    glViewport (ample*3/4, alt*3/4, ample/4, alt/4);
    pintarVerd = true;
    glUniform1f (greenLoc, 1.0);

    // Camara primera persona
    viewTransform2();
    
    if (rickCamuflado) glUniform1f (rickVerdLoc, 1.0);
    else  glUniform1f (rickVerdLoc, 0.0);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    // Rick
    glBindVertexArray (VAO_models[RICK]);
    RickTransform();
    glDrawArrays(GL_TRIANGLES, 0, models[RICK].faces().size()*3);
    
    glUniform1f (rickVerdLoc, 0.0);

    // Morty
    glBindVertexArray (VAO_models[MORTY]);
    MortyTransform();
    glDrawArrays(GL_TRIANGLES, 0, models[MORTY].faces().size()*3);
    
    // VideoCamera
    glBindVertexArray (VAO_models[VIDEOCAMERA]);

    //VideoCameraTransform();
    glDrawArrays(GL_TRIANGLES, 0, models[VIDEOCAMERA].faces().size()*3);
    
    // Terra
    creaBuffersTerra();
    glBindVertexArray (VAO_Terra);
    identTransform();
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    // Paret
    ParetTransform();
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    pintarVerd = false;
    glBindVertexArray (0);
  }
}



void MyGLWidget::creaBuffersModels ()
{
  // Càrrega dels models
  models[RICK].load("./models/Rick.obj");
  models[MORTY].load("./models/Morty.obj");
  models[VIDEOCAMERA].load("./models/VideoCamera.obj");

  // Creació de VAOs i VBOs per pintar els models
  glGenVertexArrays(3, &VAO_models[0]);
  
  float alcadaDesitjada[3] = {2,1,0.25};

  
  for (int i = 0; i < 3; i++)
  {	
	  // Calculem la capsa contenidora del model
	  calculaCapsaModel (models[i], escalaModels[i], alcadaDesitjada[i], centreBaseModels[i]);
  
	  glBindVertexArray(VAO_models[i]);

	  GLuint VBO[2];
	  glGenBuffers(2, VBO);

	  // geometria
	  glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*models[i].faces().size()*3*3,
		   models[i].VBO_vertices(), GL_STATIC_DRAW);
	  glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
	  glEnableVertexAttribArray(vertexLoc);

	  // color

	  glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*models[i].faces().size()*3*3,
		   models[i].VBO_matdiff(), GL_STATIC_DRAW);
    
	  glVertexAttribPointer(colorLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
	  glEnableVertexAttribArray(colorLoc);  

  }
  
  glBindVertexArray (0);
}



void MyGLWidget::creaBuffersTerra ()
{
  // VBO amb la posició dels vèrtexs
  glm::vec3 posTerra[4] = {
        glm::vec3(-4.0, 0.0, -4.0),
        glm::vec3(-4.0, 0.0,  4.0),
        glm::vec3( 4.0, 0.0, -4.0),
        glm::vec3( 4.0, 0.0,  4.0)
  }; 

  glm::vec3 c(0.8, 0.7, 1.0);
  glm::vec3 colTerra[4] = { c, c, c, c };


  // VAO
  glGenVertexArrays(1, &VAO_Terra);
  glBindVertexArray(VAO_Terra);

  GLuint VBO_Terra[2];
  glGenBuffers(2, VBO_Terra);

  // geometria
  glBindBuffer(GL_ARRAY_BUFFER, VBO_Terra[0]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(posTerra), posTerra, GL_STATIC_DRAW);
  glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(vertexLoc);

  // color
  glBindBuffer(GL_ARRAY_BUFFER, VBO_Terra[1]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(colTerra), colTerra, GL_STATIC_DRAW);
  glVertexAttribPointer(colorLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(colorLoc);
  
  glBindVertexArray (0);
}    


void MyGLWidget::carregaShaders()
{
  // Creem els shaders per al fragment shader i el vertex shader
  QOpenGLShader fs (QOpenGLShader::Fragment, this);
  QOpenGLShader vs (QOpenGLShader::Vertex, this);
  // Carreguem el codi dels fitxers i els compilem
  fs.compileSourceFile("shaders/basicShader.frag");
  vs.compileSourceFile("shaders/basicShader.vert");
  // Creem el program
  program = new QOpenGLShaderProgram(this);
  // Li afegim els shaders corresponents
  program->addShader(&fs);
  program->addShader(&vs);
  // Linkem el program
  program->link();
  // Indiquem que aquest és el program que volem usar
  program->bind();

  // Identificador per als  atributs
  vertexLoc = glGetAttribLocation (program->programId(), "vertex");
  colorLoc = glGetAttribLocation (program->programId(), "color");

  // Identificador per als  atributs de camera 2
  greenLoc = glGetUniformLocation (program->programId(), "Green");
  rickVerdLoc = glGetUniformLocation (program->programId(), "RickVerd");
  
  // Identificadors dels uniform locations
  transLoc = glGetUniformLocation(program->programId(), "TG");
  projLoc  = glGetUniformLocation (program->programId(), "Proj");
  viewLoc  = glGetUniformLocation (program->programId(), "View");

  // Identificadors dels uniform locations de camera 2
  transLoc2 = glGetUniformLocation(program->programId(), "TG");
  projLoc2  = glGetUniformLocation (program->programId(), "Proj");
  viewLoc2  = glGetUniformLocation (program->programId(), "View");
}
