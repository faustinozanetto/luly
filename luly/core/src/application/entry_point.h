#pragma once

#include "application.h"
#include "logging/log.h"

extern luly::core::application* luly::core::create_application();

int main(int argc, char** argv)
{
    luly::core::log::initialize();

    luly::core::application* application = luly::core::create_application();
    application->run();

    delete application;
}
