//g++ main.cpp -lGL -lGLU -lGLEW -lglut -lSDL2 -lSDL2_mixer -lfreeimage -std=c++11 -o main
#include <iostream>
#include <cmath>
#include <cstdlib>
#include <FreeImage.h>
#include <GL/glut.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_opengl.h>
#include <time.h>
#include <fstream>
#include <string>
int jx,jy,jbutton;
//jx = jy = 100;
//jbutton = 0;
Sint16 joyx,joyy;

/*
Sint16 joyx,joyy;
SDL_Joystick *joystick;
int x,y,button;
// Activa el Joystick<
if (SDL_NumJoysticks() >= 1) {
  joystick = SDL_JoystickOpen(0);
  SDL_JoystickEventState(SDL_ENABLE);
}
x = y = 100;
button = 0;
while(done == 0) {
  // lectura directa del joystick
  joyx = SDL_JoystickGetAxis(joystick, 0);
  joyy = SDL_JoystickGetAxis(joystick, 1);
  if (joyy < -10) {y-=5;}
  if (joyy > 10) {y+=5;}
  if (joyx < -10) {x-=5;}
  if (joyx > 10) {x+=5;}
  // Lectura de eventos
  while ( SDL_PollEvent(&event) ) {
    // salir del programa si se pulsa el boton del joystick
    if ( event.type == SDL_JOYBUTTONDOWN) {
      done = 1;
    }
    // salir del programa si se pulsa una tecla
    // o se cierra la ventana
    if ( event.type == SDL_KEYDOWN || event.type == SDL_QUIT)
      done = 1;
    }
}
  // cerramos el joystick
  if (SDL_JoystickOpened(0)) {
    SDL_JoystickClose(joystick);
  }
  return 0;
}
  
*/

using namespace std;
GLint W = 600, H = 600;//pantalla
GLint WT = 600, HT = 600;//pantalla tierra grande
GLint Wt = 600*0.15, Ht = 600*0.15;//pantalla tierra pequeña

