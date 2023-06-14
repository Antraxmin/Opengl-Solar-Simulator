/*
과목명 : 컴퓨터 그래픽스 
학 과 : 컴퓨터소프트웨어공학과
학 번 : 20214033
이 름 : 임채민
프로그램명 : opengl 태양계 시스템
제출일 : 2023.06.14
*/

#include <GL/glut.h>  // OpenGL 라이브러리 헤더 파일
#include <vector>     // 벡터 컨테이너를 사용하기 위한 헤더 파일
#include <random>     // 난수 생성을 위한 헤더 파일
#define PI 3.141592

// 행성 구조체 정의
struct Planet {
    std::string name;       // 행성 이름
    GLfloat radius;         // 반지름 
    GLfloat distance;       // 태양으로부터의 거리 
    GLfloat rotation;       // 공전 각도 변수)
    GLfloat speed;          // 공전 속도
    GLfloat color[3];       // 색상 -RGB
    bool hasMoon;           // 해당 행성이 달을 가지고 있는지 여부
};

std::vector<Planet> planets;  // Planet 구조체를 저장할 벡터

GLfloat cameraAngle = 0.0f;  // 카메라의 초기 각도
GLfloat cameraPosX = 0.1f;      // 카메라의 위치 X좌표
GLfloat cameraPosY = 0.1f;      // 카메라의 위치 Y좌표
GLfloat cameraPosZ = 0.1f;      // 카메라의 위치 Z좌표
GLfloat globalSpeed = 1.0f;  // 전체 속도 비율
bool isRotationPaused = false;  // 회전 일시 정지 상태를 나타내는 변수
int windowWidth = 600;  // 윈도우의 너비
int windowHeight = 600; // 윈도우의 높이

void initializePlanets() {
    // 각 행성 구조체 정보 초기화
    Planet sun = { "Sun", 0.2f, 0.0f, 0.0f, 0.0f, {1.0f, 0.0f, 0.0f}, false };          // 태양
    Planet mercury = { "Mercury", 0.02f, 0.3f, 0.0f, 0.25f, {0.7f, 0.7f, 0.7f},  false };    // 수성
    Planet venus = { "Venus", 0.05f, 0.4f, 0.0f, 0.6f, {0.93f, 0.86f, 0.5f},  false };      // 금성
    Planet earth = { "Earth", 0.05f, 0.48f, 0.0f, 1.0f, {0.27f, 0.5f, 0.7f}, true };       // 지구
    Planet mars = { "Mars", 0.025f, 0.55f, 0.0f, 1.9f, {0.72f, 0.52f, 0.04f},  false };        // 화성
    Planet jupiter = { "Jupiter", 0.10f, 0.72f, 0.0f, 0.5f, {0.87f, 0.72f, 0.52f},  false };      // 목성
    Planet saturnus = { "Saturnus", 0.08f, 0.85f, 0.0f, 0.4f, {0.96f, 0.96f, 0.86f},  false };      // 토성
    Planet uranus = { "Uranus", 0.06f, 0.95f, 0.0f, 0.3f, {0.67f, 0.84f, 0.9f},  false };        // 천왕성
    Planet neptunus = { "Neptunus", 0.05f, 1.05f, 0.0f, 0.2f, {0.1f, 0.5f, 1.0f},  false };      // 해왕성
    
    // 행성 벡터에 각 행성 정보 추가
    planets.push_back(sun);
    planets.push_back(mercury);
    planets.push_back(venus);
    planets.push_back(earth);
    planets.push_back(mars);
    planets.push_back(jupiter);
    planets.push_back(saturnus);
    planets.push_back(uranus);
    planets.push_back(neptunus);
}

// 행성과 공전 궤도를 그리는 함수 
void drawPlanet(Planet planet) {
    if (planet.hasMoon) {       // 달을 가지고 있는 행성인지 여부 판단
        glPushMatrix();
        glRotatef(-planet.rotation, 0.0f, 1.0f, 0.0f);
        glTranslatef(0.05, 0.0f, 0.0f);

         // 달 그리기
        glColor3f(1.0f, 1.0f, 1.0f);  // 달의 색상 설정 (흰색)
        glRotatef(-0.07, 0.0f, 1.0f, 0.0f);
        glTranslatef(0.4f, 0.0f, 0.0f);
        glutWireSphere(0.012f, 10, 10);  // 달 그리기
        glPopMatrix();
    }
    // 행성 그리기 
    glColor3f(planet.color[0], planet.color[1], planet.color[2]);   // 행성의 색상 설정
    glPushMatrix();    
    glRotatef(-planet.rotation, 0.0f, 1.0f, 0.0f);  
    glTranslatef(planet.distance, 0.0f, 0.0f);
    glutWireSphere(planet.radius, 20, 20);
    glPopMatrix();

    // 궤도 그리기
    const GLfloat orbitRadius = planet.distance;        // 행성의 거리를 궤도의 반지름으로 설정 
    const GLfloat orbitLineWidth = 0.002f;              // 공전 궤도의 너비 설정
    const int numSegments = 360;                       

    glColor3f(0.5f, 0.5f, 0.5f);        // 공전 궤도의 색상을 회색으로 설정
    glLineWidth(orbitLineWidth);        
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < numSegments; ++i) {
        GLfloat theta = 2.0f * 3.14159f * static_cast<GLfloat>(i) / static_cast<GLfloat>(numSegments);
        GLfloat x = orbitRadius * cosf(theta);
        GLfloat z = orbitRadius * sinf(theta);
        glVertex3f(x, 0.0f, z);
    }
    glEnd();
}

