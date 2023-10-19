#include <stdio.h>
#include <raylib.h>
#include <stdlib.h>
#include <math.h>
#define SCREEN_H 720 // HEIGHT OF THE SCREEN (px)
#define SCREEN_W (SCREEN_H * 16) / 9 // 16:9 RATIO
#define N_CELLS_X 15 // X DIMESION
#define N_CELLS_Y N_CELLS_X // FOR A SQUARE MAP (Y DIMENSION)
#define DRAW_GRID false // True IF YOU WANT THE GRID
#define THICKNESS 5.0f //THICKNESS OF THE GRID LINES
#define UPDATES 0.004f //VELOCITY OF THE SNAKE

typedef struct Vector2Int{
    int x;
    int y;
}Vector2Int;


typedef struct BodyPart{
    Vector2Int pos;
    struct BodyPart* succ;
} t_BodyPart;

typedef t_BodyPart* Ptr_body;

void DrawSnakeGrid(Rectangle gridBound);
void DrawSnakeBody(Rectangle gridBound, Ptr_body head);
void DrawSnakeApple(Rectangle gridBound, Vector2Int apple);
void DestroySnakeBody(Ptr_body l);

Ptr_body InsertHead(Ptr_body l, Vector2Int pos);
Ptr_body RemoveTail(Ptr_body l);

Vector2Int generateApple(Ptr_body head);
bool IsGameOver(Ptr_body head);

int main(void) {    

    Vector2 screen = {SCREEN_W, SCREEN_H};
    InitWindow(screen.x, screen.y, "Snake");
    SetTargetFPS(144);

    int w = GetScreenWidth();
    int h = GetScreenHeight();

    Rectangle gridBound = {h*0.5, h*0.1, h*0.8, h*0.8};

    //Init position
    Ptr_body head = NULL;
    Vector2Int pos = {4, 4};
    head = InsertHead(head, pos);
    pos.x = 5;
    head = InsertHead(head, pos);
    pos.x = 6;
    head = InsertHead(head, pos);
    Vector2Int apple;
    apple = generateApple(head);
    
    char dir = 'R'; //Initial direction
    char lastValidDir = dir;
    bool pause = false;
    bool haveEat = false;
    bool gameOver = false;

    Vector2Int tempPos;   
    float time = 0;
    int score = 0;

    while (!WindowShouldClose()){

        if(gameOver){
            if(IsKeyPressed(KEY_X)){
                CloseWindow();
                exit(1);
            }
        }

        if(IsKeyPressed(KEY_SPACE)){
            pause = !pause;
        }

        //ChangeDirection
        if(IsKeyDown(KEY_RIGHT) && lastValidDir != 'L'){
            dir = 'R';
        }
        else if(IsKeyDown(KEY_LEFT) && lastValidDir != 'R'){
            dir = 'L';
        }
        else if(IsKeyDown(KEY_UP) && lastValidDir != 'D'){
            dir = 'U';
        }
        else if(IsKeyDown(KEY_DOWN) && lastValidDir != 'U'){
            dir = 'D';
        }
                
        //Move the snake
        time += GetFrameTime();
        if(time > UPDATES && !gameOver && !pause){

            haveEat = false;
            lastValidDir = dir;

            switch (dir){
                case 'R':
                    tempPos = head->pos;
                    tempPos.x += 1;
                    head = InsertHead(head, tempPos);
                    break;

                case 'L':
                    tempPos = head->pos;
                    tempPos.x -= 1;
                    head = InsertHead(head, tempPos);
                    break;
                
                case 'U':
                    tempPos = head->pos;
                    tempPos.y -= 1;
                    head = InsertHead(head, tempPos);
                    break;

                case 'D':
                    tempPos = head->pos;
                    tempPos.y += 1;
                    head = InsertHead(head, tempPos);
                    break;
                
                default:
                    break;
            }

            //Check per vedere se ha mangiato la mela
            if(head->pos.x == apple.x && head->pos.y == apple.y){
                haveEat = true;
                apple = generateApple(head);
                score += 10;
            }

            if(!haveEat){
                head = RemoveTail(head);
            }

            //Chck per vedere se è morto
            gameOver = IsGameOver(head);

            time = 0;
        }

        

        BeginDrawing();

        ClearBackground(GREEN);
        DrawSnakeGrid(gridBound);  
        DrawSnakeBody(gridBound, head);
        DrawText(TextFormat("Score %06i", score), 5, 5, 50, BLACK);
        DrawText(TextFormat("SPACE to pause"), 5, 65, 20, BLACK);
        DrawText(TextFormat("Direction %c", dir), 5, 95, 20, BLACK);
        DrawSnakeApple(gridBound, apple);

        if(gameOver){
            DrawText(TextFormat("GAME OVER", dir), w * 0.1, h * 0.2, 175, RED);
            DrawText(TextFormat("Score %06i", score), w * 0.2, h * 0.2 + 200, 75, RED);
            DrawText(TextFormat("X to exit"), w * 0.2, h * 0.2 + 300, 75, RED);
        }
        
        EndDrawing();

    }
    
    DestroySnakeBody(head);
    if(!gameOver)
        CloseWindow();
    return 0;
}

