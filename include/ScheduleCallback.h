#ifndef CALLBACK_TYPES
#define CALLBACK_TYPES 1
#include <functional>
#include <vector>

std::vector<std::function<bool()>> functionVector;
void scheduleCallback(std::function<bool()> func) {
    functionVector.push_back(func);
}

void callbackISR() {
    for(std::vector<std::function<bool()>>::size_type i = 0; i != functionVector.size(); i++) {
        functionVector[i]();
        functionVector.erase(functionVector.begin()+i);
    }   
}
#endif