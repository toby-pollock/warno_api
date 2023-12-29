#include "header.h"

/// <summary>
/// print the text under each sub categories (<=>tree node)
/// using a different array for the "allUnits tree" and all the others tree b
/// </summary>
/// <param name="filter"></param>
/// <param name="unitcount"></param>
/// <param name="_array"></param>
/// <param name="counter"></param>
/// <param name="inputs"></param>
void GUI::displayUnits(std::string filter, int unitcount, std::vector<Unit*> unit_vector[], int* counter, params* inputs,settings_t* settings ) {

    *counter = 0;
    std::string id = "Mod weapons##";
    std::string id2 = "Mod weapons##all_";

    for (int i = 0; i < unitcount; i++) {
        if (strcmp(filter.c_str(), "") == 0) { //all units
            ImGui::Checkbox(("%s", unit_vector->at(i)->name.c_str()), &inputs->checkboxes_allUnits[i]);
            ImGui::SameLine();
            
            ImGui::Button("Show Weapons");
            if (ImGui::IsItemHovered() && ImGui::BeginTooltip()) {

                displayGuns(unit_vector->at(i));
                ImGui::EndTooltip();
            }
            /*
            * This is working but not clear for user
            * 
            ImGui::SameLine();
            id2.append(unit_vector->at(i)->name.c_str());
            if (ImGui::Button(id2.c_str())) {

                inputs->ammunitionToModify.clear();
                if (!unit_vector->at(i)->guns->ammos.size() || !unit_vector->at(i)->guns) { printf("Invalid Data"); }
                inputs->ammunitionToModify = displayGuns(unit_vector->at(i));
                inputs->modify_ammo = true;
            }
            */
            (*counter) += 1;
        }
        else if (strcmp(filter.c_str(), unit_vector->at(i)->acknow_unit_type.c_str()) == 0) { //par filtre
            ImGui::Checkbox(("%s", unit_vector->at(i)->name.c_str()),&unit_vector->at(i)->isSelected);
            ImGui::SameLine();
            
            ImGui::Button("Show Weapons");
            if (ImGui::IsItemHovered() && ImGui::BeginTooltip()) {

                displayGuns(unit_vector->at(i));
                ImGui::EndTooltip();
            }
            /*
            * This is working but not clear for user
            * 
            ImGui::SameLine();
            id.append(unit_vector->at(i)->name.c_str());
            if (ImGui::Button(id.c_str())) {

                inputs->ammunitionToModify.clear();
                if (!unit_vector->at(i)->guns->ammos.size() || !unit_vector->at(i)->guns) { printf("Invalid Data"); }
                inputs->ammunitionToModify = unit_vector->at(i)->guns->ammos;
                if(inputs->ammunitionToModify.size())
                    inputs->modify_ammo = true;
            }
            */
            (*counter) += 1;
        }
    }
    ImGui::Separator();
    ImGui::TextColored(ImVec4(1.0f, 0.0f, 1.0f, 1.0f), "total count : %d", *counter);
}
std::vector<Ammo*> GUI::displayGuns(Unit* currentUnit) {


    std::string str;
    std::vector<Ammo*> AmmoPtrForUnit;
    Weapons* gunPtrForUnit;
    if (currentUnit) {
        gunPtrForUnit = currentUnit->guns;
        if (gunPtrForUnit) {
            AmmoPtrForUnit = gunPtrForUnit->ammos;
            if (AmmoPtrForUnit.size() != 0) {

                for (int j = 0; j < AmmoPtrForUnit.size(); j++) {

                    str = AmmoPtrForUnit.at(j)->name;
                    ImGui::Text(str.c_str());
                }
            }
            else {
                ImGui::Text("nothing");
                return { 0 };
            }
        }
        else {
            printf("gunPtr is nullptr\n");
            return { 0 };
        }
    }
    else {
        printf("currentUnit is nullptr\n");
        return { 0 };
    }
    return AmmoPtrForUnit;
}
void GUI::displayAmmo(std::vector<Ammo*> weapon_vector[], std::vector<Unit*> unit_vector[],std::string familyNames[],std::string typeName[], params* inputs) {

    int size = weapon_vector->size();
    int nb_Obus = 0;
    int nb_Bombe = 0;
    int nb_Grenade = 0;
    int nb_Artillerie = 0;
    int nb_balle = 0;
    int nb_GuidedMissile = 0;
    std::string withCount;
    std::string id = "Mod units##";

    for (int i = 0; i < NUMBER_OF_AMMO_FAMILY; i++) {
        if (ImGui::TreeNode(familyNames[i].c_str())) {

            ImGui::Separator();
            int k;
            for (k = 0; k < NUMBER_OF_AMMO_TYPE; k++) {
               
                typeName[k].append("\n");
                for (int j = 0; j < size; j++) {
                        
                    if (strcmp(weapon_vector->at(j)->family.c_str(), familyNames[i].c_str()) == 0) {
                        if (strcmp(weapon_vector->at(j)->ProjectileType.c_str(), typeName[k].c_str()) == 0) {
                            
                            weapon_vector->at(j)->ammo_type_id = k;
                            if (k == 0) nb_Obus++;
                            else if (k == 1) nb_Bombe++;
                            else if (k == 2) nb_Grenade++;
                            else if (k == 3) nb_Artillerie++;
                            else if (k == 4) nb_balle++;
                            else  nb_GuidedMissile++;
                        }
                    }
                }
            }
            for (int m = 0; m < NUMBER_OF_AMMO_TYPE; m++) {
                std::stringstream ss;

                if (m == 0) {
                    ss << nb_Obus;
                    withCount = typeName[m].append(ss.str());
                }

                else if (m == 1) {
                    ss << nb_Bombe;
                    withCount = typeName[m].append(ss.str());
                }
                    
                else if (m == 2) {
                    ss << nb_Grenade;
                    withCount = typeName[m].append(ss.str());
                }
                    
                else if (m == 3) {
                    ss << nb_Artillerie;
                    withCount = typeName[m].append(ss.str());
                }
                    
                else if (m == 4) {
                    ss << nb_balle;
                    withCount = typeName[m].append(ss.str());
                }
                    
                else {
                    ss << nb_GuidedMissile;
                    withCount = typeName[m].append(ss.str());
                }
                    
                if (ImGui::TreeNode(withCount.c_str())) {

                    for (int h = 0; h < size; h++) {
                        if (strcmp(weapon_vector->at(h)->family.c_str(), familyNames[i].c_str()) == 0) {
                            if (weapon_vector->at(h)->ammo_type_id == m) {

                                ImGui::Checkbox(("%s", weapon_vector->at(h)->name.c_str()), &weapon_vector->at(h)->isSelected);
                                ImGui::SameLine();
                                ImGui::Button("Show Units");
                                if (ImGui::IsItemHovered() && ImGui::BeginTooltip()) {

                                    findUnitUsingAmmo(weapon_vector->at(h),unit_vector);
                                    ImGui::EndTooltip();
                                }
                                /*
                                 * This is working but not clear for user
                                 * 
                                ImGui::SameLine();
                                id.append(weapon_vector->at(h)->name.c_str());
                                if (ImGui::Button(id.c_str())) {

                                    inputs->unitsToModify.clear();
                                    inputs->unitsToModify = findUnitUsingAmmo(weapon_vector->at(h), unit_vector);
                                    if(inputs->unitsToModify.size())
                                        inputs->modify_units = true;
                                }
                                */
                            }
                        }
                    }
                    ImGui::TreePop();
                }
            }
            ImGui::TreePop();
        }
    }
}
std::vector<Unit*> GUI::findUnitUsingAmmo(Ammo* ammo,std::vector<Unit*> unit_vector[]) {

    std::vector<Unit*> ret;
    int size = unit_vector->size();
    for (int i = 0; i < size; i++) {
        Unit* current = unit_vector->at(i);

        if (!current->guns ) continue;
        if (!current->guns->ammos.size()) continue;

        for (int k = 0; k < current->guns->ammos.size(); k++) {
            if (!strcmp(current->guns->ammos.at(k)->name.c_str(),ammo->name.c_str())) {
                ret.push_back(current);
                ImGui::Text(current->name.c_str());
            }
        }           
    }
    return ret;
}
void GUI::displayTreeNode(std::string ack[],int unitcount ,std::vector<Unit*> unit_vector[], int* counter, params* inputs, settings_t* settings) {

    for (int i = 0; i < NUMBER_OF_ACKTYPE; i++) {

        if (ImGui::TreeNode(ack[i].c_str())) { 
            ack[i].append("\n"); 
            ImGui::Separator();
            displayUnits(ack[i], unitcount, unit_vector, counter,inputs,settings);
            ImGui::TreePop();
        }
    }
}
/// <summary>
/// return true if the unit is already in the vector
/// </summary>
/// <param name="_vector"></param>
/// <param name="test"></param>
/// <returns></returns>
bool isAlreadyPresent(std::vector<Unit*> _vector,Unit* test) {

    int size = _vector.size();
    for (int i = 0; i < size; i++) {
        if (_vector[i] == test) {
            return true;
        }
    }
    return false;
}
bool isAlreadyPresent(std::vector<Ammo*> _vector, Ammo* test) {

    int size = _vector.size();
    for (int i = 0; i < size; i++) {
        if (_vector[i] == test) {
            return true;
        }
    }
    return false;
}
/// <summary>
/// return an vector with pointers toward the units to mod
/// </summary>
/// <param name="_array"></param>
/// <param name="inputs"></param>
/// <returns></returns>
std::vector<Unit*> GUI::returnSelectedUnits(std::vector<Unit*> unit_vector[], params* inputs) {

    std::vector<Unit*> returnPtrs;

    for (int i = 0; i < unit_vector->size(); i++) { //les sous categories
        if (unit_vector->at(i) != nullptr) {
            if (unit_vector->at(i)->isSelected) {
                returnPtrs.push_back(unit_vector->at(i));
            }
        }
    }
    for (int j = 0; j < unit_vector->size(); j++) { //la sous categorie "all"
        if (inputs->checkboxes_allUnits[j]) {
            if (unit_vector->at(j) != nullptr) {
                if (!isAlreadyPresent(returnPtrs, unit_vector->at(j))) {
                    returnPtrs.push_back(unit_vector->at(j));
                }
            }
        }
    }
    return returnPtrs;
}
std::vector<Ammo*> GUI::returnSelectedAmmo(std::vector<Ammo*> weapon_vector[], params* inputs) {

    std::vector<Ammo*> returnPtrs;

    for (int i = 0; i < weapon_vector->size(); i++) { //les sous categories
        if (weapon_vector->at(i) != nullptr) {
            if (weapon_vector->at(i)->isSelected) {
                returnPtrs.push_back(weapon_vector->at(i));
            }
        }
    }
    for (int j = 0; j < weapon_vector->size(); j++) { //la sous categorie "all"
        if (inputs->checkboxes_allUnits[j]) {
            if (weapon_vector->at(j) != nullptr) {
                if (!isAlreadyPresent(returnPtrs, weapon_vector->at(j))) {
                    returnPtrs.push_back(weapon_vector->at(j));
                }
            }
        }
    }
    return returnPtrs;
}
void GUI::navBarButtons(std::vector<Unit*> unit_vector[], std::vector<Ammo*> weapon_vector[], params* user_inputs) {
    if (ImGui::Button("search in Unit")) {
        user_inputs->search_unit_results = searchUnit(user_inputs->str1, unit_vector,user_inputs);

        user_inputs->show_unit_results = true;
    }
    ImGui::SameLine();
    if (ImGui::Button("search in Ammunition")) {
        user_inputs->search_ammunition_results = searchUnit(user_inputs->str1, weapon_vector,user_inputs);
        user_inputs->show_ammunition_results = true;
    }
    ImGui::SameLine();
    if (ImGui::Button("hide results")) {
        user_inputs->show_unit_results = false;
        user_inputs->show_ammunition_results = false;
    }
   
    ImGui::SameLine();

    if (ImGui::Button("Unselect all")) {
        for (int i = 0; i < unit_vector->size(); i++) {
            unit_vector->at(i)->isSelected = false;
        }
        for (int j = 0; j < weapon_vector->size(); j++) {
            weapon_vector->at(j)->isSelected = false;
        }
    }
   
    ImGui::SameLine();

    if (ImGui::Button("modify selected")) {
       
        user_inputs->unitsToModify.clear();
        user_inputs->ammunitionToModify.clear();

        user_inputs->unitsToModify = returnSelectedUnits(unit_vector, user_inputs);
        user_inputs->ammunitionToModify = returnSelectedAmmo(weapon_vector, user_inputs);
        if (user_inputs->unitsToModify.size()) {
            user_inputs->modify_units = true;
            user_inputs->unitDone = false;
            user_inputs->old_e_value = -1;
        }
        else {
            user_inputs->modify_units = false;
        }
        if (user_inputs->ammunitionToModify.size()) {
            user_inputs->modify_ammo = true;
            user_inputs->ammoDone = false;
            user_inputs->old_e_value_ammo = -1;
        }
        else {
            user_inputs->modify_ammo = false;
        }
    }
    if (user_inputs->searchError) {
        ImGui::Text("Enter at least 1 character to use the searchbar");
    }
}
//main window
void GUI::unitWindow(int unitcount, std::vector<Unit*> unit_vector[], std::vector<Ammo*> weapon_vector[], params* user_inputs, settings_t* settings, bool* x_button,HWND hWnd)
{

    int counter;
    std::string ack_type[NUMBER_OF_ACKTYPE]; //string array containing all "AcknowUnitType"
    
#pragma region ack_type def 
    ack_type[0] = "SAM";
    ack_type[1] = "CanonAA";
    ack_type[2] = "GunArtillery";
    ack_type[3] = "ArtShell";
    ack_type[4] = "Reco";
    ack_type[5] = "TankDestroyer";
    ack_type[6] = "Command";
    ack_type[7] = "Tank";
    ack_type[8] = "Transport";
    ack_type[9] = "TankDestroyerMissile";
    ack_type[10] = "Inf";
    ack_type[11] = "Inf2";
    ack_type[12] = "Engineer";
    ack_type[13] = "Recon_INF";
    ack_type[14] = "MLRS";
    ack_type[15] = "Recon_Vehicle";
    ack_type[16] = "CommandVehicle";
    ack_type[17] = "Logistic";
    ack_type[18] = "Command_Infantry";
    ack_type[19] = "Inf_Elite";
    ack_type[20] = "Inf_Militia";
    ack_type[21] = "KaJaPa";
    ack_type[22] = "HeliAttack";
    ack_type[23] = "HeliTransport";
    ack_type[24] = "GroundAtk";
    ack_type[25] = "AirSup";
    ack_type[26] = "Air_CAS";
    ack_type[27] = "Multirole";
#pragma endregion
    std::string ammo_family[NUMBER_OF_AMMO_FAMILY];
#pragma region ammo_family_def
    ammo_family[0] = "ap";
    ammo_family[1] = "he_autocanon";
    ammo_family[2] = "ap_missile";
    ammo_family[3] = "he";
    ammo_family[4] = "howz_bombe";
    ammo_family[5] = "balledca";
    ammo_family[6] = "flamme";
    ammo_family[7] = "fmballe";
    ammo_family[8] = "superhe";
    ammo_family[9] = "howz";
    ammo_family[10] = "roquette_ap";
    ammo_family[11] = "cluster_ap";
    ammo_family[12] = "missile_he";
    ammo_family[13] = "cac";
    ammo_family[14] = "cluster";
#pragma endregion
    std::string ammo_type[NUMBER_OF_AMMO_TYPE];
#pragma region ammo_family_def
    ammo_type[0] = "Obus";
    ammo_type[1] = "Bombe";
    ammo_type[2] = "Grenade";
    ammo_type[3] = "Artillerie";
    ammo_type[4] = "Balle";
    ammo_type[5] = "GuidedMissile";
#pragma endregion

    updateImGuiWindow(hWnd,8,31,-18,-42,NULL);
    ImGui::Begin("Selector",x_button, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBackground);

 
   
    ImGui::InputTextWithHint("##unitNameBox","type here", user_inputs->str1, IM_ARRAYSIZE(user_inputs->str1));
 
    navBarButtons(unit_vector, weapon_vector, user_inputs);
   
    if (user_inputs->search_unit_results.size()) {
        if (user_inputs->show_unit_results) {
            showSearchResults(user_inputs->search_unit_results,user_inputs->unitsToModify);
        }
    }
    if (user_inputs->search_ammunition_results.size()) {
        if (user_inputs->show_ammunition_results) {
            showSearchResults(user_inputs->search_ammunition_results, user_inputs->ammunitionToModify);
        }
    }
    if (ImGui::CollapsingHeader(("Manual Search Units"))) {
        
        displayTreeNode(ack_type, unitcount, unit_vector, &counter,user_inputs,settings);
        
        if (ImGui::TreeNode("allUnits")) {

            displayUnits("", unitcount, unit_vector, &counter,user_inputs,settings);
            ImGui::TreePop();
        }
    }
    if (ImGui::CollapsingHeader(("Manual Search Ammo"))) {
        
        displayAmmo(weapon_vector,unit_vector, ammo_family,ammo_type, user_inputs);
    }


    if (user_inputs->modify_units) {
        if (!user_inputs->unitDone)
            unitSelectedWindow(unit_vector, weapon_vector, user_inputs, settings, ack_type);
        else
            user_inputs->modify_units = false;
    }
    if (user_inputs->modify_ammo) {
        if (!user_inputs->ammoDone)
            ammoSelectedWindow(weapon_vector, unit_vector, user_inputs, settings);
        else
            user_inputs->modify_ammo = false;
    }
    if (ImGui::IsWindowFocused())
    {
        user_inputs->modify_ammo = false;
        user_inputs->modify_units = false;
    }
    ImGui::End();
}
void GUI::updateStatsView(params* user_inputs,int indexToSkip) {

    int size = user_inputs->unitsToModify.size();
    for (int i = 0; i < size; i++) {
        if (i != indexToSkip) {
            user_inputs->unitsToModify[i]->viewStats = false;
        }
    }
}
void GUI::ammoSelectedWindow(std::vector<Ammo*> weapon_vector[], std::vector<Unit*> unit_vector[],params* user_inputs,settings_t* settings) {

   
    ImGui::Begin("Selected Ammunitions",&user_inputs->statusAmmoModWind);
    if (!user_inputs->statusAmmoModWind) {
        user_inputs->ammoDone = true;
        user_inputs->statusAmmoModWind = true;
    }

    static int e = 0;
    static float physical_damage = 0.f;
    static int max_range = 0;
    static int min_range = 0;
    static float suppress_damage = 0.f;
    static int suppress_damage_radius = 0;
    static float NoiseDissimulationMalus = 0.f;
    static int ShotsBeforeMaxNoise = 0;
    static float TempsEntreDeuxSalves = 0.f;
    static float TempsDeVisee = 0.f;

    int size = user_inputs->ammunitionToModify.size();

    ImGui::TextColored(ImVec4(1.0f, 0.0f, 1.0f, 1.0f), "weapons count : %d",size );
    ImGui::Separator();
    ImGui::Text("select unit to show info about it");

    std::string id = "##"; //https://github.com/ocornut/imgui/blob/master/docs/FAQ.md#q-about-the-id-stack-system

    
    for (int i = 0; i < size; i++) {

        id.append(user_inputs->ammunitionToModify[i]->name);
        ImGui::RadioButton(id.c_str(), &e, i);
        ImGui::SameLine();
        ImGui::Text("%s", user_inputs->ammunitionToModify[i]->name.c_str());
    }

    if (e >= size) e = 0;
    if ((e < size) && (e != user_inputs->old_e_value_ammo)) {
        //display current stats in the box
        physical_damage = user_inputs->ammunitionToModify[e]->physicalDamages;
        max_range = user_inputs->ammunitionToModify[e]->max_range;
        min_range = user_inputs->ammunitionToModify[e]->min_range;
        suppress_damage = user_inputs->ammunitionToModify[e]->suppressDamage;
        suppress_damage_radius = user_inputs->ammunitionToModify[e]->radiusSuppressDamage;
        NoiseDissimulationMalus = user_inputs->ammunitionToModify[e]->NoiseDissimulationMalus;
        ShotsBeforeMaxNoise = user_inputs->ammunitionToModify[e]->ShotsBeforeMaxNoise;
        TempsDeVisee = user_inputs->ammunitionToModify[e]->TempsDeVisee;
        TempsEntreDeuxSalves = user_inputs->ammunitionToModify[e]->TempsEntreDeuxSalves;

        user_inputs->old_e_value_ammo = e;
    }

    ImGui::Separator();
    ImGui::Text("Modifications : ");
    ImGui::Separator();

    ImGui::InputFloat("PhysicalDamages", &physical_damage, (0.00F), (0.0F), "%.8f");
        
    if (ImGui::CollapsingHeader("Range")) {

        ImGui::InputInt("Max Range", &max_range);
        ImGui::InputInt("Min Range", &min_range);
    }
    if (ImGui::CollapsingHeader("Suppress")) {

        ImGui::InputFloat("Suppress Damage", &suppress_damage);
        ImGui::InputInt("Suppress Damage Radius", &suppress_damage_radius);
    }
    if (ImGui::CollapsingHeader("Noise")) {

        ImGui::InputFloat("NoiseDissimulationMalus",&NoiseDissimulationMalus );
        ImGui::InputInt("ShotsBeforeMaxNoise", &ShotsBeforeMaxNoise);
    }
    if (ImGui::CollapsingHeader("Timings")) {

        ImGui::InputFloat("TempsDeVisee", &TempsDeVisee);
        ImGui::InputFloat("TempsEntreDeuxSalves", &TempsEntreDeuxSalves);
    }

    ImGui::Separator();
    //converting int to string
    std::stringstream ss;
    ss << size;
    std::string str = ss.str();
    //reusing id variable for different purpose
    id = "update the ";
    id.append(str);
    id.append(" selected ammo");

    int g = 2; //green
    ImGui::PushID(g);
    ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(g / 7.0f, 0.6f, 0.6f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(g / 7.0f, 0.7f, 0.7f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(g / 7.0f, 0.8f, 0.8f));

  
    if (ImGui::Button(id.c_str())) {

        for (int k = 0; k < size; k++) {
            user_inputs->ammunitionToModify.at(k)->new_PhysicalDamages = physical_damage;
            user_inputs->ammunitionToModify.at(k)->new_max_range = max_range;
            user_inputs->ammunitionToModify.at(k)->new_min_range = min_range;
            user_inputs->ammunitionToModify.at(k)->new_suppressDamage = suppress_damage;
            user_inputs->ammunitionToModify.at(k)->new_suppressDamageRadius = suppress_damage_radius;
            user_inputs->ammunitionToModify.at(k)->new_NoiseDissimulationMalus = NoiseDissimulationMalus;
            user_inputs->ammunitionToModify.at(k)->new_ShotsBeforeMaxNoise = ShotsBeforeMaxNoise;
            user_inputs->ammunitionToModify.at(k)->new_TempsDeVisee = TempsDeVisee;
            user_inputs->ammunitionToModify.at(k)->new_TempsEntreDeuxSalves = TempsEntreDeuxSalves;
        }
        if (writeData(user_inputs, *settings, false)) {
            user_inputs->restartApp = true;
        }
            
    }
    ImGui::PopStyleColor(3);
    ImGui::PopID();

    ImGui::Separator();
    if (user_inputs->restartApp) {
        
        showModif(unit_vector, weapon_vector);
    }

    
    ImGui::End();
}
bool isCorrectArmor(char str[]) {

    int armorVal = 0;
    std::string test = str;
    test = test.substr(25, 3);
    std::istringstream iss(test);
    iss >> armorVal;

    if (armorVal == 0) return false; 
    for (int i = 0; i < 22; i++) {//pas de armor au dessus de 21
        if (i == 18)continue; //pas de armor a 18
        if (armorVal == i)
            return true;
    }
    return false;
}
//window opened after button "modify" is pressed
void GUI::unitSelectedWindow(std::vector<Unit*> unit_vector[], std::vector<Ammo*> weapon_vector[],params* user_inputs,settings_t* settings, std::string* ack_type) {


    ImGui::Begin("Selected Units",&user_inputs->statusUnitModWind);
    if (!user_inputs->statusUnitModWind) {
        user_inputs->unitDone = true;
        user_inputs->statusUnitModWind = true;
    }
        
    static char front[128];
    static char back[128];
    static char top[128];
    static char sides[128];
    static int cp = 0;
    static int e = 0; //holding info about which radio button is selected, starting at index 0
    static int fuel = 0;
    static float fuelTime = 0.f;
    static float stealth = 0.f;
    static int maxspeed = 0;
    static float speedBonus = 0.f;
    static float opticalStrenght = 0.f;
    static int realRoadSpeed = 0;
    static int prodTime = 0;
    static int autoCoverRange = 0;
    static int occupationRadius = 0;
    int size = user_inputs->unitsToModify.size();

    //les types d'unités dont on va afficher leur speedbonusonroad et pouvoir les modifier
#pragma region speed_bonus_exists
    std::vector<std::string> speed_bonus_exists = {
        "SAM",
        "CanonAA",
        "GunArtillery",
        "ArtShell",
        "Reco",
        "TankDestroyer",
        "Command",
        "Tank",
        "Transport",
        "TankDestroyerMissile",
        "MLRS",
        "Recon_Vehicle",
        "CommandVehicle",
        "Logistic",
        "KaJaPa",
    };
#pragma endregion


    ImGui::TextColored(ImVec4(1.0f, 0.0f, 1.0f, 1.0f), "unitcount : %d", size);
    ImGui::Separator();
    ImGui::Text("select unit to show info about it");
    ImGui::Separator();

    
    
    std::string id = "##"; //https://github.com/ocornut/imgui/blob/master/docs/FAQ.md#q-about-the-id-stack-system
    

    for (int i = 0; i < size; i++) {

        id.append(user_inputs->unitsToModify[i]->name);
        ImGui::RadioButton(id.c_str(), &e, i);
        ImGui::SameLine();
        ImGui::Text("%s", user_inputs->unitsToModify[i]->name.c_str());
    }

    if (e >= size) e = 0;

    if ((e < size) && (e != user_inputs->old_e_value)) {
        cp = user_inputs->unitsToModify[e]->cost; //index du radio button
        fuel = user_inputs->unitsToModify[e]->fuel;
        fuelTime = user_inputs->unitsToModify[e]->fuelTime;
        maxspeed = user_inputs->unitsToModify[e]->maxSpeed;
        speedBonus = user_inputs->unitsToModify[e]->speedBonus;
        opticalStrenght = user_inputs->unitsToModify[e]->opticalStrenght;
        realRoadSpeed = user_inputs->unitsToModify[e]->realRoadSpeed;

        strcpy(back, user_inputs->unitsToModify[e]->armorRear.c_str());
        strcpy(front, user_inputs->unitsToModify[e]->armorFront.c_str());
        strcpy(sides, user_inputs->unitsToModify[e]->armorSides.c_str());
        strcpy(top, user_inputs->unitsToModify[e]->armorTop.c_str());

        prodTime = user_inputs->unitsToModify[e]->production_time;
        stealth = user_inputs->unitsToModify[e]->stealth;
        autoCoverRange = user_inputs->unitsToModify[e]->autoCoverRange;
        occupationRadius = user_inputs->unitsToModify[e]->occupationRadius;
        user_inputs->old_e_value = e;
    }
    
    //Check if the highlighted unit contains speedBonusOnRoad by seeing if its type is in the array speed_bonus_exists[].
    //If the ack type drawer of the highlighted unit is dropped down, then 
    // "user_inputs->unitsToModify[e]->acknow_unit_type" will have "\n" at the end.
    //That's why we compare with the strings in speed_bonus_exists, and also with those strings with "\n" appended.
    bool unit_has_speed_bonus = false;
    for (int i = 0; i < speed_bonus_exists.size(); i++) {
        if (
            (user_inputs->unitsToModify[e]->acknow_unit_type == speed_bonus_exists[i])
            ||
            (user_inputs->unitsToModify[e]->acknow_unit_type == speed_bonus_exists[i].append("\n"))
            )
        {
            unit_has_speed_bonus = true;
            break;
        }
    }

    ImGui::Separator();
    ImGui::Text("Modifications : ");
    ImGui::Separator();
    if (ImGui::CollapsingHeader("deployment")) {
        ImGui::InputInt("command points", &cp);
        ImGui::InputInt("production time", &prodTime);

    }
    if (ImGui::CollapsingHeader("Fuel attributes")) {
        ImGui::InputInt("fuel capacity", &fuel);
        ImGui::InputFloat("fuel time", &fuelTime, (0.00F), (0.0F), "%.8f");
    }

    if (ImGui::CollapsingHeader("Speed Attributes"))
    {
        if (unit_has_speed_bonus)
        {
            ImGui::Checkbox("Auto", &user_inputs->is_auto_speed_bonus);
            ImGui::SameLine();
            if (ImGui::Button("Help"))
                ImGui::OpenPopup("auto_help");

            const char help[] = "With auto enabled, speedBonusOnRoad is calculated in a way that\nthe on road speed remains the same independent of \nthe value of maxSpeed AKA the off road speed.\n\nFurthermore, changing realRoadSpeed (UI variable only) will increase \nor decrease speedBonusOnRoad in a way that the on road speed will change \nproportionally to the change made to RealRoadSpeed";


            if (ImGui::BeginPopup("auto_help"))
            {
                ImGui::Text(help);
                ImGui::EndPopup();
            }

            ImGui::InputInt("RealRoadSpeed", &realRoadSpeed);
        }
        ImGui::InputInt("MaxSpeed", &maxspeed);
        if (unit_has_speed_bonus)
        {
            int flag;
            char description[30];
            if (user_inputs->is_auto_speed_bonus) {
                flag = ImGuiInputTextFlags_::ImGuiInputTextFlags_ReadOnly;
                ImGui::Text("auto:");
                ImGui::SameLine();
                
                //maintain the same on road speed even if the off road speed (maxspeed) changes.
                //Formula: (oldMaxSpeed + (oldMaxSpeed*oldSpeedBonus)) = (newMaxSpeed + (newMaxSpeed * newSpeedBonus))
                int oldMaxSpeed = user_inputs->unitsToModify[e]->maxSpeed;
                float oldSpeedBonus = user_inputs->unitsToModify[e]->speedBonus;
                speedBonus = ((oldMaxSpeed + oldMaxSpeed * oldSpeedBonus) - maxspeed) / maxspeed;

                //Change the on road speed proportionally to the change of realRoadSpeed
                //Formula: (maxSpeed + (maxSpeed * oldSpeedBonus)) * (newRealRoadSpeed/oldRealRoadSpeed) = (maxSpeed + (maxSpeed * newSpeedBonus))
                //-> newSpeedBonus = (newRealRoadSpeed/oldRealRoadSpeed) * (1+oldSpeedBonus) - 1
                float ratio = static_cast<float>(realRoadSpeed) / static_cast<float>(user_inputs->unitsToModify[e]->realRoadSpeed);
                speedBonus = ratio * (1 + speedBonus) - 1;
            }
            else
                flag = ImGuiInputTextFlags_::ImGuiInputTextFlags_None;
            ImGui::InputFloat("SpeedBonusOnRoad", &speedBonus, (0.00F), (0.0F), "%.8f", flag);
        }
      
    }

    if (ImGui::CollapsingHeader("Optical")) {
        ImGui::InputFloat("optical strenght", &opticalStrenght, (0.00F), (0.0F), "%.8f");
    }
    if (ImGui::CollapsingHeader("Armor")) {
        ImGui::Text("change the last number between 0-22 both excluded");
        ImGui::Text("(18 will not work)");
        ImGui::Separator();
        ImGui::InputText("armorFront", front, IM_ARRAYSIZE(front));
        ImGui::InputText("armorBack", back,IM_ARRAYSIZE(front));
        ImGui::InputText("armorSides", sides,IM_ARRAYSIZE(front));
        ImGui::InputText("armorTop", top,IM_ARRAYSIZE(front));
    }
    if (ImGui::CollapsingHeader("Visibility")) {
        ImGui::InputFloat("stealth", &stealth);
        ImGui::InputInt("autoCoverRange", &autoCoverRange);
        ImGui::InputInt("occupationRadius", &occupationRadius);
    }
    ImGui::Separator();
    //converting int to string
    std::stringstream ss;
    ss << size;
    std::string str = ss.str();
    //reusing id variable for different purpose
    id = "update the ";
    id.append(str);
    id.append(" selected units");

    int g = 2; //green
    ImGui::PushID(g);
    ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(g / 7.0f, 0.6f, 0.6f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(g / 7.0f, 0.7f, 0.7f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(g / 7.0f, 0.8f, 0.8f));


    if (ImGui::Button(id.c_str())) {

        for (int u = 0; u < size; u++) {
            user_inputs->unitsToModify.at(u)->new_cp = cp;
            user_inputs->unitsToModify.at(u)->new_fuel = fuel;
            user_inputs->unitsToModify.at(u)->new_fuelTime = fuelTime;
            user_inputs->unitsToModify.at(u)->new_maxSpeed = maxspeed;
            user_inputs->unitsToModify.at(u)->new_speedBonus = speedBonus;
            user_inputs->unitsToModify.at(u)->new_optical_strenght = opticalStrenght;
            user_inputs->unitsToModify.at(u)->new_realRoadSpeed = realRoadSpeed;

            if(isCorrectArmor(front))
                user_inputs->unitsToModify.at(u)->new_armorFront = front;
            else
                user_inputs->unitsToModify.at(u)->new_armorFront = user_inputs->unitsToModify.at(u)->armorFront;
            if (isCorrectArmor(back))
                user_inputs->unitsToModify.at(u)->new_armorRear = back;
            else
                user_inputs->unitsToModify.at(u)->new_armorFront = user_inputs->unitsToModify.at(u)->armorRear;
            if (isCorrectArmor(sides))
                user_inputs->unitsToModify.at(u)->new_armorSides = sides;
            else
                user_inputs->unitsToModify.at(u)->new_armorFront = user_inputs->unitsToModify.at(u)->armorSides;
            if (isCorrectArmor(top))
                user_inputs->unitsToModify.at(u)->new_armorTop = top;
            else
                user_inputs->unitsToModify.at(u)->new_armorFront = user_inputs->unitsToModify.at(u)->armorTop;

            user_inputs->unitsToModify.at(u)->new_protection_time = prodTime;
            user_inputs->unitsToModify.at(u)->new_stealth = stealth;
            user_inputs->unitsToModify.at(u)->new_autoCoverRange = autoCoverRange;
            user_inputs->unitsToModify.at(u)->new_occupationRadius = occupationRadius;
        }
    

        if (writeData(user_inputs, *settings, true)) {
            user_inputs->restartApp = true;
        }
            
    }
    /*
    ImGui::SameLine();
    if (ImGui::Button("generate mod")) {
        generateMod(user_inputs);
    }
    */
    ImGui::PopStyleColor(3);
    ImGui::PopID();
    if (user_inputs->restartApp) {
        showModif(unit_vector,weapon_vector);
    }
    ImGui::End();
}

void showModif(std::vector<Unit*> unit_vector[], std::vector<Ammo*> weapon_vector[]) {

    ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Restart the app to see the changes in the data files");
    ImGui::Separator();
    ImGui::Text("Units: ");
    Unit* curr = nullptr;
    if (unit_vector) {
        int size_unit = unit_vector->size();

        for (int i = 0; i < size_unit; i++) {
            curr = unit_vector->at(i);

            if (curr->cost != curr->new_cp) {
                ImGui::Text("   %s ", curr->name.c_str()); ImGui::SameLine();
                ImGui::Text("cost: %d -> %d", curr->cost, curr->new_cp);
            }
            if (curr->maxSpeed != curr->new_maxSpeed) {
                ImGui::Text("   %s ", curr->name.c_str()); ImGui::SameLine();
                ImGui::Text("max_speed: %d -> %d",curr->maxSpeed, curr->new_maxSpeed);
            }
            if (curr->speedBonus != curr->new_speedBonus) {
                ImGui::Text("   %s ", curr->name.c_str()); ImGui::SameLine();
                ImGui::Text("speed_bonus: %f -> %f", curr->speedBonus, curr->new_speedBonus);
            }
            if (curr->realRoadSpeed != curr->new_realRoadSpeed) {
                ImGui::Text("   %s ", curr->name.c_str()); ImGui::SameLine();
                ImGui::Text("real_road_speed: %d -> %d", curr->realRoadSpeed, curr->new_realRoadSpeed);
            }
            if (curr->fuel != curr->new_fuel) {
                ImGui::Text("   %s ", curr->name.c_str()); ImGui::SameLine();
                ImGui::Text("fuel: %d -> %d", curr->fuel, curr->new_fuel);
            }
            if (curr->fuelTime != curr->new_fuelTime) {
                ImGui::Text("   %s ", curr->name.c_str()); ImGui::SameLine();
                ImGui::Text("real_road_speed: %d -> %d", curr->fuelTime, curr->new_fuelTime);
            }
            if (curr->opticalStrenght != curr->new_optical_strenght) {
                ImGui::Text("   %s ", curr->name.c_str()); ImGui::SameLine();
                ImGui::Text("optical_strenght: %f -> %f", curr->opticalStrenght, curr->new_optical_strenght);
            }
            if (curr->armorFront != curr->new_armorFront) {
                ImGui::Text("   %s ", curr->name.c_str()); ImGui::SameLine();
                ImGui::Text("armorFront: %s -> %s", curr->armorFront.c_str(), curr->new_armorFront.c_str());
            }
            if (curr->armorRear != curr->new_armorRear) {
                ImGui::Text("   %s ", curr->name.c_str()); ImGui::SameLine();
                ImGui::Text("armorBack: %s -> %s", curr->armorRear.c_str(), curr->new_armorRear.c_str());
            }
            if (curr->armorSides != curr->new_armorSides) {
                ImGui::Text("   %s ", curr->name.c_str()); ImGui::SameLine();
                ImGui::Text("armorSides: %s -> %s", curr->armorSides.c_str(), curr->new_armorSides.c_str());
            }
            if (curr->armorTop != curr->new_armorTop) {
                ImGui::Text("   %s ", curr->name.c_str()); ImGui::SameLine();
                ImGui::Text("armorTop: %s -> %s", curr->armorTop.c_str(), curr->new_armorTop.c_str());
            }
            if (curr->production_time != curr->new_protection_time) {
                ImGui::Text("   %s ", curr->name.c_str()); ImGui::SameLine();
                ImGui::Text("production time: %d -> %d", curr->production_time, curr->new_protection_time);
            }
            if (curr->stealth != curr->new_stealth) {
                ImGui::Text("   %s ", curr->name.c_str()); ImGui::SameLine();
                ImGui::Text("stealth: %f -> %f", curr->stealth, curr->new_stealth);
            }
            if (curr->autoCoverRange != curr->new_autoCoverRange) {
                ImGui::Text("   %s ", curr->name.c_str()); ImGui::SameLine();
                ImGui::Text("autoCoverRange: %d -> %d", curr->autoCoverRange, curr->new_autoCoverRange);
            }
            if (curr->occupationRadius != curr->new_occupationRadius) {
                ImGui::Text("   %s ", curr->name.c_str()); ImGui::SameLine();
                ImGui::Text("occupationRadius: %d -> %d", curr->occupationRadius, curr->new_occupationRadius);
            }
        }
    }
    
    ImGui::Separator();

    ImGui::Text("Ammo: ");
    Ammo* current = nullptr;
    if (weapon_vector) {
        int size_weapon = weapon_vector->size();

        for (int k = 0; k < size_weapon; k++) {
            current = weapon_vector->at(k);
            if (current->physicalDamages != current->new_PhysicalDamages) {
                ImGui::Text("   %s ", current->name.c_str()); ImGui::SameLine();
                ImGui::Text("physicalDamages: %f -> %f", current->physicalDamages, current->new_PhysicalDamages);
            }
            if (current->min_range != current->new_min_range) {
                ImGui::Text("   %s ", current->name.c_str()); ImGui::SameLine();
                ImGui::Text("min_range: %d -> %d", current->min_range, current->new_min_range);
            }
            if (current->max_range != current->new_max_range) {
                ImGui::Text("   %s ", current->name.c_str()); ImGui::SameLine();
                ImGui::Text("max_range: %d -> %d", current->max_range, current->new_max_range);
            }
            if (current->suppressDamage != current->new_suppressDamage) {
                ImGui::Text("   %s ", current->name.c_str()); ImGui::SameLine();
                ImGui::Text("suppressDamage: %f -> %f", current->suppressDamage, current->new_suppressDamage);
            }
            if (current->radiusSuppressDamage != current->new_suppressDamageRadius) {
                ImGui::Text("   %s ", current->name.c_str()); ImGui::SameLine();
                ImGui::Text("radiusSuppressDamage: %d -> %d", current->radiusSuppressDamage, current->new_suppressDamageRadius);
            }
            if (current->NoiseDissimulationMalus != current->new_NoiseDissimulationMalus) {
                ImGui::Text("   %s ", current->name.c_str()); ImGui::SameLine();
                ImGui::Text("NoiseDissimulationMalus: %f -> %f", current->NoiseDissimulationMalus, current->new_NoiseDissimulationMalus);
            }
            if (current->ShotsBeforeMaxNoise != current->new_ShotsBeforeMaxNoise) {
                ImGui::Text("   %s ", current->name.c_str()); ImGui::SameLine();
                ImGui::Text("ShotsBeforeMaxNoise: %d -> %d", current->ShotsBeforeMaxNoise, current->new_ShotsBeforeMaxNoise);
            }
            if (current->TempsDeVisee != current->new_TempsDeVisee) {
                ImGui::Text("   %s ", current->name.c_str()); ImGui::SameLine();
                ImGui::Text("TempsDeVisee: %f -> %f", current->TempsDeVisee, current->new_TempsDeVisee);
            }
            if (current->TempsEntreDeuxSalves != current->new_TempsEntreDeuxSalves) {
                ImGui::Text("   %s ", current->name.c_str()); ImGui::SameLine();
                ImGui::Text("TempsEntreDeuxSalves: %f -> %f", current->TempsEntreDeuxSalves, current->new_TempsEntreDeuxSalves);
            }
        }
    }
    
    ImGui::Separator();
}
//display the result and also add to the vector "unitToMod" if a unit is selected in the vector "units"
void GUI::showSearchResults(std::vector<Unit*> units, std::vector<Unit*> unitToMod) {

    int size = units.size();
   
    for (int i = 0; i < size; i++) {
        ImGui::Checkbox(("%s", units[i]->name).c_str(),&units[i]->isSelected);
        if (units[i]->isSelected) {

            if(!isAlreadyPresent(unitToMod,units[i]))
                unitToMod.push_back(units[i]);
        }
    }
    ImGui::Separator();
    ImGui::TextColored(ImVec4(1.0f, 0.0f, 1.0f, 1.0f), "unitcount : %d", size);
}
void GUI::showSearchResults(std::vector<Ammo*> ammunition, std::vector<Ammo*> ammunitionToMod) {

    int size = ammunition.size();
 
    for (int i = 0; i < size; i++) {
        ImGui::Checkbox(("%s", ammunition[i]->name).c_str(), &ammunition[i]->isSelected);
        if (ammunition[i]->isSelected) {

            if (!isAlreadyPresent(ammunitionToMod, ammunition[i]))
                ammunitionToMod.push_back(ammunition[i]);
        }
    }
    ImGui::Separator();
    ImGui::TextColored(ImVec4(1.0f, 0.0f, 1.0f, 1.0f), "weaponcount : %d", size);
}