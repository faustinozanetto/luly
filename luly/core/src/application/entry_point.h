#pragma once

#include "application/application.h"

#include <logging/log.h>

extern luly::core::application* luly::core::create_application();

int main(int argc, char** argv)
{
    luly::shared::log::initialize();

    const auto application = luly::core::create_application();
    application->run();

    delete application;
}
