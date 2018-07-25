#include "Context.h"

Context * Context::instance = nullptr;

Context::Context(){
    rootPath = ".";
    hostName = "10.10.11.132";
}

Context * Context::getInstance() {
    if(!instance){
        instance = new Context();
    }
    return instance;
}
