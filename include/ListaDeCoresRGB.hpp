//https://community.khronos.org/t/color-tables/22518/5
#ifndef ListaDeCoresRGB_hpp
#define ListaDeCoresRGB_hpp

#include <iostream>
#include <cmath>
#include <ctime>
#include <fstream>
#include <sys/time.h>
using namespace std;

#ifdef __APPLE__
#include <GLUT/glut.h>
#endif

#ifdef __linux__
#include <GL/glut.h>
#endif

enum {
    Aquamarine,
    BlueViolet,
    Brown,
    CadetBlue,
    Coral,
    CornflowerBlue,
    DarkGreen,
    DarkOliveGreen,
    DarkOrchid,
    DarkSlateBlue,
    DarkSlateGray,
    DarkSlateGrey,
    DarkTurquoise,
    Firebrick,
    ForestGreen,
    Gold,
    Goldenrod,
    GreenYellow,
    IndianRed,
    Khaki,
    LightBlue,
    LightSteelBlue,
    LimeGreen,
    Maroon,
    MediumAquamarine,
    MediumBlue,
    MediumForestGreen,
    MediumGoldenrod,
    MediumOrchid,
    MediumSeaGreen,
    MediumSlateBlue,
    MediumSpringGreen,
    MediumTurquoise,
    MediumVioletRed,
    MidnightBlue,
    Navy,
    NavyBlue,
    Orange,
    OrangeRed,
    Orchid,
    PaleGreen,
    Pink,
    Plum,
    Salmon,
    SeaGreen,
    Sienna,
    SkyBlue,
    SlateBlue,
    SpringGreen,
    SteelBlue,
    Tan,
    Thistle,
    Turquoise,
    Violet,
    VioletRed,
    Wheat,
    YellowGreen,
    SummerSky,
    RichBlue,
    Brass,
    Copper,
    Bronze,
    Bronze2,
    Silver,
    BrightGold,
    OldGold,
    Feldspar,
    Quartz,
    NeonPink,
    DarkPurple,
    NeonBlue,
    CoolCopper,
    MandarinOrange,
    LightWood,
    MediumWood,
    DarkWood,
    SpicyPink,
    SemiSweetChoc,
    BakersChoc,
    Flesh,
    NewTan,
    NewMidnightBlue,
    VeryDarkBrown,
    DarkBrown,
    DarkTan,
    GreenCopper,
    DkGreenCopper,
    DustyRose,
    HuntersGreen,
    Scarlet,
    Med_Purple,
    Light_Purple,
    Very_Light_Purple,
    Yellow,
    Black,
    DarkYellow,
    Red,
    Green
};

void defineCor(int c);

#endif