constexpr int OPENGL_MAJOR_VERSION = 2;
constexpr int OPENGL_MINOR_VERSION = 1;
const int JOYSTICK_DEAD_ZONE = 8000;
constexpr SDL_GLprofile OPENGL_PROFILE = SDL_GLprofile::SDL_GL_CONTEXT_PROFILE_CORE;
GLint angulo = 0;
GLfloat LightAmbient[]= { 0.5f, 0.5f, 0.5f, 1.0f };
GLfloat LightDiffuse[]= { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat LightSpecular[]={ 0.5f, 0.5f, 0.5f, 1.0f };
GLfloat LightPosition[]= { 0.0f, 0.0f, 2.0f, 1.0f };
GLuint fogMode[]= { GL_EXP, GL_EXP2, GL_LINEAR };	//Tipos de niebla
GLuint fogfilter=1;					//Usado para el modo de niebla 
GLfloat fogColor[4] = {0.5f,0.5f,0.5f,1.0f};
GLfloat red[4] = {1.0,0.0,0.0,1.0};
GLfloat satelinea[4] = {1.0,1.0,0.0,1.0};
GLfloat white[4] = {1.0,1.0,1.0,1.0};
GLUquadric *quadObj;					//Dibujara la esfera
GLfloat lx = 0.0, ly = -12.0;				//Posiciones de la camara
GLfloat r=12.0;						//Distancia de la camara con respecto al centro de la tierra
GLfloat deltaMove = 0.0;          //Movimiento de la camara
GLfloat angle = 0.0;          //Angulo de rotacion de la camara
GLfloat deltaAngle = 0.0;         //Angulo de cambio
GLint isDragging = 0;           //Mouse arrastrandose
GLint xDragStart = 0;           //Guarda la posicion del mouse cuando se arrastra
GLint punto = 0;
GLfloat radioTierra = 4.0,radioTi2=16.055; // Radio de la tierra
GLfloat ratio = W/(GLfloat)H;
GLfloat xreal , yreal , zreal ; //coordenadas en unidades de OpenGL
GLuint texID[1];          //ID de la textura de la tierra
SDL_Window* displayWindow=NULL;       //Ventana principal
SDL_GLContext context;          //Aqui se dibuja
SDL_Joystick* gGameController = NULL;
GLint pixelX, pixelY;
GLdouble fW, fH;
GLint xx;
GLint ii=67;
GLint varANG=0;
GLint estadoR=0;
GLchar* textureFileNames[1] = {"mundo.png"};
GLint estado=0;
GLfloat rad=0;
Mix_Music *gMusic = NULL;       //La variable de la musica
bool quit = false;
int sat=0,cociente;
float rojo[3]={1.0,0.0,0.0};

float blanco[3]={0.6,0.6,0.6};
void * font1 = GLUT_BITMAP_TIMES_ROMAN_24;
void * font2 = GLUT_BITMAP_HELVETICA_18;
string linea1="Gram Schmit  :";
string linea2="Householder   :";
string linea3="Givens             :";
string linea4="Gauss Seidel   :";
string frase1;
string frase2;
string frase3;
string frase4;


bool init();

bool loadTextures();

bool loadMedia();

void handleKeys( SDL_Event& e );

int cad=1;
Uint32 rotacion(Uint32 interval, void *parm){
  angulo++;
  if(angulo>360)
  angulo=0;
    /*if(estadoR==1){
    if (angle>2*M_PI) angle=0;
    angle=angle+M_PI/360;
    lx = -sin(angle)*r;
    ly = -cos(angle)*r;
  }  */
  return interval;
}
 /*
void transforma(){
  GLfloat razon = 10.0/(GLfloat)WT;
  zreal = (H/2.0 - pixelY*1.0)*razon;
  xreal = (1.0*pixelX - W/2.0)*razon*1.0;
  yreal = -sqrt(radioTierra*radioTierra - (xreal*xreal+zreal*zreal));
}
*/
void renderBitmapString(float x, float y, float z, void *font){
  //char *c;
  glColor3f(1.0,1.0,1.0);
  glRasterPos3f(x,y,z);
  glutBitmapCharacter(font, 'H');
}

void transforma(){
  if(isDragging ==1){
    GLfloat razon = 10.0/(GLfloat)WT;
    zreal = (H/2.0 - pixelY*1.0)*razon*1.0;
    xreal = (1.0*pixelX - W/2.0)*razon*1.0;
    yreal = radioTi2 - (xreal*xreal+zreal*zreal);
    if(yreal<0)
      rad=1;
    else{
      yreal =-sqrt(yreal);
      rad=0;
    }
    //if(radioTi2<(xreal*xreal+zreal*zreal+yreal*yreal))
    //  rad=1;
    //else rad=0;
  }
}
bool init()
{
  bool success = true;
  //Inicializamos SDL, video y audio
  if( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER | SDL_INIT_JOYSTICK) < 0 )
  {
    std::cerr << "There was an error initing SDL2: " << SDL_GetError() << std::endl;
    success = false;
  }
  else
  {
    //Uso de OpenGL 2.1
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, OPENGL_PROFILE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, OPENGL_MAJOR_VERSION);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, OPENGL_MINOR_VERSION);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    cout.precision(16);
    //Creacion de la ventana
    displayWindow = SDL_CreateWindow("GPS", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, W, H, SDL_WINDOW_OPENGL|SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if( displayWindow == NULL )
    {
      std::cerr << "There was an error creating the window: " << SDL_GetError() << std::endl;
      success = false;
    }
    else
    {
      //Creamos el lienzo donde se dibujara
      context = SDL_GL_CreateContext(displayWindow);
      if( context == NULL )
      {
        std::cerr << "There was an error creating OpenGL context: " << SDL_GetError() << std::endl;
        success = false;
      }
      else
      {
        const unsigned char *version = glGetString(GL_VERSION);
        if (version == NULL)
        {
          std::cerr << "There was an error with OpenGL configuration:" << std::endl;
          success = false;
        }
        else
        {
          //Cargamos las extensiones de las musicas
          int flags=MIX_INIT_MP3|MIX_INIT_MOD;
          int initted=Mix_Init(flags);
          if(initted&flags != flags)
          {
            std::cerr << "Mix_Init: Failed to init required mp3 and mod support!\nMix_Init: " << Mix_GetError() << std::endl;
            success = false;
          }
          else
          {
            if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 )
            {
              std::cerr << "SDL_mixer could not initialize! SDL_mixer Error: " << Mix_GetError() << std::endl;
              success = false;
            }
            else{
              if( SDL_NumJoysticks() < 1 )
              {
                std::cerr << "Warning: No joysticks connected!" << std::endl;
              }
              else
              {
                //Load joystick
                gGameController = SDL_JoystickOpen( 0 );
                if( gGameController == NULL )
                {
                  std::cerr << "Warning: Unable to open game controller! SDL Error: " << SDL_GetError() <<std::endl;
                }
                else
                  SDL_JoystickEventState(SDL_ENABLE);
              }
            }
          }
        }     
      }
    }
  }
  return success;
}

