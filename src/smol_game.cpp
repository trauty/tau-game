#include "smol/asset.h"
#include "smol/asset_registry.h"
#include "smol/asset_types.h"
#include "smol/assets/material.h"
#include "smol/assets/mesh.h"
#include "smol/assets/shader.h"
#include "smol/assets/texture.h"
#include "smol/color.h"
#include "smol/components/renderer.h"
#include "smol/components/transform.h"
#include "smol/ecs.h"
#include "smol/engine.h"
#include "smol/game.h"
#include "smol/hash.h"
#include "smol/log.h"
#include "smol/math.h"
#include "smol/reflection.h"
#include "smol/rendering/renderer.h"
#include "smol/rendering/rendergraph.h"
#include "smol/time.h"
#include "smol/world.h"

#include <cmath>
#include <string>

using namespace smol;

struct rotator_t
{
    i32_t test;
};

SMOL_REFLECT() { reflection::component<rotator_t>(ctx, "Rotator").field<&rotator_t::test>("Test Value"); }

struct scaler_t
{
    i32_t test;
};

SMOL_REFLECT() { reflection::component<scaler_t>(ctx, "Scaler").field<&scaler_t::test>("Test Value"); }

struct mover_t
{
    float_t scale = 1.0f;
};

SMOL_REFLECT() { reflection::component<mover_t>(ctx, "Mover").field<&mover_t::scale>("Scale"); }

void smol_game_register_types(smol::world_t* world) { smol::reflection::run_registrations(*world->reflection_ctx); }

void smol_game_init(smol::world_t* world) {}

void smol_game_update(smol::world_t* world)
{
    ecs::registry_t& reg = world->registry;

    vec3_t rot = {2.0f * (f32)time::get_time(), 0.7f * (f32)time::get_time(), 0.9f * (f32)time::get_time()};
    for (auto [entity, rotator, transform] : reg.view<rotator_t, transform_t>().each())
    {
        transform.local_rotation = quat_t::from_euler(rot);
        transform.is_dirty = true;
    }

    vec3_t scale = {2.0f * std::cos((f32)time::get_time()), 1.0f, 2.0f * std::cos((f32)time::get_time())};
    for (auto [entity, rotator, transform] : reg.view<scaler_t, transform_t>().each())
    {
        transform.local_scale = scale;
        transform.is_dirty = true;
    }

    vec3_t pos = {2.0f * std::cos((f32)time::get_time()), 1.0f, 2.0f * std::cos((f32)time::get_time())};
    for (auto [entity, mover, transform] : reg.view<mover_t, transform_t>().each())
    {
        transform.local_position = pos * mover.scale;
        transform.is_dirty = true;
    }
}

void smol_game_shutdown(smol::world_t* world) {}

SMOL_GAME_ENTRY()