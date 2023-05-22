-- DB update 2021_12_23_04 -> 2021_12_23_05
DROP PROCEDURE IF EXISTS `updateDb`;
DELIMITER //
CREATE PROCEDURE updateDb ()
proc:BEGIN DECLARE OK VARCHAR(100) DEFAULT 'FALSE';
SELECT COUNT(*) INTO @COLEXISTS
FROM information_schema.COLUMNS
WHERE TABLE_SCHEMA = DATABASE() AND TABLE_NAME = 'version_db_world' AND COLUMN_NAME = '2021_12_23_04';
IF @COLEXISTS = 0 THEN LEAVE proc; END IF;
START TRANSACTION;
ALTER TABLE version_db_world CHANGE COLUMN 2021_12_23_04 2021_12_23_05 bit;
SELECT sql_rev INTO OK FROM version_db_world WHERE sql_rev = '1639870157215820672'; IF OK <> 'FALSE' THEN LEAVE proc; END IF;
--
-- START UPDATING QUERIES
--

INSERT INTO `version_db_world` (`sql_rev`) VALUES ('1639870157215820672');

-- Amani Catacombs

-- Some mice missing movement
UPDATE `creature` SET `wander_distance`=5,`MovementType`=1 WHERE `guid` IN (82049,82051,82083,82085);

