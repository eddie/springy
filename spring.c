#include <stdlib.h>
#include <stdio.h>
#include <GLUT/glut.h>
#include <time.h>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define SLEEP 40

#define TARGET_HEIGHT 300

struct spring {
  
  int x;
  int y;

  int height;
  
  double position;
  double velocity;
};

struct vertex_2d{
  int x;
  int y;
};

typedef struct vertex_2d vertex_2d;

void handle_keypress(unsigned char key, int x, int y);
void handle_keypress(unsigned char key, int x,int y);
void handle_resize(int w,int h);
void update();
void draw_scene();
void init_rendering();

void splash(int index, float speed);
void update_spring(struct spring *s,float k,float d);
void draw_spring(struct spring *s);

#define SPRING_SEP 4
#define NUM_SPRINGS SCREEN_WIDTH/SPRING_SEP+1

struct spring *springs[NUM_SPRINGS];

float tension = 0.025f;
float dampening = 0.020f;

int 
main(int argc,char **argv)
{
  glutInit(&argc,argv);

  srand(time(NULL));
  int i = 0;
  for(i = 0; i < NUM_SPRINGS; i++){
    springs[i] = malloc(sizeof(struct spring));

    springs[i]->x = 20*i;
    springs[i]->y = SCREEN_HEIGHT;
    springs[i]->position = springs[i]->velocity = 0.0f;

    springs[i]->height = 200;
  }

  //splash(20,30);
  springs[20]->height = 0;

  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB );
  glutInitWindowSize(SCREEN_WIDTH,SCREEN_HEIGHT);
  glutCreateWindow("Spring Demo");

  init_rendering();

  glutDisplayFunc(draw_scene);
  glutKeyboardFunc(handle_keypress);
  glutReshapeFunc(handle_resize);
  glutTimerFunc(SLEEP,update,0);

  glutMainLoop();
  
  for(i = 0; i < NUM_SPRINGS; i++){
    if(springs[i]){
      free(springs[i]);
    }
  }

  return 0;
}

void 
update_spring(struct spring *s,float k,float d)
{
  float x = s->height - TARGET_HEIGHT;
  s->velocity += (-k * x) - s->velocity*d;
  s->height += s->velocity;
}

void 
draw_vertex(vertex_2d v)
{
  glVertex2f(v.x,v.y);
}

void 
draw_springs()
{
  
  double bottom = SCREEN_HEIGHT;
  double scale = SCREEN_WIDTH/(NUM_SPRINGS-1);

  float dark_blue,blue;
  dark_blue = 1.0f;
  blue = 0.3;
  int i;

  for(i = 1; i < NUM_SPRINGS; i++){

    vertex_2d p1 = {(i-1)*scale, springs[i-1]->height};
    vertex_2d p2 = {i * scale, springs[i]->height};

    vertex_2d p3 = {p2.x, bottom};
    vertex_2d p4 = {p1.x, bottom};

    glColor3f(0.0f,0.0f,dark_blue);
    draw_vertex(p1);
    draw_vertex(p2);
    glColor3f(0.0f,0.0f,blue);
    draw_vertex(p3);
    glColor3f(0.0f,0.0f,dark_blue);
    draw_vertex(p1);
    glColor3f(0.0f,0.0f,blue);
    draw_vertex(p3);
    draw_vertex(p4);

  }

}

void 
draw_scene()
{
  glClear(GL_COLOR_BUFFER_BIT);
  glMatrixMode(GL_MODELVIEW);

  glBegin(GL_TRIANGLES);

  draw_springs();
  
  glEnd(); //End triangle coordinates
  glutSwapBuffers(); //Send the 3D scene to the screen
}


void 
handle_keypress(unsigned char key, int x,int y)
{
  int loc;

  switch(key){
  
    case 115:
      loc = rand() % NUM_SPRINGS-5;
      springs[loc-1]->height = -90;
      springs[loc]->height = -80;
      springs[loc+1]->height = -90;
      break;

    case 100:
      dampening += 0.05f;
      break;

    case 116:
      tension -= 0.001f;
      break;

    case 27:
      exit(0);
      break;
  }
}

void 
splash(int index, float speed)
{
  if(index >= 0 && index < NUM_SPRINGS)
    springs[index]->velocity = speed;
}

void 
init_rendering()
{
  glDisable(GL_DEPTH_TEST);
  
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  glOrtho(0.0f, SCREEN_WIDTH,SCREEN_HEIGHT, 0.0f, -1.0f,1.0f);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  glLineWidth(10.0f);
}

void
update()
{
  float left_deltas[NUM_SPRINGS];
  float right_deltas[NUM_SPRINGS];
  int i,j;
  const float spread = 0.15f;

  for(i = 0; i < NUM_SPRINGS; i++){
    update_spring(springs[i],tension,dampening);
  }

  for(j = 0; j < 8; j++){
      
    for(i = 0; i < NUM_SPRINGS; i++){

      if(i > 0){
        left_deltas[i] = spread * (springs[i]->height - springs[i-1]->height);
        springs[i-1]->velocity += left_deltas[i];
      }
      if(i < NUM_SPRINGS-1){
        right_deltas[i] = spread * (springs[i]->height - springs[i+1]->height);
        springs[i+1]->velocity += right_deltas[i];
      }
    }

    for(i = 0; i < NUM_SPRINGS; i++){

      if(i > 0)
        springs[i-1]->height += left_deltas[i];
      
      if(i < NUM_SPRINGS-1)
        springs[i+1]->height += right_deltas[i];
    }

  }

  glutPostRedisplay();
  glutTimerFunc(SLEEP,update,0);
}

void 
handle_resize(int w,int h)
{
  glViewport(0,0,w,h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  glOrtho(0.0f, SCREEN_WIDTH,SCREEN_HEIGHT, 0.0f, -1.0f,1.0f);

}
