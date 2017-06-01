#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include "FileReading.h"
#include <iostream>
#include <vector>
#include <SOIL.h>
#include "imgui.h"

#include <OpenMesh\Core\IO\MeshIO.hh>  
#include <OpenMesh\Core\Mesh\TriMesh_ArrayKernelT.hh>  

#include <GL\freeglut.h>  
//#include <GL\glaux.h>

#define FILE_FOLDER "dataset\\"
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 500

using namespace std;
typedef OpenMesh::TriMesh_ArrayKernelT<> MyMesh;

vector<MyMesh> mesh;
vector<GLuint> flatList, wireframeList, flatLinesList;

vector<GLfloat> xRotate;
vector<GLfloat> yRotate;
vector<GLfloat> oldXPos, oldYPos;

vector<GLfloat> xOffset;
vector<GLfloat> yOffset;

vector<GLfloat> scale;

int mode = 1;

int chosenIndex = 0;
int indexSize;

//GLuint texture[6];//6 textures for 6 faces of the cube

GLfloat xRot, yRot, zRot;

//加载纹理
//AUX_RGBImageRec *LoadBMP(char *FileName)
/*AUX_RGBImageRec *LoadBMP()
{
	//char *bmpFile[1] = { "glass_dif.png" };
	//char *Filename = bmpFile[0];
	FILE *File = NULL;
	//if (!FileName)
	//	return NULL;
	File = fopen("glass_dif.png", "r");
	if (File)
	{
		fclose(File);
		return auxDIBImageLoad(L"glass_dif.png");
	}
	return NULL;
}

//load the bitmap and convert it into a texture
int LoadGLTextures()
{
	int Status = FALSE;
	//char *bmpFile[6] = { "BmpFile/Ferrary.bmp","BmpFile/Honda.bmp","BmpFile/Hust.bmp",
	//	"BmpFile/Lamborghini.bmp","BmpFile/NeHe.bmp","BmpFile/Porsche.bmp" };
	//char *bmpFile[1] = {};
	//AUX_RGBImageRec *TextureImage[6] = new AUX_RGBImageRec[6];//create storage for the texture
	//AUX_RGBImageRec *TextureImage[6] = { NULL,NULL,NULL,NULL,NULL,NULL };
	AUX_RGBImageRec *TextureImage[1] = { NULL};
	for (int i = 0; i < 1; ++i)
	{
		//memset(TextureImage[i],0,sizeof(void*) * 1);//set the point to NULL
		//if (TextureImage[i] = LoadBMP(bmpFile[i]))
		if (TextureImage[i] = LoadBMP())
		{
			Status = TRUE;
			glGenTextures(1, &texture[i]);//命名纹理对象
			glBindTexture(GL_TEXTURE_2D, texture[i]);//绑定纹理
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, TextureImage[i]->sizeX,
				TextureImage[i]->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE,
				TextureImage[i]->data);//指定纹理
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);//指定过滤模式
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		}

		if (TextureImage[i])
		{
			if (TextureImage[i]->data)
				free(TextureImage[i]->data);
			free(TextureImage[i]);
		}
	}

	return Status;
}*/
/*GLuint ATLLoadTexture(CString path)
{
	BITMAP bm;
	GLuint idTexture = 0;
	CImage img;             //需要头文件atlimage.h  
	HRESULT hr = img.Load(path);
	//if (!SUCCEEDED(hr))   //文件加载失败  
	//{
	//	MessageBox(NULL, "文件加载失败", "ERROR", 0);
	//	return NULL;
	//}
	HBITMAP hbmp = img;
	if (!GetObject(hbmp, sizeof(bm), &bm))
		return 0;

	glGenTextures(1, &idTexture);
	if (idTexture)
	{
		glBindTexture(GL_TEXTURE_2D, idTexture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		glPixelStoref(GL_PACK_ALIGNMENT, 1);
		glTexImage2D(GL_TEXTURE_2D, 0, 3, bm.bmWidth, bm.bmHeight, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, bm.bmBits); //这里不是GL_RGB  
	}
	return idTexture;
}*/
/*int LoadGLTextures()                                    // Load Bitmaps And Convert To Textures  
{
	// load an image file directly as a new OpenGL texture 
	texture = SOIL_load_OGL_texture
	(
		"data/pig_d.dds",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_INVERT_Y
	);

	if (texture == 0)
		return false;


	// Typical Texture Generation Using Data From The Bitmap  
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	return true;                                        // Return Success  
}*/