-- Mummified Headhunters
DELETE FROM `creature` WHERE `guid` IN (82048,82050,82054,82052,82056,82053,82055,82064,82062,82065,82074,82075,82073,82078,82080,82068,82084,82082,82069,82066,82058,82063);
INSERT INTO `creature` (`guid`,`id`,`map`,`zoneId`,`areaId`,`spawnMask`,`phaseMask`,`modelid`,`equipment_id`,`position_x`,`position_y`,`position_z`,`orientation`,`spawntimesecs`,`wander_distance`,`currentwaypoint`,`curhealth`,`curmana`,`MovementType`,`npcflag`,`unit_flags`,`dynamicflags`,`ScriptName`,`VerifiedBuild`) VALUES
(82048, 16342, 530, 0, 0, 1, 1, 0, 0, 7637.5894, -7450.1987, 163.21379, 4.67682647705078125, 300, 4, 0, 1, 0, 1, 0, 0, 0, '', 0),
(82050, 16342, 530, 0, 0, 1, 1, 0, 0, 7637.403, -7426.0156, 162.70844, 3.547638416290283203, 300, 0, 0, 1, 0, 2, 0, 0, 0, '', 0),
(82054, 16342, 530, 0, 0, 1, 1, 0, 0, 7608.396, -7397.3193, 157.02446, 3.424771785736083984, 300, 6, 0, 1, 0, 1, 0, 0, 0, '', 0),
(82052, 16342, 530, 0, 0, 1, 1, 0, 0, 7636.902, -7401.807, 162.225, 3.626172780990600585, 300, 4, 0, 1, 0, 1, 0, 0, 0, '', 0),
(82056, 16342, 530, 0, 0, 1, 1, 0, 0, 7637.297, -7448.315, 163.50067, 5.148721218109130859, 300, 0, 0, 1, 0, 2, 0, 0, 0, '', 0),
(82053, 16342, 530, 0, 0, 1, 1, 0, 0, 7602.3916, -7408.3423, 157.02351, 3.279497861862182617, 300, 6, 0, 1, 0, 1, 0, 0, 0, '', 0),
(82055, 16342, 530, 0, 0, 1, 1, 0, 0, 7632.376, -7365.1807, 162.41412, 4.147120475769042968, 300, 6, 0, 1, 0, 1, 0, 0, 0, '', 0),
(82064, 16342, 530, 0, 0, 1, 1, 0, 0, 7620.4507, -7354.987, 162.42554, 2.945436000823974609, 300, 6, 0, 1, 0, 1, 0, 0, 0, '', 0),
(82062, 16342, 530, 0, 0, 1, 1, 0, 0, 7644.645, -7354.8066, 162.38931, 2.281617641448974609, 300, 6, 0, 1, 0, 1, 0, 0, 0, '', 0),
(82065, 16342, 530, 0, 0, 1, 1, 0, 0, 7637.394, -7381.5513, 161.84938, 4.697824001312255859, 300, 4, 0, 1, 0, 1, 0, 0, 0, '', 0),
(82074, 16342, 530, 0, 0, 1, 1, 0, 0, 7652.493, -7302.145, 155.56497, 4.393340110778808593, 300, 6, 0, 1, 0, 1, 0, 0, 0, '', 0),
(82075, 16342, 530, 0, 0, 1, 1, 0, 0, 7618.5947, -7304.8047, 155.30917, 0.276900231838226318, 300, 6, 0, 1, 0, 1, 0, 0, 0, '', 0),
(82073, 16342, 530, 0, 0, 1, 1, 0, 0, 7599.733, -7313.3267, 155.35315, 1.624552726745605468, 300, 6, 0, 1, 0, 1, 0, 0, 0, '', 0),
(82078, 16342, 530, 0, 0, 1, 1, 0, 0, 7600.566, -7284.547, 155.71278, 3.368485450744628906, 300, 6, 0, 1, 0, 1, 0, 0, 0, '', 0),
(82080, 16342, 530, 0, 0, 1, 1, 0, 0, 7653.6553, -7257.6973, 155.38898, 6.016467094421386718, 300, 6, 0, 1, 0, 1, 0, 0, 0, '', 0),
(82068, 16342, 530, 0, 0, 1, 1, 0, 0, 7657.077, -7211.907, 156.2073, 0.953733682632446289, 300, 0, 0, 1, 0, 2, 0, 0, 0, '', 0),
(82084, 16342, 530, 0, 0, 1, 1, 0, 0, 7612.093, -7248.5767, 155.69241, 0.476564466953277587, 300, 6, 0, 1, 0, 1, 0, 0, 0, '', 0),
(82082, 16342, 530, 0, 0, 1, 1, 0, 0, 7597.764, -7261.03, 155.4633, 5.500378131866455078, 300, 6, 0, 1, 0, 1, 0, 0, 0, '', 0),
(82069, 16342, 530, 0, 0, 1, 1, 0, 0, 7569.4385, -7346.889, 161.82481, 3.185770750045776367, 300, 6, 0, 1, 0, 1, 0, 0, 0, '', 0),
(82066, 16342, 530, 0, 0, 1, 1, 0, 0, 7602.3506, -7354.4106, 162.33804, 3.782763481140136718, 300, 0, 0, 1, 0, 2, 0, 0, 0, '', 0),
(82058, 16342, 530, 0, 0, 1, 1, 0, 0, 7569.0566, -7371.011, 161.82092, 4.723871231079101562, 300, 6, 0, 1, 0, 1, 0, 0, 0, '', 0),
(82063, 16342, 530, 0, 0, 1, 1, 0, 0, 7610.98, -7364.403, 162.4298, 3.186835289001464843, 300, 6, 0, 1, 0, 1, 0, 0, 0, '', 0);

