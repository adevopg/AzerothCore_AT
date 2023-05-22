-- DB update 2021_06_17_03 -> 2021_06_17_04
DROP PROCEDURE IF EXISTS `updateDb`;
DELIMITER //
CREATE PROCEDURE updateDb ()
proc:BEGIN DECLARE OK VARCHAR(100) DEFAULT 'FALSE';
SELECT COUNT(*) INTO @COLEXISTS
FROM information_schema.COLUMNS
WHERE TABLE_SCHEMA = DATABASE() AND TABLE_NAME = 'version_db_world' AND COLUMN_NAME = '2021_06_17_03';
IF @COLEXISTS = 0 THEN LEAVE proc; END IF;
START TRANSACTION;
ALTER TABLE version_db_world CHANGE COLUMN 2021_06_17_03 2021_06_17_04 bit;
SELECT sql_rev INTO OK FROM version_db_world WHERE sql_rev = '1623161552319933800'; IF OK <> 'FALSE' THEN LEAVE proc; END IF;
--
-- START UPDATING QUERIES
--

INSERT INTO `version_db_world` (`sql_rev`) VALUES ('1623161552319933800');

SET @NPC = 6707;

UPDATE `creature` 
SET `position_x` = -9296.069336, `position_y` = -1887.173462, `position_z` = 78.376251
WHERE `guid` = @NPC;

--
-- END UPDATING QUERIES
--
UPDATE version_db_world SET date = '2021_06_17_04' WHERE sql_rev = '1623161552319933800';
COMMIT;
END //
DELIMITER ;
CALL updateDb();
DROP PROCEDURE IF EXISTS `updateDb`;
