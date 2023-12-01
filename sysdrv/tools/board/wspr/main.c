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
 #include <time.h>
 #include <stdlib.h>
 #include "wspr.h"

 #define BASE_FREQ_LOW     14097000
 // high is end of 200hz spectrum - 6 hz to take account for transmission.
 // Useful docs here: https://www.qrp-labs.com/images/ultimate3s/operation3.11.pdf
 #define BASE_FREQ_HIGH     14097194

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

int init_wspr(FILE *fen, FILE *ffreq, char *clk) {
    fen = (fopen("/proc/clk/enable", "w"));
    if(fen == NULL)
    {
       printf("Error opening /proc/clk/enable!");
       return 1;
    }
    ffreq = (fopen("/proc/clk/rate", "w"));
    if(ffreq == NULL)
    {
       printf("Error opening /proc/clk/rate!");
       return 1;
    }
    // enable clk0
    //echo enable [clk_name] > /proc/clk/enable
    fprintf(fen,"enable %s", clk);
    return 0;
}

int send_tone(uint32_t base_freq, char *data, int index, FILE *ffreq, char *clk) {
    // set freq
    // echo [clk_name] [rate(Hz)] > /proc/clk/rate
    uint8_t tone = wspr_get_tone(data, index);
    uint32_t f = base_freq + (3 * tone) / 2;
    fprintf(ffreq,"rate %s %i", clk, f);
    return 0;
}

int stop_wspr(FILE *fen, FILE *ffreq, char *clk) {
    //disable clk0
    // echo disable [clk_name] > /proc/clk/enable
    fprintf(fen,"disable %s", clk);
    fclose(fen);
    fclose(ffreq);
    return 0;
}

 int main() {
    FILE *fen;
    FILE *ffreq;
    char grid[6];
    char callsign[6] = "2X0UAJ";
    char clk_name[4] = "clk0";
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

    // Decide base frequency for transmission
    srand(time(NULL));
    uint32_t base_freq = BASE_FREQ_LOW + (rand() % (BASE_FREQ_HIGH - BASE_FREQ_LOW)); 

    // Start transmit
    if (init_wspr(fen, ffreq, clk_name) != 0) {
        printf("%s unable to init!", clk_name);
        return 1;
    }
    for (int i=0; i < WSPR_BUFFER_SIZE; i++) {
        clock_t before = clock();
        if (send_tone(base_freq, wspr_data, i, ffreq, clk_name) != 0) {
            printf("Transmit failure");
            break;
        }
        do {

        } while (clock() - before < WSPR_SYMBOL_TIME);
    }
    if (stop_wspr(fen, ffreq, clk_name) != 0) {
        printf("Disconnect device immediately!");
        return 2;
    }

    printf("Transmission Complete");

}