#include "header.h"
#define BUFFER_SIZE 256
//int or float depending on the type, set the un-used param as NULL
std::string customStr(std::string text, int* intVal,float* floatVal) {

    if (intVal && floatVal) 
        MessageBoxA(NULL,"Error1 writing data",NULL,NULL);
    if (!intVal && !floatVal)
        return text; //we just return the original str

    std::string s;
    std::string str;
    std::stringstream ss;

    if (intVal) {
        s = text;
        ss << *intVal;
        str = ss.str();
        s.append(str);
        return s;
    }
    else {
        s = text;
        ss << *floatVal;
        str = ss.str();
        s.append(str);
        return s;
    }
}
//when calling, use the correct type according to the bool (last parametre)
bool writeData(params* modPtrs,settings_t settings,bool isUnit) {

    /* File pointer to hold reference of input file */
    FILE* fPtr;
    FILE* fTemp;

    char buffer[BUFFER_SIZE];
    int  count;

    if (isUnit) {
        
        settings.original_path = settings.ud_original_path;
        settings.new_path = settings.ud_new_path;
    }
    else {

        settings.original_path = settings.am_original_path;
        settings.new_path = settings.am_new_path;
    }

    /*  Open all required files */
    fopen_s(&fPtr, settings.new_path.c_str(), "r");
    fopen_s(&fTemp,"replace.tmp", "w");

    /* fopen() return NULL if unable to open file in given mode. */
    if (fPtr == NULL || fTemp == NULL)
    {
        /* Unable to open file hence exit */
        printf("\nUnable to open file.\n");
        printf("Please check whether file exists and you have read/write privilege.\n");
        return false;
    }

    /*
    * Read line from source file and write to destination
    * file after replacing given line.
    */
    count = 1;
    bool hit = false;
    Unit* curr_unit = nullptr;
    Ammo* curr_ammo = nullptr;


    while ((fgets(buffer, BUFFER_SIZE, fPtr)) != NULL)
    {
        if (isUnit) {
            for (int i = 0; i < modPtrs->unitsToModify.size(); i++) {

                curr_unit = modPtrs->unitsToModify.at(i);
                /* If current line is line to replace */
                if(curr_unit->costLineOffset != 0)
                    if (count == (curr_unit->exportLineNumber + curr_unit->costLineOffset)) { //comand points
                    
                        fputs(customStr("                        (~/Resource_CommandPoints, ", &curr_unit->new_cp, NULL).append("),\n").c_str(), fTemp);
                        hit = true;
                    }
                if (curr_unit->fuelLineOffset != 0)
                    if (count == (curr_unit->exportLineNumber + curr_unit->fuelLineOffset)) {

                        fputs(customStr("                    FuelCapacity     = ", &curr_unit->new_fuel, NULL).append("\n").c_str(), fTemp);
                        hit = true;
                    }
                if (curr_unit->fuelTimeLineOffset != 0)
                    if (count == (curr_unit->exportLineNumber + curr_unit->fuelTimeLineOffset)) {

                        fputs(customStr("                    FuelMoveDuration = ", NULL, &curr_unit->new_fuelTime).append("\n").c_str(), fTemp);
                        hit = true;
                    }
                if (curr_unit->maxSpeedLineOffset != 0)
                    if (count == (modPtrs->unitsToModify[i]->exportLineNumber + curr_unit->maxSpeedLineOffset)) {

                        fputs(customStr("               MaxSpeed         = ", &curr_unit->new_maxSpeed, NULL).append("\n").c_str(), fTemp);
                        hit = true;
                    }
                if (curr_unit->speedBonusLineOffset != 0)
                    if (count == (curr_unit->exportLineNumber + curr_unit->speedBonusLineOffset)) {

                        fputs(customStr("                    SpeedBonusOnRoad = ", NULL, &curr_unit->new_speedBonus).append("\n").c_str(), fTemp);
                        hit = true;
                    }
                if (curr_unit->opticalStrenghtLineOffset != 0)
                    if (count == (curr_unit->exportLineNumber + curr_unit->opticalStrenghtLineOffset)) {

                        fputs(customStr("                    OpticalStrength = ", NULL, &curr_unit->new_optical_strenght).append("\n").c_str(), fTemp);
                        hit = true;
                    }
                if (curr_unit->realRoadSpeedLineOffset != 0)
                    if (count == (curr_unit->exportLineNumber + curr_unit->realRoadSpeedLineOffset)) {

                        fputs(customStr("                RealRoadSpeed = ", &curr_unit->new_realRoadSpeed, NULL).append("\n").c_str(), fTemp);
                        hit = true;
                    }
                if (curr_unit->armorFrontOffset != 0)
                    if (count == (curr_unit->exportLineNumber + curr_unit->armorFrontOffset)) {
                       
                        fputs(customStr("                        ArmorDescriptorFront = ", NULL, NULL).append(curr_unit->new_armorFront).c_str(), fTemp);
                        hit = true;
                    }
                if (curr_unit->armorSidesOffset != 0)
                    if (count == (curr_unit->exportLineNumber + curr_unit->armorSidesOffset)) {

                        fputs(customStr("                        ArmorDescriptorSides = ", NULL, NULL).append(curr_unit->new_armorSides).c_str(), fTemp);
                        hit = true;
                    }
                if (curr_unit->armorRearOffset != 0)
                    if (count == (curr_unit->exportLineNumber + curr_unit->armorRearOffset)) {

                        fputs(customStr("                        ArmorDescriptorRear = ", NULL, NULL).append(curr_unit->new_armorRear).c_str(), fTemp);
                        hit = true;
                    }
                if (curr_unit->armorTopOffset != 0)
                    if (count == (curr_unit->exportLineNumber + curr_unit->armorTopOffset)) {

                        fputs(customStr("                        ArmorDescriptorTop = ", NULL, NULL).append(curr_unit->new_armorTop).c_str(), fTemp);
                        hit = true;
                    }
                if (curr_unit->protection_timeOffset != 0)
                    if (count == (curr_unit->exportLineNumber + curr_unit->protection_timeOffset)) {

                        fputs(customStr("                    ProductionTime     = ", &curr_unit->new_protection_time, NULL).append("\n").c_str(), fTemp);
                        hit = true;
                    }
                if (curr_unit->stealthOffset != 0)
                    if (count == (curr_unit->exportLineNumber + curr_unit->stealthOffset)) {

                        fputs(customStr("            UnitConcealmentBonus = ", NULL, &curr_unit->new_stealth).append("\n").c_str(), fTemp);
                        hit = true;
                    }
                if (curr_unit->autoCoverRangeOffset != 0)
                    if (count == (curr_unit->exportLineNumber + curr_unit->autoCoverRangeOffset)) {

                        fputs(customStr("                    AutoCoverRange             = ", &curr_unit->new_autoCoverRange,NULL ).append("*Metre\n").c_str(), fTemp);
                        hit = true;
                    }
                if (curr_unit->occupationRadiusOffset != 0)
                    if (count == (curr_unit->exportLineNumber + curr_unit->occupationRadiusOffset)) {

                        fputs(customStr("                    OccupationRadius           = ", &curr_unit->new_occupationRadius,NULL).append("*Metre\n").c_str(), fTemp);
                        hit = true;
                    }
            }
            if (!hit) {
                fputs(buffer, fTemp);
            }
        }
        else //ammo
        {
         
            for (int j = 0; j < modPtrs->ammunitionToModify.size(); j++) {

                curr_ammo = modPtrs->ammunitionToModify.at(j);
                if(curr_ammo->physicalDamagesLineOffset != 0)
                    if (count == (curr_ammo->startLineNumber + curr_ammo->physicalDamagesLineOffset)) {
                        fputs(customStr("    PhysicalDamages                   = ", NULL, &curr_ammo->new_PhysicalDamages).append("\n").c_str(), fTemp);
                        hit = true;
                    }
                if (curr_ammo->max_range_offset != 0)
                    if (count == (curr_ammo->startLineNumber + curr_ammo->max_range_offset)) {
                        fputs(customStr("    PorteeMaximale                    = ((", &curr_ammo->new_max_range, NULL).append(") * Metre)\n").c_str(), fTemp);
                        hit = true;
                    }
                if (curr_ammo->min_range_offset != 0)
                    if (count == (curr_ammo->startLineNumber + curr_ammo->min_range_offset)) {
                        fputs(customStr("    PorteeMinimale                    = ((", &curr_ammo->new_min_range, NULL).append(") * Metre)\n").c_str(), fTemp);
                        hit = true;
                    }
                if (curr_ammo->suppressDamageOffset != 0)
                    if (count == (curr_ammo->startLineNumber + curr_ammo->suppressDamageOffset)) {
                        fputs(customStr("    SuppressDamages                   = ", NULL, &curr_ammo->new_suppressDamage).append("\n").c_str(), fTemp);
                        hit = true;
                    }
                if (curr_ammo->radiusSuppressDamageOffset != 0)
                    if (count == (curr_ammo->startLineNumber + curr_ammo->radiusSuppressDamageOffset)) {
                        fputs(customStr("    RadiusSplashSuppressDamages       = ", &curr_ammo->new_suppressDamageRadius, NULL).append(" * Metre\n").c_str(), fTemp);
                        hit = true;
                    }
                if (curr_ammo->NoiseDissimulationMalusOffset != 0)
                    if (count == (curr_ammo->startLineNumber + curr_ammo->NoiseDissimulationMalusOffset)) {
                        fputs(customStr("    NoiseDissimulationMalus           = ", NULL, &curr_ammo->new_NoiseDissimulationMalus).append("\n").c_str(), fTemp);
                        hit = true;
                    }
                if (curr_ammo->ShotsBeforeMaxNoiseOffset != 0)
                    if (count == (curr_ammo->startLineNumber + curr_ammo->ShotsBeforeMaxNoiseOffset)) {
                        fputs(customStr("    ShotsBeforeMaxNoise               = ", &curr_ammo->new_ShotsBeforeMaxNoise, NULL).append("\n").c_str(), fTemp);
                        hit = true;
                    }
                if (curr_ammo->TempsDeViseeOffset != 0)
                    if (count == (curr_ammo->startLineNumber + curr_ammo->TempsDeViseeOffset)) {
                        fputs(customStr("    TempsDeVisee                      = ", NULL, &curr_ammo->new_TempsDeVisee).append("\n").c_str(), fTemp);
                        hit = true;
                    }
                if (curr_ammo->TempsEntreDeuxSalvesOffset != 0)
                    if (count == (curr_ammo->startLineNumber + curr_ammo->TempsEntreDeuxSalvesOffset)) {
                        fputs(customStr("    TempsEntreDeuxSalves              = ", NULL , &curr_ammo->new_TempsEntreDeuxSalves).append("\n").c_str(), fTemp);
                        hit = true;
                    }
            }
            if (!hit) {
                fputs(buffer, fTemp);
            }
            
        }
        hit = false;
        count++;
    }

    /* Close all files to release resource */
    fclose(fPtr);
    fclose(fTemp);

    /* Delete original source file */
    if (remove(settings.new_path.c_str())) {
        MessageBox(NULL, "Fail deleting after writing", NULL, NULL);
    }

    /* Rename temporary file as original file */
    if (rename("replace.tmp", settings.new_path.c_str())) {
        MessageBox(NULL, "Fail renaming after writing", NULL, NULL);
    }

    return true; 
}