#include "includes.h"
#include <string>


bool Verification::CheckUser(int* user)
{
    DWORD HddNumber = 0;
    if (!GetVolumeInformation(skCrypt("C://"), NULL, NULL, &HddNumber, NULL, NULL, NULL, NULL))
        return false;

    if (!strcmp(std::to_string(HddNumber).c_str(), skCrypt("2720196500")))
    {
        //std::cout << skCrypt("Logged in as: Bobby").decrypt() << std::endl;
        *user = 1;
        return true;
    }
    else if (!strcmp(std::to_string(HddNumber).c_str(), skCrypt("1579337184")))
    {
        //std::cout << skCrypt("Logged in as: James").decrypt() << std::endl;
        *user = 2;
        return true;
    }
    else if (!strcmp(std::to_string(HddNumber).c_str(), skCrypt("1023393905")))
    {
        //std::cout << skCrypt("Logged in as: Eddie").decrypt() << std::endl;
        *user = 3;
        return true;
    }
    else if (!strcmp(std::to_string(HddNumber).c_str(), skCrypt("1614304552")))
    {
        return false;
    }

    return true;

}