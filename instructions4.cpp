//#include <iostream>
//
//int main() {
//    int a = 42;
//    int b, c, d, e, f, g, h, i, j;
//
//    b = a + 3;
//
//    c = (a < 5) ? 1 : 0;
//
//    d = (static_cast<unsigned int>(a) < 10) ? 1 : 0;
//
//    e = a ^ 15;
//
//    f = a | 7;
//
//    g = a & 1;
//
//    h = a << 2;
//
//    i = a >> 1;
//
//    j = a >> 2;
//
//    // SB
//    char* ptr_c = reinterpret_cast<char*>(&c);
//    char* ptr_a = reinterpret_cast<char*>(nullptr); // Assuming 'a' is a valid memory address
//    ptr_a[0] = ptr_c[0];
//
//    // SH
//    short* ptr_d = reinterpret_cast<short*>(&d);
//    ptr_a[2] = ptr_d[0];
//
//    return 0;
//}
