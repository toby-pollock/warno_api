#include "header.h"
#include <fstream>
//check if a file exist using the path or name if in the same dir
bool is_file_exist(const char* fileName)
{
	std::ifstream infile(fileName);
	return infile.good();
}
void updatePtrToNewFile(std::vector<Unit*> unit_vector[], settings_t* settings,std::string oldName,std::string newName) {
	/*
	1- checking if directories.txt is present (file containing the path to the mod)
	2- completing the path hardcoded (not user related so its ok) with the original file (UniteDescriptor.ndf) and a new one (UniteDescriptor2.txt)
	3- making a copy of the original file to the path of the new one (here the same dir)
	4- editing the new txt file (need txt to open)
	5- removing the original file
	6- renaming the new file with the name of the original file
	*/
	char buffer[256];
	FILE* direct_f;
	fopen_s(&direct_f, "directories.txt", "r");
	if (direct_f) {

		if (fgets(buffer, sizeof(buffer), direct_f)) {
			settings->original_path = (std::string)buffer;
			settings->new_path = (std::string)buffer;
			settings->original_path.append("\\GameData\\Generated\\Gameplay\\Gfx\\").append(oldName);
			settings->new_path.append("\\GameData\\Generated\\Gameplay\\Gfx\\").append(newName);
		}
	}
	else {
		MessageBoxA(NULL, "Erreur, while reading directories in getDataFromFile()", NULL, NULL);
		exit(1);
	}
	fclose(direct_f);


	if (!CopyFileA(settings->original_path.c_str(), settings->new_path.c_str(), false)) {	//true to not overwrite
		MessageBoxA(NULL, "Error while copying original ndf file", NULL, NULL);
		exit(1);
	}

	bool isUnitDesc_exist = is_file_exist(settings->new_path.c_str());
	if (!isUnitDesc_exist) {
		MessageBoxA(NULL, "Erreur, file unitDescriptor not found", NULL, NULL);
		exit(1);
	}


	fopen_s(&settings->hNew_file, settings->new_path.c_str(), "r");
	if (!settings->hNew_file) {
		MessageBox(NULL, "Error reading data from Unitdesc", NULL, NULL);
		exit(1);
	}
}
/// <summary>
/// this function read data and stores it in a big array
/// </summary>
/// <param name="array"></param>
/// <returns></returns>
int getDataFromFile(std::vector<Unit*> unit_vector[],std::vector<Ammo*> ammo_vector[], settings_t* settings) {

	char buffer[256];					//store the line read by fgets(), don't go under 200 some lines are long and it will cause issues for the line counters variables
	int line_counter_relative = 1;		//count line but is relative to the export line of the unit	
	int line_counter = 1;				//count all lines from the start to the end
	int units_counter = 0;				//count the unit
	
	

	bool is_directoriesFile = is_file_exist("directories.txt");
	if (!is_directoriesFile) {
		MessageBoxA(NULL, "Erreur, file directories not found", NULL, NULL);
		exit(1);
	}
	
	updatePtrToNewFile(unit_vector, settings, "UniteDescriptor.ndf", "UniteDescriptor2.txt");
	settings->ud_original_path = settings->original_path;
	settings->ud_new_path = settings->new_path;
	std::string line;
	//unit_descriptor loop
	while (fgets(buffer, sizeof(buffer), settings->hNew_file) != NULL) {

		line = (std::string)buffer;
		readLine(line, &line_counter_relative,&line_counter, &units_counter,nullptr, unit_vector,nullptr,1);

		line_counter_relative++;
		line_counter++;
	}

	fclose(settings->hNew_file);

	line_counter_relative = 1;
	line_counter = 1;
	int ammo_counter = 0;
	updatePtrToNewFile(unit_vector, settings, "Ammunition.ndf", "Ammunition2.txt");
	settings->am_original_path = settings->original_path;
	settings->am_new_path = settings->new_path;
	//ammunition_descriptor loop
	while (fgets(buffer, sizeof(buffer), settings->hNew_file) != NULL) {

		line = (std::string)buffer;
		readLine(line, &line_counter_relative, &line_counter, &ammo_counter, ammo_vector);

		line_counter_relative++;
		line_counter++;
	}
	fclose(settings->hNew_file);

	line_counter_relative = 1;
	line_counter = 1;
	int weapon_counter = 0;
	updatePtrToNewFile(unit_vector, settings, "WeaponDescriptor.ndf", "WeaponDescriptor2.txt");
	settings->wd_original_path = settings->original_path;
	settings->wd_new_path = settings->new_path;

	int current_unit_index;
	//ammunition_descriptor loop
	while (fgets(buffer, sizeof(buffer), settings->hNew_file) != NULL) {

		line = (std::string)buffer;
		readLine(line, &line_counter_relative, &line_counter, &weapon_counter,&current_unit_index, unit_vector,ammo_vector,2);

		line_counter_relative++;
		line_counter++;
	}
	fclose(settings->hNew_file);

	
	return units_counter;
}
//delete the space(s) at the beginning of the string
std::string deleteSpacePrefix(std::string original){

	int i = 0;
	int j = 0;
	while (original[i] == ' ') {
		i++;
	}
	std::string new_string = original.substr(i,100);
	return new_string;
}
Ammo* findAmmoPtrWithName(std::vector<Ammo*> vector[], std::string name) {

	int size = vector->size();
	int pos = name.rfind('\n');
	name = name.substr(0, pos);
	for (int i = 0; i < size; i++) {
		if (strstr(vector->at(i)->name.c_str(), name.c_str())) {
			return vector->at(i);
		}
	}
	return nullptr;
}
//read every line and checking if the line contains data that we need to store in our unit instances
//forUnit is 1 for unitReadline(), 2 for weapon descriptor read
//index is used to find the unitptr every looop
void readLine(std::string original_line, int* line_counter_relative,int* line_counter, int* units_counter, int* index,
	std::vector<Unit*> unit_vector[], std::vector<Ammo*> ammo_vector[], int forUnit) {

	//removing the indentation
	Unit* current_unit = nullptr;
	std::string buffer = deleteSpacePrefix(original_line);
	if (forUnit == 1) {
		//every time export is found in a string, we reset the line_counter (relative offset) and we create a new instance of the unit class
		if (strstr(original_line.c_str(), "export")) {
			*line_counter_relative = 0;

			current_unit = new Unit();
			if (current_unit) {
				//saving the base line to which we will had offset later to find attribute for the unit
				current_unit->exportLineNumber = *line_counter;
				unit_vector->push_back(current_unit);
				*units_counter += 1;
			}
		}
		if (*line_counter > 4) {

			current_unit = unit_vector->at((*units_counter) - 1); //-1 because the unit_counter start at 1
			if (!current_unit)
				return;

			switch (*line_counter_relative) 	//at the start of a descriptor for a unit, its always the same so we can use fixed size offset
			{
			case DEBUGNAME_OFFSET:
				buffer = buffer.substr(21, 100);	//cutting the string at index 21
				current_unit->name = buffer;		//storing into the unit
				return;
			case NATIONALITY_OFFSET:
				buffer = buffer.substr(48, 100);
				current_unit->nationality = buffer;
				return;
			case MOTHERCOUNTRY_OFFSET:
				buffer = buffer.substr(35, 100);
				current_unit->mother_country = buffer;
				return;
			case ACKNOW_UNIT_TYPE_OFFSET:
				buffer = buffer.substr(53, 100);
				current_unit->acknow_unit_type = buffer;
				return;
			case TYPE_UNIT_FORMATION_OFFSET:
				buffer = buffer.substr(35, 100);
				current_unit->type_unit_formation = buffer;
				return;
			default:
				break;
			}
			//the commands points is not at a fixed offset in the export, so we have to find it by looking for a substring
			//we extract the cost by converting from string to int and we keep the line offset for future purpose
			if (strstr(buffer.c_str(), "~/Resource_CommandPoints")) {
				int cp;
				buffer = buffer.substr(27, 4);	//on coupe le debut 
				std::istringstream iss(buffer); //convert string to int
				iss >> cp;
				current_unit->cost = cp;
				current_unit->new_cp = cp;
				current_unit->costLineOffset = *line_counter_relative;
			}

			else if (strstr(buffer.c_str(), "FuelCapacity     = ")) {
				int fuelCap;
				buffer = buffer.substr(19, 4);
				std::istringstream iss(buffer);
				iss >> fuelCap;
				current_unit->fuel = fuelCap;
				current_unit->new_fuel = fuelCap;
				current_unit->fuelLineOffset = *line_counter_relative;
			}
			else if (strstr(buffer.c_str(), "FuelMoveDuration = ")) {
				float fuelMove;
				buffer = buffer.substr(19, 4);
				std::istringstream iss(buffer);
				iss >> fuelMove;
				current_unit->fuelTime = fuelMove;
				current_unit->new_fuelTime = fuelMove;
				current_unit->fuelTimeLineOffset = *line_counter_relative;
			}
			else if (strstr(buffer.c_str(), "MaxSpeed         =")) {
				int maxspeed;
				buffer = buffer.substr(19, 4);
				std::istringstream iss(buffer);
				iss >> maxspeed;
				current_unit->maxSpeed = maxspeed;
				current_unit->new_maxSpeed = maxspeed;
				current_unit->maxSpeedLineOffset = *line_counter_relative;
			}
			else if (strstr(buffer.c_str(), "SpeedBonusOnRoad =")) {
				int speedBonus;
				buffer = buffer.substr(19, 18);	//on coupe le debut, et on prend au maximum 17 chiffres du float
				float speedBonus_f = std::stof(buffer);; //convert string to float
				current_unit->speedBonus = speedBonus_f;
				current_unit->new_speedBonus = speedBonus_f;
				current_unit->speedBonusLineOffset = *line_counter_relative;
			}
			else if (strstr(buffer.c_str(), "OpticalStrength = ")) {
				float optical;
				buffer = buffer.substr(18, 4);
				std::istringstream iss(buffer);
				iss >> optical;
				current_unit->opticalStrenght = optical;
				current_unit->new_optical_strenght = optical;
				current_unit->opticalStrenghtLineOffset = *line_counter_relative;
			}
			else if (strstr(buffer.c_str(), "RealRoadSpeed =")) {
				int realRoadSpeed;
				buffer = buffer.substr(16, 4);
				std::istringstream iss(buffer);
				iss >> realRoadSpeed;
				current_unit->realRoadSpeed = realRoadSpeed;
				current_unit->new_realRoadSpeed = realRoadSpeed;
				current_unit->realRoadSpeedLineOffset = *line_counter_relative;
			}
			else if (strstr(buffer.c_str(), "ArmorDescriptorFront =")) {
			
				buffer = buffer.substr(23, 50);
				
				current_unit->armorFront = buffer;
				current_unit->new_armorFront = buffer;
				current_unit->armorFrontOffset = *line_counter_relative;
			}
			else if (strstr(buffer.c_str(), "ArmorDescriptorSides =")) {
				
				buffer = buffer.substr(23, 50);
				
				current_unit->armorSides = buffer;
				current_unit->new_armorSides = buffer;
				current_unit->armorSidesOffset = *line_counter_relative;
			}
			else if (strstr(buffer.c_str(), "ArmorDescriptorRear =")) {
		
				buffer = buffer.substr(22, 50);
			
				current_unit->armorRear = buffer;
				current_unit->new_armorRear = buffer;
				current_unit->armorRearOffset = *line_counter_relative;
			}
			else if (strstr(buffer.c_str(), "ArmorDescriptorTop =")) {
				
				buffer = buffer.substr(21, 50);
				
				current_unit->armorTop = buffer;
				current_unit->new_armorTop = buffer;
				current_unit->armorTopOffset = *line_counter_relative;
			}
			else if (strstr(buffer.c_str(), "ProductionTime     = ")) {
				int prodTime;
				buffer = buffer.substr(21, 5);

				std::istringstream iss(buffer);
				iss >> prodTime;
				current_unit->production_time = prodTime;
				current_unit->new_protection_time = prodTime;
				current_unit->protection_timeOffset = *line_counter_relative;
			}
			else if (strstr(buffer.c_str(), "UnitConcealmentBonus = ")) {
				float stealth;
				buffer = buffer.substr(23, 5);

				std::istringstream iss(buffer);
				iss >> stealth;
				current_unit->stealth = stealth;
				current_unit->new_stealth = stealth;
				current_unit->stealthOffset = *line_counter_relative;
			}
			else if (strstr(buffer.c_str(), "AutoCoverRange             = ")) {
				int range;
				int first = buffer.find_first_of('*');
				if (first != std::string::npos) {
					buffer = buffer.substr(29, first);

					std::istringstream iss(buffer);
					iss >> range;
					current_unit->autoCoverRange = range;
					current_unit->new_autoCoverRange = range;
					current_unit->autoCoverRangeOffset = *line_counter_relative;
				}
			}
			else if (strstr(buffer.c_str(), "OccupationRadius           = ")) {
				int radius;
				int first = buffer.find_first_of('*');
				if (first != std::string::npos) {
					buffer = buffer.substr(29, first);

					std::istringstream iss(buffer);
					iss >> radius;
					current_unit->occupationRadius = radius;
					current_unit->new_occupationRadius = radius;
					current_unit->occupationRadiusOffset = *line_counter_relative;
				}
			}

		}
	}
	/*weapondescriptor is used to know which unit owns which ammo*/
	else if (forUnit == 2) { 
		int size = unit_vector->size();
		int pos;
		if (strstr(original_line.c_str(), "export")) {

			*line_counter_relative = 0;
			buffer = buffer.substr(23, 100);
			int pos = buffer.find_first_of(' ');
			buffer = buffer.substr(0, pos);
			for (int i = 0; i < size; i++) {
				
				if (strstr(unit_vector->at(i)->name.c_str(),buffer.c_str())) {
					unit_vector->at(i)->guns = new Weapons();
					*index = i;
				}
			}
		}
		if (*line_counter > 3) {
			current_unit = unit_vector->at(*index);
			if (current_unit) {

				if (strstr(buffer.c_str(), "Ammunition")) {
					buffer = buffer.substr(40, 100);
					Ammo* ptr = findAmmoPtrWithName(ammo_vector, buffer);
					if (ptr)
						current_unit->guns->ammos.push_back(ptr);

				}
			}
		}
	}
}
std::string returnName(std::string str) {

	int startPos = str.find_first_of("Ammo_");
	int pos = str.find_first_of(' ');
	if (startPos != 0) {				//its an export
		str = str.substr(7, 200);		//on enleve le "export "
		pos = str.find_first_of(' ');
	}
	if (pos == std::string::npos) {
		MessageBox(NULL, "Error while reading Ammunition.ndf", NULL, NULL);
		exit(1);
	}
	std::string new_string = str.substr(0, pos);
	return new_string;
}
//readLine for Ammunition.ndf
void readLine(std::string original_line, int* line_counter_relative, int* line_counter, int* weapon_counter, std::vector<Ammo*> weapon_vector[]) {

	//removing the indentation
	Ammo* current_weapon = nullptr;
	std::string buffer = deleteSpacePrefix(original_line);
	//every time export is found in a string, we reset the line_counter (relative offset) and we create a new instance of the unit class
	if (strstr(original_line.c_str(), "Ammo_")) {
		*line_counter_relative = 0;

		current_weapon = new Ammo();
		if (current_weapon) {
			//saving the base line to which we will had offset later to find attribute for the unit
			current_weapon->startLineNumber = *line_counter;
			current_weapon->name = returnName(buffer);
			weapon_vector->push_back(current_weapon);
			*weapon_counter += 1;
		}
	}
	if (*line_counter > 3) {
		current_weapon = weapon_vector->at((*weapon_counter) - 1);
		
		if (strstr(buffer.c_str(), "TDamageTypeRTTI(Family=")) {

			int pos = buffer.find_last_of('"');
			if ((pos > 60) && (pos != std::string::npos)) 
				buffer = buffer.substr(60, pos - 60);
			current_weapon->family = buffer;
			//we don't need the line offset (rn at least) bc we are not supossed to mod this value
		}
		else if (strstr(buffer.c_str(), "EProjectileType")) {

			if (buffer.size() > 52)
				buffer = buffer.substr(52, buffer.size() - 52);
			current_weapon->ProjectileType = buffer;
			//we don't need the line offset (rn at least) bc we are not supossed to mod this value
		}
		else if (strstr(buffer.c_str(), "PhysicalDamages")) {
			float phys;
			buffer = buffer.substr(36, 5);
			std::istringstream iss(buffer);
			iss >> phys;
			current_weapon->physicalDamages = phys;
			current_weapon->new_PhysicalDamages = phys;
			current_weapon->physicalDamagesLineOffset = *line_counter_relative;
		}
		else if (strstr(buffer.c_str(), "PorteeMaximale                    = ")) {
			int portemax;
			buffer = buffer.substr(38, 6);
			std::istringstream iss(buffer);
			iss >> portemax;
			current_weapon->max_range = portemax;
			current_weapon->new_max_range = portemax;
			current_weapon->max_range_offset = *line_counter_relative;
		}
		else if (strstr(buffer.c_str(), "PorteeMinimale                    = ")) {
			int portemin;
			buffer = buffer.substr(38, 6);
			std::istringstream iss(buffer);
			iss >> portemin;
			current_weapon->min_range = portemin;
			current_weapon->new_min_range = portemin;
			current_weapon->min_range_offset = *line_counter_relative;
		}
		else if (strstr(buffer.c_str(), "SuppressDamages                   = ")) {
			float supr;
			buffer = buffer.substr(36, 5);
			std::istringstream iss(buffer);
			iss >> supr;
			current_weapon->suppressDamage = supr;
			current_weapon->new_suppressDamage = supr;
			current_weapon->suppressDamageOffset = *line_counter_relative;
		}
		else if (strstr(buffer.c_str(), "RadiusSplashSuppressDamages       = ")) {
			int radius;
			buffer = buffer.substr(36, 5);
			std::istringstream iss(buffer);
			iss >> radius;
			current_weapon->radiusSuppressDamage = radius;
			current_weapon->new_suppressDamageRadius = radius;
			current_weapon->radiusSuppressDamageOffset = *line_counter_relative;
		}
		else if (strstr(buffer.c_str(), "NoiseDissimulationMalus           = ")) {
			float noise;
			buffer = buffer.substr(36, 5);
			std::istringstream iss(buffer);
			iss >> noise;
			current_weapon->NoiseDissimulationMalus = noise;
			current_weapon->new_NoiseDissimulationMalus = noise;
			current_weapon->NoiseDissimulationMalusOffset = *line_counter_relative;
		}
		else if (strstr(buffer.c_str(), "ShotsBeforeMaxNoise               = ")) {
			int shots;
			buffer = buffer.substr(36, 5);
			std::istringstream iss(buffer);
			iss >> shots;
			current_weapon->ShotsBeforeMaxNoise = shots;
			current_weapon->new_ShotsBeforeMaxNoise = shots;
			current_weapon->ShotsBeforeMaxNoiseOffset = *line_counter_relative;
		}
		else if (strstr(buffer.c_str(), "TempsDeVisee                      = ")) {
			float temps;
			buffer = buffer.substr(36, 5);
			std::istringstream iss(buffer);
			iss >> temps;
			current_weapon->TempsDeVisee = temps;
			current_weapon->new_TempsDeVisee = temps;
			current_weapon->TempsDeViseeOffset = *line_counter_relative;
		}
		else if (strstr(buffer.c_str(), "TempsEntreDeuxSalves              = ")) {
			float tempsDeux;
			buffer = buffer.substr(36, 5);
			std::istringstream iss(buffer);
			iss >> tempsDeux;
			current_weapon->TempsEntreDeuxSalves = tempsDeux;
			current_weapon->new_TempsEntreDeuxSalves = tempsDeux;
			current_weapon->TempsEntreDeuxSalvesOffset = *line_counter_relative;
		}
	}
}