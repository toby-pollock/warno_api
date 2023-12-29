#pragma once
class Ammo {
private:

public:
	std::string name, family, ProjectileType;
	int startLineNumber;
	float physicalDamages;
	int physicalDamagesLineOffset;
	int ammo_type_id; //refer to the string array in unitWindow()
	int max_range,max_range_offset;
	int min_range,min_range_offset;

	float suppressDamage;
	int suppressDamageOffset;

	int radiusSuppressDamage, radiusSuppressDamageOffset;
	int ShotsBeforeMaxNoise, ShotsBeforeMaxNoiseOffset;

	float NoiseDissimulationMalus;
	int NoiseDissimulationMalusOffset;

	float TempsDeVisee;
	int TempsDeViseeOffset;

	float TempsEntreDeuxSalves;
	int TempsEntreDeuxSalvesOffset;

	float new_PhysicalDamages, new_suppressDamage, new_NoiseDissimulationMalus, new_TempsEntreDeuxSalves, new_TempsDeVisee;
	int new_max_range, new_min_range, new_suppressDamageRadius, new_ShotsBeforeMaxNoise;

	int numberOfWeapons;

	bool isSelected = false;

	Ammo() {}
	Ammo(std::string _name) : name(_name) {}
};

class Weapons {
private:

public:
	std::vector<Ammo*> ammos;
	Weapons() {}
};
class Unit {
private:

public:
	int exportLineNumber;//first line of the unit in the file 

	bool isSelected = false;
	bool viewStats = false;

	std::string name, nationality, mother_country, acknow_unit_type, type_unit_formation, armorFront, armorRear,armorSides,armorTop, new_armorFront, new_armorRear, new_armorSides, new_armorTop;;
	int cost, fuel, maxSpeed, realRoadSpeed, production_time, new_protection_time, new_cp, new_speed, new_fuel, new_maxSpeed, new_realRoadSpeed,
		autoCoverRange, new_autoCoverRange, occupationRadius, new_occupationRadius;

	float fuelTime, speedBonus, opticalStrenght, stealth, new_fuelTime, new_optical_strenght,new_speedBonus, new_stealth;

	int costLineOffset, fuelTimeLineOffset, fuelLineOffset, maxSpeedLineOffset, speedBonusLineOffset, opticalStrenghtLineOffset, stealthOffset,
		realRoadSpeedLineOffset, armorFrontOffset, armorRearOffset, armorTopOffset, armorSidesOffset, protection_timeOffset, autoCoverRangeOffset,
		occupationRadiusOffset;


	Weapons* guns;

	Unit() {}
	Unit(std::string _name) : name(_name) {}
	~Unit() {}
	
};
