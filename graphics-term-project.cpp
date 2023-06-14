#include <GL/glut.h>  // OpenGL 라이브러리 헤더 파일
#include <vector>     // 벡터 컨테이너를 사용하기 위한 헤더 파일
#include <random>     // 난수 생성을 위한 헤더 파일

// 행성 구조체 정의
struct Planet {
    GLfloat radius;         // 반지름 
    GLfloat distance;       // 태양으로부터의 거리 
    GLfloat rotation;       // 공전 각도 변수)
    GLfloat speed;          // 공전 속도
    GLfloat color[3];       // 색상 -RGB
    bool isRotating;
};

std::vector<Planet> planets;  // Planet 구조체를 담는 벡터
bool isRotationPaused = false;  // 회전 일시 정지 상태를 나타내는 변수

int currentView = 0;  // 현재 시점을 나타내는 변수 (0: 기본 시점, 1: 위 시점, 2: 옆 시점, 3: 앞 시점, 4: 무작위 시점)
GLfloat cameraDistance = 0.1f;  // 카메라의 초기 거리
GLfloat cameraAngle = 0.0f;  // 카메라의 초기 각도
GLfloat cameraHeight = 1.5f;  // 카메라의 초기 높이

GLfloat globalSpeed = 1.0f;  // 전체 속도 비율

// 난수 생성기 초기화
std::random_device rd;
std::mt19937 gen(rd());
std::uniform_real_distribution<GLfloat> dis(0.0f, 1.0f);

void initializePlanets() {
    Planet sun = { 0.2f, 0.0f, 0.0f, 0.0f, {1.0f, 1.0f, 0.0f}, true };          // 태양
    Planet mercury = { 0.02f, 0.3f, 0.0f, 0.25f, {0.7f, 0.7f, 0.7f}, true };    // 수성
    Planet venus = { 0.05f, 0.35f, 0.0f, 0.6f, {0.8f, 0.5f, 0.2f}, true };      // 금성
    Planet earth = { 0.05f, 0.4f, 0.0f, 1.0f, {0.2f, 0.4f, 0.8f}, true };       // 지구
    Planet moon = { 0.012f, 0.45f, 0.0f, 0.07f, {0.7f, 0.7f, 0.7f}, true };      // 달
    Planet mars = { 0.025f, 0.6f, 0.0f, 1.9f, {0.8f, 0.3f, 0.1f}, true };        // 화성
    Planet jupiter = { 0.10f, 0.7f, 0.0f, 0.5f, {0.8f, 0.3f, 0.1f}, true };      // 목성
    Planet saturnus = { 0.08f, 0.8f, 0.0f, 0.4f, {0.8f, 0.3f, 0.1f}, true };      // 토성
    Planet uranus = { 0.06f, 0.9f, 0.0f, 0.3f, {0.8f, 0.3f, 0.1f}, true };        // 천왕성
    Planet neptunus = { 0.05f, 1.0f, 0.0f, 0.2f, {0.0f, 0.4f, 0.9f}, true };      // 해왕성

    planets.push_back(sun);
    planets.push_back(moon);
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
    glColor3fv(planet.color);
    glPushMatrix();
    glRotatef(planet.rotation, 0.0f, 1.0f, 0.0f);
    glTranslatef(planet.distance, 0.0f, 0.0f);
    glutWireSphere(planet.radius, 20, 20);
    glPopMatrix();

    // 궤도 그리기
    const GLfloat orbitRadius = planet.distance;
    const GLfloat orbitLineWidth = 0.002f;
    const int numSegments = 360;

    glColor3fv(planet.color);
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

void drawScene() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // 카메라 위치 설정
    gluLookAt(0.1f, 0.1f, cameraDistance, 0.1f, 0.0f, 0.0f, 1.0f, 1.5f, 0.0f);
    glRotatef(cameraAngle, 0.0f, 1.0f, 0.0f);

    // 시점 변경
    if (currentView == 1) {
        gluLookAt(0.1f, 0.1f, 0.1f, 0.1f, 0.0f, 0.0f, 0.0f, 1.5f, 0.0f);  // 위 시점
    }
    else if (currentView == 2) {
        gluLookAt(0.0f, 0.1f, 0.1f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f);  // 옆 시점
    }
    else if (currentView == 3) {
        gluLookAt(0.0f, 0.1f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f);  // 앞 시점
    }
    else if (currentView == 4) {
        GLfloat randomX = dis(gen) - 0.5f;
        GLfloat randomY = dis(gen) - 0.5f;
        GLfloat randomZ = dis(gen) - 0.5f;
        gluLookAt(randomX, randomY, randomZ, 0.1f, 0.0f, 0.0f, 1.0f, 1.5f, 0.0f);  // 무작위 시점
    }

    // 행성 그리기
    for (const auto& planet : planets) {
        drawPlanet(planet);
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

void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        // 행성 색상 무작위 변경
        for (auto& planet : planets) {
            planet.color[0] = dis(gen);
            planet.color[1] = dis(gen);
            planet.color[2] = dis(gen);
        }
    }
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
    currentView = choice;
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
    glutInitWindowSize(600, 600);
    glutCreateWindow("OpenGL Solar System");

    initializePlanets();

    glutDisplayFunc(drawScene);
    glutReshapeFunc(reshape);
    glutSpecialFunc(specialKeyboard);
    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse);
    createMenu();

    glutTimerFunc(16, updateScene, 0);  // 60 FPS

    glutMainLoop();

    return 0;
}