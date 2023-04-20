#ifndef UpdateGame_H_
#define UpdateGame_H_

#include "raylib.h"
#define PLAYER_JUMP_SPD 290.0f
#define PLAYER_HOR_SPD 200.0f
#define G 400 // <--- gravidade

typedef struct EnvItem {// plataformas
    Rectangle rect;
    int blocking;
    Color color;
} EnvItem;

typedef struct Player {
    Vector2 position;
    float speed;
    bool canJump;
    Texture2D teste;
} Player;

void UpdatePlayer(Player *player, EnvItem *envItems, int envItemsLength, float delta)
{
    if (IsKeyDown(KEY_UP) && player->canJump){// faz o jogador pular   
        player->speed = -PLAYER_JUMP_SPD;
        player->canJump = false;
    }
    
    if (IsKeyDown(KEY_LEFT) && player->position.x >= 320) player->position.x -= PLAYER_HOR_SPD*delta;
    if (IsKeyDown(KEY_RIGHT) && player->position.x <= 600) player->position.x += PLAYER_HOR_SPD*delta;
    if (IsKeyDown(KEY_DOWN) && player->position.y <= 300 && player->position.y ==200) player->position.y += PLAYER_HOR_SPD*delta;

    int hitObstacle = 0;
    for (int i = 0; i < envItemsLength; i++){// verificar colisão com plataformas
        EnvItem *ei = envItems + i;
        Vector2 *p = &(player->position);
        if (ei->blocking &&
            ei->rect.x <= p->x &&
            ei->rect.x + ei->rect.width >= p->x &&
            ei->rect.y >= p->y &&
            ei->rect.y <= p->y + player->speed*delta)   
        {
            hitObstacle = 1;
            player->speed = 0.0f;
            p->y = ei->rect.y;
        }
    }

    if (!hitObstacle){// caso a verificação de colisão seja falso o jogador não consegue pular
        player->position.y += player->speed*delta;
        player->speed += G*delta;
        player->canJump = false;
    }
    else player->canJump = true;
}

#endif /* UpdateGame_H_ */