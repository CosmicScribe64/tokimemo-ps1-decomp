#include "common.h"
#include "ovl/RPG_BAT.h"

void func_8014D380(void) {
    switch (D_8015EB98) {
    case 0x10000:
        D_8015EE94 = 0x8018C1F8;
        D_8015EE98 = 0x801BF9A4;
        D_8015EE90 = 0x801948E0;
        D_8015EE8C = 0x801B5758;
        D_8015EEAC = 0x8018C204;
        D_8015EEB0 = 0x80194598;
        D_8015EEA8 = 0x801948EC;
        D_8015EEA4 = 0x801B576C;
        D_8015EEC4 = 0x8018C24C;
        D_8015EEC8 = 0x8019460C;
        D_8015EEC0 = 0x8019494C;
        D_8015EEBC = 0x801B5874;
        D_8015EED8 = 0x801B594C;
        D_8015EEE0 = 0x801B794C;
        D_8015EEDC = 0x801B994C;
        D_8015EEE4 = 0x801BB94C;
        D_8015EED4 = 0x801BD94C;
        break;
    case 0x20000:
        D_8015EE94 = 0x801B781C;
        D_8015EE98 = 0x801B7864;
        D_8015EE90 = 0x8018C7E4;
        D_8015EE8C = 0x801AD620;
        D_8015EEAC = 0x80184160;
        D_8015EEB0 = 0x8018C4E0;
        D_8015EEA8 = 0x8018C7F0;
        D_8015EEA4 = 0x801AD634;
        D_8015EEC4 = 0x80184190;
        D_8015EEC8 = 0x8018C554;
        D_8015EEC0 = 0x8018C844;
        D_8015EEBC = 0x801AD73C;
        D_8015EED8 = 0x801AD814;
        D_8015EEE0 = 0x801AF814;
        D_8015EEDC = 0x801B1814;
        D_8015EEE4 = 0x801B3814;
        D_8015EED4 = 0x801B5814;
        break;
    case 0x40000:
        D_8015EE94 = 0x801B7864;
        D_8015EE98 = 0x801B78AC;
        D_8015EE90 = 0x8018C810;
        D_8015EE8C = 0x801AD638;
        D_8015EEAC = 0x801841FC;
        D_8015EEB0 = 0x8018C59C;
        D_8015EEA8 = 0x8018C81C;
        D_8015EEA4 = 0x801AD64C;
        D_8015EEC4 = 0x8018423C;
        D_8015EEC8 = 0x8018C610;
        D_8015EEC0 = 0x8018C85C;
        D_8015EEBC = 0x801AD754;
        D_8015EED8 = 0x801AD82C;
        D_8015EEE0 = 0x801AF82C;
        D_8015EEDC = 0x801B182C;
        D_8015EEE4 = 0x801B382C;
        D_8015EED4 = 0x801B582C;
        break;
    case 0x80000000:
        D_8015EE98 = 0x801CC360;
        D_8015EE90 = 0x801A1234;
        D_8015EE8C = 0x801C20A4;
        D_8015EEB0 = 0x801A0C08;
        D_8015EEA8 = 0x801A1240;
        D_8015EEA4 = 0x801C20B8;
        D_8015EEC8 = 0x801A0C7C;
        D_8015EEC0 = 0x801A12B4;
        D_8015EEBC = 0x801C21C0;
        D_8015EED8 = 0x801C2298;
        D_8015EEE4 = 0x801C8298;
        D_8015EED4 = 0x801CA298;
        D_8015EEE0 = 0x801C6298;
        D_8015EEA0 = 0x80180790;
        D_8015EEB8 = 0x801807A8;
        D_8015EED0 = 0x80180864;
        break;
    case 0x8000000:
        D_8015EE98 = 0x801CA1CC;
        D_8015EE90 = 0x80189030;
        D_8015EE8C = 0x801BFFA0;
        D_8015EEB0 = 0x8018833C;
        D_8015EEA8 = 0x8018904C;
        D_8015EEA4 = 0x801BFFB4;
        D_8015EEC8 = 0x801883B0;
        D_8015EEC0 = 0x80189174;
        D_8015EEBC = 0x801C00BC;
        D_8015EED8 = 0x801C0194;
        D_8015EEE0 = 0x801C2194;
        D_8015EEDC = 0x801C4194;
        D_8015EEE4 = 0x801C6194;
        D_8015EED4 = 0x801C8194;
        D_8015EEEC = 0x801A91D8;
        break;
    case 0x100000:
        D_8015EE98 = 0x801B0BC8;
        D_8015EE90 = 0x80188AC0;
        D_8015EEB0 = 0x8018833C;
        D_8015EEA8 = 0x80188ADC;
        D_8015EEC8 = 0x801883B0;
        D_8015EEC0 = 0x80188B50;
        D_8015EE8C = 0x801C5DC8;
        D_8015EEA4 = 0x801C5DDC;
        D_8015EEBC = 0x801C5EE4;
        D_8015EED8 = 0x801A6B90;
        D_8015EEE0 = 0x801A8B90;
        D_8015EEDC = 0x801AAB90;
        D_8015EEE4 = 0x801ACB90;
        D_8015EED4 = 0x801AEB90;
        D_8015EEEC = 0x801A4B90;
        break;
    }
    if (D_8015EBA4 == 1) {
        switch (D_8015EBA0) {
        case 0:
            D_8015EE9C = 0x801E86C0;
            D_8015EEB4 = 0x801E86D0;
            D_8015EECC = 0x801E8778;
            break;
        case 1:
            D_8015EE9C = 0x801D82A0;
            D_8015EEB4 = 0x801D82B0;
            D_8015EECC = 0x801D82F4;
            break;
        case 2:
            D_8015EE9C = 0x801D8868;
            D_8015EEB4 = 0x801D8878;
            D_8015EECC = 0x801D892C;
            break;
        case 3:
            D_8015EE9C = 0x801EC528;
            D_8015EEB4 = 0x801EC53C;
            D_8015EECC = 0x801EC59C;
            D_8015EEE8 = 0x801EC5CC;
            break;
        case 4:
            D_8015EE9C = 0x801D8268;
            D_8015EEB4 = 0x801D827C;
            D_8015EECC = 0x801D82BC;
            D_8015EEE8 = 0x801EC2E0;
            break;
        case 5:
            D_8015EE9C = 0x801D8814;
            D_8015EEB4 = 0x801D8824;
            D_8015EECC = 0x801D88A8;
            break;
        case 6:
            D_8015EE9C = 0x801E4308;
            D_8015EEB4 = 0x801E4314;
            D_8015EECC = 0x801E4364;
            break;
        case 7:
            D_8015EE9C = 0x801E49B0;
            D_8015EEB4 = 0x801E49BC;
            D_8015EECC = 0x801E4A6C;
            break;
        case 8:
            D_8015EE9C = 0x801EC58C;
            D_8015EEB4 = 0x801EC5A0;
            D_8015EECC = 0x801EC630;
            break;
        case 9:
            D_8015EE9C = 0x801ED374;
            D_8015EEB4 = 0x801ED388;
            D_8015EECC = 0x801ED580;
            break;
        case 10:
            D_8015EE9C = 0x801E43C8;
            D_8015EEB4 = 0x801E43D4;
            D_8015EECC = 0x801E4440;
            break;
        case 11:
        default:
            D_8015EE9C = 0x801D9370;
            D_8015EEB4 = 0x801D9388;
            D_8015EECC = 0x801D9550;
            break;
        }
    }
    switch (D_8015EB9C) {
    case 0:
        D_8015EEA0 = 0x801CB164;
        D_8015EEB8 = 0x801C7114;
        D_8015EED0 = 0x801C7138;
        return;
    case 1:
        D_8015EEA0 = 0x801CF2CC;
        D_8015EEB8 = 0x801CF250;
        D_8015EED0 = 0x801CF290;
        return;
    case 2:
        if (D_8015EB98 != 0x80000000) {
            D_8015EEA0 = 0x801D776C;
            D_8015EEB8 = 0x801D777C;
            D_8015EED0 = 0x801D7834;
            return;
        }
    default:
        return;
    case 3:
        D_8015EEA0 = 0x801D31F0;
        D_8015EEB8 = 0x801D31FC;
        D_8015EED0 = 0x801D323C;
        return;
    case 4:
        D_8015EEA0 = 0x801CF380;
        D_8015EEB8 = 0x801CF2EC;
        D_8015EED0 = 0x801CF34C;
        return;
    case 5:
        D_8015EEA0 = 0x801CF228;
        D_8015EEB8 = 0x801CF1B8;
        D_8015EED0 = 0x801CF1F4;
        return;
    case 6:
        D_8015EEA0 = 0x801D32F4;
        D_8015EEB8 = 0x801D3300;
        D_8015EED0 = 0x801D3358;
        return;
    case 7:
        D_8015EEA0 = 0x801D3348;
        D_8015EEB8 = 0x801D32AC;
        D_8015EED0 = 0x801D3304;
        return;
    case 8:
        D_8015EEA0 = 0x801CF2B0;
        D_8015EEB8 = 0x801C7230;
        D_8015EED0 = 0x801C7278;
        return;
    case 9:
        D_8015EEA0 = 0x801CB1FC;
        D_8015EEB8 = 0x801CB18C;
        D_8015EED0 = 0x801CB1B8;
        return;
    case 10:
        D_8015EEA0 = 0x801CF2EC;
        D_8015EEB8 = 0x801CF254;
        D_8015EED0 = 0x801CF2A4;
        return;
    case 13:
        D_8015EEA0 = 0x801D32C8;
        D_8015EEB8 = 0x801D32D4;
        D_8015EED0 = 0x801D332C;
        break;
    case 14:
        break;
    }
}
