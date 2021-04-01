#pragma once

typedef struct View_GameView View_GameView;

View_GameView* View_GameView_Create();
void           View_GameView_Destroy(View_GameView* self);

void    View_GameView_Loop(View_GameView* self);