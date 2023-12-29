//Window to choose the mod directory. 
//It should check if the necessary files exist, like UniteDescriptor.ndf .

#include "header.h"
#include <sys/types.h>
#include <sys/stat.h>
#include "..\other_includes\dirent.h" 



//Vérifier si le chemin existe
bool isPathExist(const std::string& s)
{
    struct stat buffer;
    return (stat(s.c_str(), &buffer) == 0);
}

//Vérifie si il y a bien les fichiers ndf nécéssaires avec le bon chemin
bool checkFiles(std::string s)
{
    if (!isPathExist(s + "\\GameData\\Generated\\Gameplay\\Gfx\\UniteDescriptor.ndf")) {}
    else if (!isPathExist(s + "\\GameData\\Generated\\Gameplay\\Gfx\\Ammunition.ndf")) {}
    else return true;

    return false;

}

bool checkDirectoryTxt() {
    if (!isPathExist("directories.txt"))
    {
        return false;
    }
    FILE* file;
    fopen_s(&file, "directories.txt", "r");
    if (!file) { return false; }
    char buffer[256];
    std::string line;
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        line = (std::string)buffer;
    }
    if (isPathExist(line))
    {
        if (checkFiles(line) == true)
        {
            fclose(file);
            return true;
        }
    }
    fclose(file);
    remove("directories.txt");
    return false;
}
void show_dir_content(char* path,params* user_inputs)
{

    DIR* d = opendir(path); // open the path
    if (d == NULL) return; // if was not able, return
    struct dirent* dir; // for the directory entries
    while ((dir = readdir(d)) != NULL) // if we were able to read somehting from the directory
    {
        
        if (dir->d_type != DT_DIR) // if the type is not directory just print it with blue color
        {
            ImGui::Image(user_inputs->file_tex, ImVec2(user_inputs->file_width * 0.025, user_inputs->file_height * 0.025));
            ImGui::SameLine();
            ImGui::Selectable(dir->d_name);
        }
        else
            if (dir->d_type == DT_DIR && strcmp(dir->d_name, ".") != 0 && strcmp(dir->d_name, "..") != 0) // if it is a directory
            {
                ImGui::Image(user_inputs->folder_tex, ImVec2(user_inputs->folder_width * 0.05, user_inputs->folder_height * 0.05));
                ImGui::SameLine();
                if (ImGui::Selectable(dir->d_name)) {
                    user_inputs->path.append(dir->d_name).append("\\");
                }
            }
    }
    closedir(d); // finally close the directory
}
int numberOfSlash(std::string* str) {
    int count = 0;
    int size = str->size();
    for (int i = 0; i < size; i++) {
        if (str->at(i) == '\\') {
            count++;
        }
    }
    return count;
}
void printDrives(std::string* str,params* user_inputs) {
    DWORD drives = GetLogicalDrives();
    ImGui::Text("Select a drive");
    // Loop through all the drive letters (A-Z)
    for (char drive = 'A'; drive <= 'Z'; ++drive) {
        // Check if the corresponding bit is set in the drives variable
        if (drives & (1 << (drive - 'A'))) {
            std::string drivePath = std::string(1, drive) + ":";

            // Get the volume information
            char volumeName[MAX_PATH + 1];
            DWORD volumeSerialNumber;
            DWORD maximumComponentLength;
            DWORD fileSystemFlags;
            char fileSystemName[MAX_PATH + 1];

            ImGui::Image(user_inputs->drive_tex, ImVec2(user_inputs->drive_width * 0.075, user_inputs->drive_height * 0.075));
            ImGui::SameLine();
            if (ImGui::Selectable(("%10s", drivePath.c_str()))) {
                *str = drivePath;
                str->append("\\");
            }
        }
    }
}
int backPath(std::string* str) {

    int count = 0;
    int nb = numberOfSlash(str);
    if (nb > 1) {
        for (int i = 0; i < str->size(); i++) {
            if (str->at(i) == '\\') {
                count++;
            }
            if (count == nb - 1) {
                *str = str->substr(0, i + 1);
                break;
            }
        }
    }
    else {
        str->clear();
    }
    return nb;
}
std::string fileExplorer(params* user_inputs,HWND hWnd) {

    updateImGuiWindow(hWnd, WINDOW_WIDTH / 2.1, 40, -(WINDOW_WIDTH / 2), -(WINDOW_HEIGHT*0.1), ImGuiCond_FirstUseEver);
    ImGui::Begin("File explorer");
    
    if (ImGui::SmallButton("<-") && user_inputs->path.size()) {

        backPath(&user_inputs->path);
    }

    ImGui::SameLine();
    ImGui::Text(user_inputs->path.c_str());

    if(user_inputs->path.size() == 0) {
        printDrives(&user_inputs->path,user_inputs);
    }
    
    if (ImGui::Button("use this directory")) {
        user_inputs->validDir = true;
    }

    ImGui::Separator();
    
    show_dir_content((char*)user_inputs->path.c_str(),user_inputs);

    ImGui::End();

    return user_inputs->path;
}
bool createNewMod(params* user_inputs,HWND hWnd) {

    
    return true;
}
bool directoryWindow(params* user_inputs, bool* x_button,HWND hWnd) {

    if (checkDirectoryTxt())
    {
        user_inputs->status = params::VALID;
    }
    
    ImVec4 posXY_sizeWH = updateImGuiWindow(hWnd, 20, 40, -(WINDOW_WIDTH / 2), -(WINDOW_HEIGHT * 0.1), ImGuiCond_FirstUseEver);
    ImGui::Begin("Directory Window",x_button);

    ImGui::Text("Select an existing mod below or");
    ImGui::SameLine();
    if (ImGui::Selectable("create one")) {
        user_inputs->newModWnd = true;
    }
    ImGui::InputTextWithHint("##directory", "Enter the path of your mod folder", user_inputs->user_path, IM_ARRAYSIZE(user_inputs->user_path));
    ImGui::SameLine();
    if (ImGui::Button("Enter")) {
        user_inputs->enter = true;
    }
    if (ImGui::Button("Browse")) {
        user_inputs->fileExplorer = true;
    }
    if (user_inputs->fileExplorer) {
        fileExplorer(user_inputs,hWnd);
    }
    if (user_inputs->newModWnd) {
        createNewMod(user_inputs,hWnd);
    }
    if (user_inputs->enter || user_inputs->validDir)
    {
        
        const char* path = user_inputs->user_path;
        if (user_inputs->validDir) {
            path = user_inputs->path.c_str();
        }
        std::string path_str = path;
        user_inputs->validDir = false;
        user_inputs->enter = false;

        if (isPathExist(path_str)) //Le chemin existe. Est-ce que le répertoire contient bien les fichiers NDF?
        {
            if (checkFiles(path_str) == true) //Oui il les contient
            {
                user_inputs->status = params::VALID;
                FILE* directory_path;
                fopen_s(&directory_path, "directories.txt", "w");
                if (!directory_path) { ImGui::End(); return false; }
                fputs(path, directory_path);
                fclose(directory_path);
            }
            else //Non il  ne les contient pas
            {
                user_inputs->status = params::FILES_MISSING;
            }
        }
        else if (path[0] == '\0') //Le champs est vide
            user_inputs->status = params::EMPTY_STRING;
        else if (!isPathExist(path))
        {
            user_inputs->status = params::PATH_NOT_FOUND; //Le chemin n'existe pas
            printf("ERROR: Directory not found\n");
        }
    }
    ImGui::SameLine();
    ImGui::Button("Help");
    if (ImGui::IsItemHovered() && ImGui::BeginTooltip()) {

        ImGui::Text("Path examples: ");
        ImGui::Text("C:\\Users\\myName\\Desktop\\myMod");
        ImGui::Text("C:\\Program Files (x86)\\Steam\\steamapps\\common\\WARNO\\Mods\\myMod");
        ImGui::EndTooltip();
    }

    int status = user_inputs->status;
    if (status == params::EMPTY_STRING) {
        ImGui::Text("Enter a directory path");
        ImGui::End();
        return false;
    }    
    else if (status == params::PATH_NOT_FOUND) {
        
        if (user_inputs->errorCount > 0) {
            ImGui::Separator();
            ImGui::Text("The mod directory should look like that");
            ImGui::Image(user_inputs->example_tex, ImVec2(user_inputs->example_width * 0.5, user_inputs->example_height * 0.5));
        }
        ImGui::Text("ERROR: Directory not found");
        ImGui::End();
        user_inputs->errorCount++;
        return false;
    } 
    else if (status == params::FILES_MISSING) {
        
        if (user_inputs->errorCount > 0) {
            ImGui::Separator();
            ImGui::Text("The mod directory should look like that");
            ImGui::Image(user_inputs->example_tex, ImVec2(user_inputs->example_width * 0.5, user_inputs->example_height * 0.5));
        }
        ImGui::Text("ERROR: Missing files in Mod Folder");
        ImGui::End();
        user_inputs->errorCount++;
        return false;
    }
   

    ImGui::End();
    return true;
}