class satelite
{
public:
  float x;
  float y;
  float z;
  float dist;
  satelite*next;
  satelite(float xx,float yy,float zz){
    x=xx;
    y=yy;
    z=zz;
    next=NULL;
  };
  void linea(float x,float y,float z);
  ~satelite(){};
  void graficar();
  //void mover();
};
int kk=0;

void satelite::graficar(){
  glPushMatrix();
    glColor3f(0,0,0.75);
    glTranslatef(x, y, z);
    glutSolidCube(0.1);
  glPopMatrix();
}
void satelite::linea(float x,float y,float z){
  glLineWidth(3);
  glBegin(GL_LINES);
    //glColor3f(1,0,0);
    glMaterialfv(GL_FRONT, GL_EMISSION, satelinea);
    glVertex3f(this->x,this->y,this->z);
    glVertex3f(x,y,z);
  glEnd();

}
class chasquis
{

public:
satelite*inicio;
int nm;
  chasquis(int n){
    double var=(360.0/((n/10)))*(M_PI/180);
    double nx,ny,nz;
    double avang=1;
    double r=5;
    nx=r*cos(avang);
    ny=r*sin(avang);
    nz=-4;
    nm=n;

    inicio=new satelite(nx,ny,nz);
    satelite*aux=inicio;
    float t=0;
    float k=1;
    for (int i = 1; i < n; ++i)
    { 
      var=(2+rand()%20)*M_PI/180+1.0/(1+rand()%1000);
      avang+=var;
      if(nz<-3||nz>3)
        r=3.5;
      else r=5;
      nx=r*cos(avang);
      ny=r*sin(avang);
      nz=nz+8.0/n;
      t+=8.0/n;
      aux->next=new satelite(nx,ny,nz);
      aux=aux->next;
    }
  };
  ~chasquis(){
    satelite*aux;
    int i=0;
    while(this->inicio!=NULL)
    { 
      aux=this->inicio;
      this->inicio=this->inicio->next;
      aux->~satelite();
    }

  };
  void graficar();
  void mascercanos(float x,float y,float z);
};


void chasquis::graficar(){
  satelite*aux= this->inicio;
  while(aux!=NULL)
  {
    aux->graficar();
    aux=aux->next;
  }

}
void chasquis::mascercanos(float x,float y,float z){
    satelite* menores[nm];
    satelite*aux=this->inicio;
    int k=0;
    while(aux!=NULL){
      aux->dist=sqrt(pow((aux->x-x),2)+pow((aux->y-y),2)+pow((aux->z-z),2));
      menores[k]=aux;
      aux=aux->next;
      k++;
    }
    for (int i = 0; i < 4; ++i)
      for (int j = i; j <nm ; ++j)
      {
        if(menores[i]->dist>menores[j]->dist){
         aux=menores[i];
         menores[i]=menores[j];
         menores[j]=aux;

        }
      }
    menores[0]->linea(x,y,z);
    menores[1]->linea(x,y,z);
    menores[2]->linea(x,y,z);
    menores[3]->linea(x,y,z);
    //####cargado de archivos
    ofstream coordSat;
    coordSat.open("satelites.csv");
      coordSat<<menores[0]->x<<","<<menores[0]->y<<","<<menores[0]->z<<","<<menores[1]->x<<","<<menores[1]->y<<","<<menores[1]->z<<","<<
      menores[2]->x<<","<<menores[2]->y<<","<<menores[2]->z<<","<<menores[3]->x<<","<<menores[3]->y<<","<<menores[3]->z;
    coordSat.close();
    ofstream Punto;
    Punto.open("punto.csv");
      Punto<<x<<","<<y<<","<<z;
    Punto.close();
	  //####cargado de archivos

}

