#include <GL/glut.h>  // OpenGL 라이브러리 헤더 파일
#include <vector>     // 벡터 컨테이너를 사용하기 위한 헤더 파일
#include <random>     // 난수 생성을 위한 헤더 파일
#define PI 3.141592

int windowWidth = 600;  // 윈도우의 너비
int windowHeight = 600; // 윈도우의 높이

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

Planet selectedPlanet;

std::vector<Planet> planets;  // Planet 구조체를 담는 벡터
bool isRotationPaused = false;  // 회전 일시 정지 상태를 나타내는 변수

int currentView = 0;  // 현재 시점을 나타내는 변수 (0: 기본 시점, 1: 위 시점, 2: 옆 시점, 3: 앞 시점, 4: 무작위 시점)
GLfloat cameraDistance = 0.1f;  // 카메라의 초기 거리
GLfloat cameraAngle = 0.0f;  // 카메라의 초기 각도
GLfloat cameraHeight = 1.5f;  // 카메라의 초기 높이

GLfloat cameraPosX = 0.1f;
GLfloat cameraPosY = 0.1f;
GLfloat cameraPosZ = 0.1f;

GLfloat globalSpeed = 1.0f;  // 전체 속도 비율

// 난수 생성기 초기화
std::random_device rd;
std::mt19937 gen(rd());
std::uniform_real_distribution<GLfloat> dis(0.0f, 1.0f);

void initializePlanets() {
    Planet sun = { "Sun", 0.2f, 0.0f, 0.0f, 0.0f, {1.0f, 0.0f, 0.0f}, false };          // 태양
    Planet mercury = { "Mercury", 0.02f, 0.3f, 0.0f, 0.25f, {0.7f, 0.7f, 0.7f},  false };    // 수성
    Planet venus = { "Venus", 0.05f, 0.35f, 0.0f, 0.6f, {0.93f, 0.86f, 0.5f},  false };      // 금성
    Planet earth = { "Earth", 0.05f, 0.4f, 0.0f, 1.0f, {0.27f, 0.5f, 0.7f}, true };       // 지구
    Planet mars = { "Mars", 0.025f, 0.6f, 0.0f, 1.9f, {0.72f, 0.52f, 0.04f},  false };        // 화성
    Planet jupiter = { "Jupiter", 0.10f, 0.7f, 0.0f, 0.5f, {0.87f, 0.72f, 0.52f},  false };      // 목성
    Planet saturnus = { "Saturnus", 0.08f, 0.8f, 0.0f, 0.4f, {0.96f, 0.96f, 0.86f},  false };      // 토성
    Planet uranus = { "Uranus", 0.06f, 0.9f, 0.0f, 0.3f, {0.67f, 0.84f, 0.9f},  false };        // 천왕성
    Planet neptunus = { "Neptunus", 0.05f, 1.0f, 0.0f, 0.2f, {0.1f, 0.5f, 1.0f},  false };      // 해왕성
    
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


void drawOrbit(GLfloat orbitRadius, GLfloat lineWidth) {
    const int numSegments = 360;

    glPushMatrix();
    glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
    glTranslatef(0.0f, -orbitRadius, 0.0f);
    glRotatef(90.0f, 0.0f, 0.0f, 1.0f);
    glLineWidth(lineWidth);
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < numSegments; ++i) {
        GLfloat theta = 2.0f * 3.14159f * static_cast<GLfloat>(i) / static_cast<GLfloat>(numSegments);
        GLfloat x = orbitRadius * cosf(theta);
        GLfloat y = orbitRadius * sinf(theta);
        glVertex3f(x, y, 0.0f);
    }
    glEnd();
    glPopMatrix();
}

