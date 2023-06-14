#include <GL/glut.h>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <stdio.h>
using namespace std;

static int Time = 0;        // 자전 변수 
static int mer = 0, ven = 0, ear = 0, mar = 0, jup = 0, sat = 0, ura = 0, nep = 0;     // 행성 공전 변수
static GLfloat cameraAngleX = 0.0f;
static GLfloat cameraAngleY = 0.0f;
static GLfloat cameraAngleZ = 0.0f;
static GLfloat cameraDistance = 0.1f;
GLfloat zoomFactor = 0.9f;  // 확대/축소 비율 조절

// 카메라 위치 좌표
GLfloat cameraPosX = 0.1f;             
GLfloat cameraPosY = 0.1f;             
GLfloat cameraPosZ = 0.1f;            

// 카메라 초점 좌표
GLfloat cameraCenterX = 0.1;           
GLfloat cameraCenterY = 0.0;           
GLfloat cameraCenterZ = 0.0;    

// 카메라 기울임 좌표
GLfloat cameraUpX = 1.0;
GLfloat cameraUpY = 1.5;
GLfloat cameraUpZ = 0.0;

GLfloat zoom = 1.0f;
GLfloat cameraRotate = 0.0f;
GLfloat cameraRotateX = 0.0f;
GLfloat cameraRotateY = 0.0f;
GLfloat cameraRotateZ = 0.0f;


// 공전 궤도 그리기
void DrawOrbit(float a, float b) {
    glColor3f(0.5f, 0.5f, 0.5f);
    glBegin(GL_LINE_LOOP);
    float x, y;
    for (int i = 0; i <= 360; i += 5) {
        float angle = i * 3.14159f / 180;
        x = a * cos(angle);
        y = b * sin(angle);
        glVertex3f(x, y, 0.0f);
    }
    glEnd();
}

void Display() {
    glEnable(GL_DEPTH_TEST);        // 은면제거
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(cameraPosX, cameraPosY, cameraPosZ, cameraCenterX, cameraCenterY, cameraCenterZ, cameraUpX, cameraUpY, cameraUpZ);
    glScalef(zoom, zoom, zoom);
    glRotatef(cameraRotate, cameraRotateX, cameraRotateY, cameraRotateZ);

    DrawOrbit(0.3f, 0.3f);      // 수성의 공전 궤도
    DrawOrbit(0.38f, 0.38f);      // 금성의 공전 궤도
    DrawOrbit(0.45f, 0.45f);      // 지구의 공전 궤도
    DrawOrbit(0.53f, 0.53f);      // 화성의 공전 궤도
    DrawOrbit(0.67f, 0.67f);      // 목성의 공전 궤도
    DrawOrbit(0.8f, 0.8f);      // 토성의 공전 궤도
    DrawOrbit(0.9f, 0.9f);      // 천왕성의 공전 궤도
    DrawOrbit(1.0f, 1.0f);      // 해왕성의 공전 궤도


    // 태양 
    glPushMatrix();
    glColor3f(1.0, 0.3, 0.3);
    glutWireSphere(0.2, 20, 20);
    glPopMatrix();
    
    // 지구
    glPushMatrix();
    glRotatef((GLfloat)ear, 0.0, 1.0, 0.0);
    glTranslatef(0.45, 0.0, 0.0);
    glRotatef((GLfloat)Time, 0.0, 0.0, 1.0);
    glColor3f(0.2, 0.7, 0.8);
    glutWireSphere(0.05, 20, 20);
    glPopMatrix();

    // 지구 궤도
    //DrawOrbit(0.0f, 0.5f);        // 행성을 중심으로 하는 궤도임

    // 달
    glPushMatrix();
    glTranslatef(0.08, 0.0, 0.0);
    glRotatef((GLfloat)Time, 0.0, 1.0, 0.0);
    glColor3f(0.9, 0.8, 1.0);
    glutWireSphere(0.012, 10, 10);
    glPopMatrix();

    // 수성 - 흰색
    glPushMatrix();
    glRotatef((GLfloat)mer, 0.0, 1.0, 0.0);     // Y축을 기준으로 회전
    glTranslatef(0.3f, 0.0, 0.0);
    glColor3f(1.0, 1.0, 1.0);
    glutWireSphere(0.02, 10, 8);
    glPopMatrix();

    // 금성 
    glPushMatrix();
    glRotatef((GLfloat)ven, 0.0, 1.0, 0.0);
    glTranslatef(0.38, 0.0, 0.0);
    glColor3f(0.9, 0.8, 0.1);
    glutWireSphere(0.05, 10, 8);
    glPopMatrix();

    // 화성
    glPushMatrix();
    glRotatef((GLfloat)mar, 0.0, 1.0, 0.0);
    glTranslatef(0.53, 0.0, 0.0);
    glColor3f(0.9, 0.1, 0.1);
    glutWireSphere(0.025, 10, 8);
    glPopMatrix();

    // 목성 - 노랑색 
    glPushMatrix();
    glRotatef((GLfloat)jup, 0.0, 1.0, 0.0);
    glTranslatef(0.67, 0.0, 0.0);
    glColor3f(0.9, 0.8, 0.1);
    glutWireSphere(0.1, 10, 8);
    glPopMatrix();

    // 토성  
    glPushMatrix();
    glRotatef((GLfloat)sat, 0.0, 1.0, 0.0);
    glTranslatef(0.8, 0.0, 0.0);
    glColor3f(0.7, 0.8, 0.1);
    glutWireSphere(0.08, 10, 8);
    glPopMatrix();

    // 천왕성  
    glPushMatrix();
    glRotatef((GLfloat)ura, 0.0, 1.0, 0.0);
    glTranslatef(0.9, 0.0, 0.0);
    glColor3f(0.7, 0.8, 0.1);
    glutWireSphere(0.07, 10, 8);
    glPopMatrix();

    // 해왕성  
    glPushMatrix();
    glRotatef((GLfloat)nep, 0.0, 1.0, 0.0);
    glTranslatef(1.0, 0.0, 0.0);
    glColor3f(0.7, 0.8, 0.1);
    glutWireSphere(0.06, 10, 8);
    glPopMatrix();

    glutSwapBuffers();
}

