#include <cstdlib>
#include <iostream>
#include <GL/glut.h>
#include <math.h>

void init();
void draw_test();

void mouse_test(GLint button, GLint action, GLint x, GLint y);
void mouse_test2(GLint x, GLint y);
void mouse_test3(GLint x, GLint y);

void keybord_test(GLubyte key, GLint x, GLint y);
void keybord_test2(GLint key, GLint x, GLint y);

GLint WINDOW_WIDTH = 500,
WINDOW_HEIGHT = 500;

//armazena os vértices de um objeto
struct VERTEX
{
    int x;
    int y;
};
//armazena a descrição geométrica de um objeto
struct OBJECT
{
    VERTEX* vertices;
    int nrvertices;
};

double tx = 0, ty = 0, sx = 1, sy = 1;

int angulo = 0, velocidade = 1;
double s = 0.0; // armazenar seno (da matriz de rotacao)
double c = 1.0; // armazenar cosseno (da matriz de rotacao)


OBJECT* object; //objeto global que será desenhado

OBJECT* create_object()
{
    OBJECT* obj = (OBJECT*)malloc(sizeof(OBJECT));
    obj->nrvertices = 5;
    obj->vertices = (VERTEX*)malloc(obj->nrvertices * sizeof(VERTEX));
    obj->vertices[0].x = 250;
    obj->vertices[0].y = 130;
    obj->vertices[1].x = 250;
    obj->vertices[1].y = 170;
    obj->vertices[2].x = 220;
    obj->vertices[2].y = 190;
    obj->vertices[3].x = 190;
    obj->vertices[3].y = 170;
    obj->vertices[4].x = 190;
    obj->vertices[4].y = 130;
    return obj;
}

VERTEX calculate_centroid(OBJECT* obj)
{
    int i;
    VERTEX cent;
    cent.x = 0;
    cent.y = 0;
    for (i = 0; i < obj->nrvertices; i++)
    {
        cent.x += obj->vertices[i].x;
        cent.y += obj->vertices[i].y;
    }
    cent.x /= obj->nrvertices;
    cent.y /= obj->nrvertices;
    return cent;
}

void init()
{
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glMatrixMode(GL_PROJECTION);

    gluOrtho2D(0, WINDOW_WIDTH, 0, WINDOW_HEIGHT);
    object = create_object(); //cria o objeto
}

