#include "../../src/cjson.h"
#include <stdio.h>


Map* make_correct_map() {
    Map* apple_location_map = initMap(3);
    insertFloat(&apple_location_map, "x", 0.0);
    insertFloat(&apple_location_map, "y", -0.176319);

    Map* apple_map = initMap(5);
    insertInt(&apple_map, "id", 0);
    insertMap(&apple_map, "location", apple_location_map);

    Map* coord = initMap(3);
    insertFloat(&coord, "x", 0.0);
    insertFloat(&coord, "y", 0.0);

    MapArray* player_coords = initMapArray(3);
    appendMap(&player_coords, coord);

    Map* player_map = initMap(3);
    insertInt(&player_map, "id", 1997571598);
    insertMapArray(&player_map, "coords", player_coords);

    MapArray* players_arr = initMapArray(3);
    appendMap(&players_arr, player_map);

    Map* correct_map = initMap(10);
    insertMap(&correct_map, "apple", apple_map);
    insertMapArray(&correct_map, "players", players_arr);
    insertInt(&correct_map, "test", 0);

    return correct_map;
}


int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: snake_game_state_test <json file>\n");
    }

    const char* path = argv[1];

    Map* correct_map = make_correct_map();

    Map* json = load_file(path);
    char* data = dump(json); 
    destroyMap(json);
    printf("%s\n", data);

    if (!map_cmp(json, correct_map)) {
        fprintf(stderr, "FAILED, maps are not complete.");
    } 
    else {
        printf("PASSED.");
    }

    // cleanup
    free(data);
    destroyMap(correct_map);

    return 0;
}