#define Flat 1
#define Wireframe 2
#define Flatlines 3
#define Up 4
#define Down 5
#define Right 6
#define Left 7
#define Large 8
#define Small 9
#define  GLUT_WHEEL_UP 3 //定义滚轮操作
#define  GLUT_WHEEL_DOWN 4
#define MAX_CHAR 128

void processMenuEvents(int option) {
	switch (option) {
	case Flat:
		mode = 1;
		break;
	case Wireframe:
		mode = 2;
		break;
	case Flatlines:
		mode = 3;
		break;
	case Up:
		yOffset[chosenIndex] += 0.1f;
		break;
	case Down:
		yOffset[chosenIndex] -= 0.1f;
		break;
	case Right:
		xOffset[chosenIndex] += 0.1f;
		break;
	case Left:
		xOffset[chosenIndex] -= 0.1f;
		break;
	case Large:
		scale[chosenIndex] /= 0.5;
		break;
	case Small:
		scale[chosenIndex] *= 0.5;
		break;
	default:
		break;
	}
}

void createGLUTMenus() {
	int menu;

	// 创建菜单
	menu = glutCreateMenu(processMenuEvents);

	//给菜单增加条目
	glutAddMenuEntry("Flat", Flat);
	glutAddMenuEntry("Wireframe", Wireframe);
	glutAddMenuEntry("Flatlines", Flatlines);
	glutAddMenuEntry("up", Up);
	glutAddMenuEntry("down", Down);
	glutAddMenuEntry("right", Right);
	glutAddMenuEntry("left", Left);
	glutAddMenuEntry("large", Large);
	glutAddMenuEntry("small", Small);

	// 把菜单和鼠标右键关联
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

  
void initGL() {
	//ATLLoadTexture(str);
	//if (!LoadGLTextures())
	//	return ;
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClearDepth(1.0);

	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	for (int i = 0; i < mesh.size(); i++) {
		// Initial display list  
		flatList[i] = glGenLists(1);
		wireframeList[i] = glGenLists(1);
		flatLinesList[i] = glGenLists(1);

		// Show wire frame  
		glNewList(wireframeList[i], GL_COMPILE);
		glDisable(GL_LIGHTING);
		glLineWidth(1.0f);
		glColor3f(0.2f, 0.2f, 0.2f);
		glBegin(GL_LINES);
		for (MyMesh::HalfedgeIter he_it = mesh[i].halfedges_begin(); he_it != mesh[i].halfedges_end(); ++he_it) {
			glVertex3fv(mesh[i].point(mesh[i].from_vertex_handle(*he_it)).data());
			glVertex3fv(mesh[i].point(mesh[i].to_vertex_handle(*he_it)).data());
		}
		//明亮的白光
		//GLfloat ambientLight[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		//开启光照
		//glEnable(GL_LIGHTING);
		//设置全局环境光为白光
		//glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientLight);

		glEnd();
		glEnable(GL_LIGHTING);
		glEndList();

		// Show flat  
		glNewList(flatList[i], GL_COMPILE);
		for (MyMesh::FaceIter f_it = mesh[i].faces_begin(); f_it != mesh[i].faces_end(); ++f_it) {
			glBegin(GL_TRIANGLES);
			for (MyMesh::FaceVertexIter fv_it = mesh[i].fv_iter(*f_it); fv_it.is_valid(); ++fv_it) {
				// Using vertex normal.
				glNormal3fv(mesh[i].normal(*fv_it).data());
				glVertex3fv(mesh[i].point(*fv_it).data());
			}
			glEnd();
		}
		glEndList();

		// Show flat lines
		glNewList(flatLinesList[i], GL_COMPILE);
		glCallList(wireframeList[i]);

		for (MyMesh::FaceIter f_it = mesh[i].faces_begin(); f_it != mesh[i].faces_end(); ++f_it) {
			glBegin(GL_TRIANGLES);
			for (MyMesh::FaceVertexIter fv_it = mesh[i].fv_iter(*f_it); fv_it.is_valid(); ++fv_it) {
				// Using face normal.
				glNormal3fv(mesh[i].normal(*f_it).data());
				glVertex3fv(mesh[i].point(*fv_it).data());
			}
			glEnd();
		}
		glEndList();
	}
	//LoadGLTextures();
}

void myReshape(GLint w, GLint h){
	glViewport(0, 0, static_cast<GLsizei>(w), static_cast<GLsizei>(h));
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	if (w > h)
		glOrtho(-static_cast<GLdouble>(w) / h, static_cast<GLdouble>(w) / h, -1.0, 1.0, -50.0f, 50.0f);
	else
		glOrtho(-1.0, 1.0, -static_cast<GLdouble>(h) / w, static_cast<GLdouble>(h) / w, -50.0f, 50.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

}

void myKeyboard(unsigned char key, int x, int y){
	switch (key) {
	case '1':
		mode = 1;
		break;
	case '2':
		mode  = 2;
		break;
	case '3':
		mode = 3;
		break;
	case 'w':
		yOffset[chosenIndex] += 0.1f;
		break;
	case 's':
		yOffset[chosenIndex] -= 0.1f;
		break;
	case 'a':
		xOffset[chosenIndex] -= 0.1f;
		break;
	case 'd':
		xOffset[chosenIndex] += 0.1f;
		break;
	default:
		break;
	}

	glutPostRedisplay();
}

void myMouse(int button, int state, int x, int y){
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN){
		oldXPos[chosenIndex] = x;
		oldYPos[chosenIndex] = y;
	}
	if (state == GLUT_UP && button == GLUT_WHEEL_UP){
		scale[chosenIndex] /= 0.9;
		glutPostRedisplay();
	}
	if (state == GLUT_UP && button == GLUT_WHEEL_DOWN){
		scale[chosenIndex] *= 0.9;
		glutPostRedisplay();
	}
}

void myMotion(int x, int y)
{
	GLint deltax = oldXPos[chosenIndex] - x;
	GLint deltay = oldYPos[chosenIndex] - y;
	yRotate[chosenIndex] -= 360 * (GLfloat)deltax / (GLfloat)WINDOW_WIDTH;
	xRotate[chosenIndex] -= 360 * (GLfloat)deltay / (GLfloat)WINDOW_HEIGHT;
	oldXPos[chosenIndex] = x;
	oldYPos[chosenIndex] = y;
	glutPostRedisplay();
}

void myDisplay()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	glTranslatef(xOffset[chosenIndex], yOffset[chosenIndex], 0);
	glRotatef(xRotate[chosenIndex], 1.0f, 0.0f, 0.0f);
	glRotatef(yRotate[chosenIndex], 0.0f, 1.0f, 0.0f);
	glScalef(scale[chosenIndex], scale[chosenIndex], scale[chosenIndex]);

	if (mode == 1) {
		glCallList(flatList[chosenIndex]);
	}
	else if (mode == 2) {
		glCallList(wireframeList[chosenIndex]);
	}
	else if (mode == 3) {
		glCallList(flatLinesList[chosenIndex]);
	}
	
	glutSwapBuffers();
}