// FUNÇÃO QUE REALIZA A TRANSLAÇÃO E ESCALA DO OBJETO
void aplicaTransformacoes(VERTEX vertex, VERTEX centro, double sx, double sy, double tx, double ty, double c, double s, int angulo, int* matrizFinal) {
    s = sin(((angulo) * 3.1415 / 180.0));
    c = cos(((angulo) * 3.1415 / 180.0));
    int matrizObjeto[3] = { vertex.x, vertex.y, 1 };

    /*double matrizTransformacao[3][3] = { sx,  0, -sx * centro.x + centro.x + tx,
                                         0 , sy, -sy * centro.y + centro.y + ty,
                                         0 ,  0, 1 }; */

                                         /*double matrizTransformacao[3][3] = {sx * c,  -sy * s, (centro.x * c) * (1 - sx) - (centro.y * s) * (1 - sy) + centro.x * (1 - c) + centro.y * s + tx,
                                                                             sx * s , sy * c, (centro.x * s) * (1 - sx) - (centro.y * c) * (1 - sy) + centro.y * (1 - c) - centro.x * s + ty,
                                                                             0 ,  0, 1 };*/
                                                                             /*double matrizTransformacao2[3][3] = { c, -s, centro.x - centro.x*c + centro.y*s + tx,
                                                                                                   s, c, centro.y - centro.y*c - centro.x*s + ty,
                                                                                                   0, 0, 1 };*/

    /*double one = sx * c * c - sy * s * s;
    double two = -sx * s * c + sy * c * s;
    double three = sx * s * s + sy * c * c;
    double four = (centro.x * c) * (1 - sx) + (centro.y * s) * (1 - sy);
    double five = -(centro.x * s) * (1 - sx) + (centro.y * c) * (1 - sy);*/
    /*double matrizTransformacao[3][3] = {one, two, four,
                      two, three, five,
                      0, 0, 1 };*/
    /*double matrizTransformacao[3][3] = {c, -s, centro.x - centro.x * c + centro.y * s + tx,
                                         s, c, centro.y - centro.y * c - centro.x * s + ty,
                                         0, 0, 1 };*/
    double matrizTransformacao[3][3] = { c * sx, -sy * s, (-centro.x * c * sx) + (centro.y * s * sy) + centro.x + tx,
                                         sx * s, sy * c, (-centro.x * s * sx) - (centro.y * c * sy) + centro.y + ty,
                                         0, 0, 1 };
    /*double matrizTransformacao[3][3] = {c * (one)-s * (two),  c * (two)-s * (three), c * (four)-s * (five)+centro.x * (1 - c) + centro.y * s + tx,

                                          s * (one) + c * (two), s * (two) + c * (three), s * (four) + c * (five) + centro.y * (1 - c) + centro.x * s + ty,
                                          0 ,  0, 1 };*/
    for (int j = 0; j < 3; j++)
        {
            for (int k = 0; k < 3; k++)
            {
                matrizFinal[j] += matrizObjeto[k] * matrizTransformacao[j][k];
            }
        }
   /* double treg[3][3] = {1, 0, centro.x, 0, 1, centro.y, 0, 0, 1};
    double tinv[3][3] = { 1, 0, -centro.x, 0, 1, -centro.y, 0, 0, 1 };
    double regscal[3][3] = { sx, 0, 0, 0, sy, 0, 0, 0, 1 };
    double regrot[3][3] = { c, -s, 0, s, c, 0, 0, 0, 1 };   
    double rotinv[3][3] = { c, s, 0, -s, c, 0, 0, 0, 1 };
    1 * (1 * 4 * 2) * (1 * 3 * 2)
    for (int j = 0; j < 3; j++)
    {
        for (int k = 0; k < 3; k++)
        {
            matrizFinal[j] += matrizObjeto[k] * 
        }
    }*/

    /*s = sin ( ((angulo) * 3.1415 / 180.0) );
    c = cos ( ((angulo) * 3.1415 / 180.0) );

    matrizFinal[0] = sx*(centro.x + (matrizFinal[0] + tx - centro.x) * c - (matrizFinal[1] + ty - centro.y) * s);
    matrizFinal[1] = sy*(centro.y + (matrizFinal[0] + tx - centro.x) * s - (matrizFinal[1] + ty - centro.y) * c);

    double xr = matrizFinal[0];
    double yr = matrizFinal[1];
    double matrizr[3][3] = { c, -s, xr - xr*c + yr*s,
                         s, c, yr - yr*c - xr*s,
                         0, 0, 1 };*/
                         /*for (int j = 0; j < 3; j++)
                         {
                             for (int k = 0; k < 3; k++)
                             {
                                 matrizFinal[j] += matrizObjeto[k] * matrizTransformacao[j][k];
                             }
                         }*/
}

void draw_object(OBJECT* obj)
{
    VERTEX centro = calculate_centroid(obj);
    int i, j, k;
    glBegin(GL_POLYGON); //Poligono preenchido
    for (i = 0; i < obj->nrvertices; i++)
    {
        int matrizFinal[3] = { 0,0,0 };
        aplicaTransformacoes(obj->vertices[i], centro, sx, sy, tx, ty, c, s, angulo, matrizFinal);
        glVertex2i(matrizFinal[0], matrizFinal[1]);
    }
    glEnd();
}

