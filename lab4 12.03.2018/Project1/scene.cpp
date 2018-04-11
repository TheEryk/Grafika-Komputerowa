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
	rot_x = 0.0;
	rot_y = 0.0;
	Axes = NULL;
	Cube = NULL;
	Egg = NULL;
	LightAmbient = 0.5f;
	
}
//--------------------------------------------------------------------------------------------
// Domyslny destruktor 
Scene::~Scene()
{
	// usun program przetwarzania 
	if (glIsProgram(program)) glDeleteProgram(program);
	if (Axes) delete Axes;
	if (Cube) delete Cube;	
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
        // pobranie i wy�wietlenie komunikatu b��du 
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
    // sprawdzenie poprawno�ci walidacji obiektu programu 
    glGetProgramiv( program, GL_VALIDATE_STATUS, &validate_status ); 
    if( validate_status == GL_FALSE ) 
    { 
        // pobranie i wy�wietlenie komunikatu b��du 
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
void Scene::PrepareObjects()
{  
	Cube = new glObject();
	Axes = new glObject();
	Egg = new glObject();

	Axes->BeginObject(GL_LINES);
	Axes->SetColor(1.0,0.0,0.0); // os X w kolorze czerwonym
	Axes->AddVertex(0.0,0.0,0.0);
	Axes->AddVertex(10.0,0.0,0.0);
	Axes->SetColor(0.0,1.0,0.0); // os Y w kolorze zielonym 
	Axes->AddVertex(0.0,0.0,0.0);
	Axes->AddVertex(0.0,10.0,0.0);
	Axes->SetColor(0.0,0.0,1.0); // os Z w kolorze niebieskim 
	Axes->AddVertex(0.0,0.0,0.0);
	Axes->AddVertex(0.0,0.0,10.0);
	Axes->EndObject();

	Cube->SetColor(0.0,0.5,0.0);	

	// sciany prostopadle do OX
	
	Cube->BeginObject(GL_TRIANGLE_STRIP);
	Cube->SetNormal(1.0, 0.0, 0.0);
	Cube->AddVertex(0.5,0.5,0.5);	
	Cube->AddVertex(0.5,-0.5,0.5);	
	Cube->AddVertex(0.5,0.5,-0.5);	
	Cube->AddVertex(0.5,-0.5,-0.5);	
	Cube->EndObject();



	Cube->BeginObject(GL_TRIANGLE_STRIP);	
	Cube->SetNormal(-1.0, 0.0, 0.0);
	Cube->AddVertex(-0.5,0.5,0.5);	
	Cube->AddVertex(-0.5,-0.5,0.5);	
	Cube->AddVertex(-0.5,0.5,-0.5);	
	Cube->AddVertex(-0.5,-0.5,-0.5);	
	Cube->EndObject();

	// sciany prostopadle do OY

	Cube->BeginObject(GL_TRIANGLE_STRIP);	
	Cube->SetNormal(0.0, 1.0, 0.0);
	Cube->AddVertex(-0.5,0.5,-0.5);	
	Cube->AddVertex(-0.5,0.5,0.5);	
	Cube->AddVertex(0.5,0.5,-0.5);	
	Cube->AddVertex(0.5,0.5,0.5);	
	Cube->EndObject();


	Cube->BeginObject(GL_TRIANGLE_STRIP);	
	Cube->SetNormal(0.0, -1.0, 0.0);
	Cube->AddVertex(-0.5,-0.5,-0.5);	
	Cube->AddVertex(-0.5,-0.5,0.5);	
	Cube->AddVertex(0.5,-0.5,-0.5);	
	Cube->AddVertex(0.5,-0.5,0.5);	
	Cube->EndObject();

	// sciany prostopadle do OZ

	Cube->BeginObject(GL_TRIANGLE_STRIP);	
	Cube->SetNormal(0.0, 0.0, 1.0);
	Cube->AddVertex(-0.5,0.5,0.5);	
	Cube->AddVertex(-0.5,-0.5,0.5);	
	Cube->AddVertex(0.5,0.5,0.5);	
	Cube->AddVertex(0.5,-0.5,0.5);	
	Cube->EndObject();


	Cube->BeginObject(GL_TRIANGLE_STRIP);	
	Cube->SetNormal(0.0, 0.0, -1.0);
	Cube->AddVertex(-0.5,0.5,-0.5);	
	Cube->AddVertex(-0.5,-0.5,-0.5);	
	Cube->AddVertex(0.5,0.5,-0.5);	
	Cube->AddVertex(0.5,-0.5,-0.5);	
	Cube->EndObject();

	Egg->MakeEgg(3.5,1.3,50,50);



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
	
	mProjection = glm::perspective(45.0f,(GLfloat)width/(GLfloat)height,0.1f,100.0f);
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
	// inicjalizacja modu�u glew
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		sprintf(_msg, "GLew error: %s\n", glewGetErrorString(err));	
		ThrowException(_msg);
	}
	
	// pobierz informacje o wersji openGL 
	sprintf(_msg,"OpenGL vendor: ");
	strcat(_msg,(const char*)glGetString( GL_VENDOR ));
	PrintLog(_msg);

	sprintf(_msg,"OpenGL renderer: ");
	strcat(_msg,(const char*)glGetString( GL_RENDERER));
	PrintLog(_msg);

	sprintf(_msg,"OpenGL version: ");
	strcat(_msg,(const char*)glGetString( GL_VERSION));
	PrintLog(_msg);

	//  ustaw kolor tla sceny (RGB Z=1.0)
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	
	// przygotuj programy shaderow
	PreparePrograms();

	// przygotuj obiekty do wyswietlenia 
	PrepareObjects();
	glEnable(GL_DEPTH_TEST);
	glClearDepth(1.0);
}
//--------------------------------------------------------------------------------------------
// kontrola naciskania klawiszy klawiatury
void Scene::KeyPressed(unsigned char key, int x, int y) 
{            
	if (key == ESCAPE) ThrowException((char*)"Zatrzymaj program");

	switch(key)
	{
		case 37: {rot_y -= 5.0f; break;}
		case 38: {rot_x -= 5.0f; break;}
		case 39: {rot_y += 5.0f; break;}
		case 40: {rot_x += 5.0f; break;}		
		case 'Q': {LightAmbient += 0.1f; break; }		 // F1
		case 'A': {LightAmbient -= 0.1f; break; }		// F2
	}
	
}
//--------------------------------------------------------------------------------------------
// rysuje scene OpenGL 
void Scene::Draw()
{	
		// czyscimy bufor kolorow
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		int _ModelView = glGetUniformLocation(program, "modelViewMatrix");
		int _Projection = glGetUniformLocation(program, "projectionMatrix");
		glUniformMatrix4fv(_Projection, 1, GL_FALSE, glm::value_ptr(mProjection));
		glm::mat4 mModelView = glm::lookAt(glm::vec3(5.0f, 5.0f, 5.0f),
			glm::vec3(0.0f),
			glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(_ModelView, 1, GL_FALSE, glm::value_ptr(mModelView));
		glm::vec3 LightDirection = glm::vec3(0.0, -1.0, 0.0); // kierunek swiatla
		int _LightDirection = glGetUniformLocation(program, "LightDirection");
		glUniform3fv(_LightDirection, 1, glm::value_ptr(LightDirection));
		glm::vec3 LightColor = glm::vec3(1.0, 1.0, 1.0); // kolor swiatla
		int _LightColor = glGetUniformLocation(program, "LightColor");
		glUniform3fv(_LightColor, 1, glm::value_ptr(LightColor));
		int _LightAmbient = glGetUniformLocation(program, "LightAmbient");
		glUniform1f(_LightAmbient, LightAmbient);
		int _NormalMatrix = glGetUniformLocation(program, "normalMatrix");
		glm::mat4 mTransform = glm::mat4(1.0);
		glUniformMatrix4fv(_NormalMatrix, 1, GL_FALSE,
			glm::value_ptr(glm::transpose(glm::inverse(mTransform))));
		Axes->Draw();
		mTransform = glm::rotate(glm::mat4(1.0), rot_x, glm::vec3(1.0f, 0.0f, 0.0f));
		mTransform = glm::rotate(mTransform, rot_y, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(_NormalMatrix,
			1,
			GL_FALSE,
			glm::value_ptr(glm::transpose(glm::inverse(mTransform))));
		glUniformMatrix4fv(_ModelView, 1, GL_FALSE, glm::value_ptr(mModelView*mTransform));
		Egg->Draw();
}	
//------------------------------- KONIEC PLIKU -----------------------------------------------