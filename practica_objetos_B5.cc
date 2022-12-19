//**************************************************************************
// Práctica 2
//**************************************************************************

#include <GL/glut.h>
#include <ctype.h>
#include <math.h>

#include <vector>

#include "objetos_B5.h"

using namespace std;

// tipos
typedef enum {
  CUBO,
  PIRAMIDE,
  OBJETO_PLY,
  ROTACION,
  EXTRUSION,
  ESFERA,
  CILINDRO,
  CONO,
  ROTACION_PLY,
  EXCAVADORA,
  AMETRALLADORA,
  ESFERAEJERCICIO,
  CUBOEXAMEN,
  ROTACIONEXAMEN
} _tipo_objeto;
_tipo_objeto t_objeto = CUBO;
_modo modo = POINTS;

// variables que definen la posicion de la camara en coordenadas polares
GLfloat Observer_distance;
GLfloat Observer_angle_x;
GLfloat Observer_angle_y;

// variables que controlan la ventana y la transformacion de perspectiva
GLfloat Size_x, Size_y, Front_plane, Back_plane;

// variables que determninan la posicion y tamaño de la ventana X
int Window_x = 50, Window_y = 50, Window_width = 650, Window_high = 650;
float mov_camara = 0;
bool act_cam_2 = false;
// objetos
_cubo cubo;
_piramide piramide(0.85, 1.3);
_objeto_ply ply;
_rotacion rotacion;
_extrusion *extrusion;
_cilindro cilindro(1, 2, 6, 1, 1);
_cono cono(1, 2, 6);
_esfera esfera(1, 8, 8);
_rotacion_PLY rotacionPLY;
_excavadora excavadora;
_ametralladora ametralladora;
_barridoEjercicio barridoEjercicio(0.4, 0.6, 20, 20);
_cuboExamen cuboExamen;
_rotacionExamen rotacionExamen;

// _objeto_ply *ply;

int estadoRaton, xc, yc;

float factor = 1.0;
float Ancho = Window_width, Alto = Window_high;
int cambioAOrto = 0;
// boolean

bool hay_animacion = false;
//**************************************************************************
//
//***************************************************************************

void clean_window() { glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); }

//**************************************************************************
// Funcion para definir la transformación de proyeccion
//***************************************************************************

void change_projection() {
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  // formato(x_minimo,x_maximo, y_minimo, y_maximo,plano_delantero,
  // plano_traser)
  //  plano_delantero>0  plano_trasero>PlanoDelantero)
  glFrustum(-Size_x * factor, Size_x * factor, -Size_y * factor,
            Size_y * factor, Front_plane, Back_plane);

  // HACER MULTPLICAR POR UN FACTOR
}

//**************************************************************************
// Funcion para definir la transformación*ply1 de vista (posicionar la camara)
//***************************************************************************

void change_observer() {
  // posicion del observador
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glTranslatef(0, 0, -Observer_distance);  // cambiar centro de proyección
  glRotatef(Observer_angle_x, 1, 0, 0);
  glRotatef(Observer_angle_y, 0, 1, 0);
}

//**************************************************************************
// Funcion que dibuja los ejes utilizando la primitiva grafica de lineas
//***************************************************************************

void draw_axis() {
  glDisable(GL_LIGHTING);
  glLineWidth(2);
  glBegin(GL_LINES);
  // eje X, color rojo
  glColor3f(1, 0, 0);
  glVertex3f(-AXIS_SIZE, 0, 0);
  glVertex3f(AXIS_SIZE, 0, 0);
  // eje Y, color verde
  glColor3f(0, 1, 0);
  glVertex3f(0, -AXIS_SIZE, 0);
  glVertex3f(0, AXIS_SIZE, 0);
  // eje Z, color azul
  glColor3f(0, 0, 1);
  glVertex3f(0, 0, -AXIS_SIZE);
  glVertex3f(0, 0, AXIS_SIZE);

  glEnd();
}

//**************************************************************************
// Funcion que dibuja los objetos
//****************************2***********************************************