-- Shadowpine Oracles
DELETE FROM `creature` WHERE `guid` IN (82015,82017,82091,81923);
DELETE FROM `creature` WHERE `guid` IN (81921,81922,82046,81765,82090,82067,82088,82072,82059,82089);
INSERT INTO `creature` (`guid`,`id`,`map`,`zoneId`,`areaId`,`spawnMask`,`phaseMask`,`modelid`,`equipment_id`,`position_x`,`position_y`,`position_z`,`orientation`,`spawntimesecs`,`wander_distance`,`currentwaypoint`,`curhealth`,`curmana`,`MovementType`,`npcflag`,`unit_flags`,`dynamicflags`,`ScriptName`,`VerifiedBuild`) VALUES
(81921, 16343, 530, 0, 0, 1, 1, 0, 0, 7686.932, -7525.6016, 149.63179, 0.172823652625083923, 300, 6, 0, 1, 0, 1, 0, 0, 0, '', 0),
(81922, 16343, 530, 0, 0, 1, 1, 0, 0, 7633.153, -7514.809, 153.70674, 3.576626777648925781, 300, 6, 0, 1, 0, 1, 0, 0, 0, '', 0),
(82046, 16343, 530, 0, 0, 1, 1, 0, 0, 7628.973, -7486.819, 159.18497, 2.862339973449707031, 300, 0, 0, 1, 0, 0, 0, 0, 0, '', 0),
(81765, 16343, 530, 0, 0, 1, 1, 0, 0, 7761.0938, -7133.5425, 129.07271, 2.759725809097290039, 300, 6, 0, 1, 0, 1, 0, 0, 0, '', 0),
(82090, 16343, 530, 0, 0, 1, 1, 0, 0, 7755.614, -7171.0234, 132.84755, 1.221730470657348632, 300, 0, 0, 1, 0, 0, 0, 0, 0, '', 0),
(82067, 16343, 530, 0, 0, 1, 1, 0, 0, 7729.85, -7145.407, 136.8625, 3.493113994598388671, 300, 0, 0, 1, 0, 2, 0, 0, 0, '', 0),
(82088, 16343, 530, 0, 0, 1, 1, 0, 0, 7682.8813, -7186.0435, 149.59708, 3.324099063873291015, 300, 6, 0, 1, 0, 1, 0, 0, 0, '', 0),
(82072, 16343, 530, 0, 0, 1, 1, 0, 0, 7559.098, -7356.428, 161.65436, 3.579063653945922851, 300, 0, 0, 1, 0, 2, 0, 0, 0, '', 0),
(82059, 16343, 530, 0, 0, 1, 1, 0, 0, 7556.7056, -7356.2407, 162.18187, 0.174532920122146606, 300, 0, 0, 1, 0, 0, 0, 0, 0, '', 0),
(82089, 16343, 530, 0, 0, 1, 1, 0, 0, 7714.3037, -7158.518, 140.67776, 1.098403573036193847, 300, 6, 0, 1, 0, 1, 0, 0, 0, '', 0);

-- Pathing for Mummified Headhunter Entry: 16342
SET @NPC := 82056;
SET @PATH := @NPC * 10;
DELETE FROM `creature_addon` WHERE `guid`=@NPC;
INSERT INTO `creature_addon` (`guid`,`path_id`,`mount`,`bytes1`,`bytes2`,`emote`,`visibilityDistanceType`,`auras`) VALUES (@NPC,@PATH,0,0,1,0,0, '');
DELETE FROM `waypoint_data` WHERE `id`=@PATH;
INSERT INTO `waypoint_data` (`id`,`point`,`position_x`,`position_y`,`position_z`,`orientation`,`delay`,`move_type`,`action`,`action_chance`,`wpguid`) VALUES
(@PATH,1,7638.054,-7463.083,163.40086,0,0,0,0,100,0),
(@PATH,2,7637.364,-7426.2017,162.6404,0,0,0,0,100,0),
(@PATH,3,7636.9907,-7402.186,162.14479,0,0,0,0,100,0),
(@PATH,4,7604.474,-7403.5693,156.94113,0,0,0,0,100,0),
(@PATH,5,7604.783,-7372.065,162.33171,0,0,0,0,100,0),
(@PATH,6,7605.908,-7357.83,162.28203,0,0,0,0,100,0),
(@PATH,7,7604.783,-7372.065,162.33171,0,0,0,0,100,0),
(@PATH,8,7604.474,-7403.5693,156.94113,0,0,0,0,100,0),
(@PATH,9,7636.9907,-7402.186,162.14479,0,0,0,0,100,0),
(@PATH,10,7637.364,-7426.2017,162.6404,0,0,0,0,100,0);

