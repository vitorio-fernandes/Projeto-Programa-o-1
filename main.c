//Grupo: Anderson André, Vitório Fernandes, Everton Aragão, João Frascisco

#include "raylib.h"
#include "raymath.h"
#include <stdio.h>
#include "UpdateGame.h"
#include "Shoot.h"

#define screenWidth 1280
#define screenHeight 720

int main(void)
{
    // Inicialização
    InitWindow(screenWidth, screenHeight, "Jogo Programação 1 ");

    Rectangle bullets[100];
    int bulletCount = 0;

    // inicializa e configura os inimigos
    Rectangle enemies[100];
    for (int i = 0; i < enemyCount; i++)
    {
        enemies[i].x = GetRandomValue(980,1000);
        enemies[i].y = GetRandomValue(200,300)-53;
        enemies[i].width = 50;
        enemies[i].height = 50;
    }

    // configurações do jogador
    Player player = { 0 };
    player.position = (Vector2){ (1280/2)-300, 300};
    player.speed = 0;
    player.canJump = false;
    player.teste = LoadTexture("boneco.png");

    // Carregar imagens e texturas do jogo
    Texture2D texture = LoadTexture("imagem_jogo.png");
    Texture2D moeda = LoadTexture("coin.png");
    Texture2D entrada = LoadTexture("entrada.png");
    Texture2D gameOver = LoadTexture("GameOver3.png");
    Texture2D heart3 = LoadTexture("heart3.png");

    // Criação das plataformas do jogo com cores 
    EnvItem envItems[] = {    
            {{ 0, 400, 1280, 200 }, 1, BLANK},
            {{ (1280/2)-350, 200, 700, 10 }, 1, RED },
            {{ (1280/2)-350, 300, 700, 10 }, 1, BLUE },
        };

    int envItemsLength = sizeof(envItems)/sizeof(envItems[0]);

// Som de pular
    InitAudioDevice();
    Sound som_Pular = LoadSound("coin.wav");

// Taxa de FPS
    SetTargetFPS(60);

// retangulos para hitbox do menu
    Rectangle box1 = { 350, 490, 140, 80 };
    Rectangle box2 = { 790, 490, 140, 80 };
    Rectangle box3 = { 565, 490, 140, 80 };

    int sairTela = 0;

// Mostrar a tela de início do Jogo
    while (!IsKeyPressed(KEY_ENTER))
    {   
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(GetMousePosition(), box1)) break; // inicializa o jogo
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(GetMousePosition(), box2)) return 0; // fecha o jogo
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(GetMousePosition(), box3)){ // carrega o ultimo save
            sairTela = 1;
            break;
        }

        BeginDrawing();

            ClearBackground(RAYWHITE);
            DrawTexture(entrada,0,0,WHITE);
            DrawRectangleRec(box1, BLANK);
            DrawRectangleRec(box2, BLANK);
            DrawRectangleRec(box3, BLANK);

        EndDrawing(); 
    }

    FILE *loadGame = fopen("PontosSalvos.csv","r");

    int scoreSave, vidaSave;

    if (loadGame != NULL) { // le o arquivo e salva o ultimo score do jogador
        while (!feof(loadGame)) {
            fscanf(loadGame, "%d;%d", &scoreSave,&vidaSave);
        }
    }

    int score = 0, vida = 3, pontuação = 0;

    if (scoreSave != 0 && sairTela == 1 && vidaSave != 0){
        score = scoreSave;
        vida = vidaSave;
        pontuação = 1;
    }

    FILE *saveGame = fopen("PontosSalvos.csv","w");

    bool pausa = false;

    // Loop do game
    while (!WindowShouldClose()) {
        if (IsKeyPressed(KEY_UP) && player.canJump) PlaySound(som_Pular); // Som ao clicar no espaço para pular
        
        float deltaTime = GetFrameTime();
        if (IsKeyPressed(KEY_P)) pausa = !pausa;
//---------------------------------------------------------------------------------------------------------------------------/
        UpdatePlayer(&player, envItems, envItemsLength, deltaTime);
        if (!pausa){
            BeginDrawing();
                ClearBackground(GetColor(0x052c46ff));
                    DrawTexture(texture,0,0, WHITE);
                    DrawTexture(moeda,8,20, WHITE);

                    if (pontuação == 0 && scoreSave != 0){ // muda como aparece os pontos na tela 
                        DrawText("Pontuação:", 60, 25, 20, BLACK);
                        DrawText("Ultima Pontuação:", 60, 45, 20, BLACK);
                        DrawText(TextFormat("%04i", score), 180, 25, 20, BLACK);
                        DrawText(TextFormat("%04i", scoreSave), 245, 45, 20, BLACK);
                    }else{
                        DrawText("Pontuação:", 60, 35, 20, BLACK);
                        DrawText(TextFormat("%04i", score), 180, 35, 20, BLACK);
                    }

                    DrawFPS(1180, 35);

                    if(vida == 1){
                        DrawTexture(heart3,20,80, WHITE);
                    }
                    if(vida == 2){
                        DrawTexture(heart3,20,80, WHITE);
                        DrawTexture(heart3,55,80, WHITE);
                    }
                    if(vida == 3){
                        DrawTexture(heart3,20,80, WHITE);
                        DrawTexture(heart3,55,80, WHITE);
                        DrawTexture(heart3,90,80, WHITE);
                    }
                    
                    // Atira os projéteis e detecta colisão
                    shoot(bullets, &bulletCount, &player, enemies, &score);

                    // Move os inimigos para a esquerda (muda dependendo da quantidade de pontos)
                    if (score <= 150){
                    for (int i = 0; i < enemyCount; i++) enemies[i].x -= 3;
                    }
                    else if(score > 1000){
                        for (int i = 0; i < enemyCount; i++) enemies[i].x -= 10;
                    }
                    else if(score > 500){
                        for (int i = 0; i < enemyCount; i++) enemies[i].x -= 7;
                    }
                    else if(score >400){
                        for (int i = 0; i < enemyCount; i++) enemies[i].x -= 6;
                    }
                    else if(score >300){
                        for (int i = 0; i < enemyCount; i++) enemies[i].x -= 5;
                    }
                    else if(score > 150){
                        for (int i = 0; i < enemyCount; i++) enemies[i].x -= 4;
                    }

                    for (int i = 0; i < enemyCount; i++){ //caso um inimigo passe do jogador ele salva o jogo e aparece a tela de game over
                        if (enemies[i].x < 250){
                            if (vida > 1){
                                vida -= 1;
                                enemies[i].x = GetRandomValue(1000, 1500);
                            }else{
                                vida -= 1;
                                fprintf(saveGame,"%i;%i",score,vida);
                                fclose(saveGame);
                                while(!IsKeyPressed(KEY_ENTER) && !WindowShouldClose()){
                                    BeginDrawing();
                                    ClearBackground(RAYWHITE);
                                    DrawTexture(gameOver,0,0,WHITE);
                                    DrawText("Pressione Enter para sair", 1280 / 2 - MeasureText("Pressione Enter para sair", 30) / 2, 600, 30, GRAY);
                                    EndDrawing();
                                }
                                return 0;
                            }
                        }
                    }

                    // Desenha os projéteis
                    for (int i = 0; i < bulletCount; i++) DrawRectangleRec(bullets[i], BLUE);

                    // Desenha os inimigos
                    for (int i = 0; i < enemyCount; i++) DrawRectangleRec(enemies[i], GREEN);

                    for (int i = 0; i < envItemsLength; i++) DrawRectangleRec(envItems[i].rect, envItems[i].color);

                    DrawTexture(player.teste,player.position.x-20,player.position.y-70, WHITE);

                EndMode2D();
            EndDrawing();
        }
        else{ // Caso o jogo esteja pausado, apenas exibe uma mensagem na tela
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawText("Jogo pausado (Pressione P para continuar)", 1280 / 2 - MeasureText("Jogo pausado (Pressione P para continuar)", 30) / 2, 720/2-30, 30, BLACK); 
        EndDrawing();
        }
    }
     
    if(vida != 0){
        fprintf(saveGame,"%i;%i",score,vida);
        fclose(saveGame);
    }

    UnloadTexture(moeda);
    UnloadTexture(texture);

    CloseWindow();        // Fechar a Janela
    return 0;
}