extern unsigned char sel;
extern int A, B, X0, Y0, X1, Y1, X2, Y2, X3, Y3, X4, Y4, X5, Y5, X6, Y6, X7, Y7, XD, YD;
extern short Z;
void h();
void f(void) {
    h();
    switch (sel) {
    case 0: A = X0; B = Y0; break;
    case 1: A = X1; B = Y1; break;
    case 2: A = X2; B = Y2; break;
    case 3: A = X3; B = Y3; break;
    case 4: A = X4; B = Y4; break;
    case 5: A = X5; B = Y5; break;
    case 6: A = X6; B = Y6; break;
    case 7: A = X7; B = Y7; break;
    default: A = XD; B = YD; break;
    }
    Z = 1;
    h();
}
