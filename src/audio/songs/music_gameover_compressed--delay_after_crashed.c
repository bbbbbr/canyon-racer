#include "hUGEDriver.h"
#include <stddef.h>

static const unsigned char order_cnt = 10;

static const unsigned char P0[] = {
3,90,0,0,146,253,6,18,140,3,90,2,52,132,253,132,244,1,2,149,244,255,253,136,73,0
};
static const unsigned char P2[] = {
3,90,0,0,189,253,2,8,255,154,229,1,247,154,202,1,127,200,178,0
};
static const unsigned char P3[] = {
6,60,104,255,90,0,0,144,253,2,8,253,147,235,148,238,5,37,64,0,37,48,132,250,132,244,1,48,142,229,1,80,133,229,1,80,163,229,5,15,35,90,15,2,182,178,0
};
static const unsigned char P25[] = {
3,90,0,0,131,253,5,24,50,52,90,2,133,253,154,244,146,253,2,24,32,147,235,137,214,1,19,133,226,134,220,1,26,133,214,1,27,139,208,134,238,146,184,1,22,139,202,164,253,0
};
static const unsigned char P26[] = {
3,90,0,0,137,253,5,24,50,52,90,2,133,253,154,244,140,208,3,27,31,4,143,238,141,178,1,16,133,220,133,250,1,29,133,244,1,31,133,238,134,142,134,238,135,184,139,232,1,26,139,202,164,253,0
};
static const unsigned char P28[] = {
6,90,15,5,90,0,0,171,253,2,15,60,137,205,2,12,16,172,199,3,0,0,15,151,208,1,10,145,184,147,166,8,12,7,90,12,3,90,12,1,139,238,132,73,0
};
static const unsigned char P27[] = {
15,50,24,255,40,16,0,41,16,0,90,0,0,90,8,222,132,250,149,253,2,8,255,153,229,253,232,0
};
static const unsigned char P33[] = {
3,90,0,0,185,253,6,18,140,4,90,2,52,132,253,132,244,1,3,141,244,187,169,173,109,0
};
static const unsigned char P34[] = {
3,90,0,0,191,253,6,18,140,2,90,2,52,132,253,142,244,230,253,0
};
static const unsigned char P35[] = {
6,90,8,255,90,0,0,171,253,2,8,247,139,244,1,215,166,205,1,127,174,157,172,112,0
};
static const unsigned char P36[] = {
38,36,79,2,37,64,0,38,64,0,39,64,0,40,64,0,41,64,0,42,64,0,43,64,0,44,64,0,45,80,0,46,80,0,36,48,0,90,0,138,253,26,36,92,5,37,80,0,38,80,0,39,80,0,40,80,0,41,80,0,42,80,0,43,80,0,44,80,150,208,133,205,1,3,171,205,5,15,16,90,15,2,143,190,155,241,0
};
static const unsigned char P37[] = {
3,90,0,0,140,253,6,18,140,4,90,2,52,132,253,132,244,1,5,139,244,1,3,137,232,230,253,167,118,0
};
static const unsigned char P38[] = {
3,90,0,0,146,253,6,18,140,2,90,2,52,132,253,149,244,148,205,215,235,159,118,2,2,52,0
};
static const unsigned char P39[] = {
6,90,8,255,90,0,0,222,253,220,157,0
};
static const unsigned char P40[] = {
6,90,15,2,90,0,0,135,253,2,8,255,133,250,1,253,138,238,151,247,2,43,96,159,223,192,226,167,118,2,11,2,0
};
static const unsigned char P41[] = {
3,90,0,0,149,253,6,18,140,3,90,2,52,132,253,132,244,1,2,149,244,255,253,133,70,0
};

static const unsigned char* const order1[] = {P25,P33,P0,P33,P37};
static const unsigned char* const order2[] = {P26,P34,P41,P34,P38};
static const unsigned char* const order3[] = {P28,P35,P2,P35,P39};
static const unsigned char* const order4[] = {P27,P36,P3,P36,P40};

static const unsigned char duty_instruments[] = {
0,192,135,128,
0,64,199,128,
0,128,242,128,
0,128,248,128,
0,0,241,128,
0,64,241,128,
0,128,241,128,
0,0,145,128,
0,128,248,128,
0,128,248,128,
0,128,248,128,
0,128,248,128,
0,128,248,128,
0,128,248,128,
0,128,248,128,
};
static const unsigned char wave_instruments[] = {
0,32,0,128,
0,32,1,128,
0,32,2,128,
0,32,3,128,
0,32,4,128,
0,32,5,128,
0,32,6,128,
0,32,10,128,
0,32,8,128,
0,32,9,128,
0,32,10,128,
0,32,11,128,
0,32,13,128,
0,32,13,128,
0,32,14,128,
};
static const unsigned char noise_instruments[] = {
247,42,0,0,0,0,0,0,
64,58,2,1,1,1,1,5,
8,0,0,0,0,0,0,0,
248,0,0,0,0,0,0,0,
120,0,0,0,0,0,0,0,
119,0,0,0,0,0,0,0,
248,0,0,0,0,0,0,0,
248,0,0,0,0,0,0,0,
248,0,0,0,0,0,0,0,
248,0,0,0,0,0,0,0,
248,0,0,0,0,0,0,0,
248,0,0,0,0,0,0,0,
248,0,0,0,0,0,0,0,
248,0,0,0,0,0,0,0,
248,0,0,0,0,0,0,0,
};

static const unsigned char waves[] = {
    2,70,120,153,153,152,132,50,17,0,0,0,0,0,0,0,
    2,70,120,153,153,152,135,119,96,0,0,87,0,0,0,0,
    2,70,120,153,153,152,135,119,96,0,0,17,56,128,0,0,
    2,70,120,153,153,152,135,119,96,0,0,17,16,121,183,0,
    2,70,120,149,153,152,135,119,96,0,0,17,16,121,53,220,
    2,70,56,149,153,152,135,119,96,0,0,17,16,51,70,141,
    2,66,56,37,153,146,135,119,96,0,0,65,16,51,70,141,
    2,66,56,37,149,146,135,119,96,0,0,129,16,51,70,141,
    2,66,50,37,149,146,135,119,96,0,0,193,26,51,70,141,
    1,17,17,37,149,146,135,119,102,96,0,193,26,211,70,141,
    1,17,17,35,51,146,135,114,102,96,0,241,26,243,79,143,
    1,17,17,17,17,18,135,114,99,69,103,129,26,247,79,143,
    1,17,17,17,17,18,34,114,99,69,103,134,26,247,239,143,
    1,17,17,17,17,18,34,114,99,69,187,134,26,247,239,143,
    1,35,69,103,137,171,205,204,204,221,203,170,152,201,134,84,
    1,17,17,17,17,18,34,114,99,167,187,236,26,247,239,143,
};

const hUGESong_t music_gameover = {7, &order_cnt, order1, order2, order3,order4, duty_instruments, wave_instruments, noise_instruments, NULL, waves};
