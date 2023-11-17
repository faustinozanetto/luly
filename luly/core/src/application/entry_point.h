#pragma once

#include "logging/log.h"
#include "application/application.h"

extern luly::core::application* luly::core::create_application();

int main(int argc, char** argv)
{
    luly::core::log::initialize();

    auto application = luly::core::create_application();
    application->run();

    delete application;
}
