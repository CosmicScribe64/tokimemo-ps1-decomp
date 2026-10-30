extends RefCounted
## Reviewed SHA-256 identities of the regenerated gameplay content packs.
## Every consumer pins these constants instead of its own copy of the hash.
##
## Both identities are the manifest file hashes that the current producers
## write from the original disc. They also hash the splat listings under the
## ignored asm/ tree and the tracked source files that each manifest cites as
## evidence. The earlier pins (HUD 0a884e91..., tables a1f30e57...) came from
## asm/ listings made before the 2026-10-29 recovery rebuilt the splat image.
## Those listings no longer exist, so no producer reproduces the old hashes.
## The manifests behind the old pins are lost too, so their content cannot be
## compared directly. The regenerated content was checked against the original
## executable and the emulator golden instead. The one producer change to
## content is the T-39002 room picture (see HUD_MANIFEST_SHA).

## tools/port/hud_content.py with --fresh-room-track: manifest of the
## installed `hud_manifest` pack (.asset-cache/port/hud-production-v1).
## Assets are byte-identical to hud-fresh-v1 (55336d7e...) except
## assets/spring-room.png. That picture is decoded from CDROM/BS/C_02_02H.BS
## by tools/assets/image_mdec.export_mdec; the legacy picture was a reviewed
## PNG imported from an earlier pack.
const HUD_MANIFEST_SHA := "c19f4306cd0b5947db81caa1fa2caaa9bf93916b14af836c63a06130511ab09d"

## SHA-256 of the RGBA8 pixels of the HUD_MANIFEST_SHA room picture
## (assets/spring-room.png, file d43c4a35..., 320x88): the fresh C_02_02H
## decode. The holiday route shows it again as the next command room
## (weekday room LBA 16361), so the holiday HUD and holiday scene adapters
## both pin this value. The legacy picture's pixels were 8ea14e66...
const NEXT_COMMAND_ROOM_RGBA_SHA := "efe36d89c136021d0432ce4ef5ef78fc28e9e57fe6eecb7cdde2eb447c3d4181"

## tools/port/school_day_tables.py: manifest of the installed `rule_tables`
## pack (.asset-cache/port/school-day-tables-v1-final). It carries the 16
## SLPM_86.053 tables, including the five-entry command_rank_bars from T-39006.
const RULE_TABLES_SHA := "be99ef4acc0eec5216ae93c980ec1ec5876025ddce9101508a59ac72d233fc3c"

## tools/port/spring_command_content.py --fresh-source: manifest of the
## installed `spring_media` pack. Its provenance records RULE_TABLES_SHA, so
## this identity follows the tables identity.
const SPRING_MEDIA_SHA := "f6665949eebb7eda20f6528f632763b6b157b2ee2eefb277b538cdb8d4565ae8"