void Display() {
    glEnable(GL_DEPTH_TEST);        // DEPTH_TEST 활성화하여 은면 제거 
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // 초기 시점 설정
    gluLookAt(cameraPosX, cameraPosY, cameraPosZ, 0.1f, 0.0f, 0.0f, 1.0f, 1.5f, 0.0f);
    glRotatef(cameraAngle, 0.0f, 1.0f, 0.0f);

    // 행성 그리기
    for (size_t i = 0; i < planets.size(); i++) {
        drawPlanet(planets[i]);
    }

    glutSwapBuffers();
}

// 행성이 일시정지 상태가 아니라면 계속해서 공전하도록 
void updateScene(int value) {
    if (!isRotationPaused) {
        // 행성 회전 및 공전
        for (auto& planet : planets) {
            planet.rotation += planet.speed * globalSpeed;
            planet.rotation = fmodf(planet.rotation, 360.0f);
        }
    }

    glutPostRedisplay();
    glutTimerFunc(16, updateScene, 0);  
}

void reshape(int width, int height) {
    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    gluPerspective(60.0, (GLfloat)width / (GLfloat)height, 0.5, 100.0);
    glLoadIdentity();
}

// 키보드 입력 처리 함수 
void keyboard(unsigned char key, int x, int y) {
    switch (key) {
    case 32:    // 스페이스 바를 눌렀을 때 전체 행성의 공전을 일시 정지
        isRotationPaused = !isRotationPaused;
        break;
    case '+':      // + 키를 눌렀을 때 속도 증가하도록 
        globalSpeed += 1.0f;
        break;

    case '-':      // - 키를 눌렀을 때 속도 감소하도록 
        globalSpeed -= 1.0f;
        if (globalSpeed < 0.1f) {       // 최저 속도가 0.1보다 작지 않도록 
            globalSpeed = 0.1f;
        }
        break;
    }
    glutPostRedisplay();
}

// 특수 키 이벤트 처리 함수 
void specialKeyboard(int key, int x, int y) {
    switch (key) {
    case GLUT_KEY_UP:
        glMatrixMode(GL_PROJECTION);    // 위쪽 키를 눌렀을 때
        glScaled(1.1, 1.1, 1.1);  // 화면을 1.1배로 확대하여 줌인 효과
        break;
    case GLUT_KEY_DOWN:
        glMatrixMode(GL_PROJECTION);    // 아래 키를 눌렀을 때 
        glScaled(0.9, 0.9, 0.9);  // 화면을 0.9배로 축소하여 줌아웃 효과
        break;
    case GLUT_KEY_LEFT:             // 왼쪽 키를 눌렀을 때 
        cameraAngle -= 2.0f;       // 왼쪽으로 회전
        break;
    case GLUT_KEY_RIGHT:            // 오른쪽 키를 눌렀을 때
        cameraAngle += 2.0f;        // 오른쪽으로 회전
        break;
    }
    glutPostRedisplay();
}

// 메뉴 항목을 클랙했을 때 이벤트 처리
void menuSelect(int choice) {
    switch (choice) {
    case 0:   // 기본 시점 (초기 시점)
        cameraPosX = 0.1f;
        cameraPosY = 0.1f;
        cameraPosZ = 0.1f;
        break;
    case 1:     // 위 시점
        cameraPosX = 0.1f;
        cameraPosY = 0.1f;
        cameraPosZ = 0.0f;
        break;
    case 2:     // 옆 시점
        cameraPosX = 0.0f;
        cameraPosY = 0.0f;
        cameraPosZ = 0.1f;
        break;
    case 3:     // 앞 시점
        cameraPosX = 0.0f;
        cameraPosY = 0.0f;
        cameraPosZ = 0.0f;
        break;
    case 4:     // 무작위 시점
        cameraPosX = (GLfloat)rand() / RAND_MAX * 0.1;
        cameraPosY = (GLfloat)rand() / RAND_MAX * 0.1;
        cameraPosZ = (GLfloat)rand() / RAND_MAX * 0.1;
        break;
    }
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(cameraPosX, cameraPosY, cameraPosZ, 0.1f, 0.0f, 0.0f, 1.0f, 1.5f, 0.0f); // 변경된 시점 적용
    glutPostRedisplay();

}

// 메뉴 생성 함수
void createMenu() {
    int menu = glutCreateMenu(menuSelect);  // 5개의 메뉴를 생성
    glutAddMenuEntry("기본 시점", 0);
    glutAddMenuEntry("위 시점", 1);
    glutAddMenuEntry("옆 시점", 2);
    glutAddMenuEntry("앞 시점", 3);
    glutAddMenuEntry("무작위 시점", 4);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
}

// 메인 함수
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(windowWidth, windowHeight);
    glutCreateWindow("20214033 임채민 - OpenGL Solar System");
    initializePlanets();
    glutDisplayFunc(Display);
    glutReshapeFunc(reshape);
    glutSpecialFunc(specialKeyboard);
    glutKeyboardFunc(keyboard);
    createMenu();
    glutTimerFunc(16, updateScene, 0);  // 60 FPS
    glutMainLoop();
    return 0;
}