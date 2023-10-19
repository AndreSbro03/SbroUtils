#include <stdio.h>
#include <raylib.h>
#include <math.h>
#define SCREEN_HEIGHT 720 // REGOLAZIONE ALTEZZA SCHERMO
#define SCREEN_WIDTH (SCREEN_HEIGHT * 16) / 9 // FORMATO PREDEFINITO 16:9
#define LIGHT_POINT_DIMENSION 30 // DIMESIONE DEL CERCHIO CHE INDICA L'ORIGINE DEI RAGGI
#define LIGHT_RAY_THICKNESS 2 // SPESSORE RAGGI
#define LIGHT_RAY_MAX_LEN 1200 // LUNGHEZZA MASSIMA RAGGI
/* 
POSSIBILITA' DI FAR RUOTARE I RAGGI (CON A E D), COSIGLIATO SE IL PC NON REGGE NUMEROSI RAGGI 
 E QUINDI ANCHE USANDONE POCHI, PER L'EFFETTO DELLA ROTAZIONE, SI RIESCE COMUNQUE A DARE UN EFFETTO 
 COMPLESSIVO SIMILE A QUELLO CHE SI OTTERREBBE SE SI UTILIZZASSERO MOLTI PIU' RAGGI
*/
#define RAYS_CAN_MOVE false 
#define LIGHT_RAY_ANG_SPEED 0.19 // VELOCITA' DI ROTAZIONE DEI RAGGI
#define MAX_OBS 10 // MASSIMO NUMERO DI MURI REALIZZABILI (TASTO DESTRO DEL MAUSE E TRASCINARE PER REGOLARE LA DIMENSIONE)
#define N_RAYS 1200 // NUMERO DI RAGGI
#define DELTA_µ 2*PI/N_RAYS // DISTRIBUZIONE UNIFORME DEI RAGGI

Vector2 getRayEndPos(float ang, Vector2 lightPos, Rectangle obsList[], int corrObs);
float distance(Vector2 p1, Vector2 p2);

int main(void){

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Ray Tracing");
    SetTargetFPS(60);

    int w = GetScreenWidth();
    int h = GetScreenHeight();

    Vector2 lightPos = {w * 0.5, h * 0.5};
    Vector2 lightEnd = lightPos;

    float µ = 0;

    Rectangle obsList[MAX_OBS];
    int corrObs = 0;
    bool drawing = false;

    Vector2 mInitPos = {0, 0};
    Vector2 mEndPos = {0, 0}; 

    while (!WindowShouldClose()){

        if(IsMouseButtonDown(MOUSE_BUTTON_LEFT)){
            lightPos = GetMousePosition();
        }

        //MOVE RAYS

        if(RAYS_CAN_MOVE){
            if(IsKeyDown(KEY_D))
                µ += LIGHT_RAY_ANG_SPEED;
            if(IsKeyDown(KEY_A))
                µ -= LIGHT_RAY_ANG_SPEED; 
        }

        //END MOVE RAYS
        

        //DRAW WALL    

        if(IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)){
            mInitPos = GetMousePosition();
            mEndPos = mInitPos;   
            drawing = true;       
        }

        if(IsMouseButtonUp(MOUSE_BUTTON_RIGHT) && drawing){
            mEndPos = GetMousePosition();
            drawing = false;
        }

        if((mEndPos.x != mInitPos.x || mEndPos.y != mInitPos.y) && corrObs < MAX_OBS){
            Rectangle ob = {mInitPos.x, mInitPos.y, mEndPos.x - mInitPos.x, mEndPos.y - mInitPos.y};
            obsList[corrObs] = ob;
            corrObs++;
            mEndPos = mInitPos;
        }

        //END DRAW WALL

        BeginDrawing();
        ClearBackground(BLACK);

        for(int i = 0; i < corrObs; i++){
            DrawRectangleRec(obsList[i], DARKGRAY);
        }

        DrawCircleV(lightPos, LIGHT_POINT_DIMENSION, RED);
        for (size_t i = 0; i < N_RAYS; i++)
        {
            lightEnd = getRayEndPos((µ + i * DELTA_µ), lightPos, obsList, corrObs);
            DrawLineV(lightPos, lightEnd, YELLOW);      
        }       

        EndDrawing();
    }
    
    CloseWindow();

    return 0;
}

Vector2 getRayEndPos(float ang, Vector2 lightPos, Rectangle obsList[], int corrObs){

        Vector2 lightEnd;

        //Calcolo posizione finale raggio usando coordinate polari
        lightEnd.x = lightPos.x + LIGHT_RAY_MAX_LEN * cosf(ang);
        lightEnd.y = lightPos.y + LIGHT_RAY_MAX_LEN * sinf(ang);


        //Check per le collisioni
        float minDist = LIGHT_RAY_MAX_LEN;
        for(int i = 0; i < corrObs; i++){
            Vector2 strPos;
            Vector2 endPos;
            bool hit;
            Vector2 collisionPoint;
            float dist;       

            //Prima linea verticale
            strPos.x = obsList[i].x; 
            strPos.y = obsList[i].y;
            endPos.x = obsList[i].x; 
            endPos.y = obsList[i].y + obsList[i].height;
            hit = CheckCollisionLines(lightPos, lightEnd, strPos, endPos, &collisionPoint); 
            if(hit){
                dist = distance(lightPos, collisionPoint);
                if(dist < minDist){
                    minDist = dist;
                    lightEnd = collisionPoint;
                }                    
                hit = false;
            }
            
            //Seconda linea verticale
            strPos.x = obsList[i].x + obsList[i].width;
            strPos.y = obsList[i].y;
            endPos.x = obsList[i].x + obsList[i].width;
            endPos.y = obsList[i].y + obsList[i].height;
            hit = CheckCollisionLines(lightPos, lightEnd, strPos, endPos, &collisionPoint); 
            if(hit){
                dist = distance(lightPos, collisionPoint);
                if(dist < minDist){
                    minDist = dist;
                    lightEnd = collisionPoint;
                }                    
                hit = false;
            }
            
            //Prima linea orizzontale
            strPos.x = obsList[i].x;
            strPos.y = obsList[i].y;
            endPos.x = obsList[i].x + obsList[i].width;
            endPos.y = obsList[i].y;
            hit = CheckCollisionLines(lightPos, lightEnd, strPos, endPos, &collisionPoint); 
            if(hit){
                dist = distance(lightPos, collisionPoint);
                if(dist < minDist){
                    minDist = dist;
                    lightEnd = collisionPoint;
                }                    
                hit = false;
            }
            
            //Seconda linea orizzontale
            strPos.x = obsList[i].x;
            strPos.y = obsList[i].y + obsList[i].height;
            endPos.x = obsList[i].x + obsList[i].width;
            endPos.y = obsList[i].y + obsList[i].height;
            hit = CheckCollisionLines(lightPos, lightEnd, strPos, endPos, &collisionPoint); 
            if(hit){
                dist = distance(lightPos, collisionPoint);
                if(dist < minDist){
                    minDist = dist;
                    lightEnd = collisionPoint;
                }                    
                hit = false;
            }
              

        }

        return lightEnd;
}

float distance(Vector2 p1, Vector2 p2){
    return sqrtf(powf((p1.x - p2.x),2) + powf((p1.y - p2.y),2));
}