-- Pathing for Mummified Headhunter Entry: 16342
SET @NPC := 82066;
SET @PATH := @NPC * 10;
UPDATE `creature` SET `wander_distance`=0,`MovementType`=2 WHERE `guid`=@NPC;
DELETE FROM `creature_addon` WHERE `guid`=@NPC;
INSERT INTO `creature_addon` (`guid`,`path_id`,`mount`,`bytes1`,`bytes2`,`emote`,`visibilityDistanceType`,`auras`) VALUES (@NPC,@PATH,0,0,1,0,0, '');
DELETE FROM `waypoint_data` WHERE `id`=@PATH;
INSERT INTO `waypoint_data` (`id`,`point`,`position_x`,`position_y`,`position_z`,`orientation`,`delay`,`move_type`,`action`,`action_chance`,`wpguid`) VALUES
(@PATH,1,7598.0884,-7358.5166,162.23851,0,0,0,0,100,0),
(@PATH,2,7567.907,-7359.6196,161.7376,0,0,0,0,100,0),
(@PATH,3,7598.343,-7359.2734,162.24873,0,0,0,0,100,0),
(@PATH,4,7602.319,-7365.0415,162.29167,0,0,0,0,100,0),
(@PATH,5,7636.492,-7372.3306,162.34412,0,0,0,0,100,0),
(@PATH,6,7637.08,-7403.132,162.1848,0,0,0,0,100,0),
(@PATH,7,7636.492,-7372.3306,162.34412,0,0,0,0,100,0),
(@PATH,8,7602.319,-7365.0415,162.29167,0,0,0,0,100,0),
(@PATH,9,7598.343,-7359.2734,162.24873,0,0,0,0,100,0),
(@PATH,10,7567.907,-7359.6196,161.7376,0,0,0,0,100,0),
(@PATH,11,7598.0884,-7358.5166,162.23851,0,0,0,0,100,0),
(@PATH,12,7614.816,-7347.358,162.28706,0,0,0,0,100,0),
(@PATH,13,7638.855,-7346.9956,162.30225,0,0,0,0,100,0),
(@PATH,14,7637.178,-7305.694,155.20381,0,0,0,0,100,0),
(@PATH,15,7600.2153,-7304.7056,155.2885,0,0,0,0,100,0),
(@PATH,16,7637.178,-7305.694,155.20381,0,0,0,0,100,0),
(@PATH,17,7638.855,-7346.9956,162.30225,0,0,0,0,100,0),
(@PATH,18,7614.816,-7347.358,162.28706,0,0,0,0,100,0);

-- Pathing for Mummified Headhunter Entry: 16342
SET @NPC := 82068;
SET @PATH := @NPC * 10;
UPDATE `creature` SET `wander_distance`=0,`MovementType`=2 WHERE `guid`=@NPC;
DELETE FROM `creature_addon` WHERE `guid`=@NPC;
INSERT INTO `creature_addon` (`guid`,`path_id`,`mount`,`bytes1`,`bytes2`,`emote`,`visibilityDistanceType`,`auras`) VALUES (@NPC,@PATH,0,0,1,0,0, '');
DELETE FROM `waypoint_data` WHERE `id`=@PATH;
INSERT INTO `waypoint_data` (`id`,`point`,`position_x`,`position_y`,`position_z`,`orientation`,`delay`,`move_type`,`action`,`action_chance`,`wpguid`) VALUES
(@PATH,1,7657.5044,-7211.3813,156.14449,0,0,0,0,100,0),
(@PATH,2,7635.492,-7230.29,155.59535,0,0,0,0,100,0),
(@PATH,3,7634.772,-7243.6562,155.1202,0,0,0,0,100,0),
(@PATH,4,7654.1094,-7247.0215,155.35133,0,0,0,0,100,0),
(@PATH,5,7653.972,-7258.346,155.3392,0,0,0,0,100,0),
(@PATH,6,7621.078,-7263.266,155.22394,0,0,0,0,100,0),
(@PATH,7,7604.829,-7268.905,155.221,0,0,0,0,100,0),
(@PATH,8,7605.087,-7295.863,155.19582,0,0,0,0,100,0),
(@PATH,9,7652.0156,-7296.888,155.58131,0,0,0,0,100,0),
(@PATH,10,7654.6406,-7313.5957,155.53645,0,0,0,0,100,0),
(@PATH,11,7637.54,-7314.068,155.19508,0,0,0,0,100,0),
(@PATH,12,7641.33,-7359.5435,162.333,0,0,0,0,100,0),
(@PATH,13,7594.413,-7359.258,162.22653,0,0,0,0,100,0),
(@PATH,14,7641.2607,-7359.543,162.32701,0,0,0,0,100,0),
(@PATH,15,7637.54,-7314.068,155.19508,0,0,0,0,100,0),
(@PATH,16,7654.6406,-7313.5957,155.53645,0,0,0,0,100,0),
(@PATH,17,7652.0156,-7296.888,155.58131,0,0,0,0,100,0),
(@PATH,18,7605.087,-7295.863,155.19582,0,0,0,0,100,0),
(@PATH,19,7604.829,-7268.905,155.221,0,0,0,0,100,0),
(@PATH,20,7621.078,-7263.266,155.22394,0,0,0,0,100,0),
(@PATH,21,7653.972,-7258.346,155.3392,0,0,0,0,100,0),
(@PATH,22,7654.1094,-7247.0215,155.35133,0,0,0,0,100,0),
(@PATH,23,7634.772,-7243.6562,155.1202,0,0,0,0,100,0),
(@PATH,24,7635.492,-7230.29,155.59535,0,0,0,0,100,0);

