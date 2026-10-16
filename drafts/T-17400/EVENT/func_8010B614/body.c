void func_8010B614(void) {
    u8 *temp_v0;

    D_800B1746 = 4;
    func_8004BC20(4);
    temp_v0 = (D_800B1746 * 0x34) + D_800B0860;
    *(u8 *)((unsigned char *)temp_v0 + (432)) = (u8) (*(u8 *)((unsigned char *)temp_v0 + (432)) & 0xFFFD);
    func_80011DFC();
}