void Reshape(int width, int height) {
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    GLfloat AspectRatio = (GLfloat)width / (GLfloat)height;
    gluPerspective(90.0f, AspectRatio, 0.1f, 10.0f);
    glLoadIdentity();
}


void SpecialFunc(int key, int x, int y) {
    switch (key) {
    case GLUT_KEY_UP:  // 위쪽 화살표 키를 눌렀을 때
       glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        zoomFactor -= 0.02f;
        glOrtho(-1.0 * zoomFactor, 1.0 * zoomFactor,
            -1.0 * zoomFactor, 1.0 * zoomFactor, -1.0, 1.0);
        //zoom *= 1.1f;
        break;
    case GLUT_KEY_DOWN:  // 아래쪽 화살표 키를 눌렀을 때
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        zoomFactor += 0.02f;
        glOrtho(-1.0 * zoomFactor, 1.0 * zoomFactor,
            -1.0 * zoomFactor, 1.0 * zoomFactor, -1.0, 1.0);
        break;
    case GLUT_KEY_LEFT:  // 왼쪽 화살표 키를 눌렀을 때
        cameraPosY += 0.5f;
        break;
    case GLUT_KEY_RIGHT:  // 오른쪽 화살표 키를 눌렀을 때
        cameraPosY -= 0.1f;
        break;
    }
    glutPostRedisplay();
}


