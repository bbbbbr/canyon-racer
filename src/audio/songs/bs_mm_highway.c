#include "hUGEDriver.h"
#include <stddef.h>

static const unsigned char order_cnt = 22;

static const unsigned char P2[] = {
    DN(C_4,4,0x000),
    DN(___,0,0xC07),
    DN(C_4,4,0x000),
    DN(___,0,0xC07),
    DN(C_5,4,0x000),
    DN(___,0,0x000),
    DN(C_4,4,0x000),
    DN(___,0,0xC07),
    DN(C_4,4,0x000),
    DN(___,0,0xC07),
    DN(C_5,4,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(As4,4,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(___,0,0xC07),
    DN(___,0,0x000),
    DN(Ds4,4,0x000),
    DN(___,0,0xC07),
    DN(Ds4,4,0x000),
    DN(___,0,0xC07),
    DN(F_4,4,0x000),
    DN(___,0,0x000),
    DN(As3,4,0x000),
    DN(___,0,0xC07),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(C_4,4,0x000),
    DN(___,0,0xC07),
    DN(C_4,4,0x000),
    DN(___,0,0xC07),
    DN(C_5,4,0x000),
    DN(___,0,0x000),
    DN(C_4,4,0x000),
    DN(___,0,0xC07),
    DN(C_4,4,0x000),
    DN(___,0,0xC07),
    DN(C_5,4,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(As4,4,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(___,0,0xC07),
    DN(___,0,0x000),
    DN(Ds4,4,0x000),
    DN(___,0,0xC07),
    DN(Ds4,4,0x000),
    DN(___,0,0xC07),
    DN(F_4,4,0x000),
    DN(___,0,0x000),
    DN(As3,4,0x000),
    DN(___,0,0x000),
    DN(___,0,0xC07),
    DN(___,0,0x000),
};
static const unsigned char P3[] = {
    DN(C_8,1,0x000),
    DN(___,0,0x000),
    DN(C_8,2,0x000),
    DN(___,0,0x000),
    DN(C_8,1,0x000),
    DN(___,0,0x000),
    DN(C_8,2,0x000),
    DN(___,0,0x000),
    DN(C_8,1,0x000),
    DN(___,0,0x000),
    DN(C_8,2,0x000),
    DN(___,0,0x000),
    DN(C_8,1,0x000),
    DN(___,0,0x000),
    DN(C_8,2,0x000),
    DN(___,0,0x000),
    DN(C_8,1,0x000),
    DN(___,0,0x000),
    DN(C_8,2,0x000),
    DN(___,0,0x000),
    DN(C_8,1,0x000),
    DN(___,0,0x000),
    DN(C_8,2,0x000),
    DN(___,0,0x000),
    DN(C_8,1,0x000),
    DN(___,0,0x000),
    DN(C_8,2,0x000),
    DN(___,0,0x000),
    DN(C_8,1,0x000),
    DN(___,0,0x000),
    DN(D_7,3,0x000),
    DN(___,0,0x000),
    DN(C_8,1,0x000),
    DN(___,0,0x000),
    DN(C_8,2,0x000),
    DN(___,0,0x000),
    DN(C_8,1,0x000),
    DN(___,0,0x000),
    DN(C_8,2,0x000),
    DN(___,0,0x000),
    DN(C_8,1,0x000),
    DN(___,0,0x000),
    DN(C_8,2,0x000),
    DN(___,0,0x000),
    DN(C_8,1,0x000),
    DN(___,0,0x000),
    DN(C_8,2,0x000),
    DN(___,0,0x000),
    DN(D_7,3,0x000),
    DN(___,0,0x000),
    DN(C_8,2,0x000),
    DN(___,0,0x000),
    DN(D_7,3,0x000),
    DN(___,0,0x000),
    DN(C_8,2,0x000),
    DN(___,0,0x000),
    DN(D_7,3,0x000),
    DN(___,0,0x000),
    DN(D_7,3,0x000),
    DN(___,0,0x000),
    DN(D_7,3,0x000),
    DN(D_7,3,0x000),
    DN(D_7,3,0x000),
    DN(___,0,0x000),
};
static const unsigned char P7[] = {
    DN(C_8,1,0x000),
    DN(___,0,0x000),
    DN(C_8,2,0x000),
    DN(___,0,0x000),
    DN(D_7,3,0x000),
    DN(___,0,0x000),
    DN(C_8,2,0x000),
    DN(___,0,0x000),
    DN(C_8,1,0x000),
    DN(___,0,0x000),
    DN(C_8,2,0x000),
    DN(___,0,0x000),
    DN(D_7,3,0x000),
    DN(___,0,0x000),
    DN(C_8,2,0x000),
    DN(C_8,2,0x000),
    DN(C_8,1,0x000),
    DN(___,0,0x000),
    DN(C_8,2,0x000),
    DN(___,0,0x000),
    DN(D_7,3,0x000),
    DN(___,0,0x000),
    DN(C_8,2,0x000),
    DN(___,0,0x000),
    DN(C_8,1,0x000),
    DN(___,0,0x000),
    DN(C_8,2,0x000),
    DN(___,0,0x000),
    DN(D_7,3,0x000),
    DN(___,0,0x000),
    DN(C_8,2,0x000),
    DN(___,0,0x000),
    DN(C_8,1,0x000),
    DN(C_8,2,0x000),
    DN(C_8,2,0x000),
    DN(___,0,0x000),
    DN(D_7,3,0x000),
    DN(___,0,0x000),
    DN(C_8,2,0x000),
    DN(C_8,2,0x000),
    DN(C_8,1,0x000),
    DN(___,0,0x000),
    DN(C_8,2,0x000),
    DN(___,0,0x000),
    DN(D_7,3,0x000),
    DN(___,0,0x000),
    DN(C_8,2,0x000),
    DN(___,0,0x000),
    DN(C_8,1,0x000),
    DN(___,0,0x000),
    DN(C_8,2,0x000),
    DN(___,0,0x000),
    DN(D_7,3,0x000),
    DN(___,0,0x000),
    DN(C_8,2,0x000),
    DN(___,0,0x000),
    DN(C_8,1,0x000),
    DN(___,0,0x000),
    DN(C_8,2,0x000),
    DN(C_8,2,0x000),
    DN(D_7,3,0x000),
    DN(___,0,0x000),
    DN(C_8,2,0x000),
    DN(C_8,2,0x000),
};
static const unsigned char P70[] = {
    DN(C_3,1,0xF06),
    DN(___,0,0x000),
    DN(C_3,1,0x000),
    DN(___,0,0x000),
    DN(C_4,1,0x000),
    DN(___,0,0x000),
    DN(C_3,1,0x000),
    DN(___,0,0x000),
    DN(C_3,1,0x000),
    DN(___,0,0x000),
    DN(C_4,1,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(As3,1,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(Ds3,1,0x000),
    DN(___,0,0x000),
    DN(Ds3,1,0x000),
    DN(___,0,0x000),
    DN(F_3,1,0x000),
    DN(___,0,0x000),
    DN(As3,1,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(C_3,1,0xF06),
    DN(___,0,0x000),
    DN(C_3,1,0x000),
    DN(___,0,0x000),
    DN(C_4,1,0x000),
    DN(___,0,0x000),
    DN(C_3,1,0x000),
    DN(___,0,0x000),
    DN(C_3,1,0x000),
    DN(___,0,0x000),
    DN(C_4,1,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(As3,1,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(Ds3,1,0x000),
    DN(___,0,0x000),
    DN(Ds3,1,0x000),
    DN(___,0,0x000),
    DN(F_3,1,0x000),
    DN(___,0,0x000),
    DN(As3,1,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
};
static const unsigned char P71[] = {
    DN(G_3,1,0x000),
    DN(___,0,0x000),
    DN(G_3,1,0x000),
    DN(___,0,0x000),
    DN(G_4,1,0x000),
    DN(___,0,0x000),
    DN(G_3,1,0x000),
    DN(___,0,0x000),
    DN(G_3,1,0x000),
    DN(___,0,0x000),
    DN(G_3,1,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(F_3,1,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(As3,1,0x000),
    DN(___,0,0x000),
    DN(As3,1,0x000),
    DN(___,0,0x000),
    DN(C_4,1,0x000),
    DN(___,0,0x000),
    DN(F_3,1,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(G_3,1,0x000),
    DN(___,0,0x000),
    DN(G_3,1,0x000),
    DN(___,0,0x000),
    DN(G_4,1,0x000),
    DN(___,0,0x000),
    DN(G_3,1,0x000),
    DN(___,0,0x000),
    DN(G_3,1,0x000),
    DN(___,0,0x000),
    DN(G_3,1,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(F_3,1,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(As3,1,0x000),
    DN(___,0,0x000),
    DN(As3,1,0x000),
    DN(___,0,0x000),
    DN(C_4,1,0x000),
    DN(___,0,0x000),
    DN(F_3,1,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
};
static const unsigned char P72[] = {
    DN(C_6,4,0xF06),
    DN(D_6,4,0x000),
    DN(Ds6,4,0x000),
    DN(F_6,4,0x000),
    DN(G_6,4,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(G_6,3,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(F_6,4,0x000),
    DN(Ds6,4,0x000),
    DN(D_6,4,0x000),
    DN(As5,4,0x000),
    DN(C_6,4,0x000),
    DN(___,0,0x000),
    DN(D_6,4,0x000),
    DN(___,0,0x000),
    DN(Ds6,4,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(D_6,4,0x000),
    DN(___,0,0x000),
    DN(As5,4,0x000),
    DN(___,0,0x000),
    DN(C_6,4,0x000),
    DN(___,0,0x000),
    DN(Ds6,4,0x000),
    DN(___,0,0x000),
    DN(C_6,4,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(G_5,4,0x000),
    DN(As5,4,0x000),
    DN(Ds6,4,0x000),
    DN(___,0,0x000),
    DN(C_6,4,0x000),
    DN(Ds6,4,0x000),
    DN(F_6,4,0x000),
    DN(___,0,0x000),
    DN(Ds6,4,0x000),
    DN(F_6,4,0x000),
    DN(G_6,4,0x000),
    DN(___,0,0x000),
    DN(F_6,4,0x000),
    DN(G_6,4,0x000),
    DN(As6,4,0x000),
    DN(___,0,0x000),
    DN(Ds6,4,0x000),
    DN(___,0,0x000),
};
static const unsigned char P73[] = {
    DN(G_3,1,0x000),
    DN(___,0,0x000),
    DN(G_3,1,0x000),
    DN(___,0,0x000),
    DN(G_4,1,0x000),
    DN(___,0,0x000),
    DN(G_3,1,0x000),
    DN(___,0,0x000),
    DN(G_3,1,0x000),
    DN(___,0,0x000),
    DN(G_3,1,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(F_3,1,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(As3,1,0x000),
    DN(___,0,0x000),
    DN(As3,1,0x000),
    DN(___,0,0x000),
    DN(C_4,1,0x000),
    DN(___,0,0x000),
    DN(F_3,1,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(G_3,1,0x000),
    DN(___,0,0x000),
    DN(G_3,1,0x000),
    DN(___,0,0x000),
    DN(G_4,1,0x000),
    DN(___,0,0x000),
    DN(G_3,1,0x000),
    DN(___,0,0x000),
    DN(G_3,1,0x000),
    DN(___,0,0x000),
    DN(G_3,1,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(F_3,1,0x000),
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
static const unsigned char P74[] = {
    DN(C_7,5,0xF06),
    DN(___,0,0x000),
    DN(C_7,5,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(As6,4,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(___,4,0x000),
    DN(Ds6,4,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(F_6,4,0x000),
    DN(Ds6,4,0x000),
    DN(D_6,4,0x000),
    DN(As5,4,0x000),
    DN(C_6,4,0x000),
    DN(___,0,0x000),
    DN(D_6,4,0x000),
    DN(___,0,0x000),
    DN(Ds6,4,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(F_6,4,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(G_6,4,0x000),
    DN(___,0,0x000),
    DN(F_6,4,0x000),
    DN(G_6,4,0x000),
    DN(As6,4,0x000),
    DN(___,0,0x000),
    DN(F_6,4,0x000),
    DN(Ds6,4,0x000),
    DN(F_6,4,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(Ds6,4,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(D_6,4,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(As5,4,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(Ds6,4,0x000),
    DN(___,0,0x000),
    DN(Ds6,4,0x000),
    DN(___,0,0x000),
    DN(D_6,4,0x000),
    DN(___,0,0x000),
    DN(As5,4,0x000),
    DN(___,0,0x000),
};
static const unsigned char P82[] = {
    DN(Gs4,1,0xF06),
    DN(___,0,0x000),
    DN(Gs4,1,0x000),
    DN(___,0,0x000),
    DN(Gs4,1,0x000),
    DN(___,0,0x000),
    DN(Gs6,4,0x000),
    DN(___,0,0x000),
    DN(G_6,4,0x000),
    DN(___,0,0x000),
    DN(Ds6,4,0x000),
    DN(___,0,0x000),
    DN(C_6,4,0x000),
    DN(___,0,0x000),
    DN(F_6,4,0x000),
    DN(___,0,0x000),
    DN(As4,1,0x000),
    DN(___,0,0x000),
    DN(As4,1,0x000),
    DN(___,0,0x000),
    DN(As4,1,0x000),
    DN(___,0,0x000),
    DN(As6,4,0x000),
    DN(___,0,0x000),
    DN(F_6,4,0x000),
    DN(___,0,0x000),
    DN(Ds6,4,0x000),
    DN(___,0,0x000),
    DN(D_6,4,0x000),
    DN(___,0,0x000),
    DN(As5,4,0x000),
    DN(___,0,0x000),
    DN(C_6,4,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(___,0,0x205),
    DN(___,0,0x205),
    DN(___,0,0x205),
    DN(___,0,0x205),
    DN(___,0,0x205),
    DN(___,0,0x205),
    DN(___,0,0x205),
    DN(___,0,0x205),
    DN(___,0,0x205),
    DN(___,0,0x000),
    DN(As3,1,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(Ds3,1,0x000),
    DN(___,0,0x000),
    DN(Ds3,1,0x000),
    DN(___,0,0x000),
    DN(F_3,1,0x000),
    DN(___,0,0x000),
    DN(As3,1,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
};
static const unsigned char P83[] = {
    DN(Ds3,1,0x000),
    DN(___,0,0x000),
    DN(Ds3,1,0x000),
    DN(___,0,0x000),
    DN(Ds3,1,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(D_5,9,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(F_3,1,0x000),
    DN(___,0,0x000),
    DN(F_3,1,0x000),
    DN(___,0,0x000),
    DN(F_3,1,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(C_6,9,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(G_3,1,0x000),
    DN(___,0,0x000),
    DN(G_3,1,0x000),
    DN(___,0,0x000),
    DN(G_4,1,0x000),
    DN(___,0,0x000),
    DN(G_3,1,0x000),
    DN(___,0,0x000),
    DN(G_3,1,0x000),
    DN(___,0,0x000),
    DN(G_3,1,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(F_3,1,0x000),
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
static const unsigned char P84[] = {
    DN(Gs3,4,0x000),
    DN(___,0,0xC07),
    DN(Gs3,4,0x000),
    DN(___,0,0xC07),
    DN(Gs3,4,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(___,0,0xC00),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(As3,4,0x000),
    DN(___,0,0xC07),
    DN(As3,4,0x000),
    DN(___,0,0xC07),
    DN(As3,4,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(___,0,0xC00),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(C_4,4,0x000),
    DN(___,0,0xC07),
    DN(C_4,4,0x000),
    DN(___,0,0xC07),
    DN(C_5,4,0x000),
    DN(___,0,0x000),
    DN(C_4,4,0x000),
    DN(___,0,0xC07),
    DN(C_4,4,0x000),
    DN(___,0,0xC07),
    DN(C_5,4,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(As4,4,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(___,0,0xC07),
    DN(___,0,0x000),
    DN(Ds4,4,0x000),
    DN(___,0,0xC07),
    DN(Ds4,4,0x000),
    DN(___,0,0xC07),
    DN(F_4,4,0x000),
    DN(___,0,0x000),
    DN(As3,4,0x000),
    DN(___,0,0x000),
    DN(___,0,0xC07),
    DN(___,0,0x000),
};
static const unsigned char P85[] = {
    DN(Gs4,1,0xF06),
    DN(___,0,0x000),
    DN(Gs4,1,0x000),
    DN(___,0,0x000),
    DN(Gs4,1,0x000),
    DN(___,0,0x000),
    DN(As6,4,0x000),
    DN(___,0,0x000),
    DN(F_6,4,0x000),
    DN(___,0,0x000),
    DN(Ds6,4,0x000),
    DN(___,0,0x000),
    DN(D_6,4,0x000),
    DN(___,0,0x000),
    DN(As5,4,0x000),
    DN(___,0,0x000),
    DN(As4,1,0x000),
    DN(___,0,0x000),
    DN(As4,1,0x000),
    DN(___,0,0x000),
    DN(As4,1,0x000),
    DN(___,0,0x000),
    DN(F_5,4,0x000),
    DN(___,0,0x000),
    DN(D_6,4,0x000),
    DN(___,0,0x000),
    DN(C_6,4,0x000),
    DN(___,0,0x000),
    DN(As5,4,0x000),
    DN(___,0,0x000),
    DN(G_5,4,0x000),
    DN(___,0,0x000),
    DN(C_6,4,0x000),
    DN(___,0,0x000),
    DN(As5,4,0x000),
    DN(___,0,0x000),
    DN(C_6,4,0x000),
    DN(___,0,0x000),
    DN(D_6,4,0x000),
    DN(___,0,0x000),
    DN(Ds6,4,0x000),
    DN(___,0,0x000),
    DN(D_6,4,0x000),
    DN(___,0,0x000),
    DN(As5,4,0x000),
    DN(___,0,0x000),
    DN(C_6,4,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(Ds3,1,0x000),
    DN(___,0,0x000),
    DN(Ds3,1,0x000),
    DN(___,0,0x000),
    DN(F_3,1,0x000),
    DN(___,0,0x000),
    DN(As3,1,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
    DN(___,0,0x000),
};
static const unsigned char P97[] = {
    DN(C_7,5,0xF06),
    DN(C_7,6,0x000),
    DN(Ds6,5,0x000),
    DN(Ds6,6,0x000),
    DN(D_6,5,0x000),
    DN(D_6,6,0x000),
    DN(As5,5,0x000),
    DN(As5,6,0x000),
    DN(Ds6,5,0x000),
    DN(Ds6,6,0x000),
    DN(D_6,5,0x000),
    DN(D_6,6,0x000),
    DN(C_6,5,0x000),
    DN(C_6,6,0x000),
    DN(As5,5,0x000),
    DN(As5,6,0x000),
    DN(Ds6,5,0x000),
    DN(Ds6,6,0x000),
    DN(G_5,5,0x000),
    DN(G_5,6,0x000),
    DN(F_6,5,0x000),
    DN(F_6,6,0x000),
    DN(G_5,5,0x000),
    DN(G_5,6,0x000),
    DN(G_6,5,0x000),
    DN(G_6,6,0x000),
    DN(Ds6,5,0x000),
    DN(Ds6,6,0x000),
    DN(D_6,5,0x000),
    DN(D_6,6,0x000),
    DN(As5,5,0x000),
    DN(As5,6,0x000),
    DN(As6,5,0x000),
    DN(As6,6,0x000),
    DN(Ds6,5,0x000),
    DN(Ds6,6,0x000),
    DN(D_6,5,0x000),
    DN(D_6,6,0x000),
    DN(As5,5,0x000),
    DN(As5,6,0x000),
    DN(Ds6,5,0x000),
    DN(Ds6,6,0x000),
    DN(D_6,5,0x000),
    DN(D_6,6,0x000),
    DN(C_6,5,0x000),
    DN(C_6,6,0x000),
    DN(As5,5,0x000),
    DN(As5,6,0x000),
    DN(C_6,5,0x000),
    DN(C_6,6,0x000),
    DN(G_5,5,0x000),
    DN(G_5,6,0x000),
    DN(D_6,5,0x000),
    DN(D_6,6,0x000),
    DN(As5,5,0x000),
    DN(As5,6,0x000),
    DN(Ds6,5,0x000),
    DN(Ds6,6,0x000),
    DN(D_6,5,0x000),
    DN(D_6,6,0x000),
    DN(F_6,5,0x000),
    DN(F_6,6,0x000),
    DN(As5,5,0x000),
    DN(As5,6,0x000),
};
static const unsigned char P108[] = {
    DN(C_8,1,0x000),
    DN(___,0,0x000),
    DN(C_8,2,0x000),
    DN(___,0,0x000),
    DN(D_7,3,0x000),
    DN(___,0,0x000),
    DN(C_8,2,0x000),
    DN(___,0,0x000),
    DN(C_8,1,0x000),
    DN(___,0,0x000),
    DN(C_8,2,0x000),
    DN(___,0,0x000),
    DN(D_7,3,0x000),
    DN(___,0,0x000),
    DN(C_8,2,0x000),
    DN(C_8,2,0x000),
    DN(C_8,1,0x000),
    DN(___,0,0x000),
    DN(C_8,2,0x000),
    DN(___,0,0x000),
    DN(D_7,3,0x000),
    DN(___,0,0x000),
    DN(C_8,2,0x000),
    DN(___,0,0x000),
    DN(C_8,1,0x000),
    DN(___,0,0x000),
    DN(C_8,2,0x000),
    DN(___,0,0x000),
    DN(D_7,3,0x000),
    DN(___,0,0x000),
    DN(C_8,2,0x000),
    DN(___,0,0x000),
    DN(D_7,3,0x000),
    DN(C_8,2,0x000),
    DN(C_8,2,0x000),
    DN(___,0,0x000),
    DN(D_7,3,0x000),
    DN(___,0,0x000),
    DN(C_8,2,0x000),
    DN(C_8,2,0x000),
    DN(D_7,3,0x000),
    DN(___,0,0x000),
    DN(C_8,2,0x000),
    DN(___,0,0x000),
    DN(D_7,3,0x000),
    DN(___,0,0x000),
    DN(C_8,2,0x000),
    DN(___,0,0x000),
    DN(D_7,3,0x000),
    DN(___,0,0x000),
    DN(C_8,2,0x000),
    DN(___,0,0x000),
    DN(D_7,3,0x000),
    DN(___,0,0x000),
    DN(C_8,2,0x000),
    DN(___,0,0x000),
    DN(D_7,3,0x000),
    DN(___,0,0x000),
    DN(C_8,2,0x000),
    DN(C_8,2,0x000),
    DN(D_7,3,0x000),
    DN(___,0,0x000),
    DN(C_8,2,0x000),
    DN(C_8,2,0x000),
};
static const unsigned char P110[] = {
    DN(C_6,5,0xF06),
    DN(C_6,6,0x000),
    DN(G_5,5,0x000),
    DN(G_5,6,0x000),
    DN(D_6,5,0x000),
    DN(D_6,6,0x000),
    DN(G_5,5,0x000),
    DN(D_6,6,0x000),
    DN(Ds6,5,0x000),
    DN(Ds6,6,0x000),
    DN(D_6,5,0x000),
    DN(D_6,6,0x000),
    DN(C_6,5,0x000),
    DN(C_6,6,0x000),
    DN(As5,5,0x000),
    DN(As5,6,0x000),
    DN(C_6,5,0x000),
    DN(C_6,6,0x000),
    DN(G_5,5,0x000),
    DN(G_5,6,0x000),
    DN(D_6,5,0x000),
    DN(D_6,6,0x000),
    DN(G_5,5,0x000),
    DN(D_6,6,0x000),
    DN(Ds6,5,0x000),
    DN(Ds6,6,0x000),
    DN(D_6,5,0x000),
    DN(D_6,6,0x000),
    DN(C_6,5,0x000),
    DN(C_6,6,0x000),
    DN(As5,5,0x000),
    DN(As5,6,0x000),
    DN(C_6,5,0x000),
    DN(C_6,6,0x000),
    DN(G_5,5,0x000),
    DN(G_5,6,0x000),
    DN(Gs5,5,0x000),
    DN(Gs5,6,0x000),
    DN(G_5,5,0x000),
    DN(G_5,6,0x000),
    DN(Gs5,5,0x000),
    DN(Gs5,6,0x000),
    DN(C_6,5,0x000),
    DN(C_6,6,0x000),
    DN(D_6,5,0x000),
    DN(D_6,6,0x000),
    DN(As5,5,0x000),
    DN(As5,6,0x000),
    DN(C_6,5,0x000),
    DN(C_6,6,0x000),
    DN(D_6,5,0x000),
    DN(D_6,6,0x000),
    DN(Ds6,5,0x000),
    DN(Ds6,6,0x000),
    DN(F_6,5,0x000),
    DN(F_6,6,0x000),
    DN(Ds6,5,0x000),
    DN(Ds6,6,0x000),
    DN(D_6,5,0x000),
    DN(D_6,6,0x000),
    DN(C_6,5,0x000),
    DN(C_6,6,0x000),
    DN(As5,5,0x000),
    DN(As5,6,0x000),
};
static const unsigned char P114[] = {
    DN(D_7,3,0x000),
    DN(C_8,2,0x000),
    DN(C_8,2,0x000),
    DN(___,0,0x000),
    DN(D_7,3,0x000),
    DN(___,0,0x000),
    DN(C_8,2,0x000),
    DN(C_8,2,0x000),
    DN(D_7,3,0x000),
    DN(___,0,0x000),
    DN(C_8,2,0x000),
    DN(___,0,0x000),
    DN(D_7,3,0x000),
    DN(___,0,0x000),
    DN(C_8,2,0x000),
    DN(___,0,0x000),
    DN(D_7,3,0x000),
    DN(___,0,0x000),
    DN(C_8,2,0x000),
    DN(___,0,0x000),
    DN(D_7,3,0x000),
    DN(___,0,0x000),
    DN(C_8,2,0x000),
    DN(___,0,0x000),
    DN(D_7,3,0x000),
    DN(___,0,0x000),
    DN(C_8,2,0x000),
    DN(C_8,2,0x000),
    DN(D_7,3,0x000),
    DN(___,0,0x000),
    DN(C_8,2,0x000),
    DN(C_8,2,0x000),
    DN(D_7,3,0x000),
    DN(C_8,2,0x000),
    DN(C_8,2,0x000),
    DN(___,0,0x000),
    DN(D_7,3,0x000),
    DN(___,0,0x000),
    DN(C_8,2,0x000),
    DN(C_8,2,0x000),
    DN(D_7,3,0x000),
    DN(___,0,0x000),
    DN(C_8,2,0x000),
    DN(___,0,0x000),
    DN(D_7,3,0x000),
    DN(___,0,0x000),
    DN(C_8,2,0x000),
    DN(___,0,0x000),
    DN(D_7,3,0x000),
    DN(___,0,0x000),
    DN(C_8,2,0x000),
    DN(___,0,0x000),
    DN(D_7,3,0x000),
    DN(___,0,0x000),
    DN(C_8,2,0x000),
    DN(___,0,0x000),
    DN(D_7,3,0x000),
    DN(___,0,0x000),
    DN(C_8,2,0x000),
    DN(C_8,2,0x000),
    DN(D_7,3,0x000),
    DN(___,0,0x000),
    DN(D_7,3,0x000),
    DN(D_7,3,0xB02),
};

static const unsigned char* const order1[] = {P70,P72,P74,P82,P85,P70,P110,P97,P82,P85,P70};
static const unsigned char* const order2[] = {P71,P73,P73,P83,P83,P71,P73,P73,P83,P83,P71};
static const unsigned char* const order3[] = {P2,P2,P2,P84,P84,P2,P2,P2,P84,P84,P2};
static const unsigned char* const order4[] = {P3,P7,P7,P7,P7,P7,P7,P7,P7,P108,P114};

static const unsigned char duty_instruments[] = {
8,0,25,128,
8,0,112,128,
63,192,240,128,
8,192,247,128,
8,192,242,128,
8,0,145,128,
8,128,241,128,
15,128,193,128,
8,128,0,128,
127,192,240,128,
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
145,0,0,0,0,0,0,0,
81,0,0,0,0,0,0,0,
241,44,2,3,4,7,254,32,
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
    0,0,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
    0,0,0,0,255,255,255,255,255,255,255,255,255,255,255,255,
    0,0,0,0,0,0,0,0,255,255,255,255,255,255,255,255,
    14,0,10,169,0,0,0,0,0,220,160,0,255,255,255,255,
    0,1,18,35,52,69,86,103,120,137,154,171,188,205,222,239,
    254,220,186,152,118,84,50,16,18,52,86,120,154,188,222,255,
    122,205,219,117,33,19,104,189,220,151,65,1,71,156,221,184,
    15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,
    254,252,250,248,246,244,242,240,242,244,246,248,250,252,254,255,
    254,221,204,187,170,153,136,119,138,189,241,36,87,138,189,238,
    132,17,97,237,87,71,90,173,206,163,23,121,221,32,3,71,
    96,149,103,78,87,219,131,114,184,50,218,108,68,137,3,195,
    73,155,109,205,192,10,228,230,224,170,86,142,33,96,218,62,
    158,173,156,8,49,148,182,125,173,81,107,74,97,87,71,218,
    187,46,40,124,55,2,117,13,134,3,87,235,88,62,237,199,
    34,130,30,16,20,171,182,58,43,234,5,0,83,34,83,211,
};

const hUGESong_t bs_mm_highway = {7, &order_cnt, order1, order2, order3,order4, duty_instruments, wave_instruments, noise_instruments, NULL, waves};