void DrawSnakeGrid(Rectangle gridBound){
    //GRID
        DrawRectangleLinesEx(gridBound, THICKNESS, BLACK);

        if(DRAW_GRID){

            for(int i = 1; i < N_CELLS_X; i++){
                int xCord = gridBound.x + ((gridBound.width / N_CELLS_X) * i);
                Vector2 startPos = {xCord, gridBound.y};
                Vector2 endPos = {xCord, gridBound.y + gridBound.height};
                DrawLineEx(startPos, endPos, THICKNESS, BLACK);
            }

            for(int j = 1; j < N_CELLS_Y; j++){
                int yCord = gridBound.y + ((gridBound.height / N_CELLS_Y) * j);
                Vector2 startPos = {gridBound.x, yCord};
                Vector2 endPos = {gridBound.x + gridBound.width, yCord};
                DrawLineEx(startPos, endPos, THICKNESS, BLACK);
            }
        }
       
}

void DrawSnakeBody(Rectangle gridBound, Ptr_body head){

    //Draw snake body       
    for(Ptr_body temp = head; temp != NULL; temp = temp->succ){
        
        Vector2 pos = {(gridBound.width / N_CELLS_X) * temp->pos.x, (gridBound.height / N_CELLS_Y) * temp->pos.y};
        pos.x += gridBound.x;
        pos.y += gridBound.y;
        Vector2 dim = {(gridBound.width / N_CELLS_X), (gridBound.height / N_CELLS_Y)};        

        DrawRectangleV(pos, dim, BLACK);
    }

}

Ptr_body InsertHead(Ptr_body l, Vector2Int position){
    Ptr_body temp;
    temp = malloc(sizeof(t_BodyPart));

    if(temp){
        temp->pos = position;
        temp->succ = l;
        l = temp;
    }
    else
        printf("Errore allocazione memoria\n");

    return l;
}

Ptr_body RemoveTail(Ptr_body l){

    if(l == NULL){
        return NULL;
    }
    if(l->succ == NULL){
        free(l);
        return NULL;
    }

    Ptr_body prec, temp;
    prec = NULL;
    for(temp = l; temp->succ != NULL; temp = temp->succ){
        prec = temp;
    }

    prec->succ = NULL;
    free(temp);

    return l;
}

void DestroySnakeBody(Ptr_body l){
    Ptr_body temp;
    while (l != NULL){
        temp = l;
        l = l->succ;
        free(temp);
    }

}

Vector2Int generateApple(Ptr_body head){
    Vector2Int out;
    bool magicMap[N_CELLS_X][N_CELLS_Y];
    for(Ptr_body temp = head; temp != NULL; temp = temp->succ){
        int x = temp->pos.x;
        int y = temp->pos.y;
        magicMap[x][y] = 1;
    }

    while(true){
        int Rx = GetRandomValue(0, N_CELLS_X - 1);
        int Ry = GetRandomValue(0, N_CELLS_Y - 1);   
        if(!magicMap[Rx][Ry]){
            out.x = Rx;
            out.y = Ry;
            return out;
        }
    }
}

void DrawSnakeApple(Rectangle gridBound, Vector2Int apple){
            
    Vector2 pos = {(gridBound.width / N_CELLS_X) * apple.x, (gridBound.height / N_CELLS_Y) * apple.y};
    pos.x += gridBound.x + (gridBound.width / N_CELLS_X) * 0.5;
    pos.y += gridBound.y + (gridBound.width / N_CELLS_X) * 0.5;
    float radius = (gridBound.height / N_CELLS_Y) * 0.75 / 2;        

    DrawCircleV(pos, radius, RED);
}

bool IsGameOver(Ptr_body head){
    Vector2Int headPos = head->pos;
    Ptr_body temp = head->succ;

    if((headPos.x < 0 || headPos.x >= N_CELLS_X) || (headPos.y < 0 || headPos.y >= N_CELLS_Y))
        return 1;

    while (temp != NULL && (temp->pos.x != headPos.x || temp->pos.y != headPos.y ))
        temp = temp->succ;

    if(temp == NULL)
        return 0;
    else
        return 1;

}