int main(int argc, char** argv)
{
	string file_folder(FILE_FOLDER);
	vector<string> mesh_files;

	getAllFiles(file_folder, mesh_files);
	mesh = vector<MyMesh>(mesh_files.size());

	flatList = vector<GLuint>(mesh_files.size());
	wireframeList = vector<GLuint>(mesh_files.size());
	flatLinesList = vector<GLuint>(mesh_files.size());

	xRotate = vector<GLfloat>(mesh_files.size(), 0.0f);
	yRotate = vector<GLfloat>(mesh_files.size(), 0.0f);
	oldXPos = vector<GLfloat>(mesh_files.size());
	oldYPos = vector<GLfloat>(mesh_files.size());;

	xOffset = vector<GLfloat>(mesh_files.size(), 0.0f);
	yOffset = vector<GLfloat>(mesh_files.size(), 0.0f);
	scale = vector<GLfloat>(mesh_files.size(), 1.0f);

	for (int i = 0; i < mesh_files.size(); i++) {
		mesh[i].request_vertex_normals();
		if (!mesh[i].has_vertex_normals()){
			std::cerr << "ERROR: Standard vertex property 'Normals' not available!\n";
			return 1;
		}

		// Read mesh 
		OpenMesh::IO::Options opt;
		if (!OpenMesh::IO::read_mesh(mesh[i], mesh_files[i], opt)){
			cout << "Error: Cannot read mesh from " << mesh_files[i] << endl;
			return 1;
		}

		if (!opt.check(OpenMesh::IO::Options::VertexNormal)){
			mesh[i].request_face_normals();
			mesh[i].update_normals();
		}
	}

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutCreateWindow("模型读入");
	initGL();
	glutKeyboardFunc(&myKeyboard);
	glutReshapeFunc(&myReshape);
	glutDisplayFunc(&myDisplay);
	glutMouseFunc(&myMouse);
	glutMotionFunc(&myMotion);
	createGLUTMenus();
	glutMainLoop();
	return 0;
}