void drawPlanet(Planet planet) {
    // 달 그리기
    if (planet.hasMoon) {
        glPushMatrix();
        glRotatef(-planet.rotation, 0.0f, 1.0f, 0.0f);
        glTranslatef(0.05, 0.0f, 0.0f);

         // 달 그리기
        glColor3f(1.0f, 1.0f, 1.0f);  // 달의 색상 설정 (흰색)
        glRotatef(-3.0, 0.0f, 1.0f, 0.0f);
        glTranslatef(0.4f, 0.0f, 0.0f);
        glutWireSphere(0.02f, 10, 10);  // 달을 작게 그리기
        glPopMatrix();
    }
    glColor3f(planet.color[0], planet.color[1], planet.color[2]);
    glPushMatrix();
    glRotatef(-planet.rotation, 0.0f, 1.0f, 0.0f);
    glTranslatef(planet.distance, 0.0f, 0.0f);
    glutWireSphere(planet.radius, 20, 20);
    glPopMatrix();

    

    // 궤도 그리기
    const GLfloat orbitRadius = planet.distance;
    const GLfloat orbitLineWidth = 0.002f;
    const int numSegments = 360;

    glColor3f(0.5f, 0.5f, 0.5f);
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
    glEnable(GL_DEPTH_TEST);        // 은면제거
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // 카메라 위치 설정
    gluLookAt(cameraPosX, cameraPosY, cameraPosZ, 0.1f, 0.0f, 0.0f, 1.0f, 1.5f, 0.0f);
    glRotatef(cameraAngle, 0.0f, 1.0f, 0.0f);

    // 행성 그리기
    for (size_t i = 0; i < planets.size(); i++) {
        drawPlanet(planets[i]);
    }

    glutSwapBuffers();
}


void updateScene(int value) {
    if (!isRotationPaused) {
        // 행성 회전 및 공전
        for (auto& planet : planets) {
            planet.rotation += planet.speed * globalSpeed;
            planet.rotation = fmodf(planet.rotation, 360.0f);
        }
    }

    glutPostRedisplay();
    glutTimerFunc(16, updateScene, 0);  // 60 FPS
}

void reshape(int width, int height) {
    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    gluPerspective(60.0, (GLfloat)width / (GLfloat)height, 0.5, 100.0);
    glLoadIdentity();
}

void keyboard(unsigned char key, int x, int y) {
    switch (key) {
    case 32:
        isRotationPaused = !isRotationPaused;
    case '+':
        globalSpeed += 0.1f;
        printf("속도 1 증가");

    case '-':
        globalSpeed -= 0.1f;
        if (globalSpeed < 0.1f) {
            globalSpeed = 0.1f;
        }
        printf("속도 1 감소");
    }
    glutPostRedisplay();
}

void specialKeyboard(int key, int x, int y) {
    switch (key) {
    case GLUT_KEY_UP:
        printf("전체 화면 줌인\n");
        glMatrixMode(GL_PROJECTION);
        glScaled(1.1, 1.1, 1.1);  // 화면을 0.9배로 축소하여 줌인 효과
        break;
    case GLUT_KEY_DOWN:
        printf("전체 화면 줌아웃\n");
        glMatrixMode(GL_PROJECTION);
        glScaled(0.9, 0.9, 0.9);  // 화면을 1.1배로 확대하여 줌아웃 효과
        break;
    case GLUT_KEY_LEFT:
        printf("왼쪽회전");
        cameraAngle -= 1.0f;
        break;
    case GLUT_KEY_RIGHT:
        printf("오른쪽회전");
        cameraAngle += 1.0f;
        break;
    }
    glutPostRedisplay();
}

void menuSelect(int choice) {
    switch (choice) {
    case 1:     // 위 시점
        cameraPosX = 0.1f;
        cameraPosY = 0.1f;
        cameraPosZ = 0.1f;
        break;
    case 2:     // 옆 시점
        cameraPosX = 0.0f;
        cameraPosY = 0.1f;
        cameraPosZ = 0.1f;
        break;
    case 3:     // 앞 시점
        cameraPosX = 0.0f;
        cameraPosY = 0.1f;
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
    gluLookAt(cameraPosX, cameraPosY, cameraPosZ, 0.1f, 0.0f, 0.0f, 1.0f, 1.5f, 0.0f); 
    glutPostRedisplay();

}

void createMenu() {
    int menu = glutCreateMenu(menuSelect);
    glutAddMenuEntry("기본 시점", 0);
    glutAddMenuEntry("위 시점", 1);
    glutAddMenuEntry("옆 시점", 2);
    glutAddMenuEntry("앞 시점", 3);
    glutAddMenuEntry("무작위 시점", 4);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
}



int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(windowWidth, windowHeight);
    glutCreateWindow("OpenGL Solar System");

    initializePlanets();

    glutDisplayFunc(Display);
    glutReshapeFunc(reshape);
    glutSpecialFunc(specialKeyboard);
    glutKeyboardFunc(keyboard);
    //glutMouseFunc(mouse);
    createMenu();

    glutTimerFunc(16, updateScene, 0);  // 60 FPS

    glutMainLoop();

    return 0;
}