void Menu(int value) {
    switch (value) {
    case 1:  // 기본 시점
        zoom = 1;
        cameraRotate = 0.0f;
        cameraPosX = 0.1;
        cameraPosY = 0.1;
        cameraPosZ = 0.1;
        cameraCenterX = 0.1;
        cameraCenterY = 0.0;
        cameraCenterZ = 0.0;
        cameraUpX = 1.0;
        cameraUpY = 1.5;
        cameraUpZ = 0.0;
        cameraRotateX = 0.0;
        cameraRotateY = 1.0;
        cameraRotateZ = -1.0;
        break;
    case 2:  // 위 시점
        zoom = 1;
        cameraRotate = 0.0f;
        cameraPosX = 0.0;
        cameraPosY = 0.1;
        cameraPosZ = 0.0;
        cameraCenterX = 0.0;
        cameraCenterY = 0.0;
        cameraCenterZ = 0.0;
        cameraUpX = 0.0;
        cameraUpY = 0.0;
        cameraUpZ = -1.0;
        cameraRotateX = 1.0;
        cameraRotateY = 0.0;
        cameraRotateZ = 0.0;
        break;
    case 4:  // 앞 시점
        zoom = 1;
        cameraRotate = 0.0f;
        cameraPosX = 0.0;
        cameraPosY = 0.1;
        cameraPosZ = -0.1;
        cameraCenterX = 0.0;
        cameraCenterY = 0.0;
        cameraCenterZ = 0.0;
        cameraUpX = 0.0;
        cameraUpY = -1.0;
        cameraUpZ = 0.0;
        cameraRotateX = 0.0;
        cameraRotateY = 1.0;
        cameraRotateZ = 0.0;
        break;
    case 3:  // 옆 시점
        zoom = 1;
        cameraRotate = 0.0f;
        cameraPosX = 0.1;
        cameraPosY = 0.0;
        cameraPosZ = 0.0;
        cameraCenterX = 0.0;
        cameraCenterY = 0.0;
        cameraCenterZ = 0.0;
        cameraUpX = 0.0;
        cameraUpY = -1.0;
        cameraUpZ = -0.0;
        cameraRotateX = 0.0;
        cameraRotateY = 1.0;
        cameraRotateZ = 0.0;
        break;
    case 5:  // 무작위 시점
        cameraAngleX = static_cast<GLfloat>(rand()) / static_cast<GLfloat>(RAND_MAX) * 360.0f;
        cameraAngleY = static_cast<GLfloat>(rand()) / static_cast<GLfloat>(RAND_MAX) * 360.0f;
        cameraDistance = static_cast<GLfloat>(rand()) / static_cast<GLfloat>(RAND_MAX) * 1.9f + 0.1f;
        break;
    }
    glutPostRedisplay();
}

void Timer(int Value) {
    mer = fmodf(mer + 1, 360);      // 수성의 공전 속도
    ven = fmodf(ven + 1.8, 360);      // 금성의 공전 속도 
    ear = fmodf(ear + 3, 360);      // 지구의 공전 속도
    mar = fmodf(mar + 5.7, 360);      // 화성의 공전 속도 
    jup = fmodf(jup + 2.5, 360);      // 목성의 공전 속도 
    sat = fmodf(sat + 2, 360);      // 토성의 공전 속도 
    ura = fmodf(ura + 1.5, 360);      // 천왕성의 공전 속도 
    nep = fmodf(nep + 1.1, 360);      // 해왕성의 공전 속도 
    Time = (Time + 3) % 360;        // 자전 속도
    glutPostRedisplay();
    glutTimerFunc(40, Timer, 1);
}

int main(int argc, char** argv) {
    srand(static_cast<unsigned int>(time(0)));  // 난수 초기화

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(0, 0);
    glutCreateWindow("20214033 임채민 - 컴퓨터그래픽스 텀프로젝트");

    glClearColor(0.0, 0.0, 0.0, 1.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);

    glutDisplayFunc(Display);
    glutReshapeFunc(Reshape);
    glutTimerFunc(40, Timer, 1);
    glutSpecialFunc(SpecialFunc);

    int menu = glutCreateMenu(Menu);
    glutAddMenuEntry("기본 시점", 1);
    glutAddMenuEntry("위 시점", 2);
    glutAddMenuEntry("옆 시점", 3);
    glutAddMenuEntry("앞 시점", 4);
    glutAddMenuEntry("무작위 시점", 5);
    glutAttachMenu(GLUT_RIGHT_BUTTON);

    glutMainLoop();
    return 0;
}