void draw_objects() {
  switch (t_objeto) {
    case CUBO:
      cubo.draw(modo, 1.0, 0.0, 0.0, 5);
      break;
    case PIRAMIDE:
      piramide.draw(modo, 1.0, 0.0, 0.0, 5);
      break;
    case OBJETO_PLY:
      ply.draw(modo, 1.0, 0.6, 0.0, 5);
      break;
    case ROTACION:
      rotacion.draw(modo, 1.0, 0.0, 0.0, 5);
      break;
    case EXTRUSION:
      extrusion->draw(modo, 1.0, 0.0, 0.0, 5);
      break;

    case CILINDRO:
      cilindro.draw(modo, 1.0, 0.0, 0.0, 5);
      break;
    case CONO:
      cono.draw(modo, 1.0, 0.0, 0.0, 5);
      break;
    case ESFERA:
      esfera.draw(modo, 1.0, 0.0, 0.0, 5);
      break;

    case ROTACION_PLY:
      rotacionPLY.draw(modo, 1.0, 0.0, 0.0, 5);
      break;

    case EXCAVADORA:
      excavadora.draw(modo, 1.0, 0.0, 0.0, 5);
      break;

    case AMETRALLADORA:
      ametralladora.draw(modo, 1.0, 0.0, 0.0, 5);
      break;

    case ESFERAEJERCICIO:
      barridoEjercicio.draw(modo, 1.0, 0.0, 0.0, 5);
      break;
    case CUBOEXAMEN:
      cuboExamen.draw(modo, 1.0, 0.0, 0.0, 5);
      break;
    case ROTACIONEXAMEN:
      rotacionExamen.draw(modo, 1.0, 0.0, 0.0, 5);
      break;
  }
}

void vista_orto() {
  // Primera vista
  glViewport(Ancho / 2, Alto / 2, Ancho / 2, Alto / 2);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(-5 * factor, 5 * factor, -5 * factor, 5 * factor, -100, 100);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  draw_axis();
  draw_objects();

  // Segunda vista Planta
  glViewport(0, Alto / 2, Ancho / 2, Alto / 2);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(-5 * factor, 5 * factor, -5 * factor, 5 * factor, -100, 100);

  glRotatef(90, 1, 0, 0);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  draw_axis();
  draw_objects();

  // Tercera vista (abajo izq)
  glViewport(0, 0, Ancho / 2, Alto / 2);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(-5 * factor, 5 * factor, -5 * factor, 5 * factor, -100, 100);
  glRotatef(90, 0, 1, 0);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  draw_axis();
  draw_objects();
}
//***************************************************************************
// luces
//***************************************************************************
void luces(float mov_cam_1, bool activar_cam_2) {
  GLfloat luz_ambiente[] = {0.2, 0.2, 0.2, 1.0},
          luz_difusa[] = {1.0, 1.0, 1.0, 1.0},
          luz_especular[] = {1.0, 1.0, 1.0, 1.0},
          luz_posicion[] = {X_LAMBERT, Y_LAMBERT, Z_LAMBERT, 1.0};
  // ult valor en 0 entonces la luz en el inf --> mirar transp

  // solo aplica a la luz 1

  glLightfv(GL_LIGHT1, GL_AMBIENT, luz_ambiente);
  glLightfv(GL_LIGHT1, GL_DIFFUSE, luz_difusa);
  glLightfv(GL_LIGHT1, GL_SPECULAR, luz_especular);
  glLightfv(GL_LIGHT1, GL_POSITION, luz_posicion);

  glEnable(GL_LIGHT1);

  if (activar_cam_2) {
    GLfloat luz_ambiente2[] = {0, 0.0, 0.0, 1.0},
            luz_difusa2[] = {1.0, 0, 0, 0}, luz_especular2[] = {1.0, 0, 0, 1.0},
            luz_posicion2[] = {0.0, 0.0, 20.0, 1.0};

    glLightfv(GL_LIGHT2, GL_AMBIENT, luz_ambiente2);
    glLightfv(GL_LIGHT2, GL_DIFFUSE, luz_difusa2);
    glLightfv(GL_LIGHT2, GL_SPECULAR, luz_especular2);
    glLightfv(GL_LIGHT2, GL_POSITION, luz_posicion2);

    // luz 2

    glPushMatrix();
    glRotatef(mov_cam_1, 0, 1,
              0);  // esto es el que realiza de vd la traslación de la luz
    glLightfv(GL_LIGHT2, GL_POSITION, luz_posicion2);
    glPopMatrix();

    glEnable(GL_LIGHT2);
  } else
    glDisable(GL_LIGHT2);

  glDisable(GL_LIGHT0);  // desactivar la por defecto
}

//**************************************************************************
//
//***************************************************************************

