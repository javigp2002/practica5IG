//**************************************************************************
// Práctica 2
//**************************************************************************

#include <GL/gl.h>
#include <stdlib.h>

#include <vector>

#include "vertex.h"

const float AXIS_SIZE = 5000;
typedef enum {
  POINTS,
  EDGES,
  SOLID,
  SOLID_COLORS,
  SOLID_COLORS_VERTS,
  SOLID_FLAT,
  SOLID_SMOOTH,
  SELECT
} _modo;

const int X_LAMBERT = 0, Y_LAMBERT = -20, Z_LAMBERT = -60;

struct material {
  _vertex4f ambiente_dif;
  _vertex4f spec;
  float brillo;
};
//*************************************************************************
// clase punto
//*************************************************************************

class _puntos3D {
 public:
  _puntos3D();
  void draw_puntos(float r, float g, float b, int grosor);

  vector<_vertex3f> vertices;
};

//*************************************************************************
// clase triángulo
//*************************************************************************

class _triangulos3D : public _puntos3D {
 public:
  _triangulos3D();
  void draw_aristas(float r, float g, float b, int grosor);
  void draw_solido(float r, float g, float b);
  void draw_solido_colores();
  void draw_solido_colores_vertices();
  void colors1();
  // p4
  void draw_solido_plano();
  void draw_solido_suave();

  // p5
  void draw_seleccion(int r, int g, int b);

  void draw(_modo modo, float r, float g, float b, float grosor);

  /* asignación de colores */
  void colors_random(int caras_iniciales = -1);
  void colors_random_personal();
  void colors_chess(float r1, float g1, float b1, float r2, float g2, float b2);
  void colors_Lambert_caras(float l_x, float l_y, float l_z, float r, float g,
                            float b);

  // practica 4
  void calcular_normales_caras();
  void calcular_normales_vertices();

  vector<_vertex3i> caras;
  vector<_vertex3f> colores_caras;

  // practica 4
  vector<_vertex3f> normales_caras;
  vector<_vertex3f> normales_vertices;

  // Material
  _vertex4f ambiente_difuso;
  _vertex4f especular;
  float brillo;

  // mate --> especular a 0 con brillo bajo
  // satinado --> especular a 1
  // brillante --> especular alto
  void cambiarMaterial(material m);
  material brass{
      {0.780, 0.569, 0.114, 1.0}, {0.992, 0.941, 0.808, 1.0}, 27.897};

  material gold{{0.752, 0.606, 0.226, 1.0}, {0.628, 0.555, 0.366, 1.0}, 51.2};
  material blackPlastic{{0.01, 0.01, 0.01, 1.0}, {0.5, 0.5, 0.5, 1.0}, 32};
  material polishedCopper{
      {0.551, 0.212, 0.066, 1.0}, {0.581, 0.223, 0.069, 1.0}, 51.2};
  material obsidian{
      {0.1828, 0.17, 0.225, 0.82}, {0.333, 0.329, 0.346, 0.82}, 38.4};

  // SELECCION
};

//*************************************************************************
// clase cubo
//*************************************************************************

class _cubo : public _triangulos3D {
 public:
  _cubo(float tam = 1);
};

//*************************************************************************
// clase piramide
//*************************************************************************

class _piramide : public _triangulos3D {
 public:
  _piramide(float tam = 0.5, float al = 1.0);
};

//*************************************************************************
// clase objeto ply
//*************************************************************************

class _objeto_ply : public _triangulos3D {
 public:
  _objeto_ply();

  void parametros(char *archivo);
};

//************************************************************************
// objeto por revolución
//************************************************************************
// tipo= 0 --> normal | 1 --> cono | 2 esfera
// tapa_in = 0 --> sin | 1 --> con

class _rotacion : public _triangulos3D {
 public:
  _rotacion();

  void parametros(vector<_vertex3f> perfil, int num, int tipo, int tapa_in,
                  int tapa_su);
};

//************************************************************************
// objeto por extrusión
//************************************************************************

class _extrusion : public _triangulos3D {
 public:
  _extrusion(vector<_vertex3f> poligono, float x, float y, float z);
};

//*************************************************************************
// clase Cilindro
//*************************************************************************
static int ALTURACILINDRO = 3;
static int RADIOCILINDRO = 3;

class _cilindro : public _rotacion {
 public:
  _cilindro(float radio = RADIOCILINDRO, float altura = ALTURACILINDRO,
            int num = 20, int tapa_in = 1, int tapa_su = 1);
};