-- Pathing for Mummified Headhunter Entry: 16342
SET @NPC := 82050;
SET @PATH := @NPC * 10;
UPDATE `creature` SET `wander_distance`=0,`MovementType`=2 WHERE `guid`=@NPC;
DELETE FROM `creature_addon` WHERE `guid`=@NPC;
INSERT INTO `creature_addon` (`guid`,`path_id`,`mount`,`bytes1`,`bytes2`,`emote`,`visibilityDistanceType`,`auras`) VALUES (@NPC,@PATH,0,0,1,0,0, '');
DELETE FROM `waypoint_data` WHERE `id`=@PATH;
INSERT INTO `waypoint_data` (`id`,`point`,`position_x`,`position_y`,`position_z`,`orientation`,`delay`,`move_type`,`action`,`action_chance`,`wpguid`) VALUES
(@PATH,1,7637.364,-7426.2017,162.6404,0,0,0,0,100,0),
(@PATH,2,7636.9907,-7402.186,162.14479,0,0,0,0,100,0),
(@PATH,3,7604.474,-7403.5693,156.94113,0,0,0,0,100,0),
(@PATH,4,7604.783,-7372.065,162.33171,0,0,0,0,100,0),
(@PATH,5,7605.908,-7357.83,162.28203,0,0,0,0,100,0),
(@PATH,6,7639.6763,-7357.759,162.32799,0,0,0,0,100,0),
(@PATH,7,7639.5454,-7309.7256,155.19437,0,0,0,0,100,0),
(@PATH,8,7639.6763,-7357.759,162.32799,0,0,0,0,100,0),
(@PATH,9,7605.908,-7357.83,162.28203,0,0,0,0,100,0),
(@PATH,10,7604.783,-7372.065,162.33171,0,0,0,0,100,0),
(@PATH,11,7604.474,-7403.5693,156.94113,0,0,0,0,100,0),
(@PATH,12,7636.9907,-7402.186,162.14479,0,0,0,0,100,0);

