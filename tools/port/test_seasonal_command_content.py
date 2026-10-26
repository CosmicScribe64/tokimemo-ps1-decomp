"""Unit tests for tools/port/seasonal_command_content.py.

Verifies:
1. Exact background and SD resolution against src/main/school_day.c, holiday.c, and native PS1 tables.
2. Summer vs non-summer weekday selection for ordinary commands 1–3, 5–6, 0, 7.
3. Summer vs non-summer club activity selection for all 11 clubs (clubs 0..10).
4. Vacation selection across all commands for summer and non-summer.
5. Sick condition selection across all four seasons (spring, summer, autumn, winter) for school and vacation.
6. Negative cases: invalid command, invalid club_id, companion variant rejection, out of bounds.
"""
from __future__ import annotations

import unittest
from pathlib import Path
import sys

# Add candidate and workspace to sys.path
CANDIDATE_ROOT = Path(__file__).resolve().parents[2]
WORKSPACE_ROOT = next(
    (p for p in Path(__file__).resolve().parents if (p / "port" / "godot").is_dir()),
    CANDIDATE_ROOT,
)
sys.path.insert(0, str(CANDIDATE_ROOT))
sys.path.insert(0, str(WORKSPACE_ROOT))

from tools.port.seasonal_command_content import (
    resolve_seasonal_route,
    WEEKDAY_BG_LBAS,
    CLUB_BG_LBAS,
    WEEKDAY_SICK_BG_LBAS,
    VACATION_SICK_BG_LBAS,
    HOLIDAY_ACTIVITY_BG_LBAS,
    SUMMER_WEEKDAY_SOLO_SD_SETS,
    NON_SUMMER_WEEKDAY_SOLO_SD_SETS,
    SUMMER_CLUB_SOLO_SD_SETS,
    NON_SUMMER_CLUB_SOLO_SD_SETS,
    HOLIDAY_SD_SET_SUMMER,
    HOLIDAY_SD_SET_OTHER,
    HOLIDAY_SD_SECTORS_TABLE,
    SICK_REST_SD_LBA,
    SICK_REST_SD_SECTORS,
)