//*************************************************************************
// clase cono
//*************************************************************************

class _cono : public _rotacion {
 public:
  _cono(float radio = 2, float altura = 3, int num = 10);
};

//*************************************************************************
// clase esfera
//*************************************************************************

class _esfera : public _rotacion {
 public:
  _esfera(float radio = 1, int num1 = 2, int num2 = 20);
};

//*************************************************************************
// clase rotación ply
//*************************************************************************

class _rotacion_PLY : public _rotacion {
 public:
  _rotacion_PLY();
  void parametros_PLY(char *archivo, int num);
};

//************************************************************************
// práctica 3, objeto jerárquico articulado excavadora
//************************************************************************

//************************************************************************
// piezas
//************************************************************************

//************************************************************************
// pala
//************************************************************************

class _pala : public _triangulos3D {
 public:
  _pala(float radio = 1.0, float ancho = 2.0, int num = 8);
};

//************************************************************************
// brazo
//************************************************************************

class _brazo : public _triangulos3D {
 public:
  _brazo();
  void draw(_modo modo, float r, float g, float b, float grosor);

  float ancho;
  float alto;
  float fondo;

 protected:
  _cubo cubo;
};

//************************************************************************
// cabina
//************************************************************************

class _cabina : public _triangulos3D {
 public:
  _cabina();
  void draw(_modo modo, float r, float g, float b, float grosor);

  float ancho;
  float alto;
  float fondo;

 protected:
  _cubo cubo;
};

//************************************************************************
// sustentación
//************************************************************************

class _sustentacion : public _triangulos3D {
 public:
  _sustentacion();
  void draw(_modo modo, float r, float g, float b, float grosor);

  float ancho;
  float alto;
  float fondo;

  float radio;

 protected:
  _cilindro rueda;
  _cubo base;
};

//************************************************************************
// excavadora (montaje del objeto final)
//************************************************************************

class _excavadora : public _triangulos3D {
 public:
  _excavadora();

  void draw(_modo modo, float r, float g, float b, float grosor);

  float giro_cabina;
  float giro_primer_brazo;
  float giro_segundo_brazo;
  float giro_pala;

  float giro_primer_brazo_max;
  float giro_primer_brazo_min;
  float giro_segundo_brazo_max;
  float giro_segundo_brazo_min;
  float giro_pala_max;
  float giro_pala_min;

  float tamanio_pala;

 protected:
  _pala pala;
  _brazo brazo;
  _cabina cabina;
  _sustentacion sustentacion;
};

//************************************************************************
// piezas AMETRALLADORA CIRCULAR
//************************************************************************
//************************************************************************
// mirilla
//************************************************************************

class _mirilla : public _triangulos3D {
 public:
  _mirilla();

  void draw(_modo modo, float r, float g, float b, float grosor);

  void introduceBase(float posX, float posY, float posZ, float ancho,
                     float alto, float fondo, _modo modo, float r, float g,
                     float b, float grosor, bool rotate = true);
  void introduceMira(float posX, float posY, float posZ, float ancho,
                     float alto, float fondo, _modo modo, float r, float g,
                     float b, float grosor, bool rotate = true);
  void introduceCampana(float posX, float posY, float posZ, float radio,
                        float alto, _modo modo, float r, float g, float b,
                        float grosor, bool rotate = true);
  void introduceModRotatorio(float posX, float posY, float posZ, float radio,
                             float alto, _modo modo, float r, float g, float b,
                             float grosor, bool rotate = true);

  float ancho;
  float alto;
  float fondo;
  float radio;

  float an = 1;
  float al = 1.5;
  float f = 1;
  float r = 0.45;

 protected:
  _cilindro campana, modRot;
  _cubo mira;
};
//************************************************************************
// cañon
//************************************************************************

class _canon : public _triangulos3D {
 public:
  _canon();

  void draw(_modo modo, float r, float g, float b, float grosor);
  void introduceSmallCanon(float posX, float posY, float posZ, _modo modo,
                           float radio, float alto, float r, float g, float b,
                           float grosor);
  void introduceEmbellecedor(float posX, float posY, float posZ, float radio,
                             float alto, _modo modo, float r, float g, float b,
                             float grosor);
  void introduceRotationModule(float posX, float posY, float posZ, float radio,
                               float alto, _modo modo, float r, float g,
                               float b, float grosor);

  float ancho;
  float alto;
  float fondo;
  float radio;

