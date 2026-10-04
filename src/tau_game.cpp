#include "tau/asset.h"
#include "tau/asset_registry.h"
#include "tau/asset_types.h"
#include "tau/assets/material.h"
#include "tau/assets/mesh.h"
#include "tau/assets/shader.h"
#include "tau/assets/texture.h"
#include "tau/color.h"
#include "tau/components/renderer.h"
#include "tau/components/transform.h"
#include "tau/ecs.h"
#include "tau/engine.h"
#include "tau/game.h"
#include "tau/hash.h"
#include "tau/log.h"
#include "tau/math.h"
#include "tau/reflection.h"
#include "tau/rendering/renderer.h"
#include "tau/rendering/rendergraph.h"
#include "tau/time.h"
#include "tau/world.h"

#include <cmath>
#include <string>

using namespace tau;

struct rotator_t
{
    i32 test;
};

TAU_REFLECT() { reflection::component<rotator_t>(ctx, "Rotator").field<&rotator_t::test>("Test Value"); }

struct scaler_t
{
    i32 test;
};

TAU_REFLECT() { reflection::component<scaler_t>(ctx, "Scaler").field<&scaler_t::test>("Test Value"); }

struct mover_t
{
    f32 scale = 1.0f;

    vec3_t first_rec_pos;
};

TAU_REFLECT() { reflection::component<mover_t>(ctx, "Mover").field<&mover_t::scale>("Scale"); }

void tau_game_register_types(tau::world_t* world) { tau::reflection::run_registrations(*world->reflection_ctx); }

void tau_game_init(tau::world_t* world)
{
    ecs::registry_t& reg = world->registry;
    for (auto [entity, mover, transform] : reg.view<mover_t, transform_t>().each())
    {
        mover.first_rec_pos = transform.local_position;
    }
}

void tau_game_update(tau::world_t* world)
{
    ecs::registry_t& reg = world->registry;

    vec3_t rot = {2.0f * (f32)time::get_time(), 0.7f * (f32)time::get_time(), 0.9f * (f32)time::get_time()};
    for (auto [entity, rotator, transform] : reg.view<rotator_t, transform_t>().each())
    {
        transform.local_rotation = quat_t::from_euler(rot);
    }

    vec3_t scale = {2.0f * std::cos((f32)time::get_time()), 1.0f, 2.0f * std::cos((f32)time::get_time())};
    for (auto [entity, rotator, transform] : reg.view<scaler_t, transform_t>().each())
    {
        transform.local_scale = scale;
    }

    for (auto [entity, mover, transform] : reg.view<mover_t, transform_t>().each())
    {
        vec3_t pos = {
            mover.first_rec_pos.x + std::cos((f32)time::get_time()) * mover.scale,
            transform.local_position.y,
            mover.first_rec_pos.z + std::cos((f32)time::get_time()) * mover.scale,
        };
        transform.local_position = pos;
    }
}

void tau_game_shutdown(tau::world_t* world) {}

TAU_GAME_ENTRY()