-- Pathing for Shadowpine Oracle Entry: 16343
SET @NPC := 82072;
SET @PATH := @NPC * 10;
DELETE FROM `creature_addon` WHERE `guid`=@NPC;
INSERT INTO `creature_addon` (`guid`,`path_id`,`mount`,`bytes1`,`bytes2`,`emote`,`visibilityDistanceType`,`auras`) VALUES (@NPC,@PATH,0,0,1,0,0, '');
DELETE FROM `waypoint_data` WHERE `id`=@PATH;
INSERT INTO `waypoint_data` (`id`,`point`,`position_x`,`position_y`,`position_z`,`orientation`,`delay`,`move_type`,`action`,`action_chance`,`wpguid`) VALUES
(@PATH,1,7554.355,-7347.264,162.22528,0,0,0,0,100,0),
(@PATH,2,7562.7344,-7342.3135,162.2034,0,0,0,0,100,0),
(@PATH,3,7571.8335,-7342.319,162.23224,0,0,0,0,100,0),
(@PATH,4,7579.886,-7346.8706,162.1984,0,0,0,0,100,0),
(@PATH,5,7584.5537,-7359.0415,162.26685,0,0,0,0,100,0),
(@PATH,6,7601.639,-7359.0835,162.26494,0,0,0,0,100,0),
(@PATH,7,7606.941,-7349.587,162.25523,0,0,0,0,100,0),
(@PATH,8,7622.862,-7348.6313,162.33342,0,0,0,0,100,0),
(@PATH,9,7647.9062,-7348.007,162.18117,0,0,0,0,100,0),
(@PATH,10,7648.7603,-7367.749,162.2267,0,0,0,0,100,0),
(@PATH,11,7626.3647,-7367.796,162.35571,0,0,0,0,100,0),
(@PATH,12,7608.4766,-7367.8193,162.33417,0,0,0,0,100,0),
(@PATH,13,7601.5356,-7358.9634,162.26396,0,0,0,0,100,0),
(@PATH,14,7584.6006,-7359.013,162.26694,0,0,0,0,100,0),
(@PATH,15,7580.263,-7371.6426,162.23654,0,0,0,0,100,0),
(@PATH,16,7572.5825,-7376.3057,162.19925,0,0,0,0,100,0),
(@PATH,17,7563.4775,-7376.5723,162.20667,0,0,0,0,100,0),
(@PATH,18,7555.3057,-7372.243,162.21547,0,0,0,0,100,0),
(@PATH,19,7551.6475,-7359.913,162.26942,0,0,0,0,100,0);

-- Pathing for Shadowpine Oracle Entry: 16343
SET @NPC := 82067;
SET @PATH := @NPC * 10;
DELETE FROM `creature_addon` WHERE `guid`=@NPC;
INSERT INTO `creature_addon` (`guid`,`path_id`,`mount`,`bytes1`,`bytes2`,`emote`,`visibilityDistanceType`,`auras`) VALUES (@NPC,@PATH,0,0,1,0,0, '');
DELETE FROM `waypoint_data` WHERE `id`=@PATH;
INSERT INTO `waypoint_data` (`id`,`point`,`position_x`,`position_y`,`position_z`,`orientation`,`delay`,`move_type`,`action`,`action_chance`,`wpguid`) VALUES
(@PATH,1,7728.19,-7149.0884,137.49004,0,0,0,0,100,0),
(@PATH,2,7693.2866,-7181.214,148.76553,0,0,0,0,100,0),
(@PATH,3,7667.289,-7168.3555,153.34451,0,0,0,0,100,0),
(@PATH,4,7646.2646,-7184.749,153.86041,0,0,0,0,100,0),
(@PATH,5,7637.242,-7175.19,153.3255,0,0,0,0,100,0),
(@PATH,6,7651.9375,-7166.79,153.15399,0,0,0,0,100,0),
(@PATH,7,7669.7935,-7169.5996,152.82767,0,0,0,0,100,0),
(@PATH,8,7677.769,-7193.526,150.2056,0,0,0,0,100,0),
(@PATH,9,7663.309,-7206.148,156.22491,0,0,0,0,100,0),
(@PATH,10,7635.66,-7230.913,155.57506,0,0,0,0,100,0),
(@PATH,11,7634.2363,-7247.9375,155.1528,0,0,0,0,100,0),
(@PATH,12,7601.6147,-7268.4478,155.25287,0,0,0,0,100,0),
(@PATH,13,7601.9565,-7296.327,155.24269,0,0,0,0,100,0),
(@PATH,14,7638.2275,-7308.3926,155.19878,0,0,0,0,100,0),
(@PATH,15,7639.607,-7344.186,162.29018,0,0,0,0,100,0),
(@PATH,16,7639.022,-7308.2397,155.19702,0,0,0,0,100,0),
(@PATH,17,7598.462,-7306.5703,155.30826,0,0,0,0,100,0),
(@PATH,18,7598.133,-7258.9326,155.35962,0,0,0,0,100,0),
(@PATH,19,7634.276,-7248.135,155.1535,0,0,0,0,100,0),
(@PATH,20,7635.4287,-7231.498,155.4263,0,0,0,0,100,0),
(@PATH,21,7644.346,-7223.3926,155.7942,0,0,0,0,100,0),
(@PATH,22,7678.174,-7192.7446,150.11502,0,0,0,0,100,0),
(@PATH,23,7697.3315,-7177.1694,147.36722,0,0,0,0,100,0);