void draw(void) {
  glDrawBuffer(GL_FRONT);
  clean_window();
  luces(mov_camara, act_cam_2);

  // glutSwapBuffers(); // no vamos a utilizar el doble buffer


  if (cambioAOrto == 0) {
    glViewport(0, 0, Ancho, Alto);  // esto solo se haria en el inicialize sino
                                    // entonces se pierde lo q estaba
    change_projection();
    change_observer();

    draw_axis();
    draw_objects();
  } else
    vista_orto();

  if (t_objeto == AMETRALLADORA) {
    glDrawBuffer(GL_BACK);
    clean_window();
    change_observer();
    // luces(mov_camara, act_cam_2);
    // draw_axis();
    // draw_objects();
    ametralladora.seleccion();
  }
  glFlush();  // ahora se copia dos veces | trasero + delantero
}

//***************************************************************************
// Funcion llamada cuando se produce un cambio en el tamaño de la ventana
//
// el evento manda a la funcion:
// nuevo ancho
// nuevo alto
//***************************************************************************

void change_window_size(int Ancho1, int Alto1) {
  float Aspect_ratio;

  Aspect_ratio = (float)Alto1 / (float)Ancho1;
  Size_y = Size_x * Aspect_ratio;
  change_projection();
  glViewport(0, 0, Ancho1, Alto1);
  glutPostRedisplay();
}

bool estoyDeVuelta = false;
bool estoyDeVuelta2 = false;
int vueltasCanon = 500;
void animacionAmetralladora() {
  if (hay_animacion) {
    if (!estoyDeVuelta && !estoyDeVuelta2) {
      if (ametralladora.giro_base < 45)
        ametralladora.giro_base++;
      else if (ametralladora.giro_mirilla < ametralladora.giro_mirilla_max)
        ametralladora.giro_mirilla++;
      else if (ametralladora.giro_base_up > ametralladora.giro_base_up_min)
        ametralladora.giro_base_up--;
      else if (ametralladora.giro_canion < vueltasCanon)
        ametralladora.giro_canion += 5;
      else {
        estoyDeVuelta = true;
      }

    } else if (estoyDeVuelta && !estoyDeVuelta2) {
      if (ametralladora.giro_canion < vueltasCanon * 2)
        ametralladora.giro_canion += 5;
      else if (ametralladora.giro_base_up < 0)
        ametralladora.giro_base_up++;
      else if (ametralladora.giro_mirilla > ametralladora.giro_mirilla_min)
        ametralladora.giro_mirilla--;
      else if (ametralladora.giro_base > 0)
        ametralladora.giro_base--;
      else {
        estoyDeVuelta = false;
        estoyDeVuelta2 = true;
      }
    } else if (!estoyDeVuelta && estoyDeVuelta2) {
      if (ametralladora.giro_base > -45)
        ametralladora.giro_base--;
      else if (ametralladora.giro_mirilla < ametralladora.giro_mirilla_max)
        ametralladora.giro_mirilla++;
      else if (ametralladora.giro_base_up < ametralladora.giro_base_up_max)
        ametralladora.giro_base_up++;
      else if (ametralladora.giro_canion < vueltasCanon * 3)
        ametralladora.giro_canion += 5;
      else {
        estoyDeVuelta = true;
      }
    } else {
      if (ametralladora.giro_canion < vueltasCanon * 4)
        ametralladora.giro_canion += 5;
      else if (ametralladora.giro_base_up > 0)
        ametralladora.giro_base_up--;
      else if (ametralladora.giro_mirilla > ametralladora.giro_mirilla_min)
        ametralladora.giro_mirilla--;
      else if (ametralladora.giro_base < 0)
        ametralladora.giro_base++;
      else {
        estoyDeVuelta = false;
        estoyDeVuelta2 = false;
        ametralladora.giro_canion = 0;
      }
    }

    glutPostRedisplay();
  }
}

//***************************************************************************
// Funcion llamada cuando se aprieta una tecla normal
//
// el evento manda a la funcion:
// codigo de la tecla
// posicion x del raton
// posicion y del raton
//***************************************************************************

