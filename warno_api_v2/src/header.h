#pragma once

#include <iostream>
#include <Windows.h>
#include <tchar.h>
#include <string.h>
#include <vector>
#include <sstream>
#include <stdlib.h>
#include <cctype>
#include <regex>
#include <cstdlib>
#include <cstring>

#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx9.h"

#include "../Microsoft DirectX SDK/Include/d3d9.h"
#include "../Microsoft DirectX SDK/Include/d3dx9.h"
#include "../Microsoft DirectX SDK/Include/D3dx9tex.h"
#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")
#pragma comment(lib, "D3dx9")

#include "class.h"

#define DEBUGNAME_OFFSET 3
#define NATIONALITY_OFFSET 7
#define MOTHERCOUNTRY_OFFSET 8
#define ACKNOW_UNIT_TYPE_OFFSET 9
#define TYPE_UNIT_FORMATION_OFFSET 10

#define NUMBER_OF_ACKTYPE 28
#define NUMBER_OF_AMMO_FAMILY 15
#define NUMBER_OF_AMMO_TYPE 6

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define WINDOW_POSX 100
#define WINDOW_POSY 100

//using this struct to pass data related to user choices
struct params
{
    int old_e_value = -1;       //this is the radio button value, storing so we can check the previous frame value
    int old_e_value_ammo = -1;
    int old_i_value = -1;

    char str1[128]       = "";        
    char str2[128]       = "";        
    char user_path[128]  = "";

    bool modify_units                = false;        //set to true when the user press "modify selected units"
    bool modify_ammo                 = false;
    bool show_unit_results           = false;  //set to true to see the result of the search bar
    bool show_ammunition_results     = false;
    bool opticalList                 = false;   
    bool restartApp                  = false;
    bool fileExplorer                = false;
    bool searchError                 = false;

    bool statusUnitModWind           = true;
    bool unitDone                    = false;
    bool statusAmmoModWind           = true;
    bool ammoDone                    = false;
    
    bool validDir                    = false;
    bool enter                       = false;
    bool newModWnd                   = false;
    std::string path = "";
    PDIRECT3DTEXTURE9 folder_tex;
    PDIRECT3DTEXTURE9 file_tex;
    PDIRECT3DTEXTURE9 example_tex;
    PDIRECT3DTEXTURE9 drive_tex;
   
    int folder_width = 0;
    int file_width = 0;
    int example_width = 0;
    int drive_width = 0;
    int folder_height = 0;
    int file_height = 0;
    int example_height = 0;
    int drive_height = 0;

    int errorCount = 0;
    bool *checkboxes_allUnits;          //this array is used to know if a unit is selected in the 'all' tree node
    std::vector<Unit*> unitsToModify;   //this vector store the units that are going to be modify
    std::vector<Ammo*> ammunitionToModify;

    std::vector<Unit*> search_unit_results;  //this vector store the results of the search bar as pointers
    std::vector<Ammo*> search_ammunition_results;  //this vector store the results of the search bar as pointers


    enum { VALID, EMPTY_STRING, PATH_NOT_FOUND, FILES_MISSING } status = EMPTY_STRING; 
    bool is_auto_speed_bonus = false; //If true, changing the RealRoadSpeed of a unit automatically adjusts its SpeedBonusOnRoad according to a formula
};

//used to store paths
typedef struct settings
{
    std::string original_path, new_path;
    std::string ud_original_path, ud_new_path;
    std::string am_original_path, am_new_path;
    std::string wd_original_path, wd_new_path;
    FILE* hNew_file;
}settings_t;
namespace GUI
{
    void displayUnits(std::string filter, int unitcount, std::vector<Unit*> unit_vector[], int* counter, params* inputs, settings_t* settings);
    void displayAmmo(std::vector<Ammo*> weapon_vector[], std::vector<Unit*> unit_vector[], std::string familyNames[], std::string typeName[], params* inputs);
    void displayTreeNode(std::string ack[], int unitcount, std::vector<Unit*> unit_vector[], int* counter, params* inputs, settings_t* settings);
    std::vector<Unit*> returnSelectedUnits(std::vector<Unit*> unit_vector[], params* inputs);
    std::vector<Ammo*> returnSelectedAmmo(std::vector<Ammo*> ammo_vector[], params* inputs);
    void unitWindow(int unitcount, std::vector<Unit*> unit_vector[], std::vector<Ammo*> weapon_vector[], params* user_inputs, settings_t* settings, bool* x_button,HWND hWnd);
    void unitSelectedWindow(std::vector<Unit*> unit_vector[], std::vector<Ammo*> weapon_vector[],params* user_inputs,settings_t* settings, std::string* ack_type);
    void ammoSelectedWindow(std::vector<Ammo*> weapon_vector[], std::vector<Unit*> unit_vector[],params* user_inputs, settings_t* settings);
    void updateStatsView(params* user_inputs, int indexToSkip);
    void showSearchResults(std::vector<Unit*> units, std::vector<Unit*>  user_inputs);
    void showSearchResults(std::vector<Ammo*> ammunition, std::vector<Ammo*> ammunitionToMod);
    void navBarButtons(std::vector<Unit*> unit_vector[], std::vector<Ammo*> weapon_vector[], params* user_inputs);
    
    std::vector<Ammo*> displayGuns(Unit* currentUnit);
    std::vector<Unit*> findUnitUsingAmmo(Ammo* ammo, std::vector<Unit*> unit_vector[]);
}
// Forward declarations of helper functions
bool CreateDeviceD3D(HWND hWnd);
void CleanupDeviceD3D();
void ResetDevice();
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
int getDataFromFile(std::vector<Unit*> unit_vector[],std::vector<Ammo*> weapons_vector[], settings_t* settings);
void readLine(std::string line, int* line_counter,int* line_counter_all, int* units_counter, int* index, std::vector<Unit*> unit_vector[], std::vector<Ammo*> ammo_vector[], int forUnit);
void readLine(std::string original_line, int* line_counter_relative, int* line_counter, int* weapon_counter, std::vector<Ammo*> weapon_vector[]);
Ammo* findAmmoPtrWithName(std::vector<Ammo*> vector[], std::string name);
bool writeData(params* modPtrs, settings_t settings, bool isUnit);
std::vector<Unit*> searchUnit(std::string str, std::vector<Unit*> unit_vector[], params* user_inputs);
std::vector<Ammo*> searchUnit(std::string str, std::vector<Ammo*> weapon_vector[], params* user_inputs);
bool is_file_exist(const char* fileName);
bool isPathExist(const std::string& s);
bool checkFiles(std::string s);
bool checkDirectoryTxt();
ImVec4 updateImGuiWindow(HWND hWnd, int posXoffset, int posYoffset, int sizeWoffset, int sizeHoffset,ImGuiWindowFlags flags);
void showModif(std::vector<Unit*> unit_vector[], std::vector<Ammo*> weapon_vector[]);
std::string fileExplorer(params* user_inputs,HWND hWnd);
bool directoryWindow(params* user_inputs, bool* x_button, HWND hWnd);
bool LoadTextureFromFile(const char* filename, PDIRECT3DTEXTURE9* out_texture, int* out_width, int* out_height);
bool generateMod(params* user_input);