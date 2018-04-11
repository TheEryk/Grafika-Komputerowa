//--------------------------------------------------------------------------------------------
//
// File: 	scene.cpp
// Author:	P. Katarzynski (CCE)
//
// Description: Implementacja klasy sceny OpenGL
//
//--------------------------------------------------------------------------------------------
//									ZALEZNOSI 
//--------------------------------------------------------------------------------------------

#include "scene.h"

//--------------------------------------------------------------------------------------------
// zglasza wyjatek z komunikatem do debuggowania 

//--------------------------------------------------------------------------------------------
Scene::Scene(int new_width,int new_height)
{	
	width = new_width;
	height = new_height;	
}
//--------------------------------------------------------------------------------------------
// Domyslny destruktor 
Scene::~Scene()
{
	// usun program przetwarzania 
	if (glIsProgram(program)) glDeleteProgram(program);
	
	// usun bufory 
	glDeleteBuffers(VBO_cnt,VBOs); 

	// usun tablice atrybutow wierzcholkow 
	glDeleteVertexArrays(VAO_cnt,VAOs);
}
//--------------------------------------------------------------------------------------------
// przygotowuje programy cienionwania 
void Scene::PreparePrograms()
{
	program  = glCreateProgram();
	if (!glIsProgram(program)) ThrowException((char*)"Nie udalo sie utworzyc programu");
	
	vertex_shader = LoadShader(GL_VERTEX_SHADER,"vs.glsl");
	glAttachShader(program,vertex_shader);

	fragment_shader = LoadShader(GL_FRAGMENT_SHADER,"fs.glsl");
	glAttachShader(program,fragment_shader);
	
	// linkowanie programu 
	glLinkProgram(program);

	GLint link_status; 
    glGetProgramiv( program, GL_LINK_STATUS, &link_status); 
    if( link_status == GL_FALSE ) 
    { 
        // pobranie i wyœwietlenie komunikatu b³êdu 
        GLint logLength; 
        glGetProgramiv( program, GL_INFO_LOG_LENGTH, &logLength ); 
        char *log = new char[logLength]; 
        glGetProgramInfoLog( program, logLength, NULL, log ); 
        PrintLog(log);
        delete[] log; 
		ThrowException((char*)"Blad linkowania programu");
    }
	else
		PrintLog((char*)"Program zlinkowany");

	// walidowanie programu 
	glValidateProgram(program); 
	GLint validate_status;
    // sprawdzenie poprawnoœci walidacji obiektu programu 
    glGetProgramiv( program, GL_VALIDATE_STATUS, &validate_status ); 
    if( validate_status == GL_FALSE ) 
    { 
        // pobranie i wyœwietlenie komunikatu b³êdu 
        GLint logLength; 
        glGetProgramiv( program, GL_INFO_LOG_LENGTH, &logLength ); 
        char *log = new char[logLength]; 
        glGetProgramInfoLog( program, logLength, NULL, log ); 
        PrintLog(log);
        delete[] log; 
        ThrowException((char*)"Blad walidacji programu");
    } 
	else
		PrintLog((char*)"Program prawidlowy");
	
	glUseProgram(program);
}
//--------------------------------------------------------------------------------------------
// przygotowuje obiekty do wyswietlenia 
void Scene::PrepareObjects()
{  
	// przygotuj tablice VAO
	glGenVertexArrays(VAO_cnt, VAOs); 

	// przygotuj bufory VBO
	glGenBuffers(VBO_cnt, VBOs); 
	
	// TODO: uzupelnij kod metody tutaj 
}
//--------------------------------------------------------------------------------------------
// Odpowiada za skalowanie sceny przy zmianach rozmiaru okna
void Scene::Resize(int new_width, int new_height)
{
	// przypisz nowe gabaryty do pol klasy 
	width = new_width;
	// uwzgledniaj obecnosc kontrolki wizualnej 
	height = new_height-100; 	
  	// rozszerz obszar renderowania do obszaru o wymiarach 'width' x 'height'
	glViewport(0, 100, width, height);	
}
//--------------------------------------------------------------------------------------------
// laduje program shadera z zewnetrznego pliku 
GLuint Scene::LoadShader(GLenum type,const char *file_name)
{  
  // zmienna plikowa 
  FILE *fil = NULL; 
  // sproboj otworzyc plik 
  fil = fopen(file_name,"rb");
  // sprawdz, czy plik sie otworzyl  
  sprintf(_msg,"Nie mozna otworzyc %s",file_name);
  if (fil == NULL)  ThrowException(_msg);

  // okresl rozmiar pliku
  fseek( fil, 0, SEEK_END );
  long int file_size = ftell(fil); 
  // przewin na poczatek 
  rewind(fil); 
  // utworzenie bufora na kod zrodlowy programu shadera
  GLchar *srcBuf = new GLchar[(file_size+1)*sizeof(GLchar)];

  // przeczytanie kodu shadera z pliku 
  fread(srcBuf,1,file_size,fil);

  // zamknij plik 
  fclose(fil);

  // tekst programu MUSI miec NULL na koncu
  srcBuf[file_size] = 0x00;

  // utworzenie obiektu shadera
  GLuint shader = glCreateShader(type);

  // przypisanie zrodla do shadera 
  glShaderSource(shader,1,const_cast<const GLchar**>(&srcBuf),NULL);

  // sprzatanie 
  delete[] srcBuf;

  // proba skompilowania programu shadera 
  glCompileShader(shader);

  // sprawdzenie czy sie udalo 
  GLint compile_status;
  glGetShaderiv(shader,GL_COMPILE_STATUS,&compile_status);

  if (compile_status != GL_TRUE) // nie udalo sie 
  {
	    GLint logLength; 
        glGetShaderiv( shader, GL_INFO_LOG_LENGTH, &logLength ); 
        char *log = new char[logLength]; 
        glGetShaderInfoLog( shader, logLength, NULL, log ); 
		sprintf(_msg,"Blad kompilacji pliku shadera %s",file_name);
		PrintLog(_msg);
        PrintLog(log);
		ThrowException((char*)"Blad kompilacji shadera");
	    delete []log;
  }
  else
  {
	  sprintf(_msg,"Plik shadera %s skompilowany",file_name);
	  PrintLog(_msg);
  }
  
  return shader; // zwroc id shadera 
}
//--------------------------------------------------------------------------------------------
// inicjuje proces renderowania OpenGL
void Scene::Init()
{
	// inicjalizacja modu³u glew
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		sprintf(_msg, "GLew error: %s\n", glewGetErrorString(err));	
		ThrowException(_msg);
	}
	
	// TODO: uzupelnij kod metody tutaj 
	
	
}
//--------------------------------------------------------------------------------------------
// kontrola naciskania klawiszy klawiatury
void Scene::KeyPressed(unsigned char key, int x, int y) 
{            
	if (key == ESCAPE) ThrowException((char*)"Zatrzymaj program");
}
//--------------------------------------------------------------------------------------------
// generuje zbior wspolrzednych wielokata foremnego 
// r - promien okregu opisanego na wielokacie 
// n - liczba katow 
// VAO identyfikator VAO 
// VBO identyfikator VBO
void Scene::Polygon(int n, float r,GLuint VAO, GLuint VBO)
{
	if (n<3) ThrowException((char*)"Wielokat: nieprawidlowy parametr");
	
	// TODO: zmodyfikuj pomocnicze zmienne 
	float kat = 0.0;
	float dkat = 0.0; 

	float *coords=NULL; // tablica ze wspolrzednymi (x,y,z) wierzcholkow 
	
	// TODO: tutaj dokonaj alokacji tablicy coords 	

	for (int i = 0; i < n; i++)
	{
		//TODO: tutaj wpisz kod generujacy wspolrzedne wierzcholkow
	}
	
	// podlacz obiekt z VAO
	glBindVertexArray(VAO);
	
	// podlacz bufor VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	
	// wypelnij bufor wspolrzednymi wierzcholka 
	glBufferData(GL_ARRAY_BUFFER, n*3*sizeof(float), coords, GL_STATIC_DRAW);
	
	glEnableVertexAttribArray(0); 
	// powiaz dane z bufora ze wskazanym atrybutem
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);	

	delete []coords;
}
//--------------------------------------------------------------------------------------------

// rysuje scene OpenGL 
void Scene::Draw()
{

	// TODO: zapisz kod tutaj 	
	
}	
//------------------------------- KONIEC PLIKU -----------------------------------------------