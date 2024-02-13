#include "UltimateColor.h"
UltimateColor::UltimateColor(int x1, int y1, int z1){

    x = x1;
    y = y1;
    z = z1;

}

UltimateColor::UltimateColor(){
    x = 0;
    y = 40;
    z = 200;
}

bool UltimateColor::operator==(UltimateColor other) {
    return x==other.x && y==other.y && z==other.z;
}
