#ifndef Shoot_H_
#define Shoot_H_

#define enemyCount 5

void shoot(Rectangle *bullets, int *bulletCount, Player *player, Rectangle *enemies, int *score)
{
    if (IsKeyPressed(KEY_SPACE)){// função de atirar na posição do jogador
        bullets[*bulletCount].x = player->position.x + 40;
        bullets[*bulletCount].y = player->position.y - 40 / 2 - 30;
        bullets[*bulletCount].width = 10;
        bullets[*bulletCount].height = 10;
        *bulletCount += 1;
    }

    for (int i = 0; i < *bulletCount; i++)
    {
        bullets[i].x += 10;

        if (bullets[i].x > 970)
        {
            bullets[i] = bullets[*bulletCount - 1];
            *bulletCount -= 1;
        }

        for (int j = 0; j < enemyCount; j++)
        {
            if (CheckCollisionRecs(bullets[i], enemies[j])){// verifica colisão do disparo
                bullets[i] = bullets[*bulletCount - 1];
                *bulletCount -= 1;
                enemies[j].x = GetRandomValue(1000, 1500);
                *score += 10;
            }
        }
    }
}

#endif /* Shoot_H_ */