void normal_key(unsigned char Tecla1, int x, int y) {
  switch (toupper(Tecla1)) {
    case 'Q':
      exit(0);
    case '1':
      modo = POINTS;
      break;
    case '2':
      modo = EDGES;
      break;
    case '3':
      modo = SOLID;
      break;
    case '4':
      modo = SOLID_COLORS;
      break;
    case '5':
      modo = SOLID_FLAT;
      break;
    case '6':
      modo = SOLID_SMOOTH;
      break;
    case '9':
      cambioAOrto += 1;
      cambioAOrto %= 2;
      break;

    case 'P':
      t_objeto = PIRAMIDE;
      break;
    case 'C':
      t_objeto = CUBO;
      break;
    case 'O':
      t_objeto = OBJETO_PLY;
      break;
    case 'R':
      t_objeto = ROTACION;
      break;
    case 'X':
      t_objeto = EXTRUSION;
      break;
    case 'L':
      t_objeto = CILINDRO;
      break;
    case 'N':
      t_objeto = CONO;
      break;
    case 'E':
      t_objeto = ESFERA;
      break;
    case 'A':
      t_objeto = AMETRALLADORA;
      break;

    case 'T':
      t_objeto = ROTACION_PLY;
      break;

    case 'V':
      t_objeto = EXCAVADORA;
      break;

    case 'S':
      if (hay_animacion)
        hay_animacion = false;
      else
        hay_animacion = true;
      break;

    case 'Z':
      t_objeto = ESFERAEJERCICIO;
      break;
    case 'W':
      t_objeto = CUBOEXAMEN;
      break;

    case 'I':
      t_objeto = ROTACIONEXAMEN;
      break;
  }
  glutPostRedisplay();
}

//***************************************************************************
// Funcion l-olamada cuando se aprieta una tecla especial
//
// el evento manda a la funcion:
// codigo de la tecla
// posicion x del raton
// posicion y del raton

//***************************************************************************

void special_key(int Tecla1, int x, int y) {
  switch (Tecla1) {
    case GLUT_KEY_LEFT:
      Observer_angle_y++;
      break;
    case GLUT_KEY_RIGHT:
      Observer_angle_y--;
      break;
    case GLUT_KEY_UP:
      Observer_angle_x++;
      break;
    case GLUT_KEY_DOWN:
      Observer_angle_x--;
      break;
    case GLUT_KEY_PAGE_UP:
      Observer_distance *= 1.2;
      break;
    case GLUT_KEY_PAGE_DOWN:
      Observer_distance /= 1.2;
      break;

      // case GLUT_KEY_F1:
      //   excavadora.giro_cabina += 5;
      //   break;
      // case GLUT_KEY_F2:
      //   excavadora.giro_cabina -= 5;
      //   break;
      // case GLUT_KEY_F3:
      //   excavadora.giro_primer_brazo += 1;
      //   if (excavadora.giro_primer_brazo > excavadora.giro_primer_brazo_max)
      //     excavadora.giro_primer_brazo = excavadora.giro_primer_brazo_max;
      //   break;
      // case GLUT_KEY_F4:
      //   excavadora.giro_primer_brazo -= 1;
      //   if (excavadora.giro_primer_brazo < excavadora.giro_primer_brazo_min)
      //     excavadora.giro_primer_brazo = excavadora.giro_primer_brazo_min;
      //   break;
      // case GLUT_KEY_F5:
      //   excavadora.giro_segundo_brazo += 1;
      //   if (excavadora.giro_segundo_brazo >
      //   excavadora.giro_segundo_brazo_max)
      //     excavadora.giro_segundo_brazo = excavadora.giro_segundo_brazo_max;
      //   break;
      // case GLUT_KEY_F6:
      //   excavadora.giro_segundo_brazo -= 1;
      //   if (excavadora.giro_segundo_brazo <
      //   excavadora.giro_segundo_brazo_min)
      //     excavadora.giro_segundo_brazo = excavadora.giro_segundo_brazo_min;
      //   break;
      // case GLUT_KEY_F7:
      //   excavadora.giro_pala += 1;
      //   if (excavadora.giro_pala > excavadora.giro_pala_max)
      //     excavadora.giro_pala = excavadora.giro_pala_max;
      //   break;
      // case GLUT_KEY_F8:
      //   excavadora.giro_pala -= 1;
      //   if (excavadora.giro_pala < excavadora.giro_pala_min)
      //     excavadora.giro_pala = excavadora.giro_pala_min;
      //   break;

    case GLUT_KEY_F1:
      ametralladora.giro_canion += 5;
      break;
    case GLUT_KEY_F2:
      ametralladora.giro_canion -= 5;
      break;

    case GLUT_KEY_F3:
      ametralladora.giro_base += 1;
      if (ametralladora.giro_base > ametralladora.giro_base_max)
        ametralladora.giro_base = ametralladora.giro_base_max;
      break;
    case GLUT_KEY_F4:
      ametralladora.giro_base -= 1;
      if (ametralladora.giro_base < ametralladora.giro_base_min)
        ametralladora.giro_base = ametralladora.giro_base_min;
      break;
    case GLUT_KEY_F5:
      ametralladora.giro_base_up += 1;
      if (ametralladora.giro_base_up > ametralladora.giro_base_up_max)
        ametralladora.giro_base_up = ametralladora.giro_base_up_max;
      break;
    case GLUT_KEY_F6:
      ametralladora.giro_base_up -= 1;
      if (ametralladora.giro_base_up < ametralladora.giro_base_up_min)
        ametralladora.giro_base_up = ametralladora.giro_base_up_min;
      break;
    case GLUT_KEY_F7:
      ametralladora.giro_mirilla += 1;
      if (ametralladora.giro_mirilla > ametralladora.giro_mirilla_max)
        ametralladora.giro_mirilla = ametralladora.giro_mirilla_max;
      break;
    case GLUT_KEY_F8:
      ametralladora.giro_mirilla -= 1;
      if (ametralladora.giro_mirilla < ametralladora.giro_mirilla_min)
        ametralladora.giro_mirilla = ametralladora.giro_mirilla_min;
      break;
    /*-------------------PRACTICA4-------------------*/
    case GLUT_KEY_F9:
      rotacionExamen.rotacion1 += 0.1;
      // act_cam_2 = true;
      break;
    case GLUT_KEY_F10:
      // rotacionExamen.rotacion1 -= 0.1;

      act_cam_2 = act_cam_2 ? false : true;
      break;

    case GLUT_KEY_F11:
      // rotacionExamen.rotacion2 += 2;
      mov_camara += 5;
      break;
    case GLUT_KEY_F12:
      // rotacionExamen.rotacion2 -= 2;
      mov_camara -= 5;
      break;
  }

  glutPostRedisplay();
}

