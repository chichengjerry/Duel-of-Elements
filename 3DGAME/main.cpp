/***********************************************
 * [3D_GAME]
 * メイン処理
 * GP11A341 24 張馳騁
 ***********************************************/

/***********************************************
 * インクルードファイル
 ***********************************************/
#include "main.h"
#include "bezier.h"
#include "core.h"
#include "heap.h"
#include "map.h"
#include "node.h"

/***********************************************
 * マクロ定義
 ***********************************************/
#define APP_CLASSNAME	_T("AppClass")
#define APP_TITLE		_T("")
#define MAP_SIZE		15

static int weight_map[MAP_SIZE][MAP_SIZE] = {
	{ 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 },
	{ 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 },
	{ 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 },
	{ 0,  0, -1, -1, -1, -1,  0,  0,  0,  0, -1, -1, -1,  0,  0 },
	{ 0,  0,  0,  0,  0, -1,  0,  0,  0,  0, -1,  0,  0,  0,  0 },
	{ 0,  0,  0,  0,  0, -1,  0,  0,  0,  0, -1,  0,  0,  0,  0 },
	{ 0,  0,  0,  0,  0, -1,  0,  0,  0,  0, -1,  0,  0,  0,  0 },
	{ 0,  0,  0,  0,  0, -1,  0,  0,  0,  0, -1,  0,  0,  0,  0 },
	{ 0,  0,  0,  0,  0, -1,  0,  0,  0,  0, -1,  0,  0,  0,  0 },
	{ 0,  0,  0,  0,  0, -1,  0,  0,  0,  0, -1,  0,  0,  0,  0 },
	{ 0,  0,  0,  0,  0, -1,  0,  0,  0,  0, -1,  0,  0,  0,  0 },
	{ 0,  0, -1, -1, -1, -1,  0,  0,  0,  0, -1, -1, -1,  0,  0 },
	{ 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 },
	{ 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 },
	{ 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 },
};

int main(int argc, char* argv[]) {

	static map_t* map = map_create(15, 15);
	static node_t* path[HEAP_MAX] = { NULL };

	// Initialize weight_map
	for (int y = 0; y < 15; y++) {
		for (int x = 0; x < 15; x++) {
			node_t node;

			node.came_from = NULL;
			node.f_score = -1;
			node.g_score = -1;
			node.weight = weight_map[y][x];
			node.x = x;
			node.y = y;

			map_update(map, x, y, &node);

			// node_t *value = (node_t*)map_read(map, x, y);
			// 
			// printf("%2d ", value->weight);
		}
	}

	node_t* start = map_read(map, 4, 4);
	node_t* goal = map_read(map, 11, 7);

	map_find_path_aa(path, map, start, goal);
	system("cls");
	if (path[0]) {
		for (int i = 0; path[i]; i++) {
			printf("%02d,%02d ", path[i]->x, path[i]->y);
			weight_map[path[i]->y][path[i]->x] = 99;
		}
		printf("\n\n");
		for (int y = 0; y < MAP_SIZE; y++) {
			for (int x = 0; x < MAP_SIZE; x++) {
				int t = weight_map[y][x];
				switch (t) {
				case -1:
					printf("[]");
					break;
				case 99:
					printf("**");
					break;
				default:
					printf("  ");
					break;
				}
			}
			printf("\n");
		}
	}
	else {
		printf("Can't find a path to goal.\n");
	}
	_getch();

	return 0;
}