void draw_test()
{
    glClear(GL_COLOR_BUFFER_BIT); //desenha o fundo (limpa a janela)
    glMatrixMode(GL_MODELVIEW); //garante que a matrix seja a ModelView
    glLoadIdentity(); //carrega a matrix identidade

    glColor3f(0.0, 0.0, 1.0); //altera o atributo de cor
    draw_object(object); //desenha o objeto

    VERTEX cent = calculate_centroid(object); //calcula o centróide

    glFlush(); //processa as rotinas OpenGL o mais rápido possível
}


void mouse_test(GLint button, GLint action, GLint x, GLint y)
{
    int temp = angulo;
    switch (button)
    {
    case GLUT_LEFT_BUTTON: //caso esquerdo
        angulo = angulo + velocidade + 1 * velocidade;
    case GLUT_RIGHT_BUTTON: //caso direito
        angulo = angulo - velocidade;
    }
    glutPostRedisplay();

}

// funcao para processar eventos de teclado
void keybord_test(GLubyte key, GLint x, GLint y)
{
    GLint m = glutGetModifiers();

    if (m == GLUT_ACTIVE_SHIFT)
        std::cout << "Shift ou Caps ";
    else if (m == GLUT_ACTIVE_CTRL)
        std::cout << "Control ";
    else if (m == GLUT_ACTIVE_ALT)
        std::cout << "Alt ";

    //VERIFICAR TABELA ASCII QUANDO O CTRL ESTIVER PRECIONADO COM ALGUMA 
    //LETRA  
    if (m == GLUT_ACTIVE_CTRL && (GLint)key == 4)
        exit(EXIT_SUCCESS);

    std::cout << "Tecla: " << (GLint)key << " (x:" << x << ", y:" << y << ")\n";

    // Escala
    if (key == 119) {
        sy += 0.1; // tecla W
    }
    if (key == 115) {
        sy -= 0.1; // tecla S
    }
    if (key == 97) {
        sx -= 0.1; // tecla A
    }
    if (key == 100) {
        sx += 0.1; // tecla D
    }
    glutPostRedisplay();

    // Velocidade
    if (key == 105) {
        velocidade++;
    }
    if (key == 106) {
        velocidade--;
    }

    //ESPACO == 32
    if (key == 32) {
        sy = 1;
        sx = 1;
        angulo = 0;
        tx = 0;
        ty = 0;
        velocidade = 1;
    }

    //ESC = 27
    if (key == 27)
        glutReshapeWindow(WINDOW_WIDTH, WINDOW_HEIGHT);
}

void keybord_test2(GLint key, GLint x, GLint y)
{
    //GLUT_KEY_F1 .. GLUT_KEY_F12
    //GLUT_KEY_UP, GLUT_KEY_DOWN, GLUT_KEY_LEFT, GLUT_KEY_RIGHT
    //GLUT_KEY_PAGE_DOWN, GLUT_KEY_HOME etc.  
    if (key == GLUT_KEY_F11)
        glutFullScreen();
    
    // Movimento
    if (key == 100) {
        tx--; // tecla para esquerda
    }
    if (key == 102) {
        tx++; // tecla para direita
    }
    if (key == 103) {
        ty--; // tecla para baixo
    }
    if (key == 101) {
        ty++; // tecla para cima
    }
    glutPostRedisplay();
}

int main(int argc, char* argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);

    GLint screen_width = glutGet(GLUT_SCREEN_WIDTH),
        screen_height = glutGet(GLUT_SCREEN_HEIGHT);

    glutInitWindowPosition((screen_width - WINDOW_WIDTH) / 2, (screen_height - WINDOW_WIDTH) / 2);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_WIDTH);
    glutCreateWindow("OpenGL - Transformacoes");

    init();
    glutDisplayFunc(draw_test);

    glutMouseFunc(mouse_test);

    glutKeyboardFunc(keybord_test);
    glutSpecialFunc(keybord_test2);

    glutMainLoop();

    return EXIT_SUCCESS;
}


// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
