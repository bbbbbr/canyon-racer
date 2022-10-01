#include "hUGEDriver.h"
#include <stddef.h>

static const unsigned char order_cnt = 4;

static const unsigned char P0[] = {
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
};
static const unsigned char P1[] = {
    DN(___,0,0xF05),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
};
static const unsigned char P2[] = {
    DN(Ds4,1,0x000),
    DN(___,0,0xC00),
    DN(___,0,0xC07),
    DN(___,0,0xC01),
    DN(F_4,2,0x000),
    DN(___,0,0xC00),
    DN(___,0,0xC07),
    DN(___,0,0xC01),
    DN(Fs4,3,0x000),
    DN(___,0,0xC00),
    DN(___,0,0xC07),
    DN(___,0,0xC01),
    DN(Ds4,4,0x000),
    DN(___,0,0xC00),
    DN(___,0,0xC07),
    DN(___,0,0xC01),
    DN(F_4,5,0x000),
    DN(___,0,0xC00),
    DN(___,0,0xC07),
    DN(___,0,0xC01),
    DN(Fs4,6,0x000),
    DN(___,0,0xC00),
    DN(___,0,0xC07),
    DN(___,0,0xC01),
    DN(F_4,7,0x000),
    DN(___,0,0xC00),
    DN(___,0,0xC07),
    DN(___,0,0xC01),
    DN(Cs4,8,0x000),
    DN(___,0,0xC00),
    DN(___,0,0xC07),
    DN(___,0,0xC01),
    DN(Ds4,9,0x000),
    DN(___,0,0xC00),
    DN(___,0,0xC07),
    DN(___,0,0xC01),
    DN(As3,10,0x000),
    DN(___,0,0xC00),
    DN(___,0,0xC07),
    DN(___,0,0xC01),
    DN(B_3,11,0x000),
    DN(___,0,0xC00),
    DN(___,0,0xC07),
    DN(___,0,0xC01),
    DN(As3,12,0x000),
    DN(___,0,0xC00),
    DN(___,0,0xC07),
    DN(___,0,0xC01),
    DN(Ds4,13,0x000),
    DN(___,0,0xC00),
    DN(___,0,0xC07),
    DN(___,0,0xC01),
    DN(As3,14,0x000),
    DN(___,0,0xC00),
    DN(___,0,0xC07),
    DN(___,0,0xC01),
    DN(Gs3,15,0x000),
    DN(___,0,0xC00),
    DN(___,0,0xC07),
    DN(___,0,0xC01),
    DN(F_4,14,0x000),
    DN(___,0,0xC00),
    DN(___,0,0xC07),
    DN(___,0,0xC01),
};
static const unsigned char P6[] = {
    DN(Ds4,13,0x000),
    DN(___,0,0xC00),
    DN(___,0,0xC07),
    DN(___,0,0xC01),
    DN(F_4,12,0x000),
    DN(___,0,0xC00),
    DN(___,0,0xC07),
    DN(___,0,0xC01),
    DN(Fs4,11,0x000),
    DN(___,0,0xC00),
    DN(___,0,0xC07),
    DN(___,0,0xC01),
    DN(Ds4,10,0x000),
    DN(___,0,0xC00),
    DN(___,0,0xC07),
    DN(___,0,0xC01),
    DN(F_4,9,0x000),
    DN(___,0,0xC00),
    DN(___,0,0xC07),
    DN(___,0,0xC01),
    DN(Fs4,8,0x000),
    DN(___,0,0xC00),
    DN(___,0,0xC07),
    DN(___,0,0xC01),
    DN(F_4,7,0x000),
    DN(___,0,0xC00),
    DN(___,0,0xC07),
    DN(___,0,0xC01),
    DN(Cs4,6,0x000),
    DN(___,0,0xC00),
    DN(___,0,0xC07),
    DN(___,0,0xC01),
    DN(Ds4,5,0x000),
    DN(___,0,0xC00),
    DN(___,0,0xC07),
    DN(___,0,0xC01),
    DN(As3,4,0x000),
    DN(___,0,0xC00),
    DN(___,0,0xC07),
    DN(___,0,0xC01),
    DN(B_3,3,0x000),
    DN(___,0,0xC00),
    DN(___,0,0xC07),
    DN(___,0,0xC01),
    DN(As3,2,0x000),
    DN(___,0,0xC00),
    DN(___,0,0xC07),
    DN(___,0,0xC01),
    DN(Ds4,1,0x000),
    DN(___,0,0xC00),
    DN(___,0,0xC07),
    DN(___,0,0xC01),
    DN(As3,2,0x000),
    DN(___,0,0xC00),
    DN(___,0,0xC07),
    DN(___,0,0xC01),
    DN(Gs3,3,0x000),
    DN(___,0,0xC00),
    DN(___,0,0xC07),
    DN(___,0,0xC01),
    DN(F_4,4,0x000),
    DN(___,0,0xC00),
    DN(___,0,0xC07),
    DN(___,0,0xC01),
};

static const unsigned char* const order1[] = {P0,P0};
static const unsigned char* const order2[] = {P1,P0};
static const unsigned char* const order3[] = {P2,P6};
static const unsigned char* const order4[] = {P0,P0};

static const unsigned char duty_instruments[] = {
8,0,240,128,
8,64,240,128,
8,128,240,128,
8,192,240,128,
8,0,241,128,
8,64,241,128,
8,128,241,128,
8,192,241,128,
8,128,240,128,
8,128,240,128,
8,128,240,128,
8,128,240,128,
8,128,240,128,
8,128,240,128,
8,128,240,128,
};
static const unsigned char wave_instruments[] = {
0,32,0,128,
0,32,1,128,
0,32,2,128,
0,32,3,128,
0,32,4,128,
0,32,5,128,
0,32,6,128,
0,32,7,128,
0,32,8,128,
0,32,9,128,
0,32,10,128,
0,32,11,128,
0,32,12,128,
0,32,13,128,
0,32,14,128,
};
static const unsigned char noise_instruments[] = {
240,0,0,0,0,0,0,0,
240,0,0,0,0,0,0,0,
240,0,0,0,0,0,0,0,
240,0,0,0,0,0,0,0,
240,0,0,0,0,0,0,0,
240,0,0,0,0,0,0,0,
240,0,0,0,0,0,0,0,
240,0,0,0,0,0,0,0,
240,0,0,0,0,0,0,0,
240,0,0,0,0,0,0,0,
240,0,0,0,0,0,0,0,
240,0,0,0,0,0,0,0,
240,0,0,0,0,0,0,0,
240,0,0,0,0,0,0,0,
240,0,0,0,0,0,0,0,
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
    1,17,17,17,17,18,34,114,99,167,187,236,26,247,239,143,
    1,17,17,17,17,18,34,114,99,167,187,236,26,247,239,143,
};

const hUGESong_t cyberacd = {7, &order_cnt, order1, order2, order3,order4, duty_instruments, wave_instruments, noise_instruments, NULL, waves};