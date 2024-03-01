#pragma once

#include <scene/scene.h>

class game_scene : public luly::scene::scene {
    public:
        game_scene();

	private:
		void initialize();
		void create_background_actor();
};