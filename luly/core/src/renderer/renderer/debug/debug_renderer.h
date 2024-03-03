#pragma once


#include "math/bounding_box/bounding_box.h"
#include "physics/collision_shapes/physics_box_collision.h"
#include "physics/collision_shapes/physics_sphere_collision.h"

#include <glm/glm.hpp>
#include <memory>

namespace luly::renderer
{
    struct debug_line
    {
        glm::vec3 point_a;
        glm::vec3 point_b;
        glm::vec3 color;

        debug_line(const glm::vec3& point_a, const glm::vec3& point_b, const glm::vec3& color) : point_a(point_a),
            point_b(point_b), color(color)
        {
        }
    };

    struct debug_renderer_data
    {
        std::vector<debug_line> debug_lines;
        float lines_thickness;
        
        bool enable_physics_box_collisions;
        glm::vec3 physics_box_collisions_color;
        
        bool enable_physics_sphere_collisions;
        glm::vec3 physics_sphere_collisions_color;
    };

    class debug_renderer
    {
    public:
        static void initialize();

        static debug_renderer_data& get_data();
        static void reset();

        static void submit_line(const glm::vec3& point_a, const glm::vec3& point_b, const glm::vec3& color);
        static void submit_bounding_box(const math::bounding_box& bounding_box,
                                        math::transform transform = math::transform(),
                                        const glm::vec3& color = glm::vec3(0.85f, 0.5f, 0.0f));
        static void submit_sphere(float radius, math::transform transform = math::transform(),
                                  const glm::vec3& color = glm::vec3(0.85f));
        static void submit_physics_box_collision(
            const std::shared_ptr<physics::physics_box_collision>& physics_box_collision,
            const math::transform& transform = math::transform(),
            const glm::vec3& color = glm::vec3(0.85f, 0.5f, 0.0f));
        static void submit_physics_sphere_collision(
            const std::shared_ptr<physics::physics_sphere_collision>& physics_sphere_collision,
            const math::transform& transform = math::transform(),
            const glm::vec3& color = glm::vec3(0.85f, 0.5f, 0.0f));

        static void collect_debugables();
        static void set_lines_thickness(float lines_thickness);

        static void set_physics_sphere_collisions_enabled(bool enable_physics_sphere_collisions);
        static void set_physics_sphere_collisions_color(const glm::vec3& physics_sphere_collisions_color);
        
        static void set_physics_box_collisions_enabled(bool enable_physics_box_collisions);
        static void set_physics_box_collisions_color(const glm::vec3& physics_box_collisions_color);

    private:
        static void collect_physics_box_collisions();
        static void collect_physics_sphere_collisions();

        static debug_renderer_data s_debug_renderer_data;
    };
}