//***************************************************************************
// Funcion de incializacion
//***************************************************************************

void initialize(void) {
  // se inicalizan la ventana y los planos de corte
  Size_x = 0.5;
  Size_y = 0.5;
  Front_plane = 1;
  Back_plane = 1000;

  // se incia la posicion del observador, en el eje z
  Observer_distance = 4 * Front_plane;
  Observer_angle_x = 0;
  Observer_angle_y = 0;

  // se indica el color para limpiar la ventana	(r,v,a,al)
  // blanco=(1,1,1,1) rojo=(1,0,0,1), ...
  glClearColor(1, 1, 1, 1);

  // se habilita el z-bufer
  glEnable(GL_DEPTH_TEST);
  change_projection();
  glViewport(0, 0, Window_width, Window_high);
}
//***************************************************************************
// Funciones para la selección por color
//***************************************************************************

void procesar_color(unsigned char color[3]) {
  int i;

  for (i = 0; i < ametralladora.piezas; i++) {
    if (color[0] == ametralladora.color_select[i].r &&
        color[1] == ametralladora.color_select[i].g &&
        color[2] == ametralladora.color_select[i].r) {
      if (ametralladora.activo[i] == 0) {
        ametralladora.activo[i] = 1;
      } else {
        ametralladora.activo[i] = 0;
      }
      glutPostRedisplay();
    }
  }
}

//***************************************************************************

void pick_color(int x, int y) {
  GLint viewport[4];  // var tam disp de salida
  unsigned char pixel[3];

  glGetIntegerv(GL_VIEWPORT, viewport);  // lee el tam del disp de salida
  glReadBuffer(GL_BACK);  // seleccionamos leer en el buffer trasero
  // =pto raton
  // -y pq el 0,0 del raton esta abajo izq
  // numpixels a leer 1*1
  // formatocolor
  // formato pixels
  glReadPixels(x, viewport[3] - y, 1, 1, GL_RGB, GL_UNSIGNED_BYTE,
               (GLubyte *)&pixel[0]);

  // no hace falta el printf es solo para conocer
  printf(" valor x %d, valor y %d, color %d, %d, %d \n", x, y, pixel[0],
         pixel[1], pixel[2]);

  procesar_color(pixel);
}

//***************************************************************************
// Funciones para manejo de eventos del ratón
//***************************************************************************

