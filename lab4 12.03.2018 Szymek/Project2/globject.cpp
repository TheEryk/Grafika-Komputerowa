#include "globject.h"

//--------------------------------------------------------------------------------------------
// domyslny konstruktor 
glObject::glObject()
{			
	Coords = (float *) malloc(sizeof(float));	
	Cols = (float *) malloc(sizeof(float));
	Normals = (float *) malloc(sizeof(float));
	nx = 1.0;
	ny = 0.0;
	nz = 0.0;
	col_r = 0.0;
	col_g = 0.0;
	col_b = 0.0;
	lVAO = 0;
}
//--------------------------------------------------------------------------------------------
// domyslny destruktor 
glObject::~glObject()
{	
	if (Coords) free(Coords);
	if (Cols) free(Cols);		
	if (Normals) free(Normals);
}
//--------------------------------------------------------------------------------------------
// ustawia aktualny kolor rysowania prymitywu 
void glObject::SetColor(float r, float g, float b)
{
	col_r = r;
	col_g = g; 
	col_b = b;
}
//--------------------------------------------------------------------------------------------
// ustawia aktualna normalna 
void glObject::SetNormal(float _nx, float _ny, float _nz)
{
	nx = _nx;
	ny = _ny; 
	nz = _nz;
}
//--------------------------------------------------------------------------------------------
// usuwa alokowane atrybuty
void glObject::CleanUp()
{
	lVAO = 0; 
}
//--------------------------------------------------------------------------------------------
// przygotowuje geometrie elipsoidy
void glObject::MakeEgg(GLfloat A,GLfloat k,GLfloat slices)
{
  GLfloat z = -10.0f;
  GLfloat z2;
  GLfloat x2;
  
  GLfloat plastry = 20.0f / slices;
  
  BeginObject(GL_TRIANGLES);
  
  // wierzcholki fasety 
  float v1[3];
  float v2[3];
  float v3[3];
  float v4[3];

  // normalna
  float N[3];
  
  for (GLfloat x = -10.0f; x < 10.0; x += plastry) // iteruj wzdluz OX az nie uzyskasz x=10
  {

	  for (GLfloat z = -10.0f; z < 10.0; z += plastry) // iteruj wzdluz OZ az nie uzyskasz z=10 
	   {
		   z2 = z + plastry; // kolejne wierzcholki
		   x2 = x + plastry;

		   v1[0] = x; v1[1] = (A-sqrt(x*x+z*z))*sin(k*(A - sqrt(x*x + z * z))); v1[2] = z;
		   v2[0] = x; v2[1] = (A - sqrt(x*x + z2 * z2))*sin(k*(A - sqrt(x*x + z2 * z2))); v2[2] = z2;
		   v3[0] = x2; v3[1] = (A - sqrt(x2*x2 + z2 * z2))*sin(k*(A - sqrt(x2*x2 + z2 * z2))); v3[2] = z2;
		   v4[0] = x2; v4[1] = (A - sqrt(x2*x2 + z * z))*sin(k*(A - sqrt(x2*x2 + z * z))); v4[2] = z;
		   

		   // pierwszy trojkat
		   
		   CalcNormal(v1, v2, v3, N);
		   Normalize(N);
		   SetNormal(N[0], N[1], N[2]);
		   AddVertex(v1[0],v1[1],v1[2]);
		   AddVertex(v2[0],v2[1],v2[2]);
		   AddVertex(v3[0],v3[1],v3[2]);
		   
		   //drugi trojkat		   
		   CalcNormal(v1, v3, v4, N);
		   Normalize(N);
		   SetNormal(N[0], N[1], N[2]);
		   AddVertex(v1[0],v1[1],v1[2]);
		   AddVertex(v3[0],v3[1],v3[2]);
		   AddVertex(v4[0],v4[1],v4[2]);

	   }
		    
  }
  EndObject();
}
//--------------------------------------------------------------------------------------------
// rozpoczyna tworzenie tablicy VAO dla danego prymitywu
void glObject::BeginObject(GLenum P)
{	
	
	lVAO++;
	// przypisz rodzaj prymitywu do narysowania VAO
	Primitives[lVAO-1] = P;

	// wyzeruj licznik wspolrzednych 
	lCoords[lVAO-1] = 0;
	Coords = (float *) malloc(sizeof(float));
	Cols = (float *) malloc(sizeof(float));
	Normals = (float *) malloc(sizeof(float));

	if (lVAO > MAX_VAO) ThrowException((char *)"Przekroczono maksymalna liczbe VAO w glObject");
	
	GLuint VAO_id[1];	
	// przygotuj tablice VAO
	glGenVertexArrays(1, VAO_id); 
	VAO[lVAO-1] = VAO_id[0];

	glBindVertexArray(VAO[lVAO-1]);

	GLuint VBO_id[3];	
	// przygotuj bufory VBO
	glGenBuffers(3, VBO_id);
	
	VBO[3*lVAO-3] = VBO_id[0];
	VBO[3*lVAO-2] = VBO_id[1];
	VBO[3*lVAO-1] = VBO_id[2];
	
}
//--------------------------------------------------------------------------------------------
// dodaje wierzcholek do listy
void glObject::AddVertex(float x, float y, float z)
{
	lCoords[lVAO-1] +=3;
	Coords = (float *) realloc(Coords, lCoords[lVAO-1]*sizeof(float));
	if (Coords == NULL) ThrowException((char *)"glObject:: Blad realokacji pamieci");
	Coords[lCoords[lVAO-1]-3] = x;
	Coords[lCoords[lVAO-1]-2] = y;
	Coords[lCoords[lVAO-1]-1] = z;

	Cols = (float *) realloc(Cols, lCoords[lVAO-1]*sizeof(float));
	if (Cols == NULL) ThrowException((char *)"glObject:: Blad realokacji pamieci");
	Cols[lCoords[lVAO-1]-3] = col_r;
	Cols[lCoords[lVAO-1]-2] = col_g;
	Cols[lCoords[lVAO-1]-1] = col_b;	

	Normals = (float *) realloc(Normals, lCoords[lVAO-1]*sizeof(float));
	if (Normals == NULL) ThrowException((char *)"glObject:: Blad realokacji pamieci");
	Normals[lCoords[lVAO-1]-3] = nx;
	Normals[lCoords[lVAO-1]-2] = ny;
	Normals[lCoords[lVAO-1]-1] = nz;	
}
//--------------------------------------------------------------------------------------------
void glObject::EndObject()
{
	// podlacz pierwszy obiekt z VAOs
	glBindVertexArray(VAO[lVAO-1]);
	// podlacz pierwszy bufor VBOs
	glBindBuffer(GL_ARRAY_BUFFER, VBO[3*lVAO-3]);	
	// wypelnij bufor wspolrzednymi wierzcholka 

	glBufferData(GL_ARRAY_BUFFER, lCoords[lVAO-1]*sizeof(float), Coords, GL_STATIC_DRAW);
	// wybierz atrybut indeksie 0 (wskazany w shaderze)
	glEnableVertexAttribArray(0); 
	// powiaz dane z bufora ze wskazanym atrybutem
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);	

	// podlacz drugi bufor VBOs
	glBindBuffer(GL_ARRAY_BUFFER, VBO[3*lVAO-2]);	
	// wypelnij bufor kolorami wierzcholka
	glBufferData(GL_ARRAY_BUFFER, lCoords[lVAO-1]*sizeof(float), Cols, GL_STATIC_DRAW);
	// wybierz atrybut indeksie 1 (wskazany w shaderze)
	glEnableVertexAttribArray(1); 
	// powiaz dane z bufora ze wskazanym atrybutem
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);	

	// podlacz trzeci bufor VBOs
	glBindBuffer(GL_ARRAY_BUFFER, VBO[3*lVAO-1]);	
	// wypelnij bufor kolorami wierzcholka
	glBufferData(GL_ARRAY_BUFFER, lCoords[lVAO-1]*sizeof(float), Normals, GL_STATIC_DRAW);
	// wybierz atrybut indeksie 2 (wskazany w shaderze)
	glEnableVertexAttribArray(2); 
	// powiaz dane z bufora ze wskazanym atrybutem
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);	

	glBindVertexArray(0);
}
//--------------------------------------------------------------------------------------------
void glObject::Draw()
{
	for (int i = 0; i < lVAO; i++)
	{
		glBindVertexArray(VAO[i]);
		glDrawArrays(Primitives[i],0,lCoords[i]/3);		
		glBindVertexArray(0);
	}	
}

int glObject::CalcNormal(float A[], float B[], float C[], float *N)
{
	const int x = 0;
	const int y = 1;
	const int z = 2;

	float U[3];
	float V[3]; 
	
	// oblicz wspolrzedne wektorow U oraz V 
	U[x] = A[x] - B[x];
	U[y] = A[y] - B[y];
	U[z] = A[z] - B[z];

	V[x] = B[x] - C[x];
	V[y] = B[y] - C[y];
	V[z] = B[z] - C[z];

	// wyznacz wspolrzedne normalnej 
	N[x] = U[y]*V[z] - U[z]*V[y];
	N[y] = U[z]*V[x] - U[x]*V[z];
	N[z] = U[x]*V[y] - U[y]*V[x];

	return 1; 
}


int glObject::Normalize(float *N)
{
	const int x = 0;
	const int y = 1;
	const int z = 2;

	// oblicz dlugosc wektora 
	float L = (float)sqrt(N[x] * N[x] + N[y] * N[y] + N[z] * N[z]);
	if (L < 0.01) L = 0.01;

	// wyznacz wspolrzedne normalnej 
	N[x] /= L;
	N[y] /= L;
	N[z] /= L;
	return 1;
}


//--------------------------------------------------------------------------------------------
// the end