  float an = 1;
  float al = 1.5;
  float f = 1;
  float r = 0.45;

 protected:
  _cilindro cilindro, canonSmall, rotationModule, embellecedor;
};

// cargador
//************************************************************************

class _cargador : public _triangulos3D {
 public:
  _cargador();

  void draw(_modo modo, float r, float g, float b, float grosor);
  void introduceEmbellecedor(float posX, float posY, float posZ, float ancho,
                             float alto, float fondo, _modo modo, float r,
                             float g, float b, float grosor);

  float ancho;
  float alto;
  float fondo;
  float radio;

  float an = 1;
  float al = 1.5;
  float f = 1;
  float r = 0.45;

 protected:
  _cubo cubo;
};

//************************************************************************
// housing
//************************************************************************

class _housing : public _triangulos3D {
 public:
  _housing();

  void draw(_modo modo, float r, float g, float b, float grosor,
            float giro_mira = 0.0);
  void introduceAgarre(float posX, float posY, float posZ, float radio,
                       float alto, _modo modo, float r, float g, float b,
                       float grosor, bool rotate = true);
  void introduceEmbellecedor(float posX, float posY, float posZ, float ancho,
                             float alto, float fondo, _modo modo, float r,
                             float g, float b, float grosor);
  void introduceRotationModule(float posX, float posY, float posZ, float radio,
                               float alto, _modo modo, float r, float g,
                               float b, float grosor, bool rotate = true);

  float ancho;
  float alto;
  float fondo;
  float radio;

  float an = 1;
  float al = 1.5;
  float f = 1;
  float r = 0.45;

 protected:
  _cilindro cilindro, canonSmall, rotationModule, embellecedor;
  _cono cono;
  _cubo cubo;
  _mirilla mira;
  _cargador cargador;
};

//************************************************************************
// sustentación
//************************************************************************

class _sustentacionAmetralladora : public _triangulos3D {
 public:
  _sustentacionAmetralladora();
  void draw(_modo modo, float r, float g, float b, float grosor);

  float ancho;
  float alto;
  float fondo;

  float radio;

 protected:
  _cubo base, pataIzda, pataDcha, pataAtras, baseIzda, baseDcha, baseAtras,
      paloBase;
};

//************************************************************************
// ametralladora (montaje del objeto final)
//************************************************************************

class _ametralladora : public _triangulos3D {
 public:
  _ametralladora();

  void draw(_modo modo, float r, float g, float b, float grosor);

  void seleccion(); // para hacer la seleccion dentro de ellos

  float giro_canion;
  float giro_base;
  float giro_base_up;

  float giro_base_max;
  float giro_base_min;
  float giro_base_up_max;
  float giro_base_up_min;

  float giro_mirilla;
  float giro_mirilla_max;
  float giro_mirilla_min;

  float tamanio_pala;
   // sobre el objeto jerarquico -->
  // si queremos otro entonces habra que hacer lo mismo sobre los subsistemas
  _vertex3f color_pick; // color que voy a pintar 
  vector<_vertex3i> color_select;
  vector<int> activo;
  int piezas;
  int grosor_select; // grosor linea
  
 protected:
  _canon canon;
  _sustentacionAmetralladora sustentacion;
  _housing housing;
  _mirilla mira;
  _cargador cargador;

 
};

// cargador
//************************************************************************

class _ejercicioExamen : public _triangulos3D {
 public:
  _ejercicioExamen();

  void draw(_modo modo, float r, float g, float b, float grosor);

  float ancho;
  float alto;
  float fondo;
  float radio;
  _cubo cubo;
  _cilindro cilindro;
};

//*************************************************************************
// clase esferaEjercicio
//*************************************************************************

class _barridoEjercicio : public _rotacion {
 public:
  _barridoEjercicio(float radio1 = 0.4, float ancho = 0.6, int num1 = 8,
                    int num2 = 20);
};

class _cuboExamen : public _triangulos3D {
 public:
  _cuboExamen(float tam = 0.5, float al = 1.0);
};

class _rotacionExamen : public _triangulos3D {
 public:
  _rotacionExamen();
  void draw(_modo modo, float r, float g, float b, float grosor);
  void introduceSegundoNivel(_modo modo, float r, float g, float b,
                             float grosor);

  float alto, radio, ancho, fondo, rotacion1, rotacion2;

 private:
  _cubo prisma;
  _cilindro cilindro;
  _esfera aspa = _esfera(1, 20, 20);
};