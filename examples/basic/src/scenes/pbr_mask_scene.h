#pragma once
#include "scene/scene.h"

class pbr_mask_scene : public luly::scene::scene
{
public:
    pbr_mask_scene();
    ~pbr_mask_scene();

private:
    void initialize();
    void create_mask();
};
