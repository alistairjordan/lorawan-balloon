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
 #include <errno.h>
 #include <unistd.h> 
 #include <sys/time.h> // for clock_gettime()   

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

int init_wspr(char *clk) {
    char echo_command[100];
    // use system calls for the time being, direct file access seems to be throwing all kinds of errors, and I can't workout why..
    // this needs FIXING!
    snprintf(echo_command, sizeof(echo_command), "echo 'enable clk0' > /proc/clk/enable");
    return (system(echo_command));
}

int send_tone(uint32_t base_freq, char *data, int index, char *clk) {
    char echo_command[100];
    // set freq
    // echo [clk_name] [rate(Hz)] > /proc/clk/rate
    uint8_t tone = wspr_get_tone(data, index);
    uint32_t f = base_freq + (3 * tone) / 2;
    printf("Sending tone %i of %i at %i Hz\n",index+1,WSPR_SYMBOL_COUNT,f);
    snprintf(echo_command, sizeof(echo_command), "echo 'clk0 %d' > /proc/clk/rate", tone);
    return (system(echo_command));
}

int stop_wspr(char *clk) {
    //disable clk0
    // echo disable [clk_name] > /proc/clk/enable
    char echo_command[100];
    // use system calls for the time being, direct file access seems to be throwing all kinds of errors, and I can't workout why..
    // this needs FIXING!
    snprintf(echo_command, sizeof(echo_command), "echo 'disable clk0' > /proc/clk/enable");
    return (system(echo_command));
}

 int main(int argc, char *argv[]) {
    char grid[6];
    char callsign[6];//"=2X0UAJ";
    char clk_name[5] = "clk0\0";
    int32_t lon = 0;
    int32_t lat = 0;
    uint8_t wspr_data[WSPR_BUFFER_SIZE];

    if (argc < 4) {
        printf("Usage is ./wspr lon[int32] lat[int32] CALLSIGN[char[6]]");
        return 1;
    }
    sscanf(argv[1],"%ld",&lon);
    sscanf(argv[2],"%ld",&lat);
    sscanf(argv[3],"%s.6",&callsign);

    if (gps_to_grid(grid, lon, lat) != 0) {
        printf("Cannot translate long/lat\n");
        return 1;
    }
    if (wspr_encode(wspr_data, callsign, grid, 7) != 0) {
        printf("Cannot encode WSPR message\n");
        return 1;
    }
    
    printf("Decide random frequency within band for transmission\n");
    // Decide base frequency for transmission
    srand(time(NULL));
    uint32_t base_freq = BASE_FREQ_LOW + (rand() % (BASE_FREQ_HIGH - BASE_FREQ_LOW)); 
    printf("Freqeuency decided at %i\n",base_freq);

    printf("Attempt to init %s for transmission\n", clk_name);
    // Start transmit
    if (init_wspr(clk_name) != 0) {
        printf("%s unable to init!\n", clk_name);
        return 1;
    }
    printf("%s init completete, start sending tones.\n", clk_name);

    int msec = 0;
    struct timeval start, end;
    long secs_used,micros_used;
    //printf("Start time: %d\n", start_time);
    for (int i=0; i < WSPR_SYMBOL_COUNT; i++) {
        gettimeofday(&start, NULL);
        if (send_tone(base_freq, wspr_data, i, clk_name) != 0) {
            printf("Transmit failure\n");
            break;
        }
        gettimeofday(&end, NULL);
        // account for how long it took for send_tone to work with drift correction
        secs_used=(end.tv_sec - start.tv_sec); //avoid overflow by subtracting first
        micros_used= ((secs_used*1000000) + end.tv_usec) - (start.tv_usec);        
        usleep((WSPR_SYMBOL_TIME*1000 - micros_used));
    }

    if (stop_wspr(clk_name) != 0) {
        printf("Disconnect device immediately!\n");
        return 2;
    }

    printf("Transmission Complete\n");

}