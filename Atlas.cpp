#include "Atlas.h"

Atlas* atlas_player_left = new Atlas(_T("img/player_left_%d.png"), 6);
Atlas* atlas_player_right = new Atlas(_T("img/player_right_%d.png"), 6);
Atlas* atlas_enemy_left = new Atlas(_T("img/enemy_left_%d.png"), 6);
Atlas* atlas_enemy_right = new Atlas(_T("img/enemy_right_%d.png"), 6);

Atlas* atlas_autoenemy_idle = new Atlas(_T("img/idle%d.png"), 11);
Atlas* atlas_autoenemy_blast = new Atlas(_T("img/Blast%d.png"), 13);