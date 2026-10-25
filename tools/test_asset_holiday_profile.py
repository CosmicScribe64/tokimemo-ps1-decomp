"""Invented selector and ISO-metadata boundary checks for the finite holiday profile."""
import copy
import hashlib
import struct
import unittest
from unittest import mock
from tools.assets import holiday_content_profile as profile


def entries():
    return [{"path":a["path"],"kind":"file","size":a["size"],"sha256":a["sha256"],
        "location":{"lba":a["lba"],"extent_bytes":a["size"],"record_lba":a["record_lba"],
        "record_offset":a["record_offset"],"sectors":[{}]*a["file_sectors"]}} for a in profile.ASSETS]


def native():
    return {"schema":"tokimemo.native-holiday-profile.v1","sd_profile":1,
        "assets":[{k:a[k] for k in ("role","path","lba","size","sha256")} for a in profile.ASSETS],
        "checkpoints":[{"label":"holiday_activity_sd_setup","mode":[70,5,1,0],
                        "date":[95,4,9,6],"queue_index":9,"last_command":0}]}


class HolidayProfileTests(unittest.TestCase):
    def test_room_tables_are_distinct_exact_four_entry_dependencies(self):
        mapping=profile.ROUTE_TABLE_FIELDS
        self.assertEqual(mapping["weekday_room"],("weekday_room_background_lbas","s32",[4]))
        self.assertEqual(mapping["room"],("seasonal_room_background_lbas","s32",[4]))
        tables={t["name"]:t for t in profile.EXE_TABLES}
        self.assertNotEqual(tables["weekday_room"]["raw_sha256"],tables["room"]["raw_sha256"])
        self.assertEqual(tables["weekday_room"]["size"],16)
        self.assertTrue(all("raw_hex" not in t for t in profile.EXE_TABLES))

    def test_inventory_rejects_substitution_extent_hash_and_directory_mutations(self):
        self.assertEqual(len(profile.validate_inventory(entries())),3)
        for row in range(3):
            for field,bad in (("path","invented.bin"),("size",1),("sha256","0"*64)):
                fixture=entries();fixture[row][field]=bad
                with self.subTest(row=row,field=field),self.assertRaises(ValueError):profile.validate_inventory(fixture)
            for field in ("lba","extent_bytes","record_lba","record_offset"):
                fixture=entries();fixture[row]["location"][field]+=1
                with self.subTest(row=row,field=field),self.assertRaises(ValueError):profile.validate_inventory(fixture)
            fixture=entries();fixture[row]["location"]["sectors"].pop()
            with self.assertRaises(ValueError):profile.validate_inventory(fixture)
        fixture=entries();fixture.append(copy.deepcopy(fixture[0]))
        with self.assertRaisesRegex(ValueError,"unique"):profile.validate_inventory(fixture)

    def test_rehashed_executable_table_and_layout_changes_are_rejected(self):
        data=bytearray(max(t["offset"]+t["size"] for t in profile.EXE_TABLES))
        tables=[]
        for index,table in enumerate(profile.EXE_TABLES):
            raw=bytes((i+index*7)%256 for i in range(table["size"]))
            data[table["offset"]:table["offset"]+len(raw)]=raw
            tables.append(dict(table,raw_sha256=hashlib.sha256(raw).hexdigest()))
        with mock.patch.object(profile,"EXE_TABLES",tables):
            with mock.patch.object(profile,"EXE_SHA256",hashlib.sha256(data).hexdigest()):profile.validate_executable(data)
            for table in tables:
                changed=bytearray(data);changed[table["offset"]]^=1
                with self.subTest(table=table["name"]),mock.patch.object(profile,"EXE_SHA256",hashlib.sha256(changed).hexdigest()):
                    with self.assertRaisesRegex(ValueError,"table differs"):profile.validate_executable(changed)

    def test_native_selector_mode_date_and_asset_projection_are_exact(self):
        profile.validate_native_profile(native())
        changes=[lambda n:n.update(sd_profile=0),lambda n:n.update(sd_profile=True),
                 lambda n:n["assets"][0].update(path="invented.BS"),
                 lambda n:n["checkpoints"][0].update(queue_index=8),
                 lambda n:n["checkpoints"][0].update(last_command=False),
                 lambda n:n["checkpoints"][0].update(mode=[70,5,False,0]),
                 lambda n:n["checkpoints"][0].update(date=[95,4,10,0])]
        for change in changes:
            fixture=native();change(fixture)
            with self.subTest(change=change),self.assertRaises(ValueError):profile.validate_native_profile(fixture)


if __name__=="__main__":unittest.main()