bool loadTextures() {
  int i;
  bool success=true;
  glGenTextures(1, texID);  // Obtener el Id textura .
  for (i = 0; i < 1; i++) {
    void* imgData;    // Puntero a data del Archivo.
    int imgWidth;   // Ancho de Imagen
    int imgHeight;    // Alto.
    FREE_IMAGE_FORMAT format = FreeImage_GetFIFFromFilename(textureFileNames[i]);
    if (format == FIF_UNKNOWN) {
      cout<<"Archivo de Imagen desconocido " << textureFileNames[i]<<endl;
      success=false;
      continue;
    }
    FIBITMAP* bitmap = FreeImage_Load(format, textureFileNames[i], 0);  //Leer Imagen.
    if (!bitmap) {
      cout<<"Fallo la carga de image "<< textureFileNames[i]<<endl;
      success=false;
      continue;
    }
    FIBITMAP* bitmap2 = FreeImage_ConvertTo24Bits(bitmap);  // Convierte a RGB
    FreeImage_Unload(bitmap);
    imgData = FreeImage_GetBits(bitmap2);
    imgWidth = FreeImage_GetWidth(bitmap2);
    imgHeight = FreeImage_GetHeight(bitmap2);
    if (imgData) {
      cout<<"Texture cargada "<<textureFileNames[i]<<", tamanio "<< imgWidth<<"x"<< imgHeight<<endl;
      int format;   // Formato del color.
      if ( FI_RGBA_RED == 0 )
        format = GL_RGB;
      else
        format = GL_BGR;
      glBindTexture( GL_TEXTURE_2D, texID[i] ); // Cargando textura
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imgWidth, imgHeight, 0, format,GL_UNSIGNED_BYTE, imgData);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }
    else {
      cout<<"Fallo la carga de textura "<< textureFileNames[i]<<endl;
      success=false;
    }
  }
  return success;
}