-- Pathing for Shadowpine Oracle Entry: 16343
SET @NPC := 82047;
SET @PATH := @NPC * 10;
UPDATE `creature` SET `wander_distance`=0,`MovementType`=2 WHERE `guid`=@NPC;
DELETE FROM `creature_addon` WHERE `guid`=@NPC;
INSERT INTO `creature_addon` (`guid`,`path_id`,`mount`,`bytes1`,`bytes2`,`emote`,`visibilityDistanceType`,`auras`) VALUES (@NPC,@PATH,0,0,1,0,0, '');
DELETE FROM `waypoint_data` WHERE `id`=@PATH;
INSERT INTO `waypoint_data` (`id`,`point`,`position_x`,`position_y`,`position_z`,`orientation`,`delay`,`move_type`,`action`,`action_chance`,`wpguid`) VALUES
(@PATH,1,7638.112,-7475.7544,161.22134,0,0,0,0,100,0),
(@PATH,2,7637.8716,-7446.4185,163.1492,0,0,0,0,100,0),
(@PATH,3,7637.2666,-7408.856,162.29324,0,0,0,0,100,0),
(@PATH,4,7636.4087,-7370.703,162.34296,0,0,0,0,100,0),
(@PATH,5,7606.001,-7366.907,162.31789,0,0,0,0,100,0),
(@PATH,6,7604.0034,-7400.803,156.94113,0,0,0,0,100,0),
(@PATH,7,7611.317,-7404.6704,156.94113,0,0,0,0,100,0),
(@PATH,8,7638.1753,-7401.8267,162.24667,0,0,0,0,100,0),
(@PATH,9,7637.8706,-7446.482,163.15012,0,0,0,0,100,0),
(@PATH,10,7638.158,-7475.5557,161.31046,0,0,0,0,100,0),
(@PATH,11,7643.664,-7501.5815,154.21745,0,0,0,0,100,0),
(@PATH,12,7677.9956,-7516.2095,150.39413,0,0,0,0,100,0),
(@PATH,13,7696.77,-7541.147,149.74875,0,2000,0,0,100,0),
(@PATH,14,7677.9956,-7516.2095,150.39413,0,0,0,0,100,0),
(@PATH,15,7643.664,-7501.5815,154.21745,0,0,0,0,100,0),
(@PATH,16,7638.158,-7475.5557,161.31046,0,0,0,0,100,0),
(@PATH,17,7637.8706,-7446.482,163.15012,0,0,0,0,100,0),
(@PATH,18,7638.1753,-7401.8267,162.24667,0,0,0,0,100,0),
(@PATH,19,7611.317,-7404.6704,156.94113,0,0,0,0,100,0),
(@PATH,20,7604.0034,-7400.803,156.94113,0,0,0,0,100,0),
(@PATH,21,7606.001,-7366.907,162.31789,0,0,0,0,100,0),
(@PATH,22,7636.4087,-7370.703,162.34296,0,0,0,0,100,0),
(@PATH,23,7637.265,-7408.784,162.31276,0,0,0,0,100,0),
(@PATH,24,7637.8716,-7446.4185,163.1492,0,0,0,0,100,0),
(@PATH,25,7638.112,-7475.7544,161.22134,0,0,0,0,100,0),
(@PATH,26,7638.8413,-7501.28,154.71819,0,0,0,0,100,0),
(@PATH,27,7628.654,-7523.961,153.60687,0,2000,0,0,100,0),
(@PATH,28,7638.8413,-7501.28,154.71819,0,0,0,0,100,0);

--
-- END UPDATING QUERIES
--
UPDATE version_db_world SET date = '2021_12_23_05' WHERE sql_rev = '1639870157215820672';
COMMIT;
END //
DELIMITER ;
CALL updateDb();
DROP PROCEDURE IF EXISTS `updateDb`;