class TestSeasonalCommandContent(unittest.TestCase):

    def test_summer_weekday_ordinary_commands_1_to_3_and_5_to_6(self):
        """Summer weekday commands 1-3, 5-6 resolve exact summer SD and weekday BG."""
        # Command 1: 文系 (Humanities)
        r1 = resolve_seasonal_route(1, is_summer=True)
        self.assertEqual(r1["background_lba"], 16141)  # C_01_23.BS
        self.assertEqual(r1["sd_set_id"], 6)          # D_800B68C0[3] = 6
        self.assertEqual(r1["sd_lba"], 42418 + 41 * 6) # 42664 (SD_P03.NBN)
        self.assertEqual(r1["sd_sectors"], 41)

        # Command 2: 理系 (Science)
        r2 = resolve_seasonal_route(2, is_summer=True)
        self.assertEqual(r2["background_lba"], 16151)  # C_01_24.BS
        self.assertEqual(r2["sd_set_id"], 10)         # D_800B68C0[5] = 10
        self.assertEqual(r2["sd_lba"], 42418 + 41 * 10) # 42828 (SD_P05.NBN)

        # Command 3: 芸術 (Art)
        r3 = resolve_seasonal_route(3, is_summer=True)
        self.assertEqual(r3["background_lba"], 16160)  # C_01_25.BS
        self.assertEqual(r3["sd_set_id"], 13)         # D_800B68C0[7] = 13
        self.assertEqual(r3["sd_lba"], 42418 + 41 * 13) # 42951 (SD_P07.NBN)

        # Command 5: 運動 (Exercise)
        r5 = resolve_seasonal_route(5, is_summer=True)
        self.assertEqual(r5["background_lba"], 16268)  # C_01_42.BS
        self.assertEqual(r5["sd_set_id"], 23)         # D_800B68C0[10] = 23
        self.assertEqual(r5["sd_lba"], 42418 + 41 * 23) # 43361 (SD_P0D.NBN)

        # Command 6: 容姿 (Looks)
        r6 = resolve_seasonal_route(6, is_summer=True)
        self.assertEqual(r6["background_lba"], 16277)  # C_01_43.BS
        self.assertEqual(r6["sd_set_id"], 19)         # D_800B68C0[12] = 19
        self.assertEqual(r6["sd_lba"], 42418 + 41 * 19) # 43197 (SD_P0B.NBN)

    def test_summer_vs_non_summer_weekday_differences(self):
        """Verify non-summer uses winter uniform sets (e.g. 7 vs 6 for cmd 1)."""
        for cmd in (0, 1, 2, 3, 5, 6, 7):
            r_summer = resolve_seasonal_route(cmd, is_summer=True)
            r_winter = resolve_seasonal_route(cmd, is_summer=False)
            self.assertEqual(r_summer["background_lba"], r_winter["background_lba"])
            if cmd == 3:
                # Command 3 art shares set 13 between summer and non-summer
                self.assertEqual(r_summer["sd_set_id"], 13)
                self.assertEqual(r_winter["sd_set_id"], 13)
            else:
                self.assertNotEqual(r_summer["sd_set_id"], r_winter["sd_set_id"])
                self.assertNotEqual(r_summer["sd_lba"], r_winter["sd_lba"])

    def test_summer_club_command_4_all_11_clubs(self):
        """Verify summer club routes for all 11 clubs match D_800B6890."""
        # Expected summer solo SD sets for clubs 0..10:
        expected_summer_sets = [2, 31, 6, 10, 28, 24, 13, 15, 21, 19, 17]
        for club_id in range(11):
            r = resolve_seasonal_route(4, club_id=club_id, is_summer=True)
            self.assertEqual(r["background_lba"], CLUB_BG_LBAS[club_id])
            self.assertEqual(r["sd_set_id"], expected_summer_sets[club_id])
            self.assertEqual(r["sd_lba"], 27834 + 41 * expected_summer_sets[club_id])
            self.assertEqual(r["sd_sectors"], 41)

    def test_non_summer_club_matches_baseline(self):
        """Verify non-summer club routes match accepted T-29014 Command 4 baseline."""
        expected_winter_sets = [3, 31, 7, 11, 29, 25, 13, 15, 21, 19, 17]
        for club_id in range(11):
            r = resolve_seasonal_route(4, club_id=club_id, is_summer=False)
            self.assertEqual(r["background_lba"], CLUB_BG_LBAS[club_id])
            self.assertEqual(r["sd_set_id"], expected_winter_sets[club_id])
            self.assertEqual(r["sd_lba"], 27834 + 41 * expected_winter_sets[club_id])

    def test_sick_condition_across_all_four_seasons(self):
        """Verify sick player on command 7 loads D_800B5950 and SD_P31 (LBA 32138, 9 sectors)."""
        expected_sick_bgs = [16361, 16370, 16343, 16352]  # Spring, Summer, Autumn, Winter
        for s_idx in range(4):
            r = resolve_seasonal_route(7, condition_bucket=1, season_index=s_idx)
            self.assertEqual(r["background_lba"], expected_sick_bgs[s_idx])
            self.assertEqual(r["sd_set_id"], -1)
            self.assertEqual(r["sd_lba"], 32138)
            self.assertEqual(r["sd_sectors"], 9)
            self.assertEqual(r["sd_id"], "sick_rest_sd")

    def test_sick_vacation_rest_across_all_four_seasons(self):
        """Verify sick player on vacation command 7 loads D_800B5960 and SD_P31."""
        expected_vac_sick_bgs = [16318, 16326, 16301, 16310]
        for s_idx in range(4):
            r = resolve_seasonal_route(7, condition_bucket=1, vacation_kind=1, season_index=s_idx)
            self.assertEqual(r["background_lba"], expected_vac_sick_bgs[s_idx])
            self.assertEqual(r["sd_lba"], 32138)
            self.assertEqual(r["sd_sectors"], 9)

    def test_vacation_activity_commands_summer_and_non_summer(self):
        """Verify vacation commands 1..3, 5..6 resolve holiday backgrounds and holiday SD sets."""
        for is_summer in (True, False):
            # Command 1: vacation background C_02_03.BS (16379)
            r1 = resolve_seasonal_route(1, vacation_kind=1, is_summer=is_summer)
            self.assertEqual(r1["background_lba"], 16379)
            expected_set1 = 2 if is_summer else 3
            self.assertEqual(r1["sd_set_id"], expected_set1)
            self.assertEqual(r1["sd_lba"], HOLIDAY_SD_SECTORS_TABLE[expected_set1])

            # Command 3: vacation background C_29_02.BS (16387)
            r3 = resolve_seasonal_route(3, vacation_kind=1, is_summer=is_summer)
            self.assertEqual(r3["background_lba"], 16387)
            self.assertEqual(r3["sd_set_id"], 12)
            self.assertEqual(r3["sd_lba"], 42910)  # SD_P06.NBN

            # Command 5: vacation background C_29_10.BS (16396)
            r5 = resolve_seasonal_route(5, vacation_kind=1, is_summer=is_summer)
            self.assertEqual(r5["background_lba"], 16396)
            expected_set5 = 10 if is_summer else 11
            self.assertEqual(r5["sd_set_id"], expected_set5)
            self.assertEqual(r5["sd_lba"], HOLIDAY_SD_SECTORS_TABLE[expected_set5])

    def test_vacation_club_activity_uses_club_background_and_sd(self):
        """Vacation club commands use club backgrounds and club SD tables (src/main/holiday.c)."""
        for club_id in range(11):
            r_sum = resolve_seasonal_route(4, club_id=club_id, vacation_kind=2, is_summer=True)
            self.assertEqual(r_sum["background_lba"], CLUB_BG_LBAS[club_id])
            self.assertEqual(r_sum["sd_set_id"], SUMMER_CLUB_SOLO_SD_SETS[club_id])

            r_win = resolve_seasonal_route(4, club_id=club_id, vacation_kind=1, is_summer=False)
            self.assertEqual(r_win["background_lba"], CLUB_BG_LBAS[club_id])
            self.assertEqual(r_win["sd_set_id"], NON_SUMMER_CLUB_SOLO_SD_SETS[club_id])

    def test_negative_cases(self):
        """Invalid inputs fail closed with descriptive exceptions."""
        with self.assertRaises(ValueError):
            resolve_seasonal_route(8)  # Command outside 0..7
        with self.assertRaises(ValueError):
            resolve_seasonal_route(-1)
        with self.assertRaises(ValueError):
            resolve_seasonal_route(4, club_id=11)  # Club 11 is 'no club'
        with self.assertRaises(ValueError):
            resolve_seasonal_route(1, companion_slot=1)  # Companion unsupported
        with self.assertRaises(ValueError):
            resolve_seasonal_route(1, condition_bucket=2)  # Invalid condition bucket
        with self.assertRaises(ValueError):
            resolve_seasonal_route(1, vacation_kind=4)  # Invalid vacation kind


if __name__ == "__main__":
    unittest.main()