bool loadMedia()
{
  bool success = true;
  //Se carga la musica
  success=loadTextures();   //cargamos las imagenes
  gMusic = Mix_LoadMUS( "odisea.mp3" ); //cargamos la musica
  if( gMusic == NULL )
  {
    std::cerr << "Failed to load beat music! SDL_mixer Error: " << Mix_GetError() << std::endl;
    success = false;
  } 
  return success;
}
chasquis*c1=new chasquis(200);
void handleKeys( SDL_Event& e )
{
  //Si un boton es presionado
  if( e.type == SDL_KEYDOWN && e.key.repeat == 0 )
  {
    //Activamos el movimiento
    switch( e.key.keysym.sym )
    {
      case SDLK_UP: deltaMove = 1.0;
                    break;
      case SDLK_DOWN: deltaMove = -1.0;break;

    }
  }
  //Si el boton es soltado 
  else if( e.type == SDL_KEYUP && e.key.repeat == 0 )
  {
    //Desactivamos el movimiento
    switch( e.key.keysym.sym )
    {
      case SDLK_UP: deltaMove = 0.0; break;
      case SDLK_DOWN: deltaMove = 0.0; break;
      case SDLK_s:sat=(sat+1)%2 ;break;
      case SDLK_c:cad=(cad)%4+1 ;break;
      case SDLK_ESCAPE: quit=true; break;
      case SDLK_MINUS: break;
      case SDLK_KP_MINUS: break;
      case SDLK_PLUS: break;
      case SDLK_KP_PLUS: break;
      case SDLK_1: estado=(1+estado)%2; cout<<estado;
        break;
      case SDLK_9:
      //Si aun no hay musica iniciada
      if( Mix_PlayingMusic() == 0 )
      {
        //Play cancion
        Mix_PlayMusic( gMusic, -1 );
      }
      //Si alguna cancion esta siendo reproducida
      else
      {
        //Si la cancion esta pausada
        if( Mix_PausedMusic() == 1 )
        {
          //Continua la musica
          Mix_ResumeMusic();
        }
        //Si la cancion esta siendo reproducida
        else
        {
          //Pausa la musica
          Mix_PauseMusic();
        }
      }
      break;
    }
  }
  else if( e.type == SDL_MOUSEMOTION || e.type == SDL_MOUSEBUTTONDOWN || e.type == SDL_MOUSEBUTTONUP )
  {
    //Tomamos la posicion del mouse
    int x, y;
    SDL_GetMouseState( &x, &y );
    
    switch( e.type )
    {
      case SDL_MOUSEMOTION:
      if (isDragging)
      {
        deltaAngle = (x - xDragStart) * M_PI/300;
        //actualizamos la posicon de la camara
        lx = -sin(angle + deltaAngle)*r;
        ly = -cos(angle + deltaAngle)*r;
      }     
      break;    
      case SDL_MOUSEBUTTONDOWN:
      isDragging = 1; //Empieza el desplazamiento
      xDragStart = x;
      pixelX = x;
      pixelY = y;
      transforma();
    	if(xreal <= radioTierra && yreal <= radioTierra && zreal <= radioTierra){  

  		}
  		
  		punto = 1;
      if(rad==0){system("python2.7 GPSFinal3.py");
      ifstream fichero;
    
  fichero.open ("datosLatLong.csv");
  if(fichero.is_open()) {
    string f1;
    string f2;
    string f3;
    string f4;
    getline(fichero, f1);
    getline(fichero, f2);
    getline(fichero, f3);
    getline(fichero, f4);
    fichero.close();
    frase1.swap(f1);
    frase2.swap(f2);
    frase3.swap(f3);
    frase4.swap(f4);
  }
  else{
    printf("Imposible de abrir.\n");
  }
    }
      break;  
      case SDL_MOUSEBUTTONUP:
        if(x - xDragStart != 0){
          angle += deltaAngle; //actualizamos el angulo
          cociente=(int)(angle/(2*M_PI));
          angle-=cociente*2*M_PI;
          punto=0;
        }
        else  punto=1;
        isDragging = 0;
        cout << "Angulo: " << angle << endl;
      break;
    } 
  }
  else if( e.type == SDL_JOYAXISMOTION )
  {
    int xDir = 0;
    int yDir = 0;
    //Motion on controller 0
    if( e.jaxis.which == 0 )
    {						
      //X axis motion
      if( e.jaxis.axis == 0 )
      {
        //Left of dead zone
        if( e.jaxis.value < -JOYSTICK_DEAD_ZONE )
        {
          xDir = -1;
        }
        //Right of dead zone
        else if( e.jaxis.value > JOYSTICK_DEAD_ZONE )
        {
          xDir =  1;
        }
        else
        {
          xDir = 0;
        }
      }
      //Y axis motion
      else if( e.jaxis.axis == 1 )
      {
        //Below of deadjjjjj zone
        if( e.jaxis.value < -JOYSTICK_DEAD_ZONE )
        {
          yDir = -1;
        }
        //Above of dead zone
        else if( e.jaxis.value > JOYSTICK_DEAD_ZONE )
        {
          yDir =  1;
        }
        else
        {
          yDir = 0;
        }
      }
    }
  }
}

void perspectiveGL(GLdouble fovY, GLdouble aspect, GLdouble zNear, GLdouble zFar) {
  
  fH = tan(fovY/2) * zNear;
  fW = fH * aspect;
  glFrustum(-fW, fW, -fH, fH, zNear, zFar);
}