void clickRaton(int boton, int estado, int x, int y) {
  if (boton == GLUT_RIGHT_BUTTON) {
    if (estado == GLUT_DOWN) {
      estadoRaton = 1;
      xc = x;
      yc = y;
    } else
      estadoRaton = 0;
  }
  if (boton == GLUT_LEFT_BUTTON) {
    if (estado == GLUT_DOWN) {
      estadoRaton = 2;
      xc = x;
      yc = y;
      pick_color(xc, yc);
    }
  }

  if (boton == 3) {
    factor *= 1.1;
    glutPostRedisplay();
  }
  if (boton == 4) {
    factor *= 0.9;
    glutPostRedisplay();
  }
}

/*************************************************************************/

void RatonMovido(int x, int y) {
  if (estadoRaton == 1) {
    Observer_angle_y = Observer_angle_y - (x - xc);
    Observer_angle_x = Observer_angle_x + (y - yc);
    xc = x;
    yc = y;
    glutPostRedisplay();
  }
}

//***************************************************************************
// Programa principal
//
// Se encarga de iniciar la ventana, asignar las funciones e comenzar el
// bucle de eventos
//***************************************************************************

int main(int argc, char *argv[]) {
  // perfil

  vector<_vertex3f> perfil, poligono;
  _vertex3f aux;

  aux.x = 1.0;
  aux.y = -1.0;
  aux.z = 0.0;
  perfil.push_back(aux);
  aux.x = 1.5;
  aux.y = -0.0;
  aux.z = 0.0;
  perfil.push_back(aux);
  aux.x = 1.0;
  aux.y = 1.0;
  aux.z = 0.0;
  perfil.push_back(aux);

  rotacion.parametros(perfil, 6, 0, 1, 1);

  aux.x = 1.0;
  aux.y = 0.0;
  aux.z = 1.0;
  poligono.push_back(aux);
  aux.x = 1.0;
  aux.y = 0.0;
  aux.z = -1.0;
  poligono.push_back(aux);
  aux.x = -1.2;
  aux.y = 0.0;
  aux.z = -1.0;
  poligono.push_back(aux);
  aux.x = -0.8;
  aux.y = 0.0;
  aux.z = 0.0;
  poligono.push_back(aux);
  aux.x = -1.2;
  aux.y = 0.0;
  aux.z = 1.0;
  poligono.push_back(aux);

  extrusion = new _extrusion(poligono, 0.25, 1.0, 0.25);

  // se llama a la inicialización de glut
  glutInit(&argc, argv);

  // se indica las caracteristicas que se desean para la visualización con
  // OpenGL Las posibilidades son: GLUT_SIMPLE -> memoria de imagen simple
  // GLUT_DOUBLE -> memoria de imagen doble
  // GLUT_INDEX -> memoria de imagen con color indizado
  // GLUT_RGB -> memoria de imagen con componentes rojo, verde y azul para cada
  // pixel GLUT_RGBA -> memoria de imagen con componentes rojo, verde, azul y
  // alfa para cada pixel GLUT_DEPTH -> memoria de profundidad o z-bufer
  // GLUT_STENCIL -> memoria de estarcido_rotation Rotation;
  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

  // posicion de la esquina inferior izquierdad de la ventana
  glutInitWindowPosition(Window_x, Window_y);

  // tamaño de la ventana (ancho y alto)
  glutInitWindowSize(Window_width, Window_high);

  // llamada para crear la ventana, indicando el titulo (no se visualiza hasta
  // que se llama al bucle de eventos)
  glutCreateWindow("PRACTICA - 5");

  // asignación de la funcion llamada "dibujar" al evento de dibujo
  glutDisplayFunc(draw);
  // asignación de la funcion llamada "change_window_size" al evento
  // correspondiente
  glutReshapeFunc(change_window_size);
  // asignación de la funcion llamada "normal_key" al evento correspondiente
  glutKeyboardFunc(normal_key);
  // asignación de la funcion llamada "tecla_Especial" al evento correspondiente
  glutSpecialFunc(special_key);

  // manejo del raton

  // funcion de inicialización
  initialize();

  // creación del objeto ply
  ply.parametros(argv[1]);

  // ply = new _objeto_ply(argv[1]);

  // codigo ejemplo para el objeto ply creado
  rotacionPLY.parametros_PLY(argv[2], 10);

  // inicio animacion
  glutIdleFunc(animacionAmetralladora);

  // eventos ratón
  glutMouseFunc(clickRaton);
  glutMotionFunc(RatonMovido);
  // inicio del bucle de eventos
  glutMainLoop();
  return 0;
}
