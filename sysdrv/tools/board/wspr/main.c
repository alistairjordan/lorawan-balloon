/*
 * Copyright 2023 Alistair Jordan
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */
 #include <stdio.h>
 #include <stddef.h>
 #include <stdint.h>
 #include <string.h>
 #include "wspr.h"
 #include "si5351_linux.h"

/**
 * @brief Convert GPS coordinates into the 6 character grid locator.
 * @param grid Buffer for the result
 * @param lon Longitude in 15.16 signed fixed point format.
 * @param lat Latitude in 15.16 signed fixed point format.
 * @returns 0 on uccess.
 */
static int8_t gps_to_grid(char *grid, int32_t lon, int32_t lat) {
    const int32_t SCALE = 65536;

    int32_t tmp_lon = lon + 180 * SCALE;
    int32_t tmp_lat = lat +  90 * SCALE;

    if (tmp_lon < 0 || tmp_lon > 360 * SCALE)
        return 1;
    if (tmp_lat < 0 || tmp_lat > 180 * SCALE)
        return 2;

    /* Convert the longitude */
    grid[0] = tmp_lon / (20 * SCALE) + 'A';
    tmp_lon %= (20 * SCALE);

    grid[2] = tmp_lon / (2 * SCALE) + '0';
    tmp_lon %= (2 * SCALE);

    grid[4] = tmp_lon * 12 / SCALE + 'a';

    /* Convert the latitude */
    grid[1] = tmp_lat / (10 * SCALE) + 'A';
    tmp_lat %= (10 * SCALE);

    grid[3] = tmp_lat / SCALE + '0';
    tmp_lat %= SCALE;

    grid[5] = tmp_lat * 24 / SCALE + 'a';

    return 0;
}

int init_wspr() {
    return 0;
}

int send_tone() {
    return 0;
}

int stop_wspr() {
    return 0;
}

 int main() {
    char grid[6];
    char callsign[6] = "2X0UAJ";
    int32_t lon = 0;
    int32_t lat = 0;
    uint8_t wspr_data[WSPR_BUFFER_SIZE];

    if (gps_to_grid(grid, lon, lat) != 0) {
        printf("Cannot translate long/lat");
        return 1;
    }
    if (wspr_encode(wspr_data, callsign, grid, 7) != 0) {
        printf("Cannot encode WSPR message");
        return 1;
    }

    // Start transmit
    if (init_wspr() != 0) {
        printf("si5351 unable to init!");
        return 1;
    }
    for (int i=0; i < WSPR_BUFFER_SIZE; i++) {
        if (send_tone() != 0) {
            printf("Transmit failure");
            break;
        }
    }
    if (stop_wspr() != 0) {
        printf("Disconnect device immediately!");
        return 2;
    }

}