bool Display_InitGL() {
  bool success = false;
  GLenum error = GL_NO_ERROR;
  glEnable(GL_TEXTURE_2D);  //habilitamos el uso de texturas
  glShadeModel( GL_SMOOTH); //Habilitamos el smooth
  glClearColor(0, 0, 0, 0.5); //Ponemos el background de color negro
  glClearDepth(1.0f);   //Definimos el Depth buffer
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);
  glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
  glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient); //Luz Ambiental
  glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse); //Luz Difusa
  glLightfv(GL_LIGHT1, GL_DIFFUSE, LightSpecular);
  glLightfv(GL_LIGHT1, GL_POSITION,LightPosition);  //Posicion de la luz
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT1);
  glEnable(GL_NORMALIZE);
  quadObj = gluNewQuadric();
  gluQuadricNormals(quadObj, GLU_SMOOTH);
  gluQuadricTexture(quadObj, GL_TRUE);
  glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);  // Set The Texture Generation Mode For S To Sphere Mapping (NEW)
  glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);  // Set The Texture Generation Mode For T To Sphere Mapping (NEW)
  glFogi(GL_FOG_MODE, fogMode[fogfilter]);    // Fog Mode
  glFogfv(GL_FOG_COLOR, fogColor);      // Set Fog Color
  glFogf(GL_FOG_DENSITY, 0.01f);
  glHint(GL_FOG_HINT, GL_DONT_CARE);      // Fog Hint Value
  glFogf(GL_FOG_START, 1.0f);       // Fog Start Depth
  glFogf(GL_FOG_END, 5.0f);       // Fog End Depth
  glEnable(GL_FOG);
  
  if( error != GL_NO_ERROR )
  {
    printf( "Error initializing OpenGL! %s\n", gluErrorString( error ) );
    success = true;
  }
  return success;
}

int Display_SetViewport(int width, int height) {
  /* Height / width ration */
  GLfloat ratio;
  SDL_GetWindowSize(displayWindow,&width,&height);
  /* Protect against a divide by zero */
  if (height == 0) {
    height = 1;
  }
  W=width;
  H=height;

  if(W<H){
    WT=W;
    HT=W;
    Wt=W*0.25f;
    Ht=W*0.25f;
  }
  else{
    WT=H;
    HT=H;
    Wt=H*0.25f;
    Ht=H*0.25f;
  }
  
  ratio = (GLfloat) width / (GLfloat) height;
  /* Setup our viewport. */
  glViewport(0, 0, (GLsizei) width, (GLsizei) height);
  /* change to the projection matrix and set our viewing volume. */
  glMatrixMode( GL_PROJECTION);
  glLoadIdentity();
  /* Set our perspective */
  //perspectiveGL(M_PI/3, ratio, 0.1f, 100.0f);
  glOrtho(-5,5,-5,5,0.1f,100.0f);
  /* Make ure we're chaning the model view and not the projection */
  glMatrixMode( GL_MODELVIEW);
  /* Reset The View */
  glLoadIdentity();
  return 1;
}

