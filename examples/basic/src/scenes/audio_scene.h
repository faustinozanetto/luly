#pragma once

#include "scene/scene.h"

class audio_scene : public luly::scene::scene
{
public:
    audio_scene();
    ~audio_scene();

    void on_update(float delta_time) override;

private:
    void initialize();
    void create_audio_actor();
};
