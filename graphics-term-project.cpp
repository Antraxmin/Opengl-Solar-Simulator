#include <GL/glut.h>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <stdio.h>
using namespace std;

static int Day = 0, Day1 = 0, Day2 = 0, Time = 0;
static GLfloat cameraAngleX = 0.0f;
static GLfloat cameraAngleY = 0.0f;
static GLfloat cameraAngleZ = 0.0f;
static GLfloat cameraDistance = 0.1f;
GLfloat zoomFactor = 0.9f;  // 확대/축소 비율 조절

void Display() {
    glEnable(GL_DEPTH_TEST);        // 은면제거
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // 카메라 위치 설정
    GLfloat cameraPosX = cameraDistance * sin(cameraAngleX);
    GLfloat cameraPosY = cameraDistance * sin(cameraAngleY);
    GLfloat cameraPosZ = cameraDistance * cos(cameraAngleX);
    gluLookAt(cameraPosX, cameraPosY, cameraPosZ, 0.1, 0.0, 0.0, 1.0, 1.5, 0.0);
    //gluLookAt(0.1, 0.1, 0.1, 0.1, 0.0, 0.0, 1.0, 1.5, 0.0);

    // 태양 
    glColor3f(1.0, 0.3, 0.3);
    glutWireSphere(0.2, 30, 30);  

    // 지구
    glPushMatrix();
    glRotatef((GLfloat)Day, 0.0, 1.0, 0.0);
    glTranslatef(0.7, 0.0, 0.0);
    glRotatef((GLfloat)Time, 0.0, 1.0, 0.0);
    glColor3f(0.2, 0.7, 0.8);
    glutWireSphere(0.1, 20, 20); 

    // 달
    glPushMatrix();
    glTranslatef(0.2, 0.0, 0.0);
    glRotatef((GLfloat)Time, 0.0, 1.0, 0.0);
    glColor3f(0.9, 0.8, 1.0);
    glutWireSphere(0.04, 10, 10);  
    glPopMatrix();

    // 금성
    glLoadIdentity();
    glPushMatrix();
    gluLookAt(cameraPosX, cameraPosY, cameraPosZ, 0.1, 0.0, 0.0, 1.0, 1.03, 0.0);
    //gluLookAt(0.1, 0.1, 0.1, 0.1, 0.0, 0.0, 1.0, 1.5, 0.0);
    glRotatef((GLfloat)Day1, 0.0, 1.0, 0.0);
    glTranslatef(0.5, 0.0, 0.0);
    glColor3f(0.9, 0.8, 0.1);
    glutWireSphere(0.05, 10, 8); 

    // 화성
    glLoadIdentity();
    glPushMatrix();
    //gluLookAt(0.1, 0.1, 0.1, 0.1, 0.0, 0.0, 1.0, 1.5, 0.0);
    gluLookAt(cameraPosX, cameraPosY, cameraPosZ, 0.1, 0.0, 0.0, 1.0, 1.01, 0.0);
    glRotatef((GLfloat)Day2, 0.0, 1.0, 0.0);
    glTranslatef(0.7, 0.0, 0.0);
    glColor3f(0.9, 0.1, 0.1);
    glutWireSphere(0.05, 10, 8);
    glPopMatrix();

    glPopMatrix();
    glPopMatrix();
    glLoadIdentity();
    glutSwapBuffers();
}

void Reshape(int NewWidth, int NewHeight) {
    glViewport(0, 0, NewWidth, NewHeight);
    GLfloat AspectRatio = (GLfloat)NewWidth / (GLfloat)NewHeight;
    printf("%lf", AspectRatio);
    gluPerspective(60.0f, AspectRatio, 0.1f, 10.0f);
    GLfloat WidthFactor = (GLfloat)NewWidth / (GLfloat)500;
    GLfloat HeightFactor = (GLfloat)NewHeight / (GLfloat)500;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1.0 * WidthFactor, 1.0 * WidthFactor,
        -1.0 * HeightFactor, 1.0 * HeightFactor, -1.0, 1.0);
}


void SpecialFunc(int key, int x, int y) {
    switch (key) {
    case GLUT_KEY_UP:  // 위쪽 화살표 키를 눌렀을 때
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        zoomFactor -= 0.02f;
        glOrtho(-1.0 * zoomFactor, 1.0 * zoomFactor,
            -1.0 * zoomFactor, 1.0 * zoomFactor, -1.0, 1.0);
        //cameraDistance -= 0.02f;
        //if (cameraDistance < 0.01f)
        //    cameraDistance = 0.01f;
        break;
    case GLUT_KEY_DOWN:  // 아래쪽 화살표 키를 눌렀을 때
        //cameraDistance += 0.01f;
        //if (cameraDistance > 2.0f)
        //    cameraDistance = 2.0f;
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        zoomFactor += 0.02f;
        glOrtho(-1.0 * zoomFactor, 1.0 * zoomFactor,
            -1.0 * zoomFactor, 1.0 * zoomFactor, -1.0, 1.0);
        break;
    case GLUT_KEY_LEFT:  // 왼쪽 화살표 키를 눌렀을 때
        cameraAngleX -= 0.1f;
        break;
    case GLUT_KEY_RIGHT:  // 오른쪽 화살표 키를 눌렀을 때
        cameraAngleX += 0.1f;
        break;
    }
    glutPostRedisplay();
}

void Menu(int value) {
    switch (value) {
    case 1:  // 기본 시점
        cameraAngleX = 0.0f;
        cameraAngleY = 0.0f;
        cameraDistance = 0.1f;
        break;
    case 2:  // 위 시점
        cameraAngleX = 0.0f;
        cameraAngleY = 90.0f;
        cameraDistance = 0.1f;
        break;
    case 3:  // 옆 시점
        cameraAngleX = 90.0f;
        cameraAngleY = 0.0f;
        cameraDistance = 0.1f;
        break;
    case 4:  // 앞 시점
        cameraAngleX = 180.0f;
        cameraAngleY = 0.0f;
        cameraDistance = 0.1f;
        break;
    case 5:  // 무작위 시점
        cameraAngleX = static_cast<GLfloat>(rand()) / static_cast<GLfloat>(RAND_MAX) * 360.0f;
        cameraAngleY = static_cast<GLfloat>(rand()) / static_cast<GLfloat>(RAND_MAX) * 360.0f;
        cameraDistance = static_cast<GLfloat>(rand()) / static_cast<GLfloat>(RAND_MAX) * 1.9f + 0.1f;
        break;
    }
    glutPostRedisplay();
}

// 현재는 모든 행성의 공전 속도가 같게 출력되도록 설정함. 추후 수정
void Timer(int Value) {
    Day = (Day + 3) % 360;
    Day1 = (Day1 + 3) % 360;
    Day2 = (Day2 + 3) % 360;
    Time = (Time + 3) % 360;
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