void Display_Render(SDL_Window* displayWindow) {
  int i;
  /* Clear The Screen And The Depth Buffer */
  glLoadIdentity();
  glClearColor(0.0, 0.0, 0.0, 0.75);
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  gluLookAt(lx, ly, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0);
  glBindTexture(GL_TEXTURE_2D, texID[0]);
  glViewport( (W-WT)/2.0f, (H-HT)/2.0f, WT, HT );

  gluSphere (quadObj, radioTierra,40,40);
  //######################################INICIOMODIFICACION########################################
  if(sat==1) c1->graficar();
      char output[80];
      char output2[80];
     ifstream fileLat;

 if(punto == 1&&rad==0){
  glPushMatrix();
  	glTranslatef(xreal*cos(-angle) - yreal*sin(-angle),yreal*cos(-angle) + xreal*sin(-angle),zreal);
    glMaterialfv(GL_FRONT, GL_EMISSION, rojo);
  	glutSolidSphere(0.1,20.0,20.0);
  glPopMatrix();
  c1->mascercanos(xreal*cos(-angle) - yreal*sin(-angle),yreal*cos(-angle) + xreal*sin(-angle),zreal);
  glPushMatrix();
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
      glLoadIdentity();
      gluOrtho2D(0.0, W, 0.0, H);
      glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
      glLoadIdentity();
      glColor3fv(rojo);
  glMaterialfv(GL_FRONT, GL_EMISSION, rojo);
     glRasterPos2i(24, H-24);
     switch(cad){
       case 1:
              for (string::iterator i = linea1.begin(); i != linea1.end(); ++i){
                char c = *i;
                  glutBitmapCharacter(font1, c);
              }
              for (string::iterator i = frase1.begin(); i != frase1.end(); ++i){
       char c = *i;
       glutBitmapCharacter(font1, c);
    }
              break;
       case 2:
    
    //glRasterPos2i(24, H-52);
    for (string::iterator i = linea2.begin(); i != linea2.end(); ++i){
       char c = *i;
       glutBitmapCharacter(font1, c);
    }
    for (string::iterator i = frase2.begin(); i != frase2.end(); ++i){
       char c = *i;
       glutBitmapCharacter(font1, c);
    }break;
    //glRasterPos2i(24, H-80);
case 3:
    for (string::iterator i = linea3.begin(); i != linea3.end(); ++i){
       char c = *i;
       glutBitmapCharacter(font1, c);
    }
    for (string::iterator i = frase3.begin(); i != frase3.end(); ++i){
       char c = *i;
       glutBitmapCharacter(font1, c);
    }break;
case 4:
    //glRasterPos2i(24, H-108);
    for (string::iterator i = linea4.begin(); i != linea4.end(); ++i){
       char c = *i;
       glutBitmapCharacter(font1, c);
    }
    for (string::iterator i = frase4.begin(); i != frase4.end(); ++i){
       char c = *i;
       glutBitmapCharacter(font1, c);
    }break;
}

      glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
  glPopMatrix();
glPopMatrix();
}


glMaterialfv(GL_FRONT, GL_EMISSION, blanco);


glMatrixMode( GL_PROJECTION);
  glLoadIdentity();
  /* Set our perspective */
  //perspectiveGL(M_PI/3, ratio, 0.1f, 100.0f);
  glOrtho(-5,5,-5,5,0.1f,100.0f);
  /* Make ure we're chaning the model view and not the projection */
  glMatrixMode( GL_MODELVIEW);
  /* Reset The View */
  glLoadIdentity();

glViewport( W*0.75f, H*0.75f, Wt, Ht );
//glViewport( (W-Wt)/2.0f, (H-HT)/2.0f, WT, HT );
glLoadIdentity ();
gluLookAt(0, -5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0);

glClearColor(0.0, 0.0, 0.0, 0.75);
glPushMatrix();
glRotatef(angulo, 0.0, 0.0, 1.0);
gluSphere (quadObj, 4,20,20);
glPopMatrix();



//lx = 0.0, ly = -12.0;
  SDL_GL_SwapWindow(displayWindow);//Actualiza el dibujo
}

void close()
{
  Mix_FreeMusic( gMusic );
  gMusic = NULL;
  //Destroy window
  SDL_GL_DeleteContext(context);
  SDL_DestroyWindow(displayWindow);
  displayWindow = NULL;
  Mix_Quit();
  SDL_Quit(); 
}

void update(void) 
{
  if (deltaMove) { // update camera position
    r += -(float)deltaMove * 0.1;
    lx = -sin(angle)*r;
    ly = -cos(angle)*r;
    //ly += deltaMove * 0.1;
  }
}

int main(int argc, char *argv[]) {
  srand(time(NULL));
  glutInit(&argc, argv);
  if( !init() )
  {
    printf( "Failed to initialize!\n" );
  }
  else
  {
    if( !loadMedia() )
    {
      printf( "Failed to load media!\n" );
    }
    else{
      
      SDL_GL_MakeCurrent(displayWindow, context);
      quit=Display_InitGL();
      //Event handler
      SDL_Event e;
      SDL_TimerID timerID = SDL_AddTimer(25, rotacion, NULL);
      while( !quit )
      {
        Display_SetViewport(W, H);
        //Handle events on queue
        while( SDL_PollEvent( &e ) != 0 )
        {
          //User requests quit
          if( e.type == SDL_QUIT )
          {
            quit = true;
          }
          handleKeys(e );
        }
        update();
        Display_Render(displayWindow);
      }
    SDL_RemoveTimer( timerID );
    }
  }
  close();
  c1->~chasquis();
  return 0;
}
