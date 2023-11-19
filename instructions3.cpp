//#include <iostream>
//
//int main() {
//    int a = 65536, b, c, d = 1, e, f, g, h, i, j = 20, k;
//
//    // ADDI
//    a = a + 65536;
//
//    // LW
//    int addr_b = a + 4;
//    b = *reinterpret_cast<int*>(addr_b);
//
//    c = 0 + 1;
//
//    if (a == 0) {
//        // Some code here (20 lines of code)
//    }
//
//    e = c << a;
//
//    c = 0 + e;
//
//    a = a - 1;
//
//    // JAL
//    int link_register = 0 + 4;
//    int jump_target = link_register - 16;
//    k = jump_target;
//
//    // SW
//    int addr_c = 0;
//    *reinterpret_cast<int*>(addr_c) = c;
//
//    std::cout << "ECALL" << std::endl;
//
//    return